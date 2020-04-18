#pragma once
#include "pch.h"
struct key_t {
    wchar_t a[16];
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
enum {
    BPLUSTRE_SUCCESS = 0,
    BPLUSTRE_FAILED = 1,
};
const char MAGIC_NUMBER[17] = "-DS_2020,LE,V1-";
//B+���Ļ�������С����д�ڴ����
const int BPT_CACHE_NUM = 10;
const int BPT_MAX_ENTRIES = 2;
const int BPT_MAX_ORDER = 2;
const int BPT_ROOT_SIZE = 64;
const int BPT_BLOCK_SIZE = 128;


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
 */

struct BPlusTreeNode {
    chd_t self;
    chd_t parent;
    int type;
    int ch_cnt;
    chd_t prev;
    chd_t next;

    key_t keys[BPT_MAX_ENTRIES];
    chd_t chds[BPT_MAX_ENTRIES];
    bool is_leaf() const;
    int binary_search_by_key(key_t);

    void fromFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE);
    void toFileBlock(FILE* file, chd_t file_off, size_t _BLOCK_SIZE);
};

/* @by ChrisJaunes
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
public:
    BPlusTree(const wchar_t* filename, int exist = BPLUSTREE_FILE_NON_EXIST);
    ~BPlusTree();
    int insert(key_t key, const wchar_t* value);
    int search(key_t key, wchar_t*& value);

//private:
    void* BPT_buf;

    FILE* fd;
    wchar_t* fdpath;

    int _block_size;
    chd_t fdsz;
    chd_t root;
    unsigned int level;
    unsigned int number;
    chd_t leaf_head;
    chd_t empty_head;

    void* caches;
    int used[BPT_CACHE_NUM];

    void fromFileBlock();
    void toFileBlock();
    chd_t get_empty_block();
    void get_value_by_offset(chd_t file_off, void*& value, size_t&);
    chd_t set_value_by_offest(chd_t file_off, const void* value, const size_t value_sz);

    BPlusTreeNode* cache_refer();
    void cache_defer(BPlusTreeNode* node);
    BPlusTreeNode* new_node();
    BPlusTreeNode* new_leaf_node();
    BPlusTreeNode* new_internal_node();
    BPlusTreeNode* node_fetch(chd_t chdset);
    
    //�������
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
    int insert_leaf(BPlusTreeNode* leaf, key_t key, const void* value, const size_t value_sz);

    int insert(key_t key, const void* value, const size_t value_sz);
    int search(key_t key, void*& value, size_t& value_sz);
};


/* @by ChrisJaunes
 * ������B+���ļ���Ϣ
 *  ��ʼ�� 64 byte
 * | ƫ�� | ��С |���� 
 * |---   |---  |---
 * |0	  | 20	 | ħ��, -DS_2020,LE,V1-�� ������Ϣ
 * |20	  | 4	 | sizeof(unsigned int)��ÿ���ڵ�Ԥ������С����Ӧ_block_size
 * |24	  | 8	 | sizoef(size_t), �ļ�β��λ��, ��Ӧfdsz
 * |32	  | 8	 | sizoef(size_t), ���ڵ���ļ�ƫ��ֵ, ��Ӧroot
 * |40    | 4    | sizoef(unsigned int)��B��������, ��Ӧlevel
 * |44	  | 4	 | sizeof(unsigned int)��B���Ľڵ���, ��Ӧnumber
 * |48	  | 8    | sizeof(size_t), B����Ҷ��������ʼָ��, ��Ӧleaf_head
 * |56	  | 8	 | sizeof(size_t), �ļ��տ�����λ��, empty_head
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
 * |16	  | 4	| sizeof(int) ����, �ڲ��ڵ����Ҷ�ӽڵ�
 * |20	  | 4	| sizeof(int) ��������key��ͬ�����
 * |24	  | ��	| ��������, �������ļ�
 */