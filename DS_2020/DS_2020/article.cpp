#include "article.h"

Article::Article()
{
	mdate = key = title = journal 
		= volume = month = year = ee = NULL;
	this->authors = new std::vector<LPCWSTR>;
}

Article::~Article() 
{
	delete authors;
}

Article::Article(
	LPCWSTR mdate, LPCWSTR key, LPCWSTR title, LPCWSTR journal,
	LPCWSTR volume, LPCWSTR month, LPCWSTR year, LPCWSTR cdrom,
	LPCWSTR ee, std::vector<LPCWSTR> *authors
) 
{
	this->mdate = new WCHAR[wcslen(mdate)+1];
	this->key = new WCHAR[wcslen(key) + 1];
	this->title = new WCHAR[wcslen(title) + 1];
	this->journal = new WCHAR[wcslen(journal) + 1];
	this->volume = new WCHAR[wcslen(volume) + 1];
	this->month = new WCHAR[wcslen(month) + 1];
	this->year = new WCHAR[wcslen(year) + 1];
	this->cdrom = new WCHAR[wcslen(cdrom) + 1];
	this->ee = new WCHAR[wcslen(ee) + 1];

	lstrcpyW((LPWSTR)this->mdate, mdate);
	lstrcpyW((LPWSTR)this->key, key);
	lstrcpyW((LPWSTR)this->title, title);
	lstrcpyW((LPWSTR)this->journal, journal);
	lstrcpyW((LPWSTR)this->volume, volume);
	lstrcpyW((LPWSTR)this->month, month);
	lstrcpyW((LPWSTR)this->year, year);
	lstrcpyW((LPWSTR)this->cdrom, cdrom);
	lstrcpyW((LPWSTR)this->ee, ee);

	this->authors = new std::vector<LPCWSTR>;
	for (int i = 0; i < authors->size(); i++) {
		this->authors->push_back(authors->at(i));
	}
}

OPRESULT Article::Getmdate(LPCWSTR*pmdate) 
{
	if (!pmdate) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pmdate, mdate);
	return 0;
}
OPRESULT Article::Getkey(LPCWSTR*pkey) 
{	
	if (!pkey) { CHK_OR(E_InvalidParameter); }
	lstrcpyW((LPWSTR)*pkey, key);
	return 0;
}
OPRESULT Article::Gettitle(LPCWSTR*ptitle)
{
	if (!ptitle) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*ptitle, title);
	return 0;
}
OPRESULT Article::Getjournal(LPCWSTR*pjournal)
{
	if (!pjournal) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pjournal, journal);
	return 0;
}
OPRESULT Article::Getvolume(LPCWSTR*pvolume)
{
	if (!pvolume) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pvolume, volume); 
	return 0;
}
OPRESULT Article::Getmonth(LPCWSTR*pmonth)
{
	if (!pmonth) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pmonth, month);
	return 0;
}
OPRESULT Article::Getyear(LPCWSTR*pyear)
{
	if (!pyear) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pyear, year);
	return 0;
}
OPRESULT Article::Getcdrom(LPCWSTR*pcdrom)
{
	if (!pcdrom) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pcdrom, cdrom);
	return 0;
}
OPRESULT Article::Getee(LPCWSTR*pee)
{
	if (!pee) { CHK_OR(E_InvalidParameter); }

	lstrcpyW((LPWSTR)*pee, ee);
	return 0;
}
OPRESULT Article::Getauthors(std::vector<LPCWSTR>**pauthors)
{
	if (!pauthors) { CHK_OR(E_InvalidParameter); }

	*pauthors = new std::vector<LPCWSTR>;
	for (int i = 0; i < authors->size(); i++) {
		(*pauthors)->push_back(authors->at(i));
	}
	return 0;
}

OPRESULT Article::Setmdate(LPCWSTR mdate) {
	this->mdate = new WCHAR[wcslen(mdate) + 1];
	lstrcpyW((LPWSTR)this->mdate, mdate);
	return 0;
}
OPRESULT Article::Setkey(LPCWSTR key) {
	this->key = new WCHAR[wcslen(key) + 1];

	lstrcpyW((LPWSTR)this->key, key);
	return 0;
}
OPRESULT Article::Settitle(LPCWSTR title) {
	this->title = new WCHAR[wcslen(title) + 1];
	lstrcpyW((LPWSTR)this->title, title);return 0;
}
OPRESULT Article::Setjournal(LPCWSTR journal) {
	this->journal = new WCHAR[wcslen(journal) + 1];
	lstrcpyW((LPWSTR)this->journal, journal);return 0;
}
OPRESULT Article::Setvolume(LPCWSTR volume) {
	this->volume = new WCHAR[wcslen(volume) + 1];
	lstrcpyW((LPWSTR)this->volume, volume);return 0;
}
OPRESULT Article::Setmonth(LPCWSTR month) {
	this->month = new WCHAR[wcslen(month) + 1];
	lstrcpyW((LPWSTR)this->month, month);return 0;
}
OPRESULT Article::Setyear(LPCWSTR year) {
	this->year = new WCHAR[wcslen(year) + 1];
	lstrcpyW((LPWSTR)this->year, year);return 0;
}
OPRESULT Article::Setcdrom(LPCWSTR cdrom) {
	this->cdrom = new WCHAR[wcslen(cdrom) + 1];
	lstrcpyW((LPWSTR)this->cdrom, cdrom);return 0;
}
OPRESULT Article::Setee(LPCWSTR ee) {
	this->ee = new WCHAR[wcslen(ee) + 1];
	lstrcpyW((LPWSTR)this->ee, ee);return 0;
}
OPRESULT Article::Setauthors(std::vector<LPCWSTR>*authors) {
	this->authors = new std::vector<LPCWSTR>;
	for (int i = 0; i < authors->size(); i++) {
		this->authors->push_back(authors->at(i));
	}
	return 0;
}

OPRESULT Article::Addauthors(LPCWSTR name) {
	LPCWSTR tmp=new WCHAR[wcslen(name)+1];
	lstrcpyW((LPWSTR)tmp, name);
	authors->push_back(tmp);
	return 0;
}
