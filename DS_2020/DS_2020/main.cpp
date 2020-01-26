#include "xmlhelper.h"

/*
项目demo, 单线程, 非常慢, 具体加载要多久我不想去测了...

想要修改为多线程有个问题, ms的xmllite不是线程安全的, 需要注意.

注意一下, 这里只是按照任务书中的内容, 把数据加载到内存

DEBUG编译选项在error.h中打开/关闭
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