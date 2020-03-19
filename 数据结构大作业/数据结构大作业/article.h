#pragma once
#include "pch.h"
#include "error.h"

/*
´æ´¢µ¥¸öarticleµÄclass
*/

class Article {
protected:
	bstr_t mdate;
	bstr_t key;
	bstr_t title;
	bstr_t journal;
	bstr_t volume;
	bstr_t month;
	bstr_t year;
	bstr_t cdrom;
	bstr_t ee;
	std::vector<bstr_t> *authors;
public:
	Article();
	Article(
		bstr_t, bstr_t, bstr_t, bstr_t,
		bstr_t, bstr_t, bstr_t, bstr_t, 
		bstr_t, std::vector<bstr_t>
	);
	~Article();
	OPRESULT Getmdate(bstr_t*);
	OPRESULT Getkey(bstr_t*);
	OPRESULT Gettitle(bstr_t*);
	OPRESULT Getjournal(bstr_t*);
	OPRESULT Getvolume(bstr_t*);
	OPRESULT Getmonth(bstr_t*);
	OPRESULT Getyear(bstr_t*);
	OPRESULT Getcdrom(bstr_t*);
	OPRESULT Getee(bstr_t*);
	OPRESULT Getauthors(std::vector<bstr_t>*&);

	OPRESULT Setmdate(bstr_t);
	OPRESULT Setkey(bstr_t);
	OPRESULT Settitle(bstr_t);
	OPRESULT Setjournal(bstr_t);
	OPRESULT Setvolume(bstr_t);
	OPRESULT Setmonth(bstr_t);
	OPRESULT Setyear(bstr_t);
	OPRESULT Setcdrom(bstr_t);
	OPRESULT Setee(bstr_t);
	OPRESULT Setauthors(std::vector<bstr_t>);

	OPRESULT Addauthors(bstr_t);

	static Article getArticles(bstr_t p) {}
};