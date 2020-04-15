#include "pch.h"
#include "BPIO.h"
#include "BPlusTree.h"

const off_t INVALID_OFFSET = 0xffffffff;

enum {
    BPLUSTREE_INTERNAL,
    BPLUSTREE_LEAF = 1,
};

bool BPlusTreeNode::is_leaf() const{
    return type == BPLUSTREE_LEAF;
}

int BPlusTreeNode::binary_search_by_key(key_t target) {
    int len = is_leaf() ? ch_cnt : ch_cnt - 1;
    int pos = std::lower_bound(keys, keys+len, target) - keys;
    if (pos >= len || keys[pos] != target) return -pos - 1;
    return pos;
}

inline BPlusTreeNode* BPlusTree::cache_refer() {
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        if (!used[i]) {
            used[i] = 1;
            char* buf = caches + int(BPT_BLOCK_SIZE * i);
            return (BPlusTreeNode*) buf;
        }
    }
    assert(0);
}

inline void BPlusTree::cache_defer(BPlusTreeNode* node) {
    char* buf = (char*)node;
    int i = (buf - caches) / BPT_BLOCK_SIZE;
    used[i] = 0;
}

BPlusTreeNode* BPlusTree::new_node() {
    BPlusTreeNode* node = cache_refer();

    node->self = INVALID_OFFSET;
    node->parent = INVALID_OFFSET;
    node->prev = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    node->ch_cnt = 0;
    return node;
}

BPlusTreeNode* BPlusTree::new_leaf_node() {
    BPlusTreeNode* node = new_node();
    node->type = BPLUSTREE_LEAF;
    return node;
}

BPlusTreeNode* BPlusTree::new_internal_node() {
    BPlusTreeNode* node = new_node();
    node->type = BPLUSTREE_INTERNAL;
    return node;
}

BPlusTreeNode* BPlusTree::node_fetch(off_t offset) {
    if (offset == INVALID_OFFSET) {
        return NULL;
    }

    BPlusTreeNode* node = cache_refer();
    int len = BPIO::db_read(index_fd, offset, node, BPT_BLOCK_SIZE);
    assert(len == BPT_BLOCK_SIZE);
    return node;
}

void BPlusTree::node_flush_file(BPlusTreeNode* node) {
    if (node != NULL) {
        int len = BPIO::db_write(index_fd, node->self, node, BPT_BLOCK_SIZE);
        assert(len == BPT_BLOCK_SIZE);
        cache_defer(node);
    }
}

void BPlusTree::node_flush_parent(off_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != NULL);
    child->parent = parent->self;
    node_flush_file(child);
}

off_t BPlusTree::search_by_key(key_t key) {
    off_t ret = INVALID_OFFSET;
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        int index = node->binary_search_by_key(key);
        if (node->is_leaf()) {
            ret = index >= 0 ? node->offs[index] : INVALID_OFFSET;
            break;
        }
        else {
            if (index >= 0) {
                off_t next_off = node->offs[index+1];
                cache_defer(node);
                node = node_fetch(next_off);
            }
            else {
                index = -index - 1;
                off_t next_off = node->offs[index];
                cache_defer(node);
                node = node_fetch(next_off);
            }
        }
    }
    return ret;
}

off_t BPlusTree::node_append(BPlusTreeNode* node) {
    node->self = index_fdsize;
    index_fdsize += BPT_BLOCK_SIZE;
    return node->self;
}

void BPlusTree::node_add_left(BPlusTreeNode* node, BPlusTreeNode* left) {
    node_append(left);

    BPlusTreeNode* prev = node_fetch(node->prev);
    if (prev != NULL) {
        prev->next = left->self;
        left->prev = prev->self;
        node_flush_file(prev);
    }
    else {
        left->prev = INVALID_OFFSET;
    }
    left->next = node->self;
    node->prev = left->self;
}

void BPlusTree::node_add_right(BPlusTreeNode* node, BPlusTreeNode* right) {
    node_append(right);

    BPlusTreeNode* next = node_fetch(node->next);
    if (next != NULL) {
        next->prev = right->self;
        right->next = next->self;
        node_flush_file(next);
    }
    else {
        right->next = INVALID_OFFSET;
    }
    right->prev = node->self;
    node->next = right->self;
}

