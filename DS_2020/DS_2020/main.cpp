#include "xmlhelper.h"

/*
��Ŀdemo, ���߳�, �ǳ���, �������Ҫ����Ҳ���ȥ����...

��Ҫ�޸�Ϊ���߳��и�����, ms��xmllite�����̰߳�ȫ��, ��Ҫע��.

ע��һ��, ����ֻ�ǰ����������е�����, �����ݼ��ص��ڴ�

DEBUG����ѡ����error.h�д�/�ر�
*/
int
main(int argc, char** argv)
{
	XMLParser parser;
	parser.OpenFile("D:\\Code\\ds_hw\\dblp.xml");
	parser.ParseArticles();
	puts("ok!");

	return 0;
}