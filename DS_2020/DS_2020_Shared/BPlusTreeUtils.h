#pragma once
#include "pch.h"
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
typedef size_t OFF_T;
enum class FILE_Status {
    NON_EXIST = 0x00,
    EXIST = 0x01,
};

namespace DS_BPlusTree {
    //文件IO控制
	extern double bptcntR, bptcntW;
    size_t db_EOF(FILE*);
	size_t db_read(FILE* , off_t , void* , size_t , size_t );
	size_t db_write(FILE* , off_t , const void* , size_t, size_t );
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
    enum class BPT_Res{
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
        //BPlusTreePage *P_Prev, *P_Next;
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
    //PAGE_T必须是BPlusTreePage的继承类
    template<typename PAGE_T>
    struct BPlusTreePool {
        FILE* fd;
        unsigned int _block_size;
        unsigned int _page_size;
        void* caches;
        std::unordered_map<OFF_T, PAGE_T*> pool_map;

        unsigned int _used_index;
        //BPlusTreePage *head, *tail;
        
        BPlusTreePool(FILE* const fd, const unsigned int _block_size) : fd(fd), _block_size(_block_size) {
            _page_size = ((size_t)_block_size - PAGE_T::getSize() + sizeof(PAGE_T));
            size_t sz = _page_size * BPT_CACHE_NODE_NUM;
            caches = malloc(sz);
            memset(caches, 0, sz);
            //时钟置换页面算法
            for (int i = 0; i < BPT_CACHE_NODE_NUM; i++)
                ((BPlusTreeNode*)((char*)caches + _page_size * i))->P_Status = PAGE_Status::NON_MODIFY;
            _used_index = 0;
            //LRU实现
            //for (int i = 0; i < BPT_CACHE_NODE_NUM; i++) {
            //    PAGE_T* page = (PAGE_T*)((char*)caches + _page_size * i);
            //    page->P_Status = PAGE_Status::NON_MODIFY;
            //    if (i == 0) head = page;
            //    else {
            //        page->P_Prev = (PAGE_T*)((char*)caches + _page_size * (i - 1));
            //    }
            //    if (i == BPT_CACHE_NODE_NUM - 1) tail = page;
            //    else {
            //        page->P_Next = (PAGE_T*)((char*)caches + _page_size * (i + 1));
            //    }
            //}
        }
        ~BPlusTreePool() {
            for (int i = 0; i < BPT_CACHE_NODE_NUM; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + _page_size * i);
                assert(page->P_Status == PAGE_Status::NON_MODIFY || page->P_Status == PAGE_Status::MODIFY);
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size);
                }
            }
            //fclose(fd);
        }
        PAGE_T* refer(size_t offset) {
            //时钟置换页面算法
            while (true) {
                if (_used_index == BPT_CACHE_NODE_NUM) _used_index = 0;
                ++_used_index;
                PAGE_T* page = (PAGE_T*)((char*)caches + _page_size * (_used_index - 1));
                if (page->P_Status == PAGE_Status::INIT_LOCK || page->P_Status == PAGE_Status::WRITE_LOCK || page->P_Status == PAGE_Status::READ_LOCK) continue;
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size); 
                }
                pool_map.erase(page->self);
                page->lock(PAGE_Status::INIT_LOCK);
                pool_map[offset] = page;
                return page;
            }
            assert(0);
            return nullptr;
            //LRU
            //assert(head->P_Status == PAGE_Status::MODIFY || head->P_Status == PAGE_Status::NON_MODIFY);
            //PAGE_T* page = nullptr;
            //do {
            //    page = (PAGE_T*)head;
            //    head = head->P_Next;
            //    head->P_Prev = nullptr;
            //    tail->P_Next = page;
            //    page->P_Prev = tail;
            //    page->P_Next = nullptr;
            //    tail = page;
            //} while (page->P_Status != PAGE_Status::MODIFY && page->P_Status != PAGE_Status::NON_MODIFY);
            //pool_map.erase(page->self);
            //if (page->P_Status == PAGE_Status::MODIFY) {
            //    page->serialize(fd, page->self, _block_size);
            //}
            //page->P_Status = PAGE_Status::INIT_LOCK;
            //pool_map[offset] = page;
            //return page;

        };
        void defer(PAGE_T* page) {
            assert(page != nullptr);
            page->unlock();
        }
        PAGE_T* fetch(OFF_T offset) {
            if (offset == INVALID_OFFSET) {
                return nullptr;
            }
            if (pool_map.count(offset)) {
                PAGE_T* page = pool_map[offset];
                //if (page->P_Prev == nullptr) head = page->P_Next;
                //else page->P_Prev->P_Next = page->P_Next;
                //if (page->P_Next == nullptr) tail = page->P_Prev;
                //else page->P_Next->P_Prev = page->P_Prev; 
                //tail->P_Next = page;
                //page->P_Prev = tail;
                //page->P_Next = nullptr;
                //tail = page;
                assert(page->P_Status != PAGE_Status::INIT_LOCK && page->P_Status != PAGE_Status::WRITE_LOCK && page->P_Status != PAGE_Status::READ_LOCK);
                page->lock(PAGE_Status::READ_LOCK);
                return page;
            }
            PAGE_T* page = refer(offset);
            page->P_Status = PAGE_Status::READ_LOCK;
            page->deserialize(fd, offset, _block_size);
            return page;
        };
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