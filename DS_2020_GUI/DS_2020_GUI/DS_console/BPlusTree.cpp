#include "pch.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

int BPlusTreeNode::binary_search_by_key(key_t target) {
    int len = type == BPLUSTREE_LEAF ? ch_cnt : ch_cnt - 1;
    int pos = std::lower_bound(keys, keys+len, target) - keys;
    return pos;
}

inline void* BPlusTree::cache_refer() {
    for (int i = 3; i < BPT_CACHE_NUM; i++) {
        if (!used[i]) {
            used[i] = 1;
            return (char*)caches_node + node_block_size * i;
        }
    }
    assert(0);
    return NULL;
}

inline void BPlusTree::cache_defer(void* node) {
    if (node == NULL) return;
    int i = ((char*)node - caches_node) / node_block_size;
    used[i] = 0;
}

void BPlusTree::nodeFromFileBlock(BPlusTreeNode* node, chd_t file_off) {
    void* buf = cache_refer();
    BPlusTreeUtils::db_read(fd, file_off, buf, node_block_size, 1);
    size_t off = 0;
    memmove(&node->self, (char*)buf + off, sizeof(node->self)); off += sizeof(node->self);
    memmove(&node->parent, (char*)buf + off, sizeof(node->parent)); off += sizeof(node->parent);
    memmove(&node->type, (char*)buf + off, sizeof(node->type)); off += sizeof(node->type);
    memmove(&node->ch_cnt, (char*)buf + off, sizeof(node->ch_cnt)); off += sizeof(node->ch_cnt);
    memmove(&node->prev, (char*)buf + off, sizeof(node->prev)); off += sizeof(node->prev);
    memmove(&node->next, (char*)buf + off, sizeof(node->next)); off += sizeof(node->next);
    memmove(&node->keys, (char*)buf + off, sizeof(node->keys)); off += sizeof(node->keys);
    memmove(&node->chds, (char*)buf + off, sizeof(node->chds)); off += sizeof(node->chds);
    cache_defer(buf);
}

void BPlusTree::nodeToFileBlock(BPlusTreeNode* node, chd_t file_off) {
    void* buf = cache_refer();
    size_t off = 0;
    memmove((char*)buf + off, &node->self, sizeof(node->self)); off += sizeof(node->self);
    memmove((char*)buf + off, &node->parent, sizeof(node->parent)); off += sizeof(node->parent);
    memmove((char*)buf + off, &node->type, sizeof(node->type)); off += sizeof(node->type);
    memmove((char*)buf + off, &node->ch_cnt, sizeof(node->ch_cnt)); off += sizeof(node->ch_cnt);
    memmove((char*)buf + off, &node->prev, sizeof(node->prev)); off += sizeof(node->prev);
    memmove((char*)buf + off, &node->next, sizeof(node->next)); off += sizeof(node->next);
    memmove((char*)buf + off, &node->keys, sizeof(node->keys)); off += sizeof(node->keys);
    memmove((char*)buf + off, &node->chds, sizeof(node->chds)); off += sizeof(node->chds);
    BPlusTreeUtils::db_write(fd, file_off,  buf, node_block_size, 1);
    cache_defer(buf);
}

void BPlusTree::treeFromFileBlock() {
    BPlusTreeUtils::db_read(fd, 0, caches_BPT, BPT_ROOT_SIZE, 1);

    char magic_number[8];
    sscanf((char*)caches_BPT, "%s", &magic_number);
    assert(strcmp(magic_number, MAGIC_NUMBER) == 0);
    size_t off = 8;
    memmove(&node_block_size, (char*)caches_BPT + off, sizeof(node_block_size)); off += sizeof(node_block_size);
    memmove(&data_block_size, (char*)caches_BPT + off, sizeof(data_block_size)); off += sizeof(data_block_size);
    memmove(&root, (char*)caches_BPT + off, sizeof(root)); off += sizeof(root);
    memmove(&level, (char*)caches_BPT + off, sizeof(level)); off += sizeof(level);
    memmove(&number, (char*)caches_BPT + off, sizeof(number)); off += sizeof(number);
    memmove(&leaf_head, (char*)caches_BPT + off, sizeof(leaf_head)); off += sizeof(leaf_head);
    memmove(&fdsz, (char*)caches_BPT + off, sizeof(fdsz)); off += sizeof(fdsz);
    memmove(&node_block_head, (char*)caches_BPT + off, sizeof(node_block_head)); off += sizeof(node_block_head);
    memmove(&data_block_head, (char*)caches_BPT + off, sizeof(data_block_head)); off += sizeof(data_block_head);
}

