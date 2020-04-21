#include "pch.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

int BPlusTreeNode::binary_search_by_key(key_t target) {
    int len = type == BPLUSTREE_LEAF ? ch_cnt : ch_cnt - 1;
    int pos = std::lower_bound(keys, keys+len, target) - keys;
    return pos;
}

void BPlusTree::cache_buf_lock() {
    assert(caches_buf_lock == BPT_CACHE_BUF_UNUSED);
    caches_buf_lock = BPT_CACHE_BUF_USED;
}

void BPlusTree::cache_buf_unlock() {
    caches_buf_lock = BPT_CACHE_BUF_UNUSED;
}

void BPlusTree::nodeFromFileBlock(BPlusTreeNode* node, chd_t file_off) {
    cache_buf_lock();
    void* buf = caches_buf;
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
    cache_buf_unlock();
}

void BPlusTree::nodeToFileBlock(BPlusTreeNode* node, chd_t file_off) {
    cache_buf_lock();
    void* buf = caches_node;
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
    cache_buf_unlock();
}

void BPlusTree::treeFromFileBlock() {
    BPlusTreeUtils::db_read(fd, 0, caches_BPT, BPT_ROOT_SIZE, 1);
    char magic_number[8];
    assert(sscanf((char*)caches_BPT, "%s", &magic_number) == 1);
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
    cache_buf_lock();
    void* buf = caches_buf;
    chd_t res;
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
    cache_buf_unlock();
    return res;
}

chd_t BPlusTree::get_data_block() {
    cache_buf_lock();
    void* buf = caches_buf;
    chd_t res;
    if (data_block_head == INVALID_OFFSET) {
        res = fdsz;
        BPlusTreeUtils::db_write(fd, fdsz, buf, data_block_size, 1);
        fdsz += data_block_size;
    }
    else {
        res = data_block_head;
        BPlusTreeUtils::db_read(fd, data_block_head, buf, data_block_size, 1);
        memmove(&data_block_head, (char*)buf + sizeof(data_block_head), sizeof(data_block_head));
    }
    cache_buf_unlock();
    return res;
}

void BPlusTree::get_value_by_offset(chd_t file_off, void*& value, size_t& value_sz) {
    assert(value == nullptr);
    assert(file_off < fdsz);
    cache_buf_lock();
    void *buf = caches_buf;
    void *value_tmp;
    value = nullptr;
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
    cache_buf_unlock();
}

chd_t BPlusTree::set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz) {
    cache_buf_lock(); 
    void* buf = caches_buf;

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
    cache_buf_unlock();
    return file_off;
}

BPlusTreeNode* BPlusTree::cache_refer() {
    assert(!unused.empty());
    BPlusTreeNode* node = unused.begin()->second;
    if (used_id.count(node)) {
        used.erase(node->self);
        nodeToFileBlock(node, node->self);
    }
    unused.erase(unused.begin());
    used_id[node] = ++caches_used_cnt;
    return node;
}

BPlusTreeNode* BPlusTree::new_node(chd_t _self, chd_t _parent, int _type) {
    assert(_self != INVALID_OFFSET);
    BPlusTreeNode* node = cache_refer();
    node->self = _self;
    node->parent = _parent;
    node->type = _type;
    node->ch_cnt = 0;
    node->prev = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    used[node->self] = node;
    return node;
}

BPlusTreeNode* BPlusTree::node_fetch(chd_t chdset) {
    if (chdset == INVALID_OFFSET) {
        return nullptr;
    }
    if (used.count(chdset)) {
        BPlusTreeNode* node = used[chdset];
        size_t _id = used_id[node];
        assert(!unused[_id]);
        //if (unused.count(_id)) unused.erase(_id);
        used_id[node] = ++caches_used_cnt;
        return (BPlusTreeNode*)used[chdset];
    }
    BPlusTreeNode* node = cache_refer();
    nodeFromFileBlock(node, chdset);
    used[chdset] = node; 
    return node;
}

void BPlusTree::node_detch(BPlusTreeNode* node) {
    assert(node != nullptr);
    assert(node->self != INVALID_OFFSET);
    assert(used_id.count(node));
    unused.erase(used_id[node]);
}

