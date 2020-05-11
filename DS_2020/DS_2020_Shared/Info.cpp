#include "pch.h"
#include "config.h"
#include "Info.h"
#include "InfoMarshal.h"

#ifdef TEST_DEBUG_INFO
unsigned int Info::Info_cnt = 0;
#endif

Info::Info()
	: clsid(L"")
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
	
}

Info::Info(const Info& Src)
	: clsid(Src.clsid)
	, properties(Src.properties)
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
}

Info& Info::operator=(const Info& Src)
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
	clsid = Src.clsid;
	properties = Src.properties;
	return *this;
}

Info::~Info()
{
#ifdef TEST_DEBUG_INFO
	--Info_cnt;
#endif
}

void Info::AddProperty(MYSTR ProPertyName, MYSTR ProPertyValue)
{
	if (properties.count(ProPertyName)) {
		properties.at(ProPertyName).push_back(ProPertyValue);
	}
	else {
		properties.insert(std::make_pair(ProPertyName,std::vector<MYSTR>(1, ProPertyValue)));
	}
}

void Info::SetClsid(MYSTR clsid)
{
	this->clsid = clsid;
}

MYSTR Info::GetClsid()
{
	return MYSTR(clsid);
}

std::vector<MYSTR> Info::GetProperty(MYSTR ProPertyName)
{
	if (properties.count(ProPertyName)) {
		return std::vector<MYSTR>(properties.at(ProPertyName));
	}
	else {
		return std::vector<MYSTR>();
	}
}

std::map<MYSTR, std::vector<MYSTR>> Info::GetProperties()
{
	return properties;
}

wchar_t* Info::serialize()
{
	return InfoMarshal::Marshal(*this);
}

Info Info::deserialize(MYSTR xmlcode)
{
	return InfoMarshal::Unmarshal(xmlcode);
}
