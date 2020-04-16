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
typedef size_t chd_t;
//B+������
const chd_t INVALID_OFFSET = 0xffffffff;
enum {
    BPLUSTREE_INTERNAL,
    BPLUSTREE_LEAF = 1,
};
enum {
    BPLUSTREE_FILE_EXIST,
    BPLUSTREE_FILE_NON_EXIST,
};
//B+���Ļ�������С����д�ڴ����
const int BPT_CACHE_NUM = 10;
const int BPT_MAX_ENTRIES = 10;
const int BPT_MAX_ORDER = 10;
const int BPT_ROOT_SIZE = 48;
const int BPT_BLOCK_SIZE = 4096;


/* by ChrisJaunes
 * ������B+���ڵ���Ϣ��ע��
 * type: �ڵ����Ͱ����ڲ��ڵ�(BPLUSTREE_INTERNAL)��Ҷ�ӽڵ�(BPLUSTREE_LEAF)
 * self: �����ļ�ָ��
 * parent: ���ڵ��ļ�ָ��
 * prev: ͬ����һ���ڵ���ļ�ָ��,����
 * next: ͬ����һ���ڵ���ļ�ָ��,����
 * ch_cnt: ���ӵĸ���,����B+������:�ڲ��ڵ���ch_cnt-1��key��cn_cnt������ָ��; Ҷ�ӽڵ���ch_cnt��key��cn_cnt��������ָ��
 * keys[]��key����
 * chds[]: ����������ļ�ָ��
 * isleaf(): �ж��Ƿ���Ҷ�ӽڵ�
 * binary_search_by_key()�� ���ڴ���key�ϵĶ���
 * fromFileBlock(): ���ڴ��ļ����ж�ȡ���ݲ��ҽ�����ȡ����������һ��BPlusTreeNode�ڵ�
 * toFileBlock(): �������ļ�����д�����ݲ��ҽ�������Ľڵ���Ϣд������
 */

struct BPlusTreeNode {
    int type;
    chd_t self;
    chd_t parent;
    chd_t prev;
    chd_t next;
    int ch_cnt;
    key_t keys[BPT_MAX_ENTRIES];
    chd_t chds[BPT_MAX_ENTRIES];
    bool is_leaf() const;
    int binary_search_by_key(key_t);

    int fromFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE);
    int toFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE);
};

/*
 * by ChrisJaunes
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

struct BPlusTree {
    FILE* index_fd;
    wchar_t* index_fdpath;
    chd_t index_fdsz;
    FILE* data_fd;
    wchar_t* data_fdpath;
    chd_t data_fdsz;
    
    int level;
    chd_t root;
    chd_t leaf_head;
    chd_t empty_head;

    int _block_size;
    void* caches;
    int used[BPT_CACHE_NUM];

    chd_t get_value_by_offset(chd_t file_off, wchar_t*& value);
    chd_t set_value_by_offest(chd_t file_off, const wchar_t*& value);

    BPlusTreeNode* cache_refer();
    void cache_defer(BPlusTreeNode* node);
    BPlusTreeNode* new_node();
    BPlusTreeNode* new_leaf_node();
    BPlusTreeNode* new_internal_node();
    BPlusTreeNode* node_fetch(chd_t chdset);
    
    //�������
    chd_t node_append(BPlusTreeNode* node);
    void node_flush_file(BPlusTreeNode* node);
    void node_flush_parent(chd_t node, BPlusTreeNode* parent);

    void node_add_left(BPlusTreeNode* node, BPlusTreeNode* left);
    void node_add_right(BPlusTreeNode* node, BPlusTreeNode* right);
    int node_add_parent(BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);

    key_t split_internal_L(BPlusTreeNode* node, BPlusTreeNode* left, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_M(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_R(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    void insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);
    int insert_internal(BPlusTreeNode* node, BPlusTreeNode* l_ch, BPlusTreeNode* r_ch, key_t key);

    key_t split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, chd_t value_off);
    key_t split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert, key_t key, chd_t value_off);
    void insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, chd_t value_off);
    int insert_leaf(BPlusTreeNode* leaf, key_t key, const wchar_t*& value);

    int insert(key_t key, const wchar_t* value);

    int search(key_t key, wchar_t*& value);
    
    BPlusTree(const wchar_t* filename, int exist = BPLUSTREE_FILE_NON_EXIST);
    ~BPlusTree();
};