void BPlusTree::treeToFileBlock() {
    sprintf((char*)caches_BPT, "%s", MAGIC_NUMBER);
    size_t off = 8;
    memmove((char*)caches_BPT + off, &node_block_size, sizeof(node_block_size)); off += sizeof(node_block_size);
    memmove((char*)caches_BPT + off, &data_block_size, sizeof(data_block_size)); off += sizeof(data_block_size);
    memmove((char*)caches_BPT + off, &root, sizeof(root)); off += sizeof(root);
    memmove((char*)caches_BPT + off, &level, sizeof(level)); off += sizeof(level);
    memmove((char*)caches_BPT + off, &number, sizeof(number)); off += sizeof(number);
    memmove((char*)caches_BPT + off, &leaf_head, sizeof(leaf_head)); off += sizeof(leaf_head);
    memmove((char*)caches_BPT + off, &fdsz, sizeof(fdsz)); off += sizeof(fdsz);
    memmove((char*)caches_BPT + off, &node_block_head, sizeof(node_block_head)); off += sizeof(node_block_head);
    memmove((char*)caches_BPT + off, &data_block_head, sizeof(data_block_head)); off += sizeof(data_block_head);
    BPlusTreeUtils::db_write(fd, 0, caches_BPT, BPT_ROOT_SIZE, 1);
}


chd_t BPlusTree::get_node_block() {
    chd_t res;
    void* buf = cache_refer();
    if (node_block_head == INVALID_OFFSET) {
        res = fdsz;
        BPlusTreeUtils::db_write(fd, fdsz, buf, node_block_size, 1);
        fdsz += node_block_size;
    }
    else {
        res = node_block_head;
        BPlusTreeUtils::db_read(fd, node_block_head, buf, node_block_size, 1);
        memmove(&node_block_head, (char*)buf + sizeof(node_block_head), sizeof(node_block_head));
    }
    cache_defer(buf);
    return res;
}

chd_t BPlusTree::get_data_block() {
    chd_t res;
    void* buf = caches_data; used[2] = 1;
    if (data_block_head == INVALID_OFFSET) {
        res = fdsz;
        BPlusTreeUtils::db_write(fd, fdsz, buf, node_block_size, 1);
        fdsz += node_block_size;
    }
    else {
        res = data_block_head;
        BPlusTreeUtils::db_read(fd, data_block_head, buf, node_block_size, 1);
        memmove(&data_block_head, (char*)buf + sizeof(data_block_head), sizeof(data_block_head));
    }
    used[2] = 0;
    return res;
}

void BPlusTree::get_value_by_offset(chd_t file_off, void*& value, size_t& value_sz) {
    assert(value == NULL);
    assert(file_off < fdsz);

    void *buf = cache_refer();
    void *value_tmp;
    value = NULL;
    value_sz = 0;
    size_t sz = data_block_size - sizeof(chd_t) * 2;
    while(file_off != INVALID_OFFSET) {
        BPlusTreeUtils::db_read(fd, file_off, buf, data_block_size, 1);
        memmove(&file_off, (char*)buf + sizeof(chd_t), sizeof(chd_t));
        
        value_tmp = value;
        value = malloc(value_sz + sz);
        memmove(value, (char*)buf + sizeof(chd_t) * 2, sz);
        memmove((char*)value + value_sz, value_tmp, value_sz);
        value_sz += sz;
        free(value_tmp);
    }
    cache_defer(buf);
}

chd_t BPlusTree::set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz) {
    void* buf = cache_refer();
    while (file_off != INVALID_OFFSET) {
        BPlusTreeUtils::db_read(fd, file_off, buf, data_block_size, 1);
        memmove(&file_off, (char*)buf + sizeof(chd_t), sizeof(chd_t));
        memmove((char*)buf + sizeof(chd_t), &data_block_head, sizeof(chd_t));
        memmove(&data_block_head, buf, sizeof(chd_t));
    }

    size_t value_sz_tmp = 0;
    size_t sz = sizeof(chd_t);
    assert(value_sz % (data_block_size - sz * 2) == 0);

    while (value_sz_tmp < value_sz) {
        memmove((char*)buf + sz, &file_off, sz);
        file_off = get_data_block();
        memmove(buf, &file_off, sz);
        memmove((char*)buf + sz * 2, (char*)value + value_sz_tmp, data_block_size - sz * 2);
        BPlusTreeUtils::db_write(fd, file_off, buf, data_block_size, 1);
        value_sz_tmp += data_block_size - sz * 2;
    }
    cache_defer(buf);
    return file_off;
}

