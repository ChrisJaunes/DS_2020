#pragma once
#include "pch.h"
#define STR bstr_t

// update: titleΪ��Ҫ����
class Info {
protected:
	// ��ʶ��ǰ��������
	STR clsid;
	std::map<STR, std::vector<STR>>* properties;
public:
	Info();
	Info(const Info&);
	~Info();

	// ��ʼ������
	void AddProperty(STR, STR);
	void SetClsid(STR);

	// ��ȡ����
	STR GetClsid();
	std::vector<STR> GetProperty(STR);
	std::map<STR, std::vector<STR>> GetProperties();

	// ֧����ǰ�Ľӿ�

};