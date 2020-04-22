#include "pch.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"
/* @by ChrisJaunes
 * ����ʵ�ֶ�B+���ķ�װ
 * ����T, ����ʵ�֣�
 * key_t get_key(wchar_t*&) ����һ��key_t����, ����key_t������BPlusTreeUtils������
 * serialize(wchar_t*&), ���л�����
 * deserializ(const wchar_t*), �����л�����
 * ����õ����飬���ʵ��operator=(T)
 *
 * insert: ��B+�������һ��T
 * search: ��B+�����ѯkey_t�� ����һ��vector()
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
