#include "pch.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

bool BPlusTreeNode::is_leaf() const {
    return type == BPLUSTREE_LEAF;
}

int BPlusTreeNode::binary_search_by_key(key_t target) {
    int len = is_leaf() ? ch_cnt : ch_cnt - 1;
    int pos = std::lower_bound(keys, keys+len, target) - keys;
    if (pos >= len || keys[pos] != target) return -pos - 1;
    return pos;
}

void BPlusTreeNode::fromFileBlock(FILE* file, chd_t file_off, size_t _block_size) {
    void* buf = malloc(_block_size);
    int res = BPlusTreeUtils::db_read(file, file_off, buf, _block_size, 1);
    size_t off = 0;
    memmove(&self, (char*)buf + off, sizeof(self)); off += sizeof(self);
    memmove(&parent, (char*)buf + off, sizeof(parent)); off += sizeof(parent);
    memmove(&type, (char*)buf + off, sizeof(type)); off += sizeof(type);
    memmove(&ch_cnt, (char*)buf + off, sizeof(ch_cnt)); off += sizeof(ch_cnt);
    memmove(&prev, (char*)buf + off, sizeof(prev)); off += sizeof(prev);
    memmove(&next, (char*)buf + off, sizeof(next)); off += sizeof(next);
    memmove(&keys, (char*)buf + off, sizeof(keys[0]) * BPT_MAX_ENTRIES); off += sizeof(keys[0]) * BPT_MAX_ENTRIES;
    memmove(&chds, (char*)buf + off, sizeof(chds[0]) * BPT_MAX_ENTRIES); off += sizeof(chds[0]) * BPT_MAX_ENTRIES;
    free(buf);
}

void BPlusTreeNode::toFileBlock(FILE* file, chd_t file_off, size_t _block_size) {
    void* buf = malloc(_block_size);
    size_t off = 0;
    memmove((char*)buf + off, &self, sizeof(self)); off += sizeof(self);
    memmove((char*)buf + off, &parent, sizeof(parent)); off += sizeof(parent);
    memmove((char*)buf + off, &type, sizeof(type)); off += sizeof(type);
    memmove((char*)buf + off, &ch_cnt, sizeof(ch_cnt)); off += sizeof(ch_cnt);
    memmove((char*)buf + off, &prev, sizeof(prev)); off += sizeof(prev);
    memmove((char*)buf + off, &next, sizeof(next)); off += sizeof(next);
    memmove((char*)buf + off, &keys, sizeof(keys[0]) * BPT_MAX_ENTRIES); off += sizeof(keys[0]) * BPT_MAX_ENTRIES;
    memmove((char*)buf + off, &chds, sizeof(chds[0]) * BPT_MAX_ENTRIES); off += sizeof(chds[0]) * BPT_MAX_ENTRIES;
    int res = BPlusTreeUtils::db_write(file, file_off,  buf, _block_size, 1);
    free(buf);
}

void BPlusTree::fromFileBlock() {
    BPlusTreeUtils::db_read(fd, 0, BPT_buf, BPT_ROOT_SIZE, 1);

    char magic_number[17];
    sscanf((char*)BPT_buf, "%s", &magic_number);
    assert(strcmp(magic_number, MAGIC_NUMBER) == 0);
    size_t off = 20;
    memmove(&_block_size, (char*)BPT_buf + off, sizeof(_block_size)); off += sizeof(_block_size);
    assert(_block_size <= sizeof(BPlusTreeNode));
    memmove(&fdsz, (char*)BPT_buf + off, sizeof(fdsz)); off += sizeof(fdsz);
    memmove(&root, (char*)BPT_buf + off, sizeof(root)); off += sizeof(root);
    memmove(&level, (char*)BPT_buf + off, sizeof(level)); off += sizeof(level);
    memmove(&number, (char*)BPT_buf + off, sizeof(number)); off += sizeof(number);
    memmove(&leaf_head, (char*)BPT_buf + off, sizeof(leaf_head)); off += sizeof(leaf_head);
    memmove(&empty_head, (char*)BPT_buf + off, sizeof(empty_head)); off += sizeof(empty_head);
}

