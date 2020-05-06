#pragma once
#define MYSTR bstr_t

#define DS_BPT_TEST_DB  L"../DS_2020_Test/test_file/test.bpdb" 
#define DS_BPT_TEST_ALS L"../DS_2020_Test/test.als"
#define DS_BPT_TEST_IN  L"../DS_2020_Test/test_file/test.in"
#define DS_BPT_TEST_IN2 L"../DS_2020_Test/test_file/test.in2"
#define DS_BPT_TEST_OUT L"../DS_2020_Test/test_file/test.out"
#define DS_BPT_TEST_ANS L"../DS_2020_Test/test_file/test.ans"

// ����ѡ��Ľ�������, Ҳ���ǲ���parseInfo��ֵ, ����article��...�������
#ifndef ENUMPARSEINFO
#define ENUMPARSEINFO
#define article 1
#define book 2
#define incollection 4
#define inproceedings 8
#define mastersthesis 16
#define phdthesis 32
#define proceedings 64
#define www 128
#define alltypes (article|book|incollection|inproceedings|mastersthesis|phdthesis|proceedings|www)
#endif // !ENUMPARSEINFO


#define TEST_DEBUG
//Console ���ƺ�
#define TEST_DEBUG_INFO
#define TEST_DEBUG_AUTHOR
#define TEST_DEBUG_TOP
#define TEST_DEBUG_INFO_TITLE

//GUI ���ƺ�
//#define TEST_DEBUG_HOTSPOT