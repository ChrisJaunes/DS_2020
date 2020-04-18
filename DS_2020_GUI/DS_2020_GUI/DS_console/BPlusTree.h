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
//B+������
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
//B+���Ļ�������С����д�ڴ����
const int BPT_CACHE_NUM = 10;
const int BPT_MAX_ORDER = 250;
const int BPT_ROOT_SIZE = 64;


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

    key_t keys[BPT_MAX_ORDER];
    chd_t chds[BPT_MAX_ORDER];
    int binary_search_by_key(key_t);
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
//search�ᷢ���ڴ�й©������������ԭ������
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
    //used[0]: caches_nodeǰ��������ʹ��
    //used[1]: caches_BPTʹ��
    //used[2]: caches_dataʹ��
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