void BPlusTree::toFileBlock() {
    sprintf((char*)BPT_buf, "%s", MAGIC_NUMBER);
    size_t off = 20;
    memmove((char*)BPT_buf + off, &_block_size, sizeof(_block_size)); off += sizeof(_block_size);
    memmove((char*)BPT_buf + off, &fdsz, sizeof(fdsz)); off += sizeof(fdsz);
    memmove((char*)BPT_buf + off, &root, sizeof(root)); off += sizeof(root);
    memmove((char*)BPT_buf + off, &level, sizeof(level)); off += sizeof(level);
    memmove((char*)BPT_buf + off, &number, sizeof(number)); off += sizeof(number);
    memmove((char*)BPT_buf + off, &leaf_head, sizeof(leaf_head)); off += sizeof(leaf_head);
    memmove((char*)BPT_buf + off, &empty_head, sizeof(empty_head)); off += sizeof(empty_head);
    BPlusTreeUtils::db_write(fd, 0, BPT_buf, BPT_ROOT_SIZE, 1);
}
chd_t BPlusTree::get_empty_block() {
    chd_t res;
    void* buf = malloc(_block_size);
    if (empty_head == INVALID_OFFSET) {
        res = fdsz;
        BPlusTreeUtils::db_write(fd, fdsz, buf, _block_size, 1);
        fdsz += _block_size;
    }
    else {
        res = empty_head;
        BPlusTreeUtils::db_read(fd, empty_head, buf, _block_size, 1);
        memmove(&empty_head, (char*)buf + sizeof(empty_head), sizeof(empty_head));
    }
    toFileBlock();
    free(buf);
    return res;
}
void BPlusTree::get_value_by_offset(chd_t file_off, void*& value, size_t& value_sz) {
    assert(value == NULL);
    assert(file_off < fdsz);

    void *buf = malloc(_block_size);
    void *value_tmp;
    value = NULL;
    value_sz = 0;
    size_t sz = _block_size - sizeof(chd_t) * 3;
    while(file_off != INVALID_OFFSET) {
        BPlusTreeUtils::db_read(fd, file_off, buf, _block_size, 1);
        memmove(&file_off, (char*)buf + sizeof(chd_t), sizeof(chd_t));
        
        value_tmp = value;
        value = malloc(value_sz + sz);
        memmove(value, (char*)buf + sizeof(chd_t) * 3, sz);
        memmove((char*)value + value_sz, value_tmp, value_sz);
        value_sz += sz;
        free(value_tmp);
    }
    free(buf);
}

chd_t BPlusTree::set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz) {
    void* buf = malloc(_block_size);
    while (file_off != INVALID_OFFSET) {
        BPlusTreeUtils::db_read(fd, file_off, buf, _block_size, 1);
        memmove(&file_off, (char*)buf + sizeof(chd_t), sizeof(chd_t));
        memmove((char*)buf + sizeof(chd_t), &empty_head, sizeof(chd_t));
        memmove(&empty_head, buf, sizeof(chd_t));
    }

    size_t value_sz_tmp = 0;
    size_t sz = sizeof(chd_t);
    assert(value_sz % (_block_size - sz * 3) == 0);

    while (value_sz_tmp < value_sz) {
        memmove((char*)buf + sz, &file_off, sz);
        file_off = get_empty_block();
        memmove(buf, &file_off, sz);
        memmove((char*)buf + sz * 2, &INVALID_OFFSET, sz);
        memmove((char*)buf + sz * 3, (char*)value + value_sz_tmp, _block_size - sz * 3);
        BPlusTreeUtils::db_write(fd, file_off, buf, _block_size, 1);
        value_sz_tmp += _block_size - sz * 3;
    }
    free(buf);
    return file_off;
}

inline BPlusTreeNode* BPlusTree::cache_refer() {
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        if (!used[i]) {
            used[i] = 1;
            char* buf = (char*)caches + BPT_BLOCK_SIZE * i;
            return (BPlusTreeNode*) buf;
        }
    }
    assert(0);
    return NULL;
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

BPlusTreeNode* BPlusTree::node_fetch(chd_t chdset) {
    if (chdset == INVALID_OFFSET) {
        return NULL;
    }

    BPlusTreeNode* node = cache_refer();
    int len = BPlusTreeUtils::db_read(fd, chdset, node, BPT_BLOCK_SIZE, 1);
    return node;
}

