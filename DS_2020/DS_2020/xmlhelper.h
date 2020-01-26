#pragma once
#include <iostream>
#include <xmllite.h>
#include <shlwapi.h>
#include <windows.h> 
#include <winerror.h>
#include <comdef.h>
#include "error.h"
#include "article.h"

#pragma comment(lib,"Shlwapi.lib")
#pragma comment (lib, "xmllite.lib") 

/*
打开文件

调用xmllite进行dtd解析
*/
class XMLParser {
protected:
	IXmlReader* pReader;
	XmlNodeType nodeType;
public:
	std::vector<Article*> members;
	OPRESULT OpenFile(LPCSTR);
	OPRESULT ParseArticles();


};