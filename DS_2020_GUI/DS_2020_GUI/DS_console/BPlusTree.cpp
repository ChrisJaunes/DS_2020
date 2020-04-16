#include "pch.h"
#include "config.h"
#include "BPIO.h"
#include "BPlusTree.h"

const chd_t INVALID_OFFSET = 0xffffffff;

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

int BPlusTreeNode::fromFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE) {
    void* buf = malloc(_BLOCK_SIZE);
    int res = BPIO::db_read(file, file_off, buf, _BLOCK_SIZE, 1);
    size_t off = 0;
    memmove(&type, (char*)buf + off, sizeof(type)); off += sizeof(type);
    memmove(&self, (char*)buf + off, sizeof(self)); off += sizeof(self);
    memmove(&parent, (char*)buf + off, sizeof(parent)); off += sizeof(parent);
    memmove(&prev, (char*)buf + off, sizeof(prev)); off += sizeof(prev);
    memmove(&next, (char*)buf + off, sizeof(next)); off += sizeof(next);
    memmove(&ch_cnt, (char*)buf + off, sizeof(ch_cnt)); off += sizeof(ch_cnt);
    memmove(&keys, (char*)buf + off, sizeof(keys[0]) * BPT_MAX_ENTRIES); off += sizeof(keys[0]) * BPT_MAX_ENTRIES;
    memmove(&chds, (char*)buf + off, sizeof(chds[0]) * BPT_MAX_ENTRIES); off += sizeof(chds[0]) * BPT_MAX_ENTRIES;
    free(buf);
    return res;
}

int BPlusTreeNode::toFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE) {
    void* buf = malloc(_BLOCK_SIZE);
    size_t off = 0;
    memmove((char*)buf + off, &type, sizeof(type)); off += sizeof(type);
    memmove((char*)buf + off, &self, sizeof(self)); off += sizeof(self);
    memmove((char*)buf + off, &parent, sizeof(parent)); off += sizeof(parent);
    memmove((char*)buf + off, &prev, sizeof(prev)); off += sizeof(prev);
    memmove((char*)buf + off, &next, sizeof(next)); off += sizeof(next);
    memmove((char*)buf + off, &ch_cnt, sizeof(ch_cnt)); off += sizeof(ch_cnt);
    memmove((char*)buf + off, &keys, sizeof(keys[0]) * BPT_MAX_ENTRIES); off += sizeof(keys[0]) * BPT_MAX_ENTRIES;
    memmove((char*)buf + off, &chds, sizeof(chds[0]) * BPT_MAX_ENTRIES); off += sizeof(chds[0]) * BPT_MAX_ENTRIES;
     int res = BPIO::db_write(file, file_off,  buf, _BLOCK_SIZE, 1);
    free(buf);
    return res;
}

//本处应该使用文件块链表方式，buf利用不用free掉
wchar_t* BPlusTree::get_value_by_offset(chd_t next_off) {
    void* buf = malloc(_BLOCK_SIZE);
    int off = next_off == INVALID_OFFSET ? data_fdsz : next_off;

    int res = BPIO::db_read(data_fd, off, buf, _BLOCK_SIZE, 1);

    wchar_t* value = new wchar_t[wcslen((wchar_t*)buf)];
    free(buf);
    return value;
}
//本处应该使用文件块链表方式，测试过程中假定sizeof(chd_t) * 2 + sizeof(value) <= _BLOCK_SIZE;
chd_t BPlusTree::set_value_by_offest(chd_t next_off, const value_t* value) {
    void* buf = malloc(_BLOCK_SIZE);
    int off = next_off == INVALID_OFFSET ? data_fdsz : next_off;

    int res = BPIO::db_write(data_fd, off, buf, _BLOCK_SIZE, 1);
    free(buf);
    return res;
    /*int value_size = wcslen(value), write_size = 0;
    while (write_size < value_size) {
        if (next_off != INVALID_chdsET) {
            node = node_fetch();
        }
    }
    return INVALID_chdsET;
    */
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
    int len = BPIO::db_read(index_fd, chdset, node, sizeof(node), 1);
    assert(len == BPT_BLOCK_SIZE);
    return node;
}

void BPlusTree::node_flush_file(BPlusTreeNode* node) {
    if (node != NULL) {
        int len = BPIO::db_write(index_fd, node->self, node, sizeof(node), 1);
        assert(len == BPT_BLOCK_SIZE);
        cache_defer(node);
    }
}

void BPlusTree::node_flush_parent(chd_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != NULL);
    child->parent = parent->self;
    node_flush_file(child);
}

