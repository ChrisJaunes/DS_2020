#pragma once
#include "pch.h"
struct key_t {
    wchar_t a[32];
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
    BPLUSTREE_FILE_NON_EXIST = 0,
    BPLUSTREE_FILE_EXIST = 1,
};
enum {
    BPLUSTRE_SUCCESS = 0,
    BPLUSTRE_FAILED = 1,
    BPLUSTRE_SUCCESS_REPLACE = 2,
};
const char MAGIC_NUMBER[17] = "-DS_2020,LE,V1-";
//B+树的缓存区大小，手写内存管理
const int BPT_CACHE_NUM = 10;
const int BPT_MAX_ORDER = 250;
const int BPT_ROOT_SIZE = 64;


/* @by ChrisJaunes
 * 以下是B+树节点信息的注释
 * self: 自身文件指针
 * parent: 父节点文件指针
 * type: 节点类型包括内部节点(BPLUSTREE_INTERNAL)和叶子节点(BPLUSTREE_LEAF)
 * ch_cnt: 孩子的个数,根据B+树定义:内部节点有ch_cnt-1个key、cn_cnt个孩子指针; 叶子节点有ch_cnt个key、cn_cnt个数据域指针
 * prev: 同级上一个节点的文件指针,链表
 * next: 同级下一个节点的文件指针,链表
 * keys[]：key数组
 * chds[]: 孩子数组的文件指针
 * isleaf(): 判断是否是叶子节点
 * binary_search_by_key()： 用于处理key上的二分
 * fromFileBlock(): 用于从文件块中读取数据并且借助读取的数据生成一个BPlusTreeNode节点
 * toFileBlock(): 用于向文件块中写入数据并且借助自身的节点信息写入数据
 */

struct BPlusTreeNode {
    chd_t self;
    chd_t parent;
    int type;
    int ch_cnt;
    chd_t prev;
    chd_t next;

    key_t keys[BPT_MAX_ORDER];
    chd_t chds[BPT_MAX_ORDER];
    int binary_search_by_key(key_t);
};

/* @by ChrisJaunes
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
//search会发生内存泄漏！！！，具体原因待检查
struct BPlusTree {
public:
    BPlusTree(const wchar_t* filename, int exist = BPLUSTREE_FILE_NON_EXIST);
    ~BPlusTree();
    int insert(key_t key, const wchar_t* value);
    int search(key_t key, wchar_t*& value);
    chd_t begin();
    chd_t end();
    chd_t next(chd_t node_off);

//private:
    //used[0]: caches_node前三个缓存使用
    //used[1]: caches_BPT使用
    //used[2]: caches_data使用
    void *caches_BPT, *caches_node, *caches_data;
    int used[BPT_CACHE_NUM];

    FILE* fd;
    wchar_t* fdpath;

    unsigned int node_block_size;
    unsigned int data_block_size;
    chd_t root;
    unsigned int level;
    unsigned int number;
    chd_t fdsz;
    chd_t leaf_head;
    chd_t node_block_head;
    chd_t data_block_head;

    void* cache_refer();
    void cache_defer(void* node);
    BPlusTreeNode* new_node(chd_t _self, chd_t _parent,int _type);
    BPlusTreeNode* node_fetch(chd_t chdset);


    void nodeFromFileBlock(BPlusTreeNode* node, chd_t file_off);
    void nodeToFileBlock(BPlusTreeNode* node, chd_t file_off);
    void treeFromFileBlock();
    void treeToFileBlock();
    chd_t get_node_block();
    chd_t get_data_block();
    void get_value_by_offset(chd_t file_off, void*& value, size_t&);
    chd_t set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz);
    void node_flush_file(BPlusTreeNode* node);
    void node_flush_parent(chd_t node, BPlusTreeNode* parent);

    key_t node_split(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch);
    void node_insert(BPlusTreeNode* node, const int pos, const key_t key, const chd_t value_off);
    int insert(key_t key, const void* value, const size_t value_sz);
    int search(key_t key, void*& value, size_t& value_sz);
};


/* @by ChrisJaunes
 * 以下是B+树文件信息
 *  起始块 64 byte
 * | 偏移 | 大小 |内容 
 * |---   |---  |---
 * |0	  | 8	 | 魔数, -DS,V1-； 检验信息
 * |8	  | 4	 | sizeof(unsigned int)，每个节点块预分配块大小，对应node_block_size
 * |12	  | 4	 | sizeof(unsigned sint)，每个数据块预分配块大小，对应data_block_size
 * |16	  | 8	 | sizoef(size_t), 根节点的文件偏移值, 对应root
 * |24    | 4    | sizoef(unsigned int)，B树的树高, 对应level
 * |28	  | 4	 | sizeof(unsigned int)，B树的节点数, 对应number
 * |32	  | 8    | sizeof(size_t), B树的叶子链表起始指针, 对应leaf_head
 * |40	  | 8	 | sizoef(size_t), 文件尾部位置, 对应fdsz
 * |48	  | 8	 | sizeof(size_t), 文件节点空块链表位置, node_block_head
 * |56	  | 8	 | sizeof(size_t), 文件数据空块链表位置, data_block_head
 * 
 * 节点块 包括内部节点和叶子节点
 * | 偏移 | 大小 |内容
 * |---   |---  |---
 * |0	  | 8	| sizeof(size_t) 自身文件偏移,用于检验,对应self
 * |8	  | 8	| sizeof(size_t) 父节点文件偏移,用于检验,对应parent
 * |16	  | 4	| sizeof(int) 类型, 内部节点或者叶子节点, 对应type
 * |20	  | 4	| sizeof(int) 该节点孩子数, 对应ch_cnt
 * |24	  | 8	| sizeof(size_t) 同级链上一个节点的文件偏移, 对应prev
 * |32	  | 8	| sizoef(size_t) 同级链下一个节点的文件偏移, 对应next
 * |40	  | ？	| sizoef(key_t) * BPT_MAX_ORDER, 对应keys数组
 * |？    | ？  | sizoef(chd_t) * BPT_MAX_ORDER, 对应chds数组
 * 
 * 数据块
 * | 偏移 | 大小 |内容
 * |---   |---  |---
 * |0	  | 8	| sizeof(size_t) 自身文件偏移,用于检验
 * |8	  | 8	| sizeof(size_t) 下一个数据块节点, 本质上是一个链表
 * |16	  | ？	| 数据内容, 二进制文件
 */