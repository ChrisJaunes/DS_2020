#include "pch.h"
#include "BPlusTree.h"

enum {
    INVALID_OFFSET = 0xffffffff,
};

enum {
    BPLUSTREE_INTERNAL,
    BPLUSTREE_LEAF = 1,
};

inline bool BPlusTreeNode::is_leaf() {
    return type == BPLUSTREE_LEAF;
}

key_t* BPlusTreeNode::get_keys() {
    return (key_t *)((char*)(this) + sizeof(*this));
}

off_t* BPlusTreeNode::get_offs() {
    return (off_t * )(((char*)(this) + sizeof(*this)) + BPT_MAX_ORDER * sizeof(key_t));
}

int BPlusTreeNode::binary_search_by_key(key_t target) {
    key_t* arr = get_keys();
    int len = is_leaf() ? children : children - 1;
    int pos = std::lower_bound(arr, arr+len, target) - arr;
    if (pos >= len || arr[pos] != target) return -pos - 1;
    return pos;
}

off_t BPlusTreeNode::insert_leaf_value(off_t, value_t)
{
    return off_t();
}

inline BPlusTreeNode* BPlusTree::cache_refer() {
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        if (!used[i]) {
            used[i] = 1;
            char* buf = caches + BPT_BLOCK_SIZE * i;
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
    node->children = 0;
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
    //读入文件块
    // todo
    return node;
}

BPlusTreeNode* BPlusTree::node_seek(off_t offset) {
    if (offset == INVALID_OFFSET) {
        return NULL;
    }
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        if (!used[i]) {
            char* buf = caches + BPT_BLOCK_SIZE * i;
            //读入文件块
            //assert(len == BPT_BLOCK_SIZE);
            return (BPlusTreeNode*) buf;
        }
    }
    assert(0);
}

void BPlusTree::node_flush_file(BPlusTreeNode* node) {
    if (node != NULL) {
        //写入文件块
        //todo
        cache_defer(node);
    }
}
void BPlusTree::node_flush_parent(off_t node, BPlusTreeNode* parent) {

}
off_t BPlusTree::search_by_key(key_t key) {
    off_t ret = INVALID_OFFSET;
    BPlusTreeNode* node = node_seek(root);
    while (node != NULL) {
        int index = node->binary_search_by_key(key);
        if (node->is_leaf()) {
            ret = index >= 0 ? (node->get_keys())[index] : INVALID_OFFSET;
            break;
        }
        else {
            if (index >= 0) {
                node = node_seek((node->get_offs())[index + 1]);
            }
            else {
                index = -index - 1;
                node = node_seek((node->get_offs())[index]);
            }
        }
    }
    return ret;
}


off_t BPlusTree::node_append(BPlusTreeNode* node) {
    return off_t();
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
        //需要新建父节点，并且更新root
        BPlusTreeNode* parent = new_internal_node();
        (parent->get_keys())[0] = key;
        (parent->get_offs())[0] = lch->self;
        (parent->get_offs())[1] = rch->self;
        parent->children = 2;

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
    left->children = split;
    node->children = BPT_MAX_ORDER - split + 1;

    // left有split个孩子,共有split-1个关键词, split个孩子指针
    // 进行key移动, 其中pivot 位置空出
    memmove(&(left->get_keys())[0], &(node->get_keys())[0], pivot * sizeof(key_t));
    memmove(&(left->get_keys())[pivot+1], &(node->get_keys())[pivot], (split-pivot-1) * sizeof(key_t));
    
    // 进行off移动， 其中pivot 和 pivot+1需要重新处理
    memmove(&(left->get_offs())[0], &(node->get_offs())[0], pivot * sizeof(off_t));
    memmove(&(left->get_offs())[pivot+1], &(node->get_offs())[pivot], (split-pivot-1) * sizeof(off_t));

    for(int i = 0; i < left->children; i++) 
        if (i != pivot && i != pivot + 1) {
            node_flush_parent((left->get_offs())[i], left);
        }

    //插入新值
    (left->get_keys())[pivot] = key;
    if (pivot == split - 1) {
        (left->get_offs())[pivot] = lch->self;
        lch->parent = left->self;

        (node->get_offs())[0] = rch->self;
        rch->parent = node->self;
    }
    else {
        (left->get_offs())[pivot] = lch->self;
        lch->parent = left->self;

        (left->get_offs())[pivot+1] = rch->self;
        rch->parent = left->self;

        (node->get_offs())[0] = (node->get_offs())[split - 1];

        assert((left->get_keys())[split - 1] == (node->get_keys())[split - 2]);
    }
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(node->get_keys())[0], &(node->get_keys())[split - 1], (node->children - 1) * sizeof(key_t));
    memmove(&(node->get_offs())[1], &(node->get_offs())[split], (node->children - 1) * sizeof(off_t));

    return (left->get_keys())[split - 1];
}