int BPlusTree::search_by_key(key_t key, value_t*& value) {
    int ret = 0;
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        int index = node->binary_search_by_key(key);
        if (node->is_leaf()) {
            if (index >= 0) {
                ret = 1;
                if (value) delete value;
                value = get_value_by_offset(node->chds[index]);
            }
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

chd_t BPlusTree::node_append(BPlusTreeNode* node) {
    node->self = index_fdsz;
    index_fdsz += _BLOCK_SIZE;
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
        parent->chds[0] = lch->self;
        parent->chds[1] = rch->self;
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

int BPlusTree::insert_leaf(BPlusTreeNode* leaf, key_t key, const value_t* value) {
    int ins_pos = leaf->binary_search_by_key(key);
    
    if (ins_pos >= 0) {
        //已经存在
        set_value_by_offest(leaf->chds[ins_pos], value);
        node_flush_file(leaf);
    }
    else {
        ins_pos = -ins_pos - 1;
        chd_t value_off = set_value_by_offest(INVALID_OFFSET, value);
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

int BPlusTree::insert(key_t key, const value_t* value) {
    BPlusTreeNode* node = node_fetch(root);
    while (node != NULL) {
        if (node->is_leaf()) {
            return insert_leaf(node, key, value);
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
    new_rt->chds[0] = set_value_by_offest(INVALID_OFFSET, value);
    root = node_append(new_rt);
    node_flush_file(new_rt);
    level = 1;
    return 0;
}

BPlusTree::BPlusTree(wchar_t* filename, int exist) {
    int sz = wcslen(filename);

    index_fdpath = new wchar_t[wcslen(DS_DATABASE_DIR) + wcslen(L"db/") +  sz + wcslen(L"_index.db") + 1];
    wcscat_s(index_fdpath, sizeof(index_fdpath), DS_DATABASE_DIR);
    wcscat_s(index_fdpath, sizeof(index_fdpath), L"db/");
    wcscat_s(index_fdpath, sizeof(index_fdpath), filename);
    wcscat_s(index_fdpath, sizeof(index_fdpath), L"_index.db");
    assert((index_fd = _wfopen(index_fdpath, L"wb+")) != NULL);

    data_fdpath = new wchar_t[wcslen(DS_DATABASE_DIR) + wcslen(L"db/") + sz + wcslen(L"_data.db") + 1];
    wcscat_s(data_fdpath, sizeof(data_fdpath), DS_DATABASE_DIR);
    wcscat_s(data_fdpath, sizeof(data_fdpath), L"db/");
    wcscat_s(data_fdpath, sizeof(data_fdpath), filename);
    wcscat_s(data_fdpath, sizeof(data_fdpath), L"_data.db");
    assert((data_fd = _wfopen(data_fdpath, L"wb+")) != NULL);

    void* buf = malloc(48);
    if (exist) {
        BPIO::db_read(index_fd, 0, buf, 48, 1);

        size_t off = 0;
        memmove(&_BLOCK_SIZE, (char*)buf + off, sizeof(_BLOCK_SIZE)); off += sizeof(_BLOCK_SIZE);
        memmove(&index_fdsz, (char*)buf + off, sizeof(index_fdsz)); off += sizeof(index_fdsz);
        memmove(&data_fdsz, (char*)buf + off, sizeof(data_fdsz)); off += sizeof(data_fdsz);
        memmove(&level, (char*)buf + off, sizeof(level)); off += sizeof(level);
        memmove(&root, (char*)buf + off, sizeof(root)); off += sizeof(root);
        memmove(&leadf_head, (char*)buf + off, sizeof(leadf_head)); off += sizeof(leadf_head);
        assert(_BLOCK_SIZE <= sizeof(BPlusTreeNode));
    }
    else {
        _BLOCK_SIZE = BPT_BLOCK_SIZE;
        assert(_BLOCK_SIZE <= sizeof(BPlusTreeNode));
        index_fdsz = 48;
        data_fdsz = 0;
        level = -1;
        root = INVALID_OFFSET;
        leadf_head = INVALID_OFFSET;
        
        size_t off = 0;
        memmove((char*)buf + off, &_BLOCK_SIZE, sizeof(_BLOCK_SIZE)); off += sizeof(_BLOCK_SIZE);
        memmove((char*)buf + off, &index_fdsz, sizeof(index_fdsz)); off += sizeof(index_fdsz);
        memmove((char*)buf + off, &data_fdsz, sizeof(data_fdsz)); off += sizeof(data_fdsz);
        memmove((char*)buf + off, &level, sizeof(level)); off += sizeof(level);
        memmove((char*)buf + off, &root, sizeof(root)); off += sizeof(root);
        memmove((char*)buf + off, &leadf_head, sizeof(leadf_head)); off += sizeof(leadf_head);
        BPIO::db_write(index_fd, 0, buf, 48, 1);
    }
    caches = malloc(_BLOCK_SIZE * BPT_CACHE_NUM);
    memset(used, 0, sizeof(used));
    free(buf);
}

BPlusTree::~BPlusTree() {
    fclose(index_fd);
    fclose(data_fd);
    delete index_fdpath;
    delete data_fdpath;
    delete caches;
}