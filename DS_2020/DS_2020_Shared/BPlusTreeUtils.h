#pragma once
#include "pch.h"
#include "config.h"
struct key_t {
    wchar_t a[20];
    key_t() {}
    key_t(MYSTR _s) {
        int len = min(19, (int)wcslen(_s));
        wcscpy(a, _s);
        a[len + 1] = L'\0';
    }
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
typedef size_t OFF_T;
enum class FILE_Status {
    NON_EXIST = 0x00,
    EXIST = 0x01,
};

namespace DS_BPlusTree {
    //文件IO控制
    extern double bptcntR, bptcntW;
    size_t db_EOF(FILE*);
    size_t db_read(FILE*, off_t, void*, size_t, size_t);
    size_t db_write(FILE*, off_t, const void*, size_t, size_t);
    //B+树常量
    enum class PAGE_Status {
        INIT_LOCK = 0x00,
        READ_LOCK = 0x01,
        WRITE_LOCK = 0x02,
        NON_MODIFY = 0x04,
        MODIFY = 0x08,
    };
    enum class PAGE_Type {
        NODE_INTERNAL = 0x00,
        NODE_LEAF = 0x01,
    };
    enum class BPT_Res {
        SUCCESS = 0,
        FAILED = 1,
        SUCCESS_REPLACE = 2,
    };

    const size_t INVALID_OFFSET = 0xffffffff;
    const char MAGIC_NUMBER[] = "DS_2020";
    //B+树的缓存区大小，手写内存管理
    const int BPT_CACHE_NODE_NUM = 1000;
    const int BPT_CACHE_DATA_NUM = 5;
    const int BPT_CACHE_TEMP_NUM = 5;
    //const int BPT_CACHE_BUF_USED = 1;
    //const int BPT_CACHE_BUF_UNUSED = 0;
    //const int BPT_MAX_ORDER = 1000;
    const int BPT_ROOT_SIZE = 64;
    const int BPT_NODE_BLOCK_SIZE = 1024 * 4;
    const int BPT_DATA_BLOCK_SIZE = 1024;

    //page
    struct BPlusTreePage {
        PAGE_Status P_Status;
        BPlusTreePage *P_Prev, *P_Next;
        BPlusTreePage(const BPlusTreePage&) = delete;
        BPlusTreePage& operator=(const BPlusTreePage&) = delete;
        inline void lock(const PAGE_Status _status) {
            //待实现锁
            //while (P_Status & BLOCK_WRITE_LOCK) ;
            assert(P_Status == PAGE_Status::MODIFY || P_Status == PAGE_Status::NON_MODIFY);
            if (P_Status == PAGE_Status::MODIFY) P_Status = PAGE_Status::WRITE_LOCK; else P_Status = _status;
        };
        inline void unlock() {
            assert(P_Status != PAGE_Status::MODIFY && P_Status != PAGE_Status::NON_MODIFY);
            if (P_Status == PAGE_Status::WRITE_LOCK) P_Status = PAGE_Status::MODIFY; else P_Status = PAGE_Status::NON_MODIFY;
        }
        //virtual void serialize(FILE* fd, OFF_T file_off, size_t _block_size) = 0;
        //virtual void deserialize(FILE* fd, OFF_T file_off, size_t _block_size) = 0;
    };
    //node page
    struct BPlusTreeNode : public BPlusTreePage {
        OFF_T self;
        OFF_T parent;
        PAGE_Type type;
        unsigned int ch_cnt;
        OFF_T prev;
        OFF_T next;
        char data[0];
        inline static size_t getSize() {
            return sizeof(self) + sizeof(parent) + sizeof(type) + sizeof(ch_cnt) + sizeof(prev) + sizeof(next);
        }
        inline void serialize(FILE* fd, OFF_T file_off, size_t _block_size) {
            assert(this->self == file_off);
            //size_t off = 0;
            //memmove(buf + off, &this->self, sizeof(this->self)); off += sizeof(this->self);
            //memmove(buf + off, &this->parent, sizeof(this->parent)); off += sizeof(this->parent);
            //memmove(buf + off, &this->type, sizeof(this->type)); off += sizeof(this->type);
            //memmove(buf + off, &this->ch_cnt, sizeof(this->ch_cnt)); off += sizeof(this->ch_cnt);
            //memmove(buf + off, &this->prev, sizeof(this->prev)); off += sizeof(this->prev);
            //memmove(buf + off, &this->next, sizeof(this->next)); off += sizeof(this->next);
            //memmove(buf + off, &this->data[0], _block_size - off);
            db_write(fd, file_off, &this->self, _block_size, 1);
        }
        inline void deserialize(FILE* fd, OFF_T file_off, size_t _block_size) {
            db_read(fd, file_off, &this->self, _block_size, 1);
            size_t off = 0;
            //memmove(&this->self, buf + off, sizeof(this->self)); off += sizeof(this->self);
            //memmove(&this->parent, buf + off, sizeof(this->parent)); off += sizeof(this->parent);
            //memmove(&this->type, buf + off, sizeof(this->type)); off += sizeof(this->type);
            //memmove(&this->ch_cnt, buf + off, sizeof(this->ch_cnt)); off += sizeof(this->ch_cnt);
            //memmove(&this->prev, buf + off, sizeof(this->prev)); off += sizeof(this->prev);
            //memmove(&this->next, buf + off, sizeof(this->next)); off += sizeof(this->next);
            //memmove(&this->data[0], buf + off, _block_size - off);
        }
    };
    //data page
    struct BPlusTreeData : public BPlusTreePage {
        OFF_T self;
        OFF_T next;
        char  data[0];
        inline static size_t getSize() {
            return sizeof(self) + sizeof(next);
        }
        inline void serialize(FILE* fd, OFF_T file_off, size_t _block_size) {
            assert(this->self == file_off);
            //size_t off = 0;
            //memmove((char*)buf + off, &this->self, sizeof(this->self)); off += sizeof(this->self);
            //memmove((char*)buf + off, &this->next, sizeof(this->next)); off += sizeof(this->next);
            //memmove((char*)buf + off, &(this->data[0]), _block_size - off);
            db_write(fd, file_off, &this->self, _block_size, 1);
        }
        inline void deserialize(FILE* fd, OFF_T file_off, size_t _block_size) {
            db_read(fd, file_off, &this->self, _block_size, 1);
            size_t off = 0;
            //memmove(&this->self, (char*)buf + off, sizeof(this->self)); off += sizeof(this->self);
            //memmove(&this->next, (char*)buf + off, sizeof(this->next)); off += sizeof(this->next);
            //memmove(&(this->data[0]), (char*)buf + off, _block_size - off);
        }

    };

    //事件模块
    /*struct BPlusTreeEvent{
        BPlusTreePage* page[8];
        unsigned int level;
        BPlusTreeEvent():level(0) {}
        BPlusTreeEvent(const BPlusTreeEvent& a) = delete;
        void operator=(const BPlusTreeEvent& a) = delete;
        void addPage(BPlusTreePage* _page, BLOCK_Status _status) {
            _page->lock(_status);
            page[level++] = _page;
        }
        void delPage() {
            page[--level]->unlock();
        }
    };*/
};