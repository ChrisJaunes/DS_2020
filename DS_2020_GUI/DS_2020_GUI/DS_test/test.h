#pragma once
//本文件用于测试
#ifndef TEST_H
#define TEST_H
#define TEST_DEBUG
#define TEST_DEBUG_INFO
#define TEST_DEBUG_AUTHOR
#define TEST_DEBUG_TOP
#define TEST_DEBUG_INFO_TITLE
//#define TEST_DEBUG_HOTSPOT
#ifdef TEST_DEBUG
namespace FST {
	void tmain();
}
#endif

#endif