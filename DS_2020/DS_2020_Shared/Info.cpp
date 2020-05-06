#include "pch.h"
#include "config.h"
#include "Info.h"
#include "InfoMarshal.h"

#ifdef TEST_DEBUG_INFO
unsigned int Info::Info_cnt = 0;
#endif

Info::Info()
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
	clsid = "";
	properties = new std::map<MYSTR, std::vector<MYSTR>>;
}

Info::Info(const Info& Src)
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
	clsid = Src.clsid;
	properties = new std::map<MYSTR, std::vector<MYSTR>>;
	for (auto i : *Src.properties) {
		properties->insert(i);
	}
}

Info& Info::operator=(const Info& Src)
{
#ifdef TEST_DEBUG_INFO
	++Info_cnt;
#endif
	if (properties != nullptr) delete[] properties;
	clsid = Src.clsid;
	if (Src.deserialize != nullptr) {
		properties = new std::map<MYSTR, std::vector<MYSTR> >(*Src.properties);
	}
	else {
		assert(0);
	}
	return *this;
}

Info::~Info()
{
#ifdef TEST_DEBUG_INFO
	--Info_cnt;
#endif
	delete properties;
}

void Info::AddProperty(MYSTR ProPertyName, MYSTR ProPertyValue)
{
	if (this->properties->count(ProPertyName)) {
		this->properties->at(ProPertyName).push_back(ProPertyValue);
	}
	else {
		std::vector<MYSTR>* tmp = new std::vector<MYSTR>;
		tmp->push_back(ProPertyValue);
		this->properties->insert(std::make_pair(ProPertyName,*tmp));
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
	if (this->properties->count(ProPertyName)) {
		return std::vector<MYSTR>(this->properties->at(ProPertyName));
	}
	else {
		return std::vector<MYSTR>();
	}
}

std::map<MYSTR, std::vector<MYSTR>> Info::GetProperties()
{
	return std::map<MYSTR, std::vector<MYSTR>>(*properties);
}

wchar_t* Info::serialize()
{
	return InfoMarshal::Marshal(*this);
}

Info Info::deserialize(MYSTR xmlcode)
{
	return InfoMarshal::Unmarshal(xmlcode);
}
