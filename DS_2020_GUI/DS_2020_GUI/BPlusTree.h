#pragma once
#define key_t int
#define value_t int
//B+树的缓存区大小，手写内存管理

const int BPT_CACHE_NUM = 100;
const int BPT_MAX_CHILDRENS = 10;
const int BPT_MAX_ENTRIES = 10;
const int BPT_MAX_ORDER = 10;


struct BPlusTreeNode {
    int type;
    off_t self;
    off_t parent;
    off_t prev;
    off_t next;
    /* If leaf node, it specifies  count of entries,
     * if non-leaf node, it specifies count of children(branches) */
    int children;
    inline bool is_leaf();
    key_t* get_keys();
    off_t* get_offs();
    int binary_search_by_key(key_t);
    static off_t insert_leaf_value(off_t, value_t);
};
struct BPlusTree {
    //缓存块构成: BPlusTreeNode + 孩子区间的值 + 孩子指针
    char* caches;
    int used[BPT_CACHE_NUM];
    const int BPT_BLOCK_SIZE; //节点块大小
    off_t root;
    int level;
    BPlusTreeNode* cache_refer();
    void cache_defer(BPlusTreeNode* node);
    BPlusTreeNode* new_node();
    BPlusTreeNode* new_leaf_node();
    BPlusTreeNode* new_internal_node();

    BPlusTreeNode* node_fetch(off_t offset);
    BPlusTreeNode* node_seek(off_t offset);
    
    off_t node_append(BPlusTreeNode* node);
    void node_flush_file(BPlusTreeNode* node);
    void node_flush_parent(off_t node, BPlusTreeNode* parent);

    void node_add_left(BPlusTreeNode* node, BPlusTreeNode* left);
    void node_add_right(BPlusTreeNode* node, BPlusTreeNode* right);
    int node_add_parent(BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);

    key_t split_internal_L(BPlusTreeNode* node, BPlusTreeNode* left, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_M(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);
    key_t split_internal_R(BPlusTreeNode* node, BPlusTreeNode* right, int ins_pos, key_t key, BPlusTreeNode* lch, BPlusTreeNode* rch);

    key_t split_leaf_L(BPlusTreeNode* leaf, BPlusTreeNode* left, int insert, key_t key, off_t data);
    key_t split_leaf_R(BPlusTreeNode* leaf, BPlusTreeNode* right, int insert, key_t key, off_t dataoff);


    void insert_internal_simple(BPlusTreeNode* node, int ins_pos, BPlusTreeNode* lch, BPlusTreeNode* rch, key_t key);
    void insert_leaf_simple(BPlusTreeNode* leaf, int ins_pos, key_t key, off_t value_off);
    int insert_internal(BPlusTreeNode* node, BPlusTreeNode* l_ch, BPlusTreeNode* r_ch, key_t key);
    int insert_leaf(BPlusTreeNode* leaf, key_t key, value_t data);


    int insert(key_t key, value_t data);
    long search_by_key(key_t key);
};
