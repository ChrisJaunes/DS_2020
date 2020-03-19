#include "pch.h"
#include "xmlhelper.h"
#include "FrequencyRanking.h"
#include "Article.h"
#include "Author.h"
#include "BPTrees.h"
/*
项目demo, 单线程, 非常慢, 具体加载要多久我不想去测了...

想要修改为多线程有个问题, ms的xmllite不是线程安全的, 需要注意.

注意一下, 这里只是按照任务书中的内容, 把数据加载到内存

DEBUG编译选项在error.h中打开/关闭

TODO: 将分类加在xmlparser中
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
	std::cout << "The run time is:" << (end_time - start_time) * 1.00 / 1000 << "s!" << std::endl;//输出运行时间
	system("pause");
	return 0;
}