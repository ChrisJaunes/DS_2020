#pragma once
struct key_t {
    wchar_t a[255];
    bool operator != (const key_t& x) const {
        return wcscmp(&a[0], &(x.a[0])) != 0;
    }
    bool operator < (const key_t& x) const {
        return wcscmp(&a[0], &(x.a[0])) < 0;
    }
    bool operator == (const key_t& x) const {
        return wcscmp(&a[0], &(x.a[0])) == 0;
    }
};

#define value_t wchar_t
//B+���Ļ�������С����д�ڴ����

const int BPT_CACHE_NUM = 100;
const int BPT_MAX_CHILDRENS = 10;
const int BPT_MAX_ENTRIES = 10;
const int BPT_MAX_ORDER = 10;

/* by ChrisJaunes
 * ������B+���ڵ���Ϣ��ע��
 * type: �ڵ����Ͱ����ڲ��ڵ�(BPLUSTREE_INTERNAL)��Ҷ�ӽڵ�(BPLUSTREE_LEAF)
 * self: �����ļ�ָ��
 * parent: ���ڵ��ļ�ָ��
 * prev: ͬ����һ���ڵ���ļ�ָ��,����
 * next: ͬ����һ���ڵ���ļ�ָ��,����
 * ch_cnt: ���ӵĸ���,����B+������:�ڲ��ڵ���ch_cnt-1��key��cn_cnt������ָ��; Ҷ�ӽڵ���ch_cnt��key��cn_cnt��������ָ��
 * keys[]��key����
 * offs[]: off����
 * isleaf(): �ж��Ƿ���Ҷ�ӽڵ�
 * binary_search_by_key()�� ���ڴ���key�ϵĶ���
 * fromFileBlock(): ���ڴ��ļ����ж�ȡ���ݲ��ҽ�����ȡ����������һ��BPlusTreeNode�ڵ�
 * toFileBlock(): �������ļ�����д�����ݲ��ҽ�������Ľڵ���Ϣд������
 */

struct BPlusTreeNode {
    int type;
    off_t self;
    off_t parent;
    off_t prev;
    off_t next;
    int ch_cnt;
    key_t keys[BPT_MAX_ENTRIES];
    off_t offs[BPT_MAX_ENTRIES];
    bool is_leaf() const;
    int binary_search_by_key(key_t);

    int fromFileBlock(FILE* file, off_t file_off, size_t _BLOCK_SIZE);
    int toFileBlock(FILE* file, off_t file_off, size_t _BLOCK_SIZE);
};

/*
 * by ChrisJaunes
 * ������B+����Ϣ��ע��
 * root: �������ļ�ָ��
 * level: ����
 * �ֶ��ڴ����
 * BPT_BLOCK_SIZE��BPlusTreeNode���С
 * caches: �ڴ����
 * used[]: �ڴ����
 * cache_refer: ��ȡһ���ڴ�ռ�
 * cache_refer: �ͷ�һ���ڴ�ռ�
 */
struct BPlusTree {
    off_t root;
    int level;

    FILE* index_fd;
    off_t index_fdsize;
    FILE* data_fd;
    off_t data_fdsize;

    const int BPT_BLOCK_SIZE = sizeof(BPlusTreeNode);
    char* caches;
    int used[BPT_CACHE_NUM];
    BPlusTreeNode* cache_refer();
    void cache_defer(BPlusTreeNode* node);
    BPlusTreeNode* new_node();
    BPlusTreeNode* new_leaf_node();
    BPlusTreeNode* new_internal_node();
    BPlusTreeNode* node_fetch(off_t offset);

    
    off_t search_by_key(key_t key);
    
    //�������
    off_t node_append(BPlusTreeNode* node);
    void node_flush_file(BPlusTreeNode* node);
    void node_flush_parent(off_t node, BPlusTreeNode* parent);

    void node_add_left(BPlusTreeNode* node, BPlusTreeNode* left);
    void node_add_right(BPlusTreeNode* node, BPlusTreeNode* right);
    int node_add_parent(BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);

    key_t split_internal_L(BPlusTreeNode* node, BPlusTreeNode* left, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_M(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_R(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    void insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);
    int insert_internal(BPlusTreeNode* node, BPlusTreeNode* l_ch, BPlusTreeNode* r_ch, key_t key);

    key_t split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, off_t data);
    key_t split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert, key_t key, off_t dataoff);
    void insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, off_t value_off);
    int insert_leaf(BPlusTreeNode* leaf, key_t key, value_t* data);

    off_t insert_leaf_value(off_t, value_t*);
    int insert(key_t key, value_t* data);
    
};
