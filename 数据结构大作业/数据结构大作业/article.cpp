#include "pch.h"
#include "Article.h"

Article::Article()
{
	mdate = key = title = journal 
		= volume = month = year = ee = "";
	this->authors = new std::vector<bstr_t>;
}

Article::~Article() 
{
	delete authors;
}

Article::Article(
	bstr_t mdate, bstr_t key, bstr_t title, bstr_t journal,
	bstr_t volume, bstr_t month, bstr_t year, bstr_t cdrom,
	bstr_t ee, std::vector<bstr_t> authors
) 
{
	this->mdate = mdate.copy();
	this->key = key.copy();
	this->title = title.copy();
	this->journal = journal.copy();
	this->volume = volume.copy();
	this->month = month.copy();
	this->year = year.copy();
	this->cdrom = cdrom.copy();
	this->ee = ee.copy();

	this->authors = new std::vector<bstr_t>;
	for (int i = 0; i < authors.size(); i++) {
		this->authors->push_back(authors.at(i).copy());
	}
}

OPRESULT Article::Getmdate(bstr_t* pmdate) 
{
	if (!pmdate) { CHK_OR(E_InvalidParameter); }

	*pmdate = mdate.copy();
	return 0;
}
OPRESULT Article::Getkey(bstr_t*pkey) 
{	
	if (!pkey) { CHK_OR(E_InvalidParameter); }

	*pkey = key.copy();
	return 0;
}

OPRESULT Article::Gettitle(bstr_t*ptitle)
{
	if (!ptitle) { CHK_OR(E_InvalidParameter); }

	*ptitle = title.copy();
	return 0;
}
OPRESULT Article::Getjournal(bstr_t*pjournal)
{
	if (!pjournal) { CHK_OR(E_InvalidParameter); }
	*pjournal = journal.copy();
	return 0;
}
OPRESULT Article::Getvolume(bstr_t*pvolume)
{
	if (!pvolume) { CHK_OR(E_InvalidParameter); }

	*pvolume = volume.copy();
	return 0;
}
OPRESULT Article::Getmonth(bstr_t*pmonth)
{
	if (!pmonth) { CHK_OR(E_InvalidParameter); }
	
	*pmonth = month.copy();
	return 0;
}
OPRESULT Article::Getyear(bstr_t*pyear)
{
	if (!pyear) { CHK_OR(E_InvalidParameter); }
	*pyear = year.copy();

	return 0;
}
OPRESULT Article::Getcdrom(bstr_t*pcdrom)
{
	if (!pcdrom) { CHK_OR(E_InvalidParameter); }
	*pcdrom = cdrom.copy();
	return 0;
}
OPRESULT Article::Getee(bstr_t*pee)
{
	if (!pee) { CHK_OR(E_InvalidParameter); }

	*pee=ee.copy();
	return 0;
}
OPRESULT Article::Getauthors(std::vector<bstr_t>* &pauthors)
{
	pauthors = new std::vector<bstr_t>;
	for (int i = 0; i < authors->size(); i++) {
		(pauthors)->push_back(authors->at(i).copy());
	}
	return 0;
}
OPRESULT Article::Setmdate(bstr_t mdate) {
	this->mdate = mdate.copy();
	return 0;
}
OPRESULT Article::Setkey(bstr_t key) {
	this->key = key.copy();
	return 0;
}
OPRESULT Article::Settitle(bstr_t title) {
	this->title = title.copy();
	return 0;
}
OPRESULT Article::Setjournal(bstr_t journal) {
	this->journal = journal.copy();
	return 0;
}
OPRESULT Article::Setvolume(bstr_t volume) {
	this->volume = volume.copy();
	return 0;
}
OPRESULT Article::Setmonth(bstr_t month) {
	this->month = month.copy();
	return 0;
}
OPRESULT Article::Setyear(bstr_t year) {
	this->year = year.copy();
	return 0;
}
OPRESULT Article::Setcdrom(bstr_t cdrom) {
	this->cdrom = cdrom.copy();
	return 0;
}
OPRESULT Article::Setee(bstr_t ee) {
	this->ee = ee.copy();
	return 0;
}
OPRESULT Article::Setauthors(std::vector<bstr_t> authors) {
	this->authors = new std::vector<bstr_t>;
	for (int i = 0; i < authors.size(); i++) {
		this->authors->push_back(authors.at(i).copy());
	}
	return 0;
}
OPRESULT Article::Addauthors(bstr_t name) {
	authors->push_back(name.copy());
	return 0;
}
