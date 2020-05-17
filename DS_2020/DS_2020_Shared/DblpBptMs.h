#pragma once
#include "pch.h"
#include "Info.h"
#include "Author.h"
#include "Solver.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

struct InfoKey {
    wchar_t a[150];
    InfoKey() { memset(a, 0, sizeof(a)); }
    InfoKey(const MYSTR &_s) {
        int len = min(149, (int)wcslen(_s));
        for (int i = 0; i < len; i++) a[i] = ((const wchar_t *) _s)[i];
        a[len] = L'\0';
    }
    bool operator != (const InfoKey& x) const {
        return wcscmp(a, x.a) != 0;
    }
    bool operator < (const InfoKey& x) const {
        return wcscmp(a, x.a) < 0;
    }
    bool operator == (const InfoKey& x) const {
        return wcscmp(a, x.a) == 0;
    }
};

struct AuthorKey {
    wchar_t a[50];
    AuthorKey() { memset(a, 0, sizeof(a)); }
    AuthorKey(const MYSTR &_s) {
        int len = min(49, (int)wcslen(_s));
        for (int i = 0; i < len; i++) a[i] = ((const wchar_t*)_s)[i];
        a[len] = L'\0';
    }
    bool operator != (const AuthorKey& x) const {
        return wcscmp(a, x.a) != 0;
    }
    bool operator < (const AuthorKey& x) const {
        return wcscmp(a, x.a) < 0;
    }
    bool operator == (const AuthorKey& x) const {
        return wcscmp(a, x.a) == 0;
    }
};

class DblpBptMs : public ISolver
{
public:
	DblpBptMs(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist = FILE_Status::NON_EXIST);
	~DblpBptMs();
	void InitMemory();
	void InsertObject(Info& _info);
	
//private:
	DS_BPlusTree::BPlusTree<InfoKey> info_bpt;
	DS_BPlusTree::BPlusTree<AuthorKey> author_bpt;
	unsigned int info_data_block_sz;
	unsigned int author_data_block_sz;
	void insertInfo(Info&);
	void updateAuthor(Author&);
    std::pair<OPRESULT, Author> getAuthorByName(const MYSTR&);
    std::pair<OPRESULT, Info> getInfoByTitle(const MYSTR&);

#ifdef TEST_DEBUG
    int info_cnt;
#endif
};