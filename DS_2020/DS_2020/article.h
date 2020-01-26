#pragma once
#include <windows.h>
#include <vector>

#include "error.h"

/*
´æ´¢µ¥¸öarticleµÄclass
*/

class Article {
protected:
	LPCWSTR mdate;
	LPCWSTR key;
	LPCWSTR title;
	LPCWSTR journal;
	LPCWSTR volume;
	LPCWSTR month;
	LPCWSTR year;
	LPCWSTR cdrom;
	LPCWSTR ee;
	std::vector<LPCWSTR> *authors;
public:
	Article();
	Article(
		LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR,
		LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, 
		LPCWSTR, std::vector<LPCWSTR> *
	);
	~Article();
	OPRESULT Getmdate(LPCWSTR*);
	OPRESULT Getkey(LPCWSTR*);
	OPRESULT Gettitle(LPCWSTR*);
	OPRESULT Getjournal(LPCWSTR*);
	OPRESULT Getvolume(LPCWSTR*);
	OPRESULT Getmonth(LPCWSTR*);
	OPRESULT Getyear(LPCWSTR*);
	OPRESULT Getcdrom(LPCWSTR*);
	OPRESULT Getee(LPCWSTR*);
	OPRESULT Getauthors(std::vector<LPCWSTR>**);

	OPRESULT Setmdate(LPCWSTR);
	OPRESULT Setkey(LPCWSTR);
	OPRESULT Settitle(LPCWSTR);
	OPRESULT Setjournal(LPCWSTR);
	OPRESULT Setvolume(LPCWSTR);
	OPRESULT Setmonth(LPCWSTR);
	OPRESULT Setyear(LPCWSTR);
	OPRESULT Setcdrom(LPCWSTR);
	OPRESULT Setee(LPCWSTR);
	OPRESULT Setauthors(std::vector<LPCWSTR>*);

	OPRESULT Addauthors(LPCWSTR);
};