#pragma once
#include "pch.h"
/**
 * @ChrisJaunes_hjj
 * 从dblp.xml创建缓存文件
 * xmlfile: dblp.xml文件，在config中定义默认位置
 * parseInfo为解析类型，在config中定义相关宏
 * 
**/
void generate_cache(const wchar_t* xmlfile, const DWORD parseInfo);