void BPlusTree::node_flush_parent(chd_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != nullptr);
    child->parent = parent->self;
    node_detch(child);
}

int BPlusTree::search(key_t key, void *& value, size_t& value_sz) {
    BPlusTreeNode* node = node_fetch(root);
    if (node == nullptr) {
        return BPLUSTRE_FAILED;
    }
    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        node_detch(node);
        node = node_fetch(next_off);
    }
    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        get_value_by_offset(node->chds[pos], value, value_sz);
        node_detch(node);
        return BPLUSTRE_SUCCESS;
    }
    node_detch(node);
    return BPLUSTRE_FAILED;
}

key_t BPlusTree::node_split(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch) {
    assert(node->ch_cnt == BPT_MAX_ORDER);
    BPlusTreeNode* sibling = new_node(get_node_block(), node->parent, node->type);

    int split = (BPT_MAX_ORDER + 1) / 2;
    sibling->ch_cnt = split;
    node->ch_cnt = BPT_MAX_ORDER - split + 1;

    if ((sibling->prev = node->prev) != INVALID_OFFSET) {
        BPlusTreeNode* prev = node_fetch(sibling->prev);
        prev->next = sibling->self;
        node_detch(prev);
    }
    else {
        leaf_head = sibling->self;
    }
    node->prev = sibling->self;
    sibling->next - node->self;

    cache_buf_lock();
    size_t sz = sizeof(key_t), off = 0;
    memmove((char*)caches_node + off, &(node->keys[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &key, sz); off += sz;
    memmove((char*)caches_node + off, &(node->keys[pos]), ((size_t)BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->keys[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->keys[0]), (char*)caches_node + off, node->ch_cnt * sz);

    sz = sizeof(chd_t); off = 0;
    memmove((char*)caches_node + off, &(node->chds[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &value_off, sz); off += sz;
    memmove((char*)caches_node + off, &(node->chds[pos]), ((size_t)BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->chds[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->chds[0]), (char*)caches_node + off, node->ch_cnt * sz);
    cache_buf_unlock();
    
    if (sibling->type == BPLUSTREE_INTERNAL) {
        for (int i = 0; i < sibling->ch_cnt; i++)
            node_flush_parent(sibling->chds[i], sibling);
    }

    key_t res = sibling->keys[split - 1];
    lch = sibling;
    rch = node;
    used[2] = 0;
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
    if (node == nullptr) {
        node = new_node(get_node_block(), INVALID_OFFSET, BPLUSTREE_LEAF);
        node->ch_cnt = 1;
        node->keys[0] = key;
        node->chds[0] = set_value_by_offest(INVALID_OFFSET, value, value_sz);
        
        root = node->self;
        level = 1;
        ++number;
        leaf_head = node->self;
        node_detch(node);
        return BPLUSTRE_SUCCESS;
    }

    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        node_detch(node);
        node = node_fetch(next_off);
    }

    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        //�Ѿ�����
        set_value_by_offest(node->chds[pos], value, value_sz);
        node_detch(node);
        return BPLUSTRE_SUCCESS_REPLACE;
    }
    chd_t value_off = set_value_by_offest(INVALID_OFFSET, value, value_sz);
    BPlusTreeNode *lch, *rch;
    while (true) {
        if (node->ch_cnt < BPT_MAX_ORDER) {
            node_insert(node, pos, key, value_off);
            node_detch(node);
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
            node_detch(lch);
            node_detch(rch);
            node_detch(node);
            return BPLUSTRE_SUCCESS;
        }
        value_off = lch->self;
        node = node_fetch(lch->parent);
        pos = node->binary_search_by_key(key);
        node_detch(lch);
        node_detch(rch);
    }
    assert(0);
    return BPLUSTRE_FAILED;
}

BPlusTree::BPlusTree(const wchar_t* filepath, int exist) 
    : fdpath(nullptr)
{
    caches_BPT = malloc(BPT_ROOT_SIZE);

    size_t sz = wcslen(filepath) + 1;
    fdpath = new wchar_t[sz];
    memset(fdpath, 0, sz);
    wcscat(fdpath, filepath);
    
    if (exist) {
        assert((fd = _wfopen(fdpath, L"rb")) != nullptr);
        treeFromFileBlock();
    }
    else {
        assert((fd = _wfopen(fdpath, L"wb+")) != nullptr);
        node_block_size = BPT_NODE_BLOCK_SIZE;
        data_block_size = BPT_DATA_BLOCK_SIZE;
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
    size_t caches_size = max(node_block_size, data_block_size) * 3;
    caches_buf = malloc(caches_size);
    caches_buf_lock = BPT_CACHE_BUF_UNUSED;
    caches_size = (size_t)node_block_size * BPT_CACHE_NUM;
    caches_node = malloc(caches_size);
    caches_used_cnt = 0;
    used = std::unordered_map<chd_t, BPlusTreeNode*>();
    used_id = std::unordered_map<BPlusTreeNode*, size_t>();
    unused = std::map<size_t, BPlusTreeNode*> ();
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        ++caches_used_cnt;
        unused[caches_used_cnt] = (BPlusTreeNode*)caches_node + (size_t)node_block_size * i;
    }
}

BPlusTree::~BPlusTree() {
    treeToFileBlock();
    free(caches_BPT);
    delete[] fdpath;
    fclose(fd);
    free(caches_buf);
    free(caches_node);
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
    node_detch(node);
    return res;
}
/*
#pragma once
#include "pch.h"
#include "BPlusTreeUtils.h"
/* @by ChrisJaunes
 * ������B+���ڵ���Ϣ��ע��
 * self: �����ļ�ָ��
 * parent: ���ڵ��ļ�ָ��
 * type: �ڵ����Ͱ����ڲ��ڵ�(BPLUSTREE_INTERNAL)��Ҷ�ӽڵ�(BPLUSTREE_LEAF)
 * ch_cnt: ���ӵĸ���,����B+������:�ڲ��ڵ���ch_cnt-1��key��cn_cnt������ָ��; Ҷ�ӽڵ���ch_cnt��key��cn_cnt��������ָ��
 * prev: ͬ����һ���ڵ���ļ�ָ��,����
 * next: ͬ����һ���ڵ���ļ�ָ��,����
 * keys[]��key����
 * chds[]: ����������ļ�ָ��
 * isleaf(): �ж��Ƿ���Ҷ�ӽڵ�
 * binary_search_by_key()�� ���ڴ���key�ϵĶ���
 * fromFileBlock(): ���ڴ��ļ����ж�ȡ���ݲ��ҽ�����ȡ����������һ��BPlusTreeNode�ڵ�
 * toFileBlock(): �������ļ�����д�����ݲ��ҽ�������Ľڵ���Ϣд������
 *
 * ������B+����Ϣ��ע��
 * index_fd: �ڵ�洢�ļ����ļ�ָ��
 * index_fdpath: �ڵ�洢�ļ����ļ�·��
 * index_fdsz: �ڵ�洢�ļ����ļ���С
 * data_fd: ���ݴ洢�ļ����ļ�ָ��
 * data_fdpath: ���ݴ洢�ļ����ļ�·��
 * data_fdsz: ���ݴ洢�ļ����ļ���С
 * root: �������ļ�ָ��
 * level: ����
 * leaf_head: Ҷ�ӽڵ������ļ�ָ��
 * �ֶ��ڴ����
 * BPT_BLOCK_SIZE��BPlusTreeNode���С
 * caches: �ڴ����
 * used[]: �ڴ����
 * cache_refer: ��ȡһ���ڴ�ռ�
 * cache_refer: �ͷ�һ���ڴ�ռ�
 */



template<typename KEY_T, typename VALUE_T, typename OFF_T = size_t, typename CMP = less<KEY_T> >
class BPlusTree {
public:
    explicit BPlusTree(const wchar_t* filename, int exist = BPLUSTREE_FILE_NON_EXIST);
    ~BPlusTree();
    int insert(const KEY_T& key, const VALUE_T& value);
    int search(const KEY_T& key, VALUE_T*& value);
    //chd_t begin();
    //chd_t end();
    //chd_t next(chd_t node_off);

//private:
    struct BPlusTreeNode {
        OFF_T self;
        OFF_T parent;
        unsigned int type;
        unsigned int ch_cnt;
        OFF_T prev;
        OFF_T next;
        KEY_T keys[BPT_MAX_ORDER];
        OFF_T chds[BPT_MAX_ORDER];
        int binary_search_by_key(KEY_T);
    };
    struct BPlusTreeCaches {
        void* caches;
        void cache_lock();
        void cache_unlock();
        void* cache_refer();
        void cache_defer();
    };
    wchar_t* fdpath;
    FILE* fd;

    unsigned int node_block_size;
    unsigned int data_block_size;
    chd_t root;
    unsigned int level;
    unsigned int number;
    chd_t fdsz;
    chd_t leaf_head;
    chd_t node_block_head;
    chd_t data_block_head;

    void* caches_buf;
    size_t caches_buf_lock;
    void* caches_node;
    size_t caches_used_cnt;
    std::unordered_map<chd_t, BPlusTreeNode*> used;
    std::unordered_map<BPlusTreeNode*, size_t> used_id;
    std::map<size_t, BPlusTreeNode*> unused;

    void nodeFromFileBlock(BPlusTreeNode* node, chd_t file_off);
    void nodeToFileBlock(BPlusTreeNode* node, chd_t file_off);
    void treeFromFileBlock();
    void treeToFileBlock();
    chd_t get_node_block();
    chd_t get_data_block();
    void get_value_by_offset(chd_t file_off, void*& value, size_t&);
    chd_t set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz);

    BPlusTreeNode* new_node(chd_t _self, chd_t _parent, int _type);
    BPlusTreeNode* node_fetch(chd_t chdset);
    void node_detch(BPlusTreeNode* node);
    void node_flush_parent(chd_t node, BPlusTreeNode* parent);


    key_t node_split(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch);
    void node_insert(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off);
};


/* @by ChrisJaunes
 * ������B+���ļ���Ϣ
 *  ��ʼ�� 64 byte
 * | ƫ�� | ��С |����
 * |---   |---  |---
 * |0	  | 8	 | ħ��, -DS,V1-�� ������Ϣ
 * |8	  | 4	 | sizeof(unsigned int)��ÿ���ڵ��Ԥ������С����Ӧnode_block_size
 * |12	  | 4	 | sizeof(unsigned sint)��ÿ�����ݿ�Ԥ������С����Ӧdata_block_size
 * |16	  | 8	 | sizoef(size_t), ���ڵ���ļ�ƫ��ֵ, ��Ӧroot
 * |24    | 4    | sizoef(unsigned int)��B��������, ��Ӧlevel
 * |28	  | 4	 | sizeof(unsigned int)��B���Ľڵ���, ��Ӧnumber
 * |32	  | 8    | sizeof(size_t), B����Ҷ��������ʼָ��, ��Ӧleaf_head
 * |40	  | 8	 | sizoef(size_t), �ļ�β��λ��, ��Ӧfdsz
 * |48	  | 8	 | sizeof(size_t), �ļ��ڵ�տ�����λ��, node_block_head
 * |56	  | 8	 | sizeof(size_t), �ļ����ݿտ�����λ��, data_block_head
 *
 * �ڵ�� �����ڲ��ڵ��Ҷ�ӽڵ�
 * | ƫ�� | ��С |����
 * |---   |---  |---
 * |0	  | 8	| sizeof(size_t) �����ļ�ƫ��,���ڼ���,��Ӧself
 * |8	  | 8	| sizeof(size_t) ���ڵ��ļ�ƫ��,���ڼ���,��Ӧparent
 * |16	  | 4	| sizeof(int) ����, �ڲ��ڵ����Ҷ�ӽڵ�, ��Ӧtype
 * |20	  | 4	| sizeof(int) �ýڵ㺢����, ��Ӧch_cnt
 * |24	  | 8	| sizeof(size_t) ͬ������һ���ڵ���ļ�ƫ��, ��Ӧprev
 * |32	  | 8	| sizoef(size_t) ͬ������һ���ڵ���ļ�ƫ��, ��Ӧnext
 * |40	  | ��	| sizoef(key_t) * BPT_MAX_ORDER, ��Ӧkeys����
 * |��    | ��  | sizoef(chd_t) * BPT_MAX_ORDER, ��Ӧchds����
 *
 * ���ݿ�
 * | ƫ�� | ��С |����
 * |---   |---  |---
 * |0	  | 8	| sizeof(size_t) �����ļ�ƫ��,���ڼ���
 * |8	  | 8	| sizeof(size_t) ��һ�����ݿ�ڵ�, ��������һ������
 * |16	  | ��	| ��������, �������ļ�
 */
/*
#include "pch.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

int BPlusTreeNode::binary_search_by_key(key_t target) {
    int len = type == BPLUSTREE_LEAF ? ch_cnt : ch_cnt - 1;
    int pos = std::lower_bound(keys, keys + len, target) - keys;
    return pos;
}

void BPlusTree::cache_buf_lock() {
    assert(caches_buf_lock == BPT_CACHE_BUF_UNUSED);
    caches_buf_lock = BPT_CACHE_BUF_USED;
}

void BPlusTree::cache_buf_unlock() {
    caches_buf_lock = BPT_CACHE_BUF_UNUSED;
}

void BPlusTree::nodeFromFileBlock(BPlusTreeNode* node, chd_t file_off) {
    cache_buf_lock();
    void* buf = caches_buf;
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
    cache_buf_unlock();
}

void BPlusTree::nodeToFileBlock(BPlusTreeNode* node, chd_t file_off) {
    cache_buf_lock();
    void* buf = caches_node;
    size_t off = 0;
    memmove((char*)buf + off, &node->self, sizeof(node->self)); off += sizeof(node->self);
    memmove((char*)buf + off, &node->parent, sizeof(node->parent)); off += sizeof(node->parent);
    memmove((char*)buf + off, &node->type, sizeof(node->type)); off += sizeof(node->type);
    memmove((char*)buf + off, &node->ch_cnt, sizeof(node->ch_cnt)); off += sizeof(node->ch_cnt);
    memmove((char*)buf + off, &node->prev, sizeof(node->prev)); off += sizeof(node->prev);
    memmove((char*)buf + off, &node->next, sizeof(node->next)); off += sizeof(node->next);
    memmove((char*)buf + off, &node->keys, sizeof(node->keys)); off += sizeof(node->keys);
    memmove((char*)buf + off, &node->chds, sizeof(node->chds)); off += sizeof(node->chds);
    BPlusTreeUtils::db_write(fd, file_off, buf, node_block_size, 1);
    cache_buf_unlock();
}

void BPlusTree::treeFromFileBlock() {
    BPlusTreeUtils::db_read(fd, 0, caches_BPT, BPT_ROOT_SIZE, 1);
    char magic_number[8];
    assert(sscanf((char*)caches_BPT, "%s", &magic_number) == 1);
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
    cache_buf_lock();
    void* buf = caches_buf;
    chd_t res;
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
    cache_buf_unlock();
    return res;
}

chd_t BPlusTree::get_data_block() {
    cache_buf_lock();
    void* buf = caches_buf;
    chd_t res;
    if (data_block_head == INVALID_OFFSET) {
        res = fdsz;
        BPlusTreeUtils::db_write(fd, fdsz, buf, data_block_size, 1);
        fdsz += data_block_size;
    }
    else {
        res = data_block_head;
        BPlusTreeUtils::db_read(fd, data_block_head, buf, data_block_size, 1);
        memmove(&data_block_head, (char*)buf + sizeof(data_block_head), sizeof(data_block_head));
    }
    cache_buf_unlock();
    return res;
}

void BPlusTree::get_value_by_offset(chd_t file_off, void*& value, size_t& value_sz) {
    assert(value == nullptr);
    assert(file_off < fdsz);
    cache_buf_lock();
    void* buf = caches_buf;
    void* value_tmp;
    value = nullptr;
    value_sz = 0;
    size_t sz = data_block_size - sizeof(chd_t) * 2;
    while (file_off != INVALID_OFFSET) {
        BPlusTreeUtils::db_read(fd, file_off, buf, data_block_size, 1);
        memmove(&file_off, (char*)buf + sizeof(chd_t), sizeof(chd_t));

        value_tmp = value;
        value = malloc(value_sz + sz);
        memmove(value, (char*)buf + sizeof(chd_t) * 2, sz);
        memmove((char*)value + value_sz, value_tmp, value_sz);
        value_sz += sz;
        free(value_tmp);
    }
    cache_buf_unlock();
}

chd_t BPlusTree::set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz) {
    cache_buf_lock();
    void* buf = caches_buf;

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
    cache_buf_unlock();
    return file_off;
}

BPlusTreeNode* BPlusTree::cache_refer() {
    assert(!unused.empty());
    BPlusTreeNode* node = unused.begin()->second;
    if (used_id.count(node)) {
        used.erase(node->self);
        nodeToFileBlock(node, node->self);
    }
    unused.erase(unused.begin());
    used_id[node] = ++caches_used_cnt;
    return node;
}

BPlusTreeNode* BPlusTree::new_node(chd_t _self, chd_t _parent, int _type) {
    assert(_self != INVALID_OFFSET);
    BPlusTreeNode* node = cache_refer();
    node->self = _self;
    node->parent = _parent;
    node->type = _type;
    node->ch_cnt = 0;
    node->prev = INVALID_OFFSET;
    node->next = INVALID_OFFSET;
    used[node->self] = node;
    return node;
}

BPlusTreeNode* BPlusTree::node_fetch(chd_t chdset) {
    if (chdset == INVALID_OFFSET) {
        return nullptr;
    }
    if (used.count(chdset)) {
        BPlusTreeNode* node = used[chdset];
        size_t _id = used_id[node];
        assert(!unused[_id]);
        //if (unused.count(_id)) unused.erase(_id);
        used_id[node] = ++caches_used_cnt;
        return (BPlusTreeNode*)used[chdset];
    }
    BPlusTreeNode* node = cache_refer();
    nodeFromFileBlock(node, chdset);
    used[chdset] = node;
    return node;
}

void BPlusTree::node_detch(BPlusTreeNode* node) {
    assert(node != nullptr);
    assert(node->self != INVALID_OFFSET);
    assert(used_id.count(node));
    unused.erase(used_id[node]);
}

void BPlusTree::node_flush_parent(chd_t node, BPlusTreeNode* parent) {
    BPlusTreeNode* child = node_fetch(node);
    assert(child != nullptr);
    child->parent = parent->self;
    node_detch(child);
}

int BPlusTree::search(key_t key, void*& value, size_t& value_sz) {
    BPlusTreeNode* node = node_fetch(root);
    if (node == nullptr) {
        return BPLUSTRE_FAILED;
    }
    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        node_detch(node);
        node = node_fetch(next_off);
    }
    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        get_value_by_offset(node->chds[pos], value, value_sz);
        node_detch(node);
        return BPLUSTRE_SUCCESS;
    }
    node_detch(node);
    return BPLUSTRE_FAILED;
}

key_t BPlusTree::node_split(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch) {
    assert(node->ch_cnt == BPT_MAX_ORDER);
    BPlusTreeNode* sibling = new_node(get_node_block(), node->parent, node->type);

    int split = (BPT_MAX_ORDER + 1) / 2;
    sibling->ch_cnt = split;
    node->ch_cnt = BPT_MAX_ORDER - split + 1;

    if ((sibling->prev = node->prev) != INVALID_OFFSET) {
        BPlusTreeNode* prev = node_fetch(sibling->prev);
        prev->next = sibling->self;
        node_detch(prev);
    }
    else {
        leaf_head = sibling->self;
    }
    node->prev = sibling->self;
    sibling->next - node->self;

    cache_buf_lock();
    size_t sz = sizeof(key_t), off = 0;
    memmove((char*)caches_node + off, &(node->keys[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &key, sz); off += sz;
    memmove((char*)caches_node + off, &(node->keys[pos]), ((size_t)BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->keys[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->keys[0]), (char*)caches_node + off, node->ch_cnt * sz);

    sz = sizeof(chd_t); off = 0;
    memmove((char*)caches_node + off, &(node->chds[0]), pos * sz); off += pos * sz;
    memmove((char*)caches_node + off, &value_off, sz); off += sz;
    memmove((char*)caches_node + off, &(node->chds[pos]), ((size_t)BPT_MAX_ORDER - pos) * sz);
    off = 0;
    memmove(&(sibling->chds[0]), (char*)caches_node + off, sibling->ch_cnt * sz); off += sibling->ch_cnt * sz;
    memmove(&(node->chds[0]), (char*)caches_node + off, node->ch_cnt * sz);
    cache_buf_unlock();

    if (sibling->type == BPLUSTREE_INTERNAL) {
        for (int i = 0; i < sibling->ch_cnt; i++)
            node_flush_parent(sibling->chds[i], sibling);
    }

    key_t res = sibling->keys[split - 1];
    lch = sibling;
    rch = node;
    used[2] = 0;
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
    if (node == nullptr) {
        node = new_node(get_node_block(), INVALID_OFFSET, BPLUSTREE_LEAF);
        node->ch_cnt = 1;
        node->keys[0] = key;
        node->chds[0] = set_value_by_offest(INVALID_OFFSET, value, value_sz);

        root = node->self;
        level = 1;
        ++number;
        leaf_head = node->self;
        node_detch(node);
        return BPLUSTRE_SUCCESS;
    }

    int pos;
    while (node->type == BPLUSTREE_INTERNAL) {
        pos = node->binary_search_by_key(key);
        chd_t next_off = node->chds[pos];
        node_detch(node);
        node = node_fetch(next_off);
    }

    pos = node->binary_search_by_key(key);
    if (pos < node->ch_cnt && node->keys[pos] == key) {
        //�Ѿ�����
        set_value_by_offest(node->chds[pos], value, value_sz);
        node_detch(node);
        return BPLUSTRE_SUCCESS_REPLACE;
    }
    chd_t value_off = set_value_by_offest(INVALID_OFFSET, value, value_sz);
    BPlusTreeNode* lch, * rch;
    while (true) {
        if (node->ch_cnt < BPT_MAX_ORDER) {
            node_insert(node, pos, key, value_off);
            node_detch(node);
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
            node_detch(lch);
            node_detch(rch);
            node_detch(node);
            return BPLUSTRE_SUCCESS;
        }
        value_off = lch->self;
        node = node_fetch(lch->parent);
        pos = node->binary_search_by_key(key);
        node_detch(lch);
        node_detch(rch);
    }
    assert(0);
    return BPLUSTRE_FAILED;
}

BPlusTree::BPlusTree(const wchar_t* filepath, int exist)
    : fdpath(nullptr)
{
    caches_BPT = malloc(BPT_ROOT_SIZE);

    size_t sz = wcslen(filepath) + 1;
    fdpath = new wchar_t[sz];
    memset(fdpath, 0, sz);
    wcscat(fdpath, filepath);

    if (exist) {
        assert((fd = _wfopen(fdpath, L"rb")) != nullptr);
        treeFromFileBlock();
    }
    else {
        assert((fd = _wfopen(fdpath, L"wb+")) != nullptr);
        node_block_size = BPT_NODE_BLOCK_SIZE;
        data_block_size = BPT_DATA_BLOCK_SIZE;
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
    size_t caches_size = max(node_block_size, data_block_size) * 3;
    caches_buf = malloc(caches_size);
    caches_buf_lock = BPT_CACHE_BUF_UNUSED;
    caches_size = (size_t)node_block_size * BPT_CACHE_NUM;
    caches_node = malloc(caches_size);
    caches_used_cnt = 0;
    used = std::unordered_map<chd_t, BPlusTreeNode*>();
    used_id = std::unordered_map<BPlusTreeNode*, size_t>();
    unused = std::map<size_t, BPlusTreeNode*>();
    for (int i = 0; i < BPT_CACHE_NUM; i++) {
        ++caches_used_cnt;
        unused[caches_used_cnt] = (BPlusTreeNode*)caches_node + (size_t)node_block_size * i;
    }
}

BPlusTree::~BPlusTree() {
    treeToFileBlock();
    free(caches_BPT);
    delete[] fdpath;
    fclose(fd);
    free(caches_buf);
    free(caches_node);
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
    node_detch(node);
    return res;
}*/