int BPlusTree::node_add_parent(BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    if (lch->parent == INVALID_OFFSET && rch->parent == INVALID_OFFSET) {
        //根节点分裂,需要新建根节点,并且更新level
        BPlusTreeNode* parent = new_internal_node();
        parent->keys[0] = key;
        parent->offs[0] = lch->self;
        parent->offs[1] = rch->self;
        parent->ch_cnt = 2;

        root = node_append(parent);
        lch->parent = parent->self;
        rch->parent = parent->self;
        ++level;
        
        node_flush_file(lch);
        node_flush_file(rch);
        node_flush_file(parent);
        return 0;
    }
    else if (rch->parent == INVALID_OFFSET) {
        return insert_internal(node_fetch(lch->parent), lch, rch, key);
    }
    else {
        return insert_internal(node_fetch(rch->parent), lch, rch, key);
    }
}

key_t BPlusTree::split_internal_L(BPlusTreeNode* node, BPlusTreeNode* left, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch) {
    node_add_left(node, left);
    
    int split = (BPT_MAX_ORDER + 1) / 2;
    int pivot = ins_pos;
    left->ch_cnt = split;
    node->ch_cnt = BPT_MAX_ORDER - split + 1;

    // left有split个孩子,共有split-1个关键词, split个孩子指针
    // 进行key移动, 其中pivot 位置空出
    memmove(&(left->keys[0]), &(node->keys[0]), pivot * sizeof(key_t));
    memmove(&(left->keys[pivot+1]), &(node->keys[pivot]), (split - pivot - 1) * sizeof(key_t));
    
    // 进行off移动， 其中pivot 和 pivot+1需要重新处理
    memmove(&(left->offs[0]), &(node->offs[0]), pivot * sizeof(off_t));
    memmove(&(left->offs[pivot+1]), &(node->offs[pivot]), (split - pivot - 1) * sizeof(off_t));

    for(int i = 0; i < left->ch_cnt; i++) 
        if (i != pivot && i != pivot + 1) {
            node_flush_parent(left->offs[i], left);
        }

    //插入新值
    left->keys[pivot] = key;
    if (pivot == split - 1) {
        left->offs[pivot] = lch->self;
        lch->parent = left->self;

        node->offs[0] = rch->self;
        rch->parent = node->self;
    }
    else {
        left->offs[pivot] = lch->self;
        lch->parent = left->self;

        left->offs[pivot+1] = rch->self;
        rch->parent = left->self;

       node->offs[0] = node->offs[split - 1];

        assert(left->keys[split - 1] == node->keys[split - 2]);
    }
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(node->keys[0]), &(node->keys[split - 1]), ((node->ch_cnt) - 1) * sizeof(key_t));
    memmove(&(node->offs[1]), &(node->offs[split]), ((node->ch_cnt) - 1) * sizeof(off_t));

    return (left->keys[split - 1]);
}

key_t BPlusTree::split_internal_M(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch) {
    node_add_right(node, right);
    int split = (BPT_MAX_ORDER + 1) / 2;

    key_t split_key = node->keys[split - 1];

    int pivot = 0;
    node->ch_cnt = split;
    right->ch_cnt = BPT_MAX_ORDER - split + 1;

    right->keys[pivot] = key;
    right->offs[pivot] = lch->self;
    lch->parent = right->self;

    right->offs[pivot + 1] = rch->self;
    rch->parent = right->self;
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(right->keys[pivot + 1]), &(right->keys[split]), ((right->ch_cnt) - 2) * sizeof(key_t));
    memmove(&(right->offs[pivot + 2]), &(right->offs[split + 1]), ((right->ch_cnt) - 2) * sizeof(off_t));

    for (int i = pivot + 2; i < right->ch_cnt; i++) {
        node_flush_parent(right->offs[i], right);
    }

    return split_key;
}

