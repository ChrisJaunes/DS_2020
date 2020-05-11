#pragma once
#include "pch.h"
#include "BPlusTreeUtils.h"
#include "BPlusTreePool.h"
namespace DS_BPlusTree {
    /* @by ChrisJaunes_hjj;
     * 以下是B+树节点信息的注释
     * self: 自身文件指针
     * parent: 父节点文件指针
     * type: 节点类型包括内部节点(BPLUSTREE_INTERNAL)和叶子节点(BPLUSTREE_LEAF)
     * ch_cnt: 孩子的个数,根据B+树定义:内部节点有ch_cnt-1个key、cn_cnt个孩子指针; 叶子节点有ch_cnt个key、cn_cnt个数据域指针
     * prev: 同级上一个节点的文件指针,链表
     * next: 同级下一个节点的文件指针,链表
     * keys[]：key数组
     * offs[]: 孩子数组的文件指针
     * isleaf(): 判断是否是叶子节点
     * binary_search_by_key()： 用于处理key上的二分
     * fromFileBlock(): 用于从文件块中读取数据并且借助读取的数据生成一个BPlusTreeNode节点
     * toFileBlock(): 用于向文件块中写入数据并且借助自身的节点信息写入数据
     *
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

    #define keys(node) ( (KEY_T *)( (char *)(node) + p_KEYS) )
    #define offs(node) ( (OFF_T *)( (char *)(node) + p_OFFS) )
     //B+树主体
    template<typename KEY_T = key_t>
    class BPlusTree {
    public:
        explicit BPlusTree(const wchar_t* filepath, FILE_Status exist = FILE_Status::NON_EXIST, int _node_block_size = BPT_NODE_BLOCK_SIZE, int _data_block_size = BPT_DATA_BLOCK_SIZE);
        BPlusTree(const BPlusTree& a) = delete;
        void operator=(const BPlusTree& a) = delete;
        ~BPlusTree();
        BPT_Res insert(KEY_T key, const void* value, const size_t value_sz);
        BPT_Res search(KEY_T key, void*& value, size_t& value_sz);
    //private:
        //BPT常用量
        size_t p_KEYS;
        size_t p_OFFS;
        //节点类
        unsigned int binary_search_by_key(BPlusTreeNode* node, const KEY_T& key);
        //B+树基本信息
        void* BPT_caches;
        size_t node_max_order;
        unsigned int node_block_size;
        unsigned int data_block_size;
        OFF_T root;
        unsigned int level;
        unsigned int number;
        OFF_T leaf_head;
        OFF_T node_block_head;
        OFF_T data_block_head;
        void treeFromFileBlock();
        void treeToFileBlock();
        FILE* fd;
        //节点块缓存池
        BPlusTreePool1<BPlusTreeNode> *nodes;
        OFF_T node_empty_block();
        //数据块缓存池
        OFF_T data_empty_block();
        BPlusTreePool3<BPlusTreeData> *datas;
        void valueFromFileBlock(OFF_T file_off, void*& value, size_t& value_sz);
        OFF_T valueToFileBlock(OFF_T file_off, const void* value, const size_t value_sz);

        //临时使用缓存池
        unsigned int temp_block_size;
        void* temp_caches;
        int temp_used[BPT_CACHE_TEMP_NUM];
        void* swap_refer();
        void swap_defer(void*);

        BPlusTreeNode* node_new(OFF_T _self, OFF_T _parent, PAGE_Type _type);
        void node_flush_parent(OFF_T offset, BPlusTreeNode* parent);
        KEY_T node_split(BPlusTreeNode* node, const int pos, const KEY_T key, const OFF_T value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch);
        void node_insert(BPlusTreeNode* node, const int pos, const KEY_T key, const OFF_T value_off);
    };

    template<typename KEY_T>
    inline BPlusTree<KEY_T>::BPlusTree(const wchar_t* filepath, FILE_Status exist, int _node_block_size, int _data_block_size)
    {
        size_t sz = wcslen(filepath) + 1;
        wchar_t* fdpath = new wchar_t[sz];
        memset(fdpath, 0, sz);
        wcscat(fdpath, filepath);

        BPT_caches = malloc(BPT_ROOT_SIZE);

        if (exist == FILE_Status::EXIST) {
            assert((fd = _wfopen(fdpath, L"rb+")) != nullptr);
            treeFromFileBlock();
            node_max_order = (node_block_size - BPlusTreeNode::getSize()) / (sizeof(KEY_T) + sizeof(OFF_T));
            assert(node_max_order >= 2);
        }
        else {
            assert((fd = _wfopen(fdpath, L"wb+")) != nullptr);
            node_block_size = _node_block_size;
            data_block_size = _data_block_size;
            node_max_order = (node_block_size - BPlusTreeNode::getSize()) / (sizeof(KEY_T) + sizeof(OFF_T));
            assert(node_max_order >= 2);
            root = INVALID_OFFSET;
            level = 0;
            number = 0;
            leaf_head = INVALID_OFFSET;
            node_block_head = INVALID_OFFSET;
            data_block_head = INVALID_OFFSET;
            treeToFileBlock();
        }
        p_KEYS = sizeof(BPlusTreeNode);
        p_OFFS = sizeof(BPlusTreeNode) + node_max_order * sizeof(KEY_T);

        nodes = new BPlusTreePool1<BPlusTreeNode>(fd, node_block_size, BPT_CACHE_NODE_NUM);
        datas = new BPlusTreePool3<BPlusTreeData>(fd, data_block_size, BPT_CACHE_DATA_NUM);

        temp_block_size = max(node_block_size, data_block_size) * 3;
        temp_caches = malloc((size_t)temp_block_size * BPT_CACHE_TEMP_NUM);
        memset(temp_used, 0, sizeof(temp_used));

        delete[] fdpath;
    }

    template<typename KEY_T>
    inline BPlusTree<KEY_T>::~BPlusTree()
    {
        treeToFileBlock();
        free(BPT_caches);
        delete nodes;
        delete datas;
        free(temp_caches);
        fclose(fd);
    }

    template<typename KEY_T>
    inline BPT_Res BPlusTree<KEY_T>::insert(KEY_T key, const void* value, const size_t value_sz)
    {
        if (root == INVALID_OFFSET) {
            BPlusTreeNode* node = node_new(node_empty_block(), INVALID_OFFSET, PAGE_Type::NODE_LEAF);
            node->ch_cnt = 1;
            keys(node)[0] = key;
            offs(node)[0] = valueToFileBlock(INVALID_OFFSET, value, value_sz);

            root = node->self;
            level = 1;
            number = 1;
            leaf_head = node->self;
            nodes->defer(node);
            return BPT_Res::SUCCESS;
        }
        int pos;
        BPlusTreeNode* node = nodes->fetch(root);
        while (node->type == PAGE_Type::NODE_INTERNAL) {
            pos = binary_search_by_key(node, key);
            OFF_T next_off = offs(node)[pos];
            nodes->defer(node);
            node = nodes->fetch(next_off);
        }
        pos = binary_search_by_key(node, key);
        if (pos < node->ch_cnt && keys(node)[pos] == key) {
            offs(node)[pos] = valueToFileBlock(offs(node)[pos], value, value_sz);
            nodes->defer(node);
            return BPT_Res::SUCCESS_REPLACE;
        }
        OFF_T value_off = valueToFileBlock(INVALID_OFFSET, value, value_sz);
        BPlusTreeNode* lch, * rch;
        while (true) {
            if (node->ch_cnt < node_max_order) {
                node_insert(node, pos, key, value_off);
                nodes->defer(node);
                return BPT_Res::SUCCESS;;
            }
            key = node_split(node, pos, key, value_off, lch, rch);
            if (lch->parent == INVALID_OFFSET && rch->parent == INVALID_OFFSET) {
                node = node_new(node_empty_block(), INVALID_OFFSET, PAGE_Type::NODE_INTERNAL);
                node->ch_cnt = 2;
                keys(node)[0] = key;
                offs(node)[0] = lch->self;
                offs(node)[1] = rch->self;
                lch->parent = node->self;
                rch->parent = node->self;
                root = node->self;
                ++level;
                nodes->defer(lch);
                nodes->defer(rch);
                nodes->defer(node);
                return BPT_Res::SUCCESS;
            }
            value_off = lch->self;
            node = nodes->fetch(lch->parent);
            pos = binary_search_by_key(node, key);
            nodes->defer(lch);
            nodes->defer(rch);
        }
        assert(0);
        return BPT_Res::FAILED;
    }

    template<typename KEY_T>
    inline BPT_Res BPlusTree<KEY_T>::search(KEY_T key, void*& value, size_t& value_sz)
    {
        if (root == INVALID_OFFSET) {
            return BPT_Res::FAILED;
        }
        int pos;
        BPlusTreeNode* node = nodes->fetch(root);
        while (node->type == PAGE_Type::NODE_INTERNAL) {
            pos = binary_search_by_key(node, key);
            OFF_T next_off = offs(node)[pos];
            nodes->defer(node);
            node = nodes->fetch(next_off);
        }
        pos = binary_search_by_key(node, key);
        if (pos < node->ch_cnt && keys(node)[pos] == key) {
            valueFromFileBlock(offs(node)[pos], value, value_sz);
            nodes->defer(node);
            return BPT_Res::SUCCESS;
        }
        nodes->defer(node);
        return BPT_Res::FAILED;
    }

    template<typename KEY_T>
    inline unsigned int BPlusTree<KEY_T>::binary_search_by_key(BPlusTreeNode* node, const KEY_T& key)
    {
        int len = node->type == PAGE_Type::NODE_LEAF ? node->ch_cnt : node->ch_cnt - 1;
        KEY_T* beg = &(keys(node)[0]), * end = &(keys(node)[len]);
        unsigned int pos = (unsigned int)(std::lower_bound(beg, end, key) - beg);
        return pos;
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::treeFromFileBlock()
    {
        db_read(fd, 0, BPT_caches, BPT_ROOT_SIZE, 1);

        char magic_number[8];
        assert(sscanf((char*)BPT_caches, "%s", &magic_number) == 1);
        assert(strcmp(magic_number, MAGIC_NUMBER) == 0);
        size_t off = 8;
        memmove(&node_block_size, (char*)BPT_caches + off, sizeof(node_block_size)); off += sizeof(node_block_size);
        memmove(&data_block_size, (char*)BPT_caches + off, sizeof(data_block_size)); off += sizeof(data_block_size);
        memmove(&root, (char*)BPT_caches + off, sizeof(root)); off += sizeof(root);
        memmove(&level, (char*)BPT_caches + off, sizeof(level)); off += sizeof(level);
        memmove(&number, (char*)BPT_caches + off, sizeof(number)); off += sizeof(number);
        memmove(&leaf_head, (char*)BPT_caches + off, sizeof(leaf_head)); off += sizeof(leaf_head);
        memmove(&node_block_head, (char*)BPT_caches + off, sizeof(node_block_head)); off += sizeof(node_block_head);
        memmove(&data_block_head, (char*)BPT_caches + off, sizeof(data_block_head)); off += sizeof(data_block_head);
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::treeToFileBlock()
    {
        sprintf((char*)BPT_caches, "%s", MAGIC_NUMBER);
        size_t off = 8;
        memmove((char*)BPT_caches + off, &node_block_size, sizeof(node_block_size)); off += sizeof(node_block_size);
        memmove((char*)BPT_caches + off, &data_block_size, sizeof(data_block_size)); off += sizeof(data_block_size);
        memmove((char*)BPT_caches + off, &root, sizeof(root)); off += sizeof(root);
        memmove((char*)BPT_caches + off, &level, sizeof(level)); off += sizeof(level);
        memmove((char*)BPT_caches + off, &number, sizeof(number)); off += sizeof(number);
        memmove((char*)BPT_caches + off, &leaf_head, sizeof(leaf_head)); off += sizeof(leaf_head);
        memmove((char*)BPT_caches + off, &node_block_head, sizeof(node_block_head)); off += sizeof(node_block_head);
        memmove((char*)BPT_caches + off, &data_block_head, sizeof(data_block_head)); off += sizeof(data_block_head);
        db_write(fd, 0, BPT_caches, BPT_ROOT_SIZE, 1);
    }

    template<typename KEY_T>
    inline OFF_T BPlusTree<KEY_T>::node_empty_block()
    {
        //总所周知, 只有插入和查询的B+树，节点只会插在文件尾部
        void* buf = swap_refer();
        assert(node_block_head == INVALID_OFFSET);
        OFF_T res = db_EOF(fd);
        memmove(buf, &res, sizeof(res));
        db_write(fd, res, buf, node_block_size, 1);
        swap_defer(buf);
        return res;
    }

    template<typename KEY_T>
    inline OFF_T BPlusTree<KEY_T>::data_empty_block()
    {
        OFF_T res;
        if (data_block_head == INVALID_OFFSET) {
            res = db_EOF(fd);
            void* buf = swap_refer();
            OFF_T res = db_EOF(fd);
            memmove(buf, &res, sizeof(res));
            db_write(fd, res, buf, data_block_size, 1);
            swap_defer(buf);
        }
        else {
            res = data_block_head;
            BPlusTreeData* data = datas->fetch(data_block_head);
            memmove(&data_block_head, &data->next, sizeof(data_block_head));
            datas->defer(data);
        }
        return res;
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::valueFromFileBlock(OFF_T file_off, void*& value, size_t& value_sz)
    {
        assert(value == NULL);

        void* value_tmp;
        value = NULL;
        value_sz = 0;
        size_t sz = data_block_size - sizeof(OFF_T) - sizeof(OFF_T);
        while (file_off != INVALID_OFFSET) {
            BPlusTreeData* buf = datas->fetch(file_off);
            memmove(&file_off, &buf->next, sizeof(OFF_T));

            value_tmp = value;
            value = malloc(value_sz + sz);
            memmove(value, &buf->data, sz);
            memmove((char*)value + sz, value_tmp, value_sz);
            value_sz += sz;
            free(value_tmp);
            datas->defer(buf);
        }
    }

    template<typename KEY_T>
    inline OFF_T BPlusTree<KEY_T>::valueToFileBlock(OFF_T file_off, const void* value, const size_t value_sz)
    {
        BPlusTreeData* buf;
        while (file_off != INVALID_OFFSET) {
            buf = datas->fetch(file_off);
            buf->P_Status = PAGE_Status::WRITE_LOCK;
            memmove(&file_off, &buf->next, sizeof(OFF_T));
            memmove(&buf->next, &data_block_head, sizeof(OFF_T));
            memmove(&data_block_head, &buf->self, sizeof(OFF_T));
            datas->defer(buf);
        }

        size_t value_sz_tmp = 0, next_off;
        size_t sz = (data_block_size - sizeof(OFF_T) - sizeof(OFF_T));
        assert(value_sz % sz == 0);

        while (value_sz_tmp < value_sz) {
            next_off = file_off;
            file_off = data_empty_block();
            buf = datas->fetch(file_off);
            buf->P_Status = PAGE_Status::WRITE_LOCK;
            memmove(&buf->self, &file_off, sizeof(OFF_T));
            memmove(&buf->next, &next_off, sizeof(OFF_T));
            memmove(&buf->data, (char*)value + value_sz_tmp, sz);
            datas->defer(buf);
            value_sz_tmp += sz;
        }
        return file_off;
    }

    template<typename KEY_T>
    inline void* BPlusTree<KEY_T>::swap_refer()
    {
        for (int i = 0; i < BPT_CACHE_TEMP_NUM; i++) {
            if (!temp_used[i]) {
                temp_used[i] = 1;
                return (char*)temp_caches + temp_block_size * i;
            }
        }
        assert(0);
        return nullptr;
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::swap_defer(void* node)
    {
        if (node == nullptr) return;
        int i = (int)(((char*)node - (char*)temp_caches) / temp_block_size);
        temp_used[i] = 0;
    }
    template<typename KEY_T>
    inline BPlusTreeNode* BPlusTree<KEY_T>::node_new(OFF_T _self, OFF_T _parent, PAGE_Type _type)
    {
        assert(_self != INVALID_OFFSET);
        BPlusTreeNode* node = nodes->refer(_self);
        node->P_Status = PAGE_Status::WRITE_LOCK;
        node->self = _self;
        node->parent = _parent;
        node->type = _type;
        node->ch_cnt = 0;
        node->prev = INVALID_OFFSET;
        node->next = INVALID_OFFSET;
        ++number;
        return node;
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::node_flush_parent(OFF_T offset, BPlusTreeNode* parent)
    {
        assert(offset != INVALID_OFFSET && parent != nullptr);
        BPlusTreeNode* child = nodes->fetch(offset);
        child->P_Status = PAGE_Status::WRITE_LOCK;
        child->parent = parent->self;
        nodes->defer(child);
    }
    template<typename KEY_T>
    inline KEY_T BPlusTree<KEY_T>::node_split(BPlusTreeNode* node, const int pos, const KEY_T key, const OFF_T value_off, BPlusTreeNode*& lch, BPlusTreeNode*& rch)
    {
        assert(node->ch_cnt == node_max_order);
        node->P_Status = PAGE_Status::WRITE_LOCK;
        BPlusTreeNode* sibling = node_new(node_empty_block(), node->parent, node->type);

        int split = (int)((node_max_order + 1) / 2);
        sibling->ch_cnt = split;
        node->ch_cnt = (unsigned int)(node_max_order - split + 1);

        if ((sibling->prev = node->prev) != INVALID_OFFSET) {
            BPlusTreeNode* prev = nodes->fetch(sibling->prev);
            prev->P_Status = PAGE_Status::WRITE_LOCK;
            prev->next = sibling->self;
            nodes->defer(prev);
        }
        else {
            leaf_head = sibling->self;
        }
        node->prev = sibling->self;
        sibling->next = node->self;
        void* buf = swap_refer();
        size_t off = 0;
        memmove((char*)buf + off, &(keys(node)[0]), pos * sizeof(KEY_T)); off += pos * sizeof(KEY_T);
        memmove((char*)buf + off, &key, sizeof(KEY_T)); off += sizeof(KEY_T);
        memmove((char*)buf + off, &(keys(node)[pos]), (node_max_order - pos) * sizeof(KEY_T));
        off = 0;
        memmove(&(keys(sibling)[0]), (char*)buf + off, sibling->ch_cnt * sizeof(KEY_T)); off += sibling->ch_cnt * sizeof(KEY_T);
        memmove(&(keys(node)[0]), (char*)buf + off, node->ch_cnt * sizeof(KEY_T));

        off = 0;
        memmove((char*)buf + off, &(offs(node)[0]), pos * sizeof(OFF_T)); off += pos * sizeof(OFF_T);
        memmove((char*)buf + off, &value_off, sizeof(OFF_T)); off += sizeof(OFF_T);
        memmove((char*)buf + off, &(offs(node)[pos]), (node_max_order - pos) * sizeof(OFF_T));
        off = 0;
        memmove(&(offs(sibling)[0]), (char*)buf + off, sibling->ch_cnt * sizeof(OFF_T)); off += sibling->ch_cnt * sizeof(OFF_T);
        memmove(&(offs(node)[0]), (char*)buf + off, node->ch_cnt * sizeof(OFF_T));
        swap_defer(buf);
        if (sibling->type == PAGE_Type::NODE_INTERNAL) {
            for (int i = 0; i < sibling->ch_cnt; i++)
                node_flush_parent(offs(sibling)[i], sibling);
        }

        KEY_T res = keys(sibling)[split - 1];
        lch = sibling;
        rch = node;
        return res;
    }

    template<typename KEY_T>
    inline void BPlusTree<KEY_T>::node_insert(BPlusTreeNode* node, const int pos, const KEY_T key, const OFF_T value_off)
    {
        node->P_Status = PAGE_Status::WRITE_LOCK;
        memmove(&(keys(node)[pos + 1]), &(keys(node)[pos]), (node->ch_cnt - pos) * sizeof(KEY_T));
        memmove(&(offs(node)[pos + 1]), &(offs(node)[pos]), (node->ch_cnt - pos) * sizeof(OFF_T));
        keys(node)[pos] = key;
        offs(node)[pos] = value_off;
        node->ch_cnt++;
        if (node->type == PAGE_Type::NODE_INTERNAL) node_flush_parent(value_off, node);
    }
}