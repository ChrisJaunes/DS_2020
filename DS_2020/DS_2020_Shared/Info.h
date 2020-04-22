#pragma once
#include "pch.h"
#include "config.h"
// update: titleΪ��Ҫ����
class Info {
protected:
	// ��ʶ��ǰ��������
	MYSTR clsid;
	std::map<MYSTR, std::vector<MYSTR>>* properties;
public:
	Info();
	Info(const Info&);
	Info& operator=(const Info&);
	~Info();

	// ��ʼ������
	void AddProperty(MYSTR, MYSTR);
	void SetClsid(MYSTR);

	// ��ȡ����
	MYSTR GetClsid();
	std::vector<MYSTR> GetProperty(MYSTR);
	std::map<MYSTR, std::vector<MYSTR>> GetProperties();

	wchar_t* serialize();
	static Info deserialize(MYSTR);
#ifdef TEST_DEBUG_INFO
	static unsigned int Info_cnt;
#endif
};