key_t BPlusTree::split_internal_R(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch) {
    node_add_right(node, right);
    
    int split = (BPT_MAX_ORDER + 1) / 2;
    int pivot = ins_pos - (split + 1);
    node->ch_cnt = split + 1;
    right->ch_cnt = BPT_MAX_ORDER - split;
    key_t split_key = node->keys[split];
    
    // 进行key移动, 其中pivot 位置空出
    memmove(&(right->keys[0]), &(node->keys[split + 1]), pivot * sizeof(key_t));
    memmove(&(right->keys[pivot + 1]), &(node->keys[ins_pos]), (BPT_MAX_ORDER - ins_pos - 1) * sizeof(key_t));
    
    // 进行off移动， 其中pivot 和 pivot+1需要重新处理
    memmove(&(right->offs[0]), &(node->offs[split + 1]), pivot * sizeof(off_t));
    memmove(&(right->offs[pivot + 2]), &(node->offs[ins_pos + 1]), (BPT_MAX_ORDER - ins_pos - 1) * sizeof(off_t));
    
    for (int i = 0; i < right->ch_cnt; i++)
        if (i != pivot && i != pivot + 1) {
            BPlusTree::node_flush_parent(right->offs[i], right);
        }

    right->keys[pivot] = key;
    right->offs[pivot] = lch->self;
    lch->parent = right->self;

    right->offs[pivot + 1] = rch->self;
    rch->parent = right->self;
    node_flush_file(lch);
    node_flush_file(rch);

    return split_key;
}

void BPlusTree::insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    memmove(&(node->keys[ins_pos + 1]), &(node->keys[ins_pos]), (node->ch_cnt - 1 - ins_pos) * sizeof(key_t));
    memmove(&(node->offs[ins_pos + 2]), &(node->offs[ins_pos + 1]), (node->ch_cnt - 1 - ins_pos) * sizeof(off_t));
    /* insert new key and sub-nodes */
    node->keys[ins_pos] = key;
    node->offs[ins_pos] = lch->self;
    lch->parent = node->self;
    node_flush_file(lch);
    node->offs[ins_pos + 1] = rch->self;
    rch->parent = node->self;
    node_flush_file(rch);
    node->ch_cnt++;
}

int BPlusTree::insert_internal(BPlusTreeNode* node, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    int ins_pos = node->binary_search_by_key(key);
    assert(ins_pos < 0);
    ins_pos = -ins_pos - 1;

    if (node->ch_cnt == BPT_MAX_ORDER) {
        key_t split_key;
        int split = (node->ch_cnt + 1) / 2;
        BPlusTreeNode* sibling = new_internal_node();

        if (ins_pos < split) {
            split_key = split_internal_L(node, sibling, ins_pos, key, lch, rch);
            return node_add_parent(sibling, node, split_key);
        }
        else{
            if (ins_pos == split) {
                split_key = split_internal_M(node, sibling, ins_pos, key, lch, rch);
            }
            else {
                split_key = split_internal_R(node, sibling, ins_pos, key, lch, rch);
            }
            return node_add_parent(node, sibling, split_key);
        }
    }
    else {
        insert_internal_simple(node, ins_pos, lch, rch, key);
        node_flush_file(node);
    }
    return 0;
}

key_t BPlusTree::split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, off_t value_off) {
    node_add_left(leaf, left);

    int pivot = insert;
    int split = (leaf->ch_cnt + 1) / 2;
    left->ch_cnt = split;
    leaf->ch_cnt = BPT_MAX_ENTRIES - split + 1;

    // leaf移动 pivot 个给左兄弟left
    memmove(&(left->keys[0]), &(leaf->keys[0]), pivot * sizeof(key_t));
    memmove(&(left->offs[0]), &(leaf->offs[0]), pivot * sizeof(off_t));

    // 插入key 和 dataoff
    (left->keys[pivot]) = key;
    (left->offs[pivot]) = value_off;

    // leaf 移动 split - pivot - 1 个给左兄弟
    memmove(&(left->keys[pivot + 1]), &(leaf->keys[pivot]), (split - pivot - 1) * sizeof(key_t));
    memmove(&(left->offs[pivot + 1]), &(leaf->offs[pivot]), (split - pivot - 1) * sizeof(off_t));

    //维护原来的leaf节点
    memmove(&(leaf->keys[0]), &(leaf->keys[split - 1]), leaf->ch_cnt * sizeof(key_t));
    memmove(&(leaf->offs[0]), &(leaf->offs[split - 1]), leaf->ch_cnt * sizeof(off_t));

    return (leaf->keys[0]);
}

