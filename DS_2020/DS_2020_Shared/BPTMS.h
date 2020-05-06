#pragma once
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
class BPTMS{
public:
    BPTMS(const wchar_t* filename, FILE_Status exist = FILE_Status::NON_EXIST);
    void insertMulti(KEY_T key, OBJ obj);
    void insertReplace(KEY_T key, OBJ obj);
    std::vector<OBJ> search(KEY_T);
//private:
    DS_BPlusTree::BPlusTree<KEY_T> db;
    size_t value_block_sz;
};

template<typename KEY_T, typename OBJ>
inline BPTMS<KEY_T, OBJ>::BPTMS(const wchar_t* filepath, FILE_Status exist)
    : db(filepath, exist)
{
    value_block_sz = db.data_block_size - sizeof(OFF_T) * 2;

}

template<typename KEY_T, typename OBJ>
inline void BPTMS<KEY_T, OBJ>::insertMulti(KEY_T key, OBJ obj) {
    /*std::vector<wchar_t*> ress = searchBase(KEY_T);
    ress.push_back(obj.serialize());
    std::vector<unsigned int> resf(ress.size());
    size_t obj_sz = 0;
    for (size_t i = 0; i < ress.size(); i++) {
        resf[i] = sizeof(unsigned int) + (wcslen(ress[i]) + 1) * sizeof(wchar_t);
        obj_sz += resf[i];
    }
    size_t value_sz = obj_sz / value_block_sz + 1;
    void* value = malloc(value_sz);
    memset(value, 0, value_sz);

    for (size_t i = 0; i < ress.size(); i++) {
        unsigned int objf = resf[i];
        wchar_t* objs = ress[i];
        memmove((char*)value + off, &objf, sizeof(objf)); off += sizeof(objf);
        memmove((char*)value + off, &objf, (size_t)objf); off += objf;
    }

    insert(key, value, value_sz);

    free(value);
    delete[] value;*/
}

template<typename KEY_T, typename OBJ>
inline void BPTMS<KEY_T, OBJ>::insertReplace(KEY_T key, OBJ obj) {
    wchar_t * value = obj.serialize();
    size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
    size_t _value_sz = (value_sz + value_block_sz - 1) / value_block_sz * value_block_sz;
    void* _value = malloc(_value_sz);
    memset(_value, 0, _value_sz);
    memcpy(_value, value, value_sz);

    db.insert(key, _value, _value_sz);

    free(_value);
    delete[] value;
}

template<typename KEY_T, typename OBJ>
inline std::vector<OBJ> BPTMS<KEY_T, OBJ>::search(KEY_T key)
{
    void* _value = nullptr;
    size_t _value_sz;
    if (db.search(key, _value, _value_sz) == DS_BPlusTree::BPT_Res::FAILED) return std::vector<OBJ>();
    std::vector<OBJ> res;
    res.push_back(OBJ::deserialize((wchar_t*)_value));
    free(_value);
    return res;
}
