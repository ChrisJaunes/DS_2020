#pragma once
#include "pch.h"
#define STR bstr_t

// update: title为必要属性
class Info {
protected:
	// 标识当前大类属性
	STR clsid;
	std::map<STR, std::vector<STR>>* properties;
public:
	Info();
	Info(const Info&);
	~Info();

	// 初始化函数
	void AddProperty(STR, STR);
	void SetClsid(STR);

	// 获取属性
	STR GetClsid();
	std::vector<STR> GetProperty(STR);
	std::map<STR, std::vector<STR>> GetProperties();

	// 支持以前的接口

};