void BPlusTree::node_flush_file(BPlusTreeNode* node) {
    assert(node != NULL);
    assert(node->self != NULL);
    BPlusTreeUtils::db_write(fd, node->self, node, BPT_BLOCK_SIZE, 1);
    cache_defer(node);
}

void BPlusTree::node_flush_parent(chd_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != NULL);
    child->parent = parent->self;
    node_flush_file(child);
}

int BPlusTree::search(key_t key, void *& value, size_t& value_sz) {
    int ret = BPLUSTRE_FAILED;
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        int index = node->binary_search_by_key(key);
        if (node->is_leaf()) {
            if (index >= 0) {
                ret = BPLUSTRE_SUCCESS;
                get_value_by_offset(node->chds[index], value, value_sz);
            }
            cache_defer(node);
            break;
        }
        else {
            if (index >= 0) {
                chd_t next_off = node->chds[index+1];
                cache_defer(node);
                node = node_fetch(next_off);
            }
            else {
                index = -index - 1;
                chd_t next_off = node->chds[index];
                cache_defer(node);
                node = node_fetch(next_off);
            }
        }
    }
    return ret;
}

void BPlusTree::node_add_left(BPlusTreeNode* node, BPlusTreeNode* left) {
    ++number;
    left->self = get_empty_block();

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
    ++number;
    right->self = get_empty_block();

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
    if (lch->parent != INVALID_OFFSET) {
        return insert_internal(node_fetch(lch->parent), lch, rch, key);
    }
    if (rch->parent != INVALID_OFFSET) {
        return insert_internal(node_fetch(rch->parent), lch, rch, key);
    }
    //根节点分裂,需要新建根节点,并且更新level
    BPlusTreeNode* parent = new_internal_node();
    ++number;
    root = parent->self = get_empty_block();
    parent->keys[0] = key;
    parent->chds[0] = lch->self;
    parent->chds[1] = rch->self;
    parent->ch_cnt = 2;

    lch->parent = parent->self;
    rch->parent = parent->self;
    ++level;
    
    node_flush_file(lch);
    node_flush_file(rch);
    node_flush_file(parent);
    return BPLUSTRE_SUCCESS;
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
    memmove(&(left->chds[0]), &(node->chds[0]), pivot * sizeof(chd_t));
    memmove(&(left->chds[pivot+1]), &(node->chds[pivot]), (split - pivot - 1) * sizeof(chd_t));

    for(int i = 0; i < left->ch_cnt; i++) 
        if (i != pivot && i != pivot + 1) {
            node_flush_parent(left->chds[i], left);
        }

    //插入新值
    left->keys[pivot] = key;
    if (pivot == split - 1) {
        left->chds[pivot] = lch->self;
        lch->parent = left->self;

        node->chds[0] = rch->self;
        rch->parent = node->self;
    }
    else {
        left->chds[pivot] = lch->self;
        lch->parent = left->self;

        left->chds[pivot+1] = rch->self;
        rch->parent = left->self;

       node->chds[0] = node->chds[split - 1];

        assert(left->keys[split - 1] == node->keys[split - 2]);
    }
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(node->keys[0]), &(node->keys[split - 1]), ((node->ch_cnt) - 1) * sizeof(key_t));
    memmove(&(node->chds[1]), &(node->chds[split]), ((node->ch_cnt) - 1) * sizeof(chd_t));

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
    right->chds[pivot] = lch->self;
    lch->parent = right->self;

    right->chds[pivot + 1] = rch->self;
    rch->parent = right->self;
    node_flush_file(lch);
    node_flush_file(rch);

    memmove(&(right->keys[pivot + 1]), &(right->keys[split]), ((right->ch_cnt) - 2) * sizeof(key_t));
    memmove(&(right->chds[pivot + 2]), &(right->chds[split + 1]), ((right->ch_cnt) - 2) * sizeof(chd_t));

    for (int i = pivot + 2; i < right->ch_cnt; i++) {
        node_flush_parent(right->chds[i], right);
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
    memmove(&(right->chds[0]), &(node->chds[split + 1]), pivot * sizeof(chd_t));
    memmove(&(right->chds[pivot + 2]), &(node->chds[ins_pos + 1]), (BPT_MAX_ORDER - ins_pos - 1) * sizeof(chd_t));
    
    for (int i = 0; i < right->ch_cnt; i++)
        if (i != pivot && i != pivot + 1) {
            BPlusTree::node_flush_parent(right->chds[i], right);
        }

    right->keys[pivot] = key;
    right->chds[pivot] = lch->self;
    lch->parent = right->self;

    right->chds[pivot + 1] = rch->self;
    rch->parent = right->self;
    node_flush_file(lch);
    node_flush_file(rch);

    return split_key;
}

