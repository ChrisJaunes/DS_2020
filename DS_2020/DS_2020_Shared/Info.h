#pragma once
#include "pch.h"
#include "config.h"
// update: title为必要属性
class Info {
protected:
	// 标识当前大类属性
	MYSTR clsid;
	std::map<MYSTR, std::vector<MYSTR>>* properties;
public:
	Info();
	Info(const Info&);
	Info& operator=(const Info&);
	~Info();

	// 初始化函数
	void AddProperty(MYSTR, MYSTR);
	void SetClsid(MYSTR);

	// 获取属性
	MYSTR GetClsid();
	std::vector<MYSTR> GetProperty(MYSTR);
	std::map<MYSTR, std::vector<MYSTR>> GetProperties();

	wchar_t* serialize();
	static Info deserialize(MYSTR);
#ifdef TEST_DEBUG_INFO
	static unsigned int Info_cnt;
#endif
};