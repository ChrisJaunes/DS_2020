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
//B+树常量
const chd_t INVALID_OFFSET = 0xffffffff;
enum {
    BPLUSTREE_INTERNAL,
    BPLUSTREE_LEAF = 1,
};
enum {
    BPLUSTREE_FILE_EXIST,
    BPLUSTREE_FILE_NON_EXIST,
};
//B+树的缓存区大小，手写内存管理
const int BPT_CACHE_NUM = 10;
const int BPT_MAX_ENTRIES = 10;
const int BPT_MAX_ORDER = 10;
const int BPT_ROOT_SIZE = 48;
const int BPT_BLOCK_SIZE = 4096;


/* by ChrisJaunes
 * 以下是B+树节点信息的注释
 * type: 节点类型包括内部节点(BPLUSTREE_INTERNAL)和叶子节点(BPLUSTREE_LEAF)
 * self: 自身文件指针
 * parent: 父节点文件指针
 * prev: 同级上一个节点的文件指针,链表
 * next: 同级下一个节点的文件指针,链表
 * ch_cnt: 孩子的个数,根据B+树定义:内部节点有ch_cnt-1个key、cn_cnt个孩子指针; 叶子节点有ch_cnt个key、cn_cnt个数据域指针
 * keys[]：key数组
 * chds[]: 孩子数组的文件指针
 * isleaf(): 判断是否是叶子节点
 * binary_search_by_key()： 用于处理key上的二分
 * fromFileBlock(): 用于从文件块中读取数据并且借助读取的数据生成一个BPlusTreeNode节点
 * toFileBlock(): 用于向文件块中写入数据并且借助自身的节点信息写入数据
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
 * 以下是B+树信息的注释
 * index_fd: 节点存储文件的文件指针
 * index_fdpath: 节点存储文件的文件路径
 * index_fdsz: 节点存储文件的文件大小
 * data_fd: 数据存储文件的文件指针
 * data_fdpath: 数据存储文件的文件路径
 * data_fdsz: 数据存储文件的文件大小
 * root: 树根的文件指针
 * level: 树高
 * leaf_head: 叶子节点链的文件指针
 * 手动内存管理
 * BPT_BLOCK_SIZE：BPlusTreeNode块大小
 * caches: 内存管理
 * used[]: 内存管理
 * cache_refer: 获取一块内存空间
 * cache_refer: 释放一块内存空间
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
    
    //插入操作
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
