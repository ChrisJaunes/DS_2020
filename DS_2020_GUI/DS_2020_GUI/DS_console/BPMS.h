#include "pch.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"
/* @by ChrisJaunes
 * 用于实现对B+树的封装
 * 对于T, 必须实现：
 * key_t get_key(wchar_t*&) 返回一个key_t类型, 其中key_t类型在BPlusTreeUtils中声明
 * serialize(wchar_t*&), 序列化函数
 * deserializ(const wchar_t*), 反序列化函数
 * 如果用到数组，务必实现operator=(T)
 *
 * insert: 在B+树里插入一个T
 * search: 在B+树里查询key_t， 返回一个vector(), 
 * 
 */
template<typename T>
class BPMS{
public:
    BPMS(const wchar_t* filename, int exist = 0);
    void insert(T);
    std::vector<T> search(T);
//private:
    BPlusTree db;
    size_t value_block_sz;
};

template<typename T>
inline BPMS<T>::BPMS(const wchar_t* filepath, int exist)
    : db(filepath, exist)
{
    value_block_sz = db.data_block_size - sizeof(chd_t) * 2;

}

template<typename T>
inline void BPMS<T>::insert(T obj) {
    wchar_t *key = NULL, *value = NULL;
    T.get_key(key);
    size_t key_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _key_sz = min(key_sz, sizeof(&(_key.a[0])));
    key_t _key;
    memset(&(_key.a[0]), 0, _key_sz);
    memcpy(&(_key.a[0]), key, _key_sz);

    T.serialize(value);
    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + value_block_sz - 1) / value_block_sz * value_block_sz;
    void* _value = malloc(_value_sz);
    memset(_value, 0, _value_sz);
    memcpy(_value, value, value_sz);

    insert(key, _value, _value_sz);
    
    free(_value);
    delete value;
}

template<typename T>
inline std::vector<T> BPMS<T>::search(T obj)
{
    /*
int BPlusTree::search(key_t key, wchar_t*& value) {
    assert(value == nullptr);
    void* _value = nullptr;
    size_t _value_sz;
    if (search(key, _value, _value_sz) == BPLUSTRE_FAILED) return BPLUSTRE_FAILED;
    size_t value_sz = _value_sz / sizeof(wchar_t);
    value = new wchar_t[value_sz];
    memset(value, 0, _value_sz);
    wcscat_s(value, value_sz, (wchar_t*)_value);
    return BPLUSTRE_SUCCESS;
}*/
}
