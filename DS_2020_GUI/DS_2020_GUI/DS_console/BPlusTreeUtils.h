#pragma once
#include "pch.h"
namespace BPlusTreeUtils {
	extern int bptcnt;
    size_t db_EOF(FILE*);
	size_t db_read(FILE* , off_t , void* , size_t , size_t );
	size_t db_write(FILE* , off_t , const void* , size_t, size_t );
};

struct key_t {
    wchar_t a[20];
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
//B+树常量
typedef size_t OFF_T;
const size_t INVALID_OFFSET = 0xffffffff;
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
const char MAGIC_NUMBER[] = "DS_2020";
//B+树的缓存区大小，手写内存管理
const int BPT_CACHE_NUM = 1000;
const int BPT_CACHE_TEMP_NUM = 10;
//const int BPT_CACHE_BUF_USED = 1;
//const int BPT_CACHE_BUF_UNUSED = 0;
//const int BPT_MAX_ORDER = 1000;
const int BPT_ROOT_SIZE = 64;
const int BPT_NODE_BLOCK_SIZE = 16 * 1024;
const int BPT_DATA_BLOCK_SIZE = 128;

