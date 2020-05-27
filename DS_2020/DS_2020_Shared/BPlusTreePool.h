#pragma once
#include "BPlusTreeUtils.h"
namespace DS_BPlusTree {
    //PAGE_T必须是BPlusTreePage的继承类
    //时钟置换页面算法
    template<typename PAGE_T>
    struct BPlusTreePool1 {
        FILE* fd;
        unsigned int _block_size;
        unsigned int _page_size;
        unsigned int _page_num;
        void* caches;
        std::unordered_map<OFF_T, PAGE_T*> pool_map;
        unsigned int _used_index;
        BPlusTreePool1(FILE* const fd, const unsigned int _block_size, unsigned int _page_num)
            : fd(fd)
            , _block_size(_block_size)
            , _page_num(_page_num)
        {
            _page_size = (unsigned int) ((size_t)_block_size - PAGE_T::getSize() + sizeof(PAGE_T));
            size_t sz = (size_t)_page_size * _page_num;
            caches = malloc(sz);
            memset(caches, 0, sz);
            
            for (unsigned int i = 0; i < _page_num; i++)
                ((BPlusTreeNode*)((char*)caches + (size_t)_page_size * i))->P_Status = PAGE_Status::NON_MODIFY;
            _used_index = 0;
        }
        ~BPlusTreePool1() {
            for (unsigned int i = 0; i < _page_num; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + (size_t)_page_size * i);
                assert(page->P_Status == PAGE_Status::NON_MODIFY || page->P_Status == PAGE_Status::MODIFY);
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size);
                }
            }
            //fclose(fd);
        }
        PAGE_T* refer(size_t offset) {
            while (true) {
                if (_used_index == _page_num) _used_index = 0;
                ++_used_index;
                PAGE_T* page = (PAGE_T*)((char*)caches + (size_t)_page_size * (_used_index - 1));
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
        };
        void defer(PAGE_T* page) {
            if (page == nullptr) return;
            //assert(page != nullptr);
            page->unlock();
        }
        PAGE_T* fetch(OFF_T offset) {
            if (offset == INVALID_OFFSET) {
                return nullptr;
            }
            if (pool_map.count(offset)) {
                PAGE_T* page = pool_map[offset];
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

    //PAGE_T必须是BPlusTreePage的继承类
    //LRU页面置换算法
    template<typename PAGE_T>
    struct BPlusTreePool2 {
        FILE* fd;
        unsigned int _block_size;
        unsigned int _page_size;
        unsigned int _page_num;
        void* caches;
        std::unordered_map<OFF_T, PAGE_T*> pool_map;
        BPlusTreePage* head, * tail;

        BPlusTreePool2(FILE* const fd, const unsigned int _block_size, unsigned int _page_num)
            : fd(fd)
            , _block_size(_block_size)
            , _page_num(_page_num)
        {
            _page_size = ((size_t)_block_size - PAGE_T::getSize() + sizeof(PAGE_T));
            size_t sz = _page_size * _page_num;
            caches = malloc(sz);
            memset(caches, 0, sz);
            for (int i = 0; i < _page_num; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + _page_size * i);
                page->P_Status = PAGE_Status::NON_MODIFY;
                if (i == 0) head = page;
                else {
                    page->P_Prev = (PAGE_T*)((char*)caches + _page_size * (i - 1));
                }
                if (i == _page_num - 1) tail = page;
                else {
                    page->P_Next = (PAGE_T*)((char*)caches + _page_size * (i + 1));
                }
            }
        }
        ~BPlusTreePool2() {
            for (int i = 0; i < _page_num; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + _page_size * i);
                assert(page->P_Status == PAGE_Status::NON_MODIFY || page->P_Status == PAGE_Status::MODIFY);
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size);
                }
            }
            //fclose(fd);
        }
        PAGE_T* refer(size_t offset) {
            assert(head->P_Status == PAGE_Status::MODIFY || head->P_Status == PAGE_Status::NON_MODIFY);
            PAGE_T* page = nullptr;
            do {
                page = (PAGE_T*)head;
                head = head->P_Next;
                head->P_Prev = nullptr;
                tail->P_Next = page;
                page->P_Prev = tail;
                page->P_Next = nullptr;
                tail = page;
            } while (page->P_Status != PAGE_Status::MODIFY && page->P_Status != PAGE_Status::NON_MODIFY);
            pool_map.erase(page->self);
            if (page->P_Status == PAGE_Status::MODIFY) {
                page->serialize(fd, page->self, _block_size);
            }
            page->P_Status = PAGE_Status::INIT_LOCK;
            pool_map[offset] = page;
            return page;
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
                if (page->P_Prev == nullptr) head = page->P_Next;
                else page->P_Prev->P_Next = page->P_Next;
                if (page->P_Next == nullptr) tail = page->P_Prev;
                else page->P_Next->P_Prev = page->P_Prev;
                tail->P_Next = page;
                page->P_Prev = tail;
                page->P_Next = nullptr;
                tail = page;
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
    //PAGE_T必须是BPlusTreePage的继承类
    //暴力池, data专用
    template<typename PAGE_T>
    struct BPlusTreePool3 {
        FILE* fd;
        unsigned int _block_size;
        unsigned int _page_size;
        unsigned int _page_num;
        void* caches;
        unsigned int _used_index;

        BPlusTreePool3(FILE* const fd, const unsigned int _block_size, unsigned int _page_num)
            : fd(fd)
            , _block_size(_block_size)
            , _page_num(_page_num)
        {
            _page_size = (unsigned int)((size_t)_block_size - PAGE_T::getSize() + sizeof(PAGE_T));
            size_t sz = (size_t)_page_size * _page_num;
            caches = malloc(sz);
            memset(caches, 0, sz);
            for (unsigned int i = 0; i < _page_num; i++)
                ((PAGE_T*)((char*)caches + (size_t)_page_size * i))->P_Status = PAGE_Status::NON_MODIFY;
            _used_index = 0;
        }
        ~BPlusTreePool3() {
            for (unsigned int i = 0; i < _page_num; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + (size_t)_page_size * i);
                assert(page->P_Status == PAGE_Status::NON_MODIFY || page->P_Status == PAGE_Status::MODIFY);
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size);
                }
            }
            //fclose(fd);
        }
        PAGE_T* refer(size_t offset) {
            while (true) {
                if (++_used_index == _page_num) _used_index = 0;
                PAGE_T* page = (PAGE_T*)((char*)caches + (size_t)_page_size * _used_index);
                if (page->P_Status == PAGE_Status::INIT_LOCK || page->P_Status == PAGE_Status::WRITE_LOCK || page->P_Status == PAGE_Status::READ_LOCK) continue;
                if (page->P_Status == PAGE_Status::MODIFY) {
                    page->serialize(fd, page->self, _block_size);
                }
                page->lock(PAGE_Status::INIT_LOCK);
                return page;
            }
            assert(0);
            return nullptr;
        };
        void defer(PAGE_T* page) {
            assert(page != nullptr);
            page->unlock();
        }
        PAGE_T* fetch(OFF_T offset) {
            if (offset == INVALID_OFFSET) {
                return nullptr;
            }
            for (unsigned int i = 0; i < _page_num; i++) {
                PAGE_T* page = (PAGE_T*)((char*)caches + (size_t)_page_size * i);
                if (page->self == offset) {
                    assert(page->P_Status != PAGE_Status::INIT_LOCK && page->P_Status != PAGE_Status::WRITE_LOCK && page->P_Status != PAGE_Status::READ_LOCK);
                    page->lock(PAGE_Status::READ_LOCK);
                    return page;
                }
            }
            PAGE_T* page = refer(offset);
            page->P_Status = PAGE_Status::READ_LOCK;
            page->deserialize(fd, offset, _block_size);
            return page;
        };
    };
}