void BPlusTree::insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key) {
    memmove(&(node->keys[ins_pos + 1]), &(node->keys[ins_pos]), (node->ch_cnt - 1 - ins_pos) * sizeof(key_t));
    memmove(&(node->chds[ins_pos + 2]), &(node->chds[ins_pos + 1]), (node->ch_cnt - 1 - ins_pos) * sizeof(chd_t));
    /* insert new key and sub-nodes */
    node->keys[ins_pos] = key;
    node->chds[ins_pos] = lch->self;
    lch->parent = node->self;
    node_flush_file(lch);
    node->chds[ins_pos + 1] = rch->self;
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
        if (ins_pos == split) {
            split_key = split_internal_M(node, sibling, ins_pos, key, lch, rch);
        }
        else {
            split_key = split_internal_R(node, sibling, ins_pos, key, lch, rch);
        }
        return node_add_parent(node, sibling, split_key);
    }
    insert_internal_simple(node, ins_pos, lch, rch, key);
    node_flush_file(node);
    return BPLUSTRE_SUCCESS;
}

key_t BPlusTree::split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, chd_t value_off) {
    node_add_left(leaf, left);

    int pivot = insert;
    int split = (leaf->ch_cnt + 1) / 2;
    left->ch_cnt = split;
    leaf->ch_cnt = BPT_MAX_ENTRIES - split + 1;

    // leaf移动 pivot 个给左兄弟left
    memmove(&(left->keys[0]), &(leaf->keys[0]), pivot * sizeof(key_t));
    memmove(&(left->chds[0]), &(leaf->chds[0]), pivot * sizeof(chd_t));

    // 插入key 和 dataoff
    (left->keys[pivot]) = key;
    (left->chds[pivot]) = value_off;

    // leaf 移动 split - pivot - 1 个给左兄弟
    memmove(&(left->keys[pivot + 1]), &(leaf->keys[pivot]), (split - pivot - 1) * sizeof(key_t));
    memmove(&(left->chds[pivot + 1]), &(leaf->chds[pivot]), (split - pivot - 1) * sizeof(chd_t));

    //维护原来的leaf节点
    memmove(&(leaf->keys[0]), &(leaf->keys[split - 1]), leaf->ch_cnt * sizeof(key_t));
    memmove(&(leaf->chds[0]), &(leaf->chds[split - 1]), leaf->ch_cnt * sizeof(chd_t));

    return (leaf->keys[0]);
}

key_t BPlusTree::split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert, key_t key, chd_t dataoff) {
    node_add_right(leaf, right);

    int split = (leaf->ch_cnt + 1) / 2;
    int pivot = insert - split;
    leaf->ch_cnt = split;
    right->ch_cnt = BPT_MAX_ENTRIES - split + 1;

    memmove(&(right->keys[0]), &(leaf->keys[split]), pivot * sizeof(key_t));
    memmove(&(right->chds[0]), &(leaf->chds[split]), pivot * sizeof(chd_t));

    right->keys[pivot] = key;
    right->chds[pivot] = dataoff;

    memmove(&(right->keys[pivot + 1]), &(leaf->keys[insert]), (BPT_MAX_ENTRIES - insert) * sizeof(key_t));
    memmove(&(right->chds[pivot + 1]), &(leaf->chds[insert]), (BPT_MAX_ENTRIES - insert) * sizeof(chd_t));

    return (right->keys[0]);
}

void BPlusTree::insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, chd_t value_off) {
    memmove(&(leaf->keys[ins_pos + 1]), &(leaf->keys[ins_pos]), (leaf->ch_cnt - ins_pos) * sizeof(key_t));
    memmove(&(leaf->chds[ins_pos + 1]), &(leaf->chds[ins_pos]), (leaf->ch_cnt - ins_pos) * sizeof(chd_t));
    leaf->keys[ins_pos] = key;
    leaf->chds[ins_pos] = value_off;
    leaf->ch_cnt++;
}