key_t BPlusTree::split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert, key_t key, off_t dataoff) {
    node_add_right(leaf, right);

    int split = (leaf->ch_cnt + 1) / 2;
    int pivot = insert - split;
    leaf->ch_cnt = split;
    right->ch_cnt = BPT_MAX_ENTRIES - split + 1;

    memmove(&(right->keys[0]), &(leaf->keys[split]), pivot * sizeof(key_t));
    memmove(&(right->offs[0]), &(leaf->offs[split]), pivot * sizeof(off_t));

    right->keys[pivot] = key;
    right->offs[pivot] = dataoff;

    memmove(&(right->keys[pivot + 1]), &(leaf->keys[insert]), (BPT_MAX_ENTRIES - insert) * sizeof(key_t));
    memmove(&(right->offs[pivot + 1]), &(leaf->offs[insert]), (BPT_MAX_ENTRIES - insert) * sizeof(off_t));

    return (right->keys[0]);
}

void BPlusTree::insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, off_t value_off) {
    memmove(&(leaf->keys[ins_pos + 1]), &(leaf->keys[ins_pos]), (leaf->ch_cnt - ins_pos) * sizeof(key_t));
    memmove(&(leaf->offs[ins_pos + 1]), &(leaf->offs[ins_pos]), (leaf->ch_cnt - ins_pos) * sizeof(off_t));
    leaf->keys[ins_pos] = key;
    leaf->offs[ins_pos] = value_off;
    leaf->ch_cnt++;
}

int BPlusTree::insert_leaf(BPlusTreeNode* leaf, key_t key, value_t* value) {
    int ins_pos = leaf->binary_search_by_key(key);
    
    if (ins_pos >= 0) {
        //已经存在
        insert_leaf_value(leaf->offs[ins_pos], value);
        node_flush_file(leaf);
    }
    else {
        ins_pos = -ins_pos - 1;
        off_t value_off = insert_leaf_value(INVALID_OFFSET, value);
        if (leaf->ch_cnt == BPT_MAX_ENTRIES) {
            key_t split_key;
            int split = (BPT_MAX_ENTRIES + 1) / 2;
            BPlusTreeNode* sibling = new_leaf_node();

            if (ins_pos < split) {
                split_key = split_leaf_L(leaf, sibling, ins_pos, key, value_off);
                return node_add_parent(sibling, leaf, split_key);
            }
            else {
                split_key = split_leaf_R(leaf, sibling, ins_pos, key, value_off);
                return node_add_parent(leaf, sibling, split_key);
            }
        }
        else {
            insert_leaf_simple(leaf, ins_pos, key, value_off);
            node_flush_file(leaf);
        }
    }
    return 0;
}

off_t BPlusTree::insert_leaf_value(off_t next_off, value_t* value) {
    /*int value_size = wcslen(value), write_size = 0;
    while (write_size < value_size) {
        if (next_off != INVALID_OFFSET) {
            node = node_fetch();
        }
    }
    return INVALID_OFFSET;
    */
    return BPIO::db_write(data_fd, data_fdsize, value, wcslen(value));
}

int BPlusTree::insert(key_t key, value_t* value) {
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        if (node->is_leaf()) {
            return insert_leaf(node, key, value);
        }
        else {
            int index = node->binary_search_by_key(key);
            if (index >= 0) {
                off_t next_off = node->offs[index + 1];
                cache_defer(node);
                node = node_fetch(next_off);
            }
            else {
                index = -index - 1;
                off_t next_off = node->offs[index];
                cache_defer(node);
                node = node_fetch(next_off);
            }
        }
    }
    
    BPlusTreeNode* new_rt = new_leaf_node();
    new_rt->ch_cnt = 1;
    new_rt->keys[0] = key;
    new_rt->offs[0] = insert_leaf_value(INVALID_OFFSET, value);
    root = node_append(new_rt);
    node_flush_file(new_rt);
    level = 1;
    return 0;
}