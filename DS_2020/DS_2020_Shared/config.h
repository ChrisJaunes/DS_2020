#pragma once
#define MYSTR bstr_t

#define DS_BPT_TEST_DB  L"../DS_2020_Test/test_file/test.bpdb" 
#define DS_BPT_TEST_ALS L"../DS_2020_Test/test.als"
#define DS_BPT_TEST_IN  L"../DS_2020_Test/test_file/test.in"
#define DS_BPT_TEST_IN2 L"../DS_2020_Test/test_file/test.in2"
#define DS_BPT_TEST_OUT L"../DS_2020_Test/test_file/test.out"
#define DS_BPT_TEST_ANS L"../DS_2020_Test/test_file/test.ans"
#define DS_DBLP         L"../DS_2020_Test/test_file/dblp.xml"
#define DS_DBLP_Info    L"../DS_2020_Test/test_file/dblp_info.bpt"
#define DS_DBLP_Author  L"../DS_2020_Test/test_file/dblp_author.bpt"
#define DS_F4_HASHTABLE "../DS_2020_Test/f4/hashtable.txt"
#define DS_F4_HASHTABLE_INFO "../DS_2020_Test/f4/hashtable_info.txt"
#define DS_F4_INFO_TO_TITLE "../DS_2020_Test/f4/info_to_title.dat"
#define DS_F4_TITLE "../DS_2020_Test/f4/title.txt"

// 可以选择的解析对象, 也就是参数parseInfo的值, 其中article和...数量最多
#ifndef ENUMPARSEINFO
#define ENUMPARSEINFO
#define XMLPARSETYPE_article  1
#define XMLPARSETYPE_book 2
#define XMLPARSETYPE_incollection 4
#define XMLPARSETYPE_inproceedings 8
#define XMLPARSETYPE_mastersthesis 16
#define XMLPARSETYPE_phdthesis 32
#define XMLPARSETYPE_proceedings 64
#define XMLPARSETYPE_www 128
#define XMLPARSETYPE_alltypes (XMLPARSETYPE_article | XMLPARSETYPE_book | XMLPARSETYPE_incollection | XMLPARSETYPE_inproceedings | XMLPARSETYPE_mastersthesis | XMLPARSETYPE_phdthesis | XMLPARSETYPE_proceedings | XMLPARSETYPE_www)
#endif // !ENUMPARSEINFO


#define TEST_DEBUG
//Console 控制宏
#define TEST_DEBUG_INFO
#define TEST_DEBUG_AUTHOR
#define TEST_DEBUG_TOP
#define TEST_DEBUG_INFO_TITLE

//GUI 控制宏
//#define TEST_DEBUG_HOTSPOT