int BPlusTree::insert_leaf(BPlusTreeNode* leaf, key_t key, const void* value, const size_t value_sz) {
    int ins_pos = leaf->binary_search_by_key(key);
    
    if (ins_pos >= 0) {
        //已经存在
        set_value_by_offest(leaf->chds[ins_pos], value, value_sz);
        node_flush_file(leaf);
        return BPLUSTRE_SUCCESS;
    }
    ins_pos = -ins_pos - 1;
    chd_t value_off = set_value_by_offest(INVALID_OFFSET, value, value_sz);
    if (leaf->ch_cnt == BPT_MAX_ENTRIES) {
        key_t split_key;
        int split = (BPT_MAX_ENTRIES + 1) / 2;
        BPlusTreeNode* sibling = new_leaf_node();

        if (ins_pos < split) {
            split_key = split_leaf_L(leaf, sibling, ins_pos, key, value_off);
            return node_add_parent(sibling, leaf, split_key);
        }
        split_key = split_leaf_R(leaf, sibling, ins_pos, key, value_off);
        return node_add_parent(leaf, sibling, split_key);
    }
    insert_leaf_simple(leaf, ins_pos, key, value_off);
    node_flush_file(leaf);
    return BPLUSTRE_SUCCESS;
}

int BPlusTree::insert(key_t key, const void* value, const size_t value_sz) {
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        if (node->is_leaf()) {
            return insert_leaf(node, key, value, value_sz);
        }
        else {
            int index = node->binary_search_by_key(key);
            if (index >= 0) {
                chd_t next_off = node->chds[index + 1];
                cache_defer(node);
                node = node_fetch(next_off);
            }
            else {
                index = -index - 1;
                chd_t next_off = node->chds[index];
                cache_defer(node);
                node = node_fetch(next_off);
            }
        }
    }
    
    BPlusTreeNode* new_rt = new_leaf_node();
    new_rt->ch_cnt = 1;
    new_rt->keys[0] = key;
    new_rt->chds[0] = set_value_by_offest(INVALID_OFFSET, value, value_sz);
    ++number;
    root = new_rt->self = get_empty_block();
    node_flush_file(new_rt);
    level = 1;
    return BPLUSTRE_SUCCESS;
}

int BPlusTree::insert(key_t key, const wchar_t* value) {
    size_t sz = _block_size - sizeof(chd_t) * 3;
    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + sz - 1) / sz * sz;
    void* _value = malloc(_value_sz);
    memcpy(_value, value, value_sz);
    insert(key, _value, _value_sz);
    free(_value);
    return 0;
}

int BPlusTree::search(key_t key, wchar_t*& value) {
    assert(value == NULL);
    void* _value = NULL;
    size_t _value_sz;
    if(search(key, _value, _value_sz) == BPLUSTRE_FAILED) return BPLUSTRE_FAILED;
    size_t value_sz = _value_sz / sizeof(wchar_t);
    value = new wchar_t[value_sz];
    memset(value, 0, _value_sz);
    wcscat_s(value, value_sz, (wchar_t*)_value);
    return BPLUSTRE_SUCCESS;
}

BPlusTree::BPlusTree(const wchar_t* filepath, int exist) {
    size_t sz = wcslen(filepath) + 1;
    fdpath = new wchar_t[sz];
    memset(fdpath, 0, sz);
    wcscat(fdpath, filepath);

    BPT_buf = malloc(BPT_ROOT_SIZE);
    
    if (exist) {
        assert((fd = _wfopen(fdpath, L"rb")) != NULL);
        fromFileBlock();
    }
    else {
        assert((fd = _wfopen(fdpath, L"wb+")) != NULL);
        _block_size = BPT_BLOCK_SIZE;
        assert(sizeof(BPlusTreeNode) <= _block_size);
        fdsz = BPT_ROOT_SIZE;
        root = INVALID_OFFSET;
        level = -1;
        number = 0;
        leaf_head = INVALID_OFFSET;
        empty_head = INVALID_OFFSET;
        toFileBlock();
    }

    caches = malloc((size_t)_block_size * BPT_CACHE_NUM);
    memset(used, 0, sizeof(used));
}

BPlusTree::~BPlusTree() {
    toFileBlock();
    free(BPT_buf);
    fclose(fd);
    delete[] fdpath;
    free(caches);
}