BPlusTreeNode* BPlusTree::new_node(chd_t _self, chd_t _parent, int _type) {
    BPlusTreeNode* node = (BPlusTreeNode*)cache_refer();

    node->self = _self;
    node->parent = _parent;
    node->type = _type;
    node->ch_cnt = 0;
    node->prev = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    return node;
}

BPlusTreeNode* BPlusTree::node_fetch(chd_t chdset) {
    if (chdset == INVALID_OFFSET) {
        return NULL;
    }

    BPlusTreeNode* node = (BPlusTreeNode*)cache_refer();
    nodeFromFileBlock(node, chdset);
    return node;
}

void BPlusTree::node_flush_file(BPlusTreeNode* node) {
    assert(node != NULL);
    assert(node->self != NULL);
    BPlusTreeUtils::db_write(fd, node->self, node, node_block_size, 1);
    cache_defer(node);
}

void BPlusTree::node_flush_parent(chd_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != NULL);
    child->parent = parent->self;
    node_flush_file(child);
}

int BPlusTree::search(key_t key, void *& value, size_t& value_sz) {
    BPlusTreeNode* node = node_fetch(root);
    if (node == NULL) {
        cache_defer(node);
        return BPLUSTRE_FAILED;
    }
    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        cache_defer(node);
        node = node_fetch(next_off);
    }
    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        get_value_by_offset(node->chds[pos], value, value_sz);
        cache_defer(node);
        return BPLUSTRE_SUCCESS;
    }
    cache_defer(node);
    return BPLUSTRE_FAILED;
}