key_t BPlusTree::split_internal_M(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch) {
    node_add_right(node, right);
    int split = (BPT_MAX_ORDER + 1) / 2;

    key_t split_key = (node->get_keys())[split - 1];

    int pivot = 0;
    node->children = split;
    right->children = BPT_MAX_ORDER - split + 1;

    (right->get_keys())[pivot] = key;
    (right->get_offs())[pivot] = lch->self;
    lch->parent = right->self;

    (right->get_offs())[pivot + 1] = rch->self;
    rch->parent = right->self;
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(right->get_keys())[pivot + 1], &(right->get_keys())[split], (right->children - 2) * sizeof(key_t));
    memmove(&(right->get_offs())[pivot + 2], &(right->get_offs())[split + 1], (right->children - 2) * sizeof(off_t));

    for (int i = pivot + 2; i < right->children; i++) {
        node_flush_parent((right->get_offs())[i], right);
    }

    return split_key;
}

key_t BPlusTree::split_internal_R(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch) {
    node_add_right(node, right);
    
    int split = (BPT_MAX_ORDER + 1) / 2;
    int pivot = ins_pos - (split + 1);
    node->children = split + 1;
    right->children = BPT_MAX_ORDER - split;
    key_t split_key = (node->get_keys())[split];
    
    // 进行key移动, 其中pivot 位置空出
    memmove(&(right->get_keys())[0], &(node->get_keys())[split + 1], pivot * sizeof(key_t));
    memmove(&(right->get_keys())[pivot + 1], &(node->get_keys())[ins_pos], (BPT_MAX_ORDER - ins_pos - 1) * sizeof(key_t));
    
    // 进行off移动， 其中pivot 和 pivot+1需要重新处理
    memmove(&(right->get_offs())[0], &(node->get_offs())[split + 1], pivot * sizeof(off_t));
    memmove(&(right->get_offs())[pivot + 2], &(node->get_offs())[ins_pos + 1], (ins_pos - ins_pos - 1) * sizeof(off_t));
    
    for (int i = 0; i < right->children; i++)
        if (i != pivot && i != pivot + 1) {
            BPlusTree::node_flush_parent((right->get_offs())[i], right);
        }

    (right->get_keys())[pivot] = key;
    (right->get_offs())[pivot] = lch->self;
    lch->parent = right->self;

    (right->get_offs())[pivot + 1] = rch->self;
    rch->parent = right->self;
    BPlusTree::node_flush_file(lch);
    BPlusTree::node_flush_file(rch);

    return split_key;
}

key_t BPlusTree::split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, off_t dataoff) {
    node_add_left(leaf, left);
    
    int pivot = insert;
    int split = (leaf->children + 1) / 2;
    left->children = split;
    leaf->children = BPT_MAX_ENTRIES - split + 1;

    // leaf移动 pivot 个给左兄弟left
    memmove(&(left->get_keys())[0], &(leaf->get_keys())[0], pivot * sizeof(key_t));
    memmove(&(left->get_offs())[0], &(leaf->get_offs())[0], pivot * sizeof(off_t));

    // 插入key 和 dataoff
    (left->get_keys())[pivot] = key;
    (left->get_offs())[pivot] = dataoff;

    // leaf 移动 (split - pivot - 1 个给左兄弟
    memmove(&(left->get_keys())[pivot + 1], &(leaf->get_keys())[pivot], (split - pivot - 1) * sizeof(key_t));
    memmove(&(left->get_offs())[pivot + 1], &(leaf->get_offs())[pivot], (split - pivot - 1) * sizeof(off_t));

    //维护原来的leaf节点
    memmove(&(leaf->get_keys())[0], &(leaf->get_keys())[split - 1], leaf->children * sizeof(key_t));
    memmove(&(leaf->get_offs())[0], &(leaf->get_offs())[split - 1], leaf->children * sizeof(off_t));

    return (leaf->get_keys())[0];
}

key_t BPlusTree::split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert,  key_t key, off_t dataoff) {
    node_add_right(leaf, right);
    
    int split = (leaf->children + 1) / 2;
    int pivot = insert - split;
    leaf->children = split;
    right->children = BPT_MAX_ENTRIES - split + 1;


    memmove(&(right->get_keys())[0], &(leaf->get_keys())[split], pivot * sizeof(key_t));
    memmove(&(right->get_offs())[0], &(leaf->get_offs())[split], pivot * sizeof(off_t));

    (right->get_keys())[pivot] = key;
    (right->get_offs())[pivot] = dataoff;

    memmove(&(right->get_keys())[pivot + 1], &(leaf->get_keys())[insert], (BPT_MAX_ENTRIES - insert) * sizeof(key_t));
    memmove(&(right->get_offs())[pivot + 1], &(leaf->get_offs())[insert], (BPT_MAX_ENTRIES - insert) * sizeof(off_t));

    return (right->get_keys())[0];
}

void BPlusTree::insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    memmove(&(node->get_keys())[ins_pos + 1], &(node->get_keys())[ins_pos], (node->children - 1 - ins_pos) * sizeof(key_t));
    memmove(&(node->get_offs())[ins_pos + 2], &(node->get_offs())[ins_pos + 1], (node->children - 1 - ins_pos) * sizeof(off_t));
    /* insert new key and sub-nodes */
    (node->get_keys())[ins_pos] = key;
    (node->get_offs())[ins_pos] = lch->self;
    lch->parent = node->self;
    node_flush_file(lch);
    (node->get_offs())[ins_pos + 1] = rch->self;
    rch->parent = node->self;
    node_flush_file(rch);
    node->children++;
}

int BPlusTree::insert_internal(BPlusTreeNode* node, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    int ins_pos = node->binary_search_by_key(key);
    assert(ins_pos < 0);
    ins_pos = -ins_pos - 1;

    if (node->children == BPT_MAX_ORDER) {
        key_t split_key;
        int split = (node->children + 1) / 2;
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

void BPlusTree::insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, off_t value_off) {
    memmove(&(leaf->get_keys())[ins_pos + 1], &(leaf->get_keys())[ins_pos], (leaf->children - ins_pos) * sizeof(key_t));
    memmove(&(leaf->get_offs())[ins_pos + 1], &(leaf->get_offs())[ins_pos], (leaf->children - ins_pos) * sizeof(off_t));
    (leaf->get_keys())[ins_pos] = key;
    (leaf->get_offs())[ins_pos] = value_off;
    leaf->children++;
}

int BPlusTree::insert_leaf(BPlusTreeNode* leaf, key_t key, value_t value) {
    int ins_pos = leaf->binary_search_by_key(key);
    
    //已经存在
    if (ins_pos >= 0) {
        BPlusTreeNode::insert_leaf_value((leaf->get_offs())[ins_pos], value);
        return -1; 
    }
    ins_pos = -ins_pos - 1;
    off_t value_off = BPlusTreeNode::insert_leaf_value(INVALID_OFFSET, value);
    

    /* fetch from free node caches */
    int i = ((char*)leaf - caches) / BPT_BLOCK_SIZE;
    used[i] = 1;

    if (leaf->children == BPT_MAX_ENTRIES) {
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
    return 0;
}

int BPlusTree::insert(key_t key, value_t value) {
    BPlusTreeNode* node = node_seek(root);
    while (node != NULL) {
        if (node->is_leaf()) {
            return insert_leaf(node, key, value);
        }
        else {
            int index = node->binary_search_by_key(key);
            if (index >= 0) {
                node = node_seek((node->get_keys())[index + 1]);
            }
            else {
                index = -index - 1;
                node = node_seek((node->get_keys())[index]);
            }
        }
    }
    
    BPlusTreeNode* new_rt = new_leaf_node();
    new_rt->children = 1;
    (new_rt->get_keys())[0] = key;
    (new_rt->get_offs())[0] = BPlusTreeNode::insert_leaf_value(INVALID_OFFSET, value);
    root = node_append(new_rt);
    node_flush_file(new_rt);
    level = 1;
    return 0;
}