#include "pch.h"
#include "xmlhelper.h"
#include "FrequencyRanking.h"
#include "Article.h"
#include "Author.h"
#include "BPTrees.h"
/*
��Ŀdemo, ���߳�, �ǳ���, �������Ҫ����Ҳ���ȥ����...

��Ҫ�޸�Ϊ���߳��и�����, ms��xmllite�����̰߳�ȫ��, ��Ҫע��.

ע��һ��, ����ֻ�ǰ����������е�����, �����ݼ��ص��ڴ�

DEBUG����ѡ����error.h�д�/�ر�

TODO: ���������xmlparser��
*/

//106.766
int _tmain()
{
	DWORD start_time = GetTickCount();
	XMLParser parser;
	parser.OpenFile(L"D:\\Code\\ds_hw\\dblp.xml");
	parser.ParseArticlesToVector();
	std::cout << "count: " << parser.members.size() << std::endl;
	DWORD end_time = GetTickCount();
	std::cout << "The run time is:" << (end_time - start_time) * 1.00 / 1000 << "s!" << std::endl;//�������ʱ��
	system("pause");
	return 0;
}