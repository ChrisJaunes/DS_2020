#include "pch.h"
#include "Info.h"

Info::Info()
{
	clsid = "";
	properties = new std::map<STR, std::vector<STR>>;
}

Info::Info(const Info& Src)
{
	clsid = Src.clsid;
	properties = new std::map<STR, std::vector<STR>>;
	for (auto i : *Src.properties) {
		properties->insert(i);
	}
}

Info::~Info()
{
	delete properties;
}

void Info::AddProperty(STR ProPertyName, STR ProPertyValue)
{
	if (this->properties->count(ProPertyName)) {
		this->properties->at(ProPertyName).push_back(ProPertyValue);
	}
	else {
		std::vector<STR>* tmp = new std::vector<STR>;
		tmp->push_back(ProPertyValue);
		this->properties->insert(std::make_pair(ProPertyName,*tmp));
	}
}

void Info::SetClsid(STR clsid)
{
	this->clsid = clsid;
}

STR Info::GetClsid()
{
	return STR(clsid);
}

std::vector<STR> Info::GetProperty(STR ProPertyName)
{
	if (this->properties->count(ProPertyName)) {
		return std::vector<STR>(this->properties->at(ProPertyName));
	}
	else {
		return std::vector<STR>();
	}
}

std::map<STR, std::vector<STR>> Info::GetProperties()
{
	return std::map<STR, std::vector<STR>>(*properties);
}

#include "xmlhelper.h"
wchar_t* Info::serialize()
{
	return XMLMarshal::Marshal(*this);
}

Info Info::deserialize(STR xmlcode)
{
	return XMLMarshal::Unmarshal(xmlcode);
}
