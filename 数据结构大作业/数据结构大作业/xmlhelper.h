#pragma once
#include "pch.h"
#include "error.h"
#include "article.h"

#include <xmllite.h>
#include <shlwapi.h>
#pragma comment (lib, "xmllite.lib") 
#pragma comment(lib,"Shlwapi.lib")

/*
���ļ�

����xmllite����dtd����
*/
class XMLParser {
protected:
	IXmlReader* pReader;
	XmlNodeType nodeType;
public:
	std::vector<Article*> members;
	OPRESULT OpenFile(LPCWSTR);
	OPRESULT ParseArticlesToVector();
	OPRESULT ParseAll();

};