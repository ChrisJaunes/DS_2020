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
 * search: 在B+树里查询key_t， 返回一个vector()
 */
template<typename KEY_T, typename OBJ>
class BPMS{
public:
    BPMS(const wchar_t* filename, int exist = 0);
    void insertMulti(KEY_T key, OBJ obj);
    void insertReplace(KEY_T key, OBJ obj);
    std::vector<OBJ> search(KEY_T);
//private:
    BPlusTree<KEY_T> db;
    size_t value_block_sz;
};

template<typename KEY_T, typename OBJ>
inline BPMS<KEY_T, OBJ>::BPMS(const wchar_t* filepath, int exist)
    : db(filepath, exist)
{
    value_block_sz = db.data_block_size - sizeof(chd_t) * 2;

}

template<typename KEY_T, typename OBJ>
inline void BPMS<KEY_T, OBJ>::insertMulti(KEY_T key, OBJ obj) {
    wchar_t* value = obj.serialize();

    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + value_block_sz - 1) / value_block_sz * value_block_sz;
    void* _value = malloc(_value_sz);
    memset(_value, 0, _value_sz);
    memcpy(_value, value, value_sz);

    insert(key, _value, _value_sz);

    free(_value);
    delete[] value;
}

template<typename KEY_T, typename OBJ>
inline void BPMS<KEY_T, OBJ>::insertReplace(KEY_T key, OBJ obj) {
    wchar_t * value = obj.serialize();
    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + value_block_sz - 1) / value_block_sz * value_block_sz;
    void* _value = malloc(_value_sz);
    memset(_value, 0, _value_sz);
    memcpy(_value, value, value_sz);

    insert(key, _value, _value_sz);

    free(_value);
    delete[] value;
}

template<typename KEY_T, typename OBJ>
inline std::vector<OBJ> BPMS<KEY_T, OBJ>::search(KEY_T key)
{
    void* _value = nullptr;
    size_t _value_sz;
    if (search(key, _value, _value_sz) == BPLUSTRE_FAILED) return vector<OBJ>();
    vector<OBJ> res;
    res.push_back(OBJ::deserialize());
}