key_t BPlusTree::node_split(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch) {
    assert(node->ch_cnt == BPT_MAX_ORDER);
    used[0] = 1;
    BPlusTreeNode* sibling = new_node(get_node_block(), node->parent, node->type);

    int split = (BPT_MAX_ORDER + 1) / 2;
    sibling->ch_cnt = split;
    node->ch_cnt = BPT_MAX_ORDER - split + 1;

    if (node->type == BPLUSTREE_LEAF) {
        if ((sibling->prev = node->prev) != INVALID_OFFSET) {
            BPlusTreeNode* prev = node_fetch(sibling->prev);
            prev->next = sibling->self;
            node_flush_file(prev);
        }
        else {
            leaf_head = sibling->self;
            treeToFileBlock();
        }
        node->prev = sibling->self;
        sibling->next - node->self;
    }
    size_t sz = sizeof(key_t), off = 0;
    memmove((char*)caches_node + off, &(node->keys[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &key, sz); off += sz;
    memmove((char*)caches_node + off, &(node->keys[pos]), (BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->keys[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->keys[0]), (char*)caches_node + off, node->ch_cnt * sz);

    sz = sizeof(chd_t); off = 0;
    memmove((char*)caches_node + off, &(node->chds[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &value_off, sz); off += sz;
    memmove((char*)caches_node + off, &(node->chds[pos]), (BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->chds[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->chds[0]), (char*)caches_node + off, node->ch_cnt * sz);

    if (sibling->type == BPLUSTREE_INTERNAL) {
        for (int i = 0; i < sibling->ch_cnt; i++)
            node_flush_parent(sibling->chds[i], sibling);
    }

    key_t res = sibling->keys[split - 1];
    lch = sibling;
    rch = node;
    used[0] = 0;
    return res;
}

void BPlusTree::node_insert(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off) {
    memmove(&(node->keys[pos + 1]), &(node->keys[pos]), (node->ch_cnt - pos) * sizeof(key_t));
    memmove(&(node->chds[pos + 1]), &(node->chds[pos]), (node->ch_cnt - pos) * sizeof(chd_t));
    node->keys[pos] = key;
    node->chds[pos] = value_off;
    node->ch_cnt++;
    if (node->type == BPLUSTREE_INTERNAL) node_flush_parent(value_off, node);
}

int BPlusTree::insert(key_t key, const void* value, const size_t value_sz) {
    BPlusTreeNode* node = node_fetch(root);
    if (node == NULL) {
        cache_defer(node);
        node = new_node(get_node_block(), INVALID_OFFSET, BPLUSTREE_LEAF);
        node->ch_cnt = 1;
        node->keys[0] = key;
        node->chds[0] = set_value_by_offest(INVALID_OFFSET, value, value_sz);
        
        root = node->self;
        level = 1;
        ++number;
        leaf_head = node->self;
        node_flush_file(node);
        return BPLUSTRE_SUCCESS;
    }

    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        cache_defer(node);
        node = node_fetch(next_off);
    }

    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        //ÒÑ¾­´æÔÚ
        set_value_by_offest(node->chds[pos], value, value_sz);
        node_flush_file(node);
        return BPLUSTRE_SUCCESS_REPLACE;
    }
    chd_t value_off = set_value_by_offest(INVALID_OFFSET, value, value_sz);
    BPlusTreeNode *lch, *rch;
    while (true) {
        if (node->ch_cnt < BPT_MAX_ORDER) {
            node_insert(node, pos, key, value_off);
            node_flush_file(node);
            return BPLUSTRE_SUCCESS;;
        }
        key = node_split(node, pos, key, value_off, lch, rch);
        if (lch->parent == INVALID_OFFSET && rch->parent == INVALID_OFFSET) {
            node = new_node(get_node_block(), INVALID_OFFSET, BPLUSTREE_INTERNAL);
            node->ch_cnt = 2;
            node->keys[0] = key;
            node->chds[0] = lch->self;
            node->chds[1] = rch->self;
            lch->parent = node->self;
            rch->parent = node->self;
            ++number;
            root = node->self;
            ++level;
            node_flush_file(lch);
            node_flush_file(rch);
            node_flush_file(node);
            return BPLUSTRE_SUCCESS;
        }
        value_off = lch->self;
        node = node_fetch(lch->parent);
        pos = node->binary_search_by_key(key);
        node_flush_file(lch);
        node_flush_file(rch);
    }
    assert(0);
    return BPLUSTRE_FAILED;
}

BPlusTree::BPlusTree(const wchar_t* filepath, int exist) {
    size_t sz = wcslen(filepath) + 1;
    fdpath = new wchar_t[sz];
    memset(fdpath, 0, sz);
    wcscat(fdpath, filepath);

    caches_BPT = malloc(BPT_ROOT_SIZE);
    
    if (exist) {
        assert((fd = _wfopen(fdpath, L"rb")) != NULL);
        treeFromFileBlock();
    }
    else {
        assert((fd = _wfopen(fdpath, L"wb+")) != NULL);
        node_block_size = 16384;
        data_block_size = 16384;
        assert(sizeof(BPlusTreeNode) <= node_block_size);
        root = INVALID_OFFSET;
        level = 0;
        number = 0;
        fdsz = BPT_ROOT_SIZE;
        leaf_head = INVALID_OFFSET;
        node_block_head = INVALID_OFFSET;
        data_block_head = INVALID_OFFSET;
        treeToFileBlock();
    }

    caches_node = malloc(node_block_size * BPT_CACHE_NUM);
    caches_data = malloc(node_block_size);
    memset(used, 0, sizeof(used));
}

BPlusTree::~BPlusTree() {
    treeToFileBlock();
    free(caches_BPT);
    free(caches_node);
    free(caches_data);
    fclose(fd);
    delete[] fdpath;
}

int BPlusTree::insert(key_t key, const wchar_t* value) {
    size_t sz = data_block_size - sizeof(chd_t) * 2;
    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + sz - 1) / sz * sz;
    void* _value = malloc(_value_sz);
    memset(_value, 0, _value_sz);
    memcpy(_value, value, value_sz);
    insert(key, _value, _value_sz);
    free(_value);
    return 0;
}

int BPlusTree::search(key_t key, wchar_t*& value) {
    assert(value == NULL);
    void* _value = NULL;
    size_t _value_sz;
    if (search(key, _value, _value_sz) == BPLUSTRE_FAILED) return BPLUSTRE_FAILED;
    size_t value_sz = _value_sz / sizeof(wchar_t);
    value = new wchar_t[value_sz];
    memset(value, 0, _value_sz);
    wcscat_s(value, value_sz, (wchar_t*)_value);
    return BPLUSTRE_SUCCESS;
}

chd_t BPlusTree::begin() {
    return leaf_head;
}
chd_t BPlusTree::end() {
    return INVALID_OFFSET;
}
chd_t BPlusTree::next(chd_t node_off) {
    BPlusTreeNode* node = node_fetch(node_off);
    chd_t res = node->next;
    cache_defer(node);
    return res;
}