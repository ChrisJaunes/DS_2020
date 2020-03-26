#include "pch.h"
#include "FrequencyRanking.h"



OPRESULT FrequencyRanking::Insert(Article *inobj) {
	// 分析单词
	bstr_t title;
	bstr_t left;
	bstr_t year;

	inobj->Getyear(&year);
	inobj->Gettitle(&title);
	CString t(title.GetBSTR());
	int idx= t.Find(_T(" "));


	while (1 && (idx = t.Find(_T(" "))) >= 0) {
		left = t.Left(idx);
		t = t.Right(t.GetLength() - 1 - idx);
		if (CheckIfIgnore(left)) { continue; }
		
		if (!all.count(year)) {
			std::map<bstr_t, uint64_t> *tmp = new std::map<bstr_t, uint64_t>;
			all[year] = *tmp;
		}

		if (all.at(year).count(left)) { all.at(year).at(left) += 1; }
		else { all.at(year)[left] = 1; }

		// 插入到top10
		if (!top10.count(year)) {
			std::map<bstr_t, uint64_t>* tmp = new std::map<bstr_t, uint64_t>;
			top10[year] = *tmp;
		}

		if (top10.at(year).size() < 10) {
			top10.at(year)[left] = all.at(year)[left];
		}
		else {
			std::vector<PAIR>* pPair=_SortTop10(year);
			if (pPair->at(pPair->size() - 1).second < all.at(year)[left]) {
				top10.at(year).erase(pPair->at(pPair->size() - 1).first);
				top10.at(year)[left] = all.at(year)[left];
			}
		}
	}

	if (idx < 0) {
		left = t;
		if (CheckIfIgnore(left)) { return 0; }
		if (!all.count(year)) {
			std::map<bstr_t, uint64_t> *tmp = new std::map<bstr_t, uint64_t>;
			all[year] = *tmp;
		}

		if (all.at(year).count(left)) { all.at(year).at(left) += 1; }
		else { all.at(year)[left] = 1; }

		if (!top10.count(year)) {
			std::map<bstr_t, uint64_t>* tmp = new std::map<bstr_t, uint64_t>;
			top10[year] = *tmp;
		}
		if (top10.at(year).size() < 10) {
			top10.at(year)[left] = all.at(year)[left];
		}
		else {
			std::vector<PAIR>* pPair = _SortTop10(year);
			if (pPair->at(pPair->size() - 1).second < all.at(year)[left]) {
				top10.at(year).erase(pPair->at(pPair->size() - 1).first);
				top10.at(year)[left] = all.at(year)[left];
			}
		}
	}
	return 0;
}

bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) {
	return lhs.second > rhs.second;
}

// 返回名字
std::vector<bstr_t> FrequencyRanking::Get(bstr_t year) {
	std::vector<bstr_t> *ret = new std::vector<bstr_t>;
	if (!top10.count(year)) { return *ret; }

	std::vector<PAIR> *name_score_vec = _SortTop10(year);
	for (int i = 0; i != name_score_vec->size(); ++i) {
		ret->push_back((*name_score_vec)[i].first);
	}
	return *ret;
}

std::vector<PAIR>* FrequencyRanking::_SortTop10(bstr_t year) {
	std::map<bstr_t, uint64_t> sm = top10.at(year);
	std::vector<PAIR> *name_score_vec=new std::vector<PAIR>(sm.begin(), sm.end());
	sort(name_score_vec->begin(), name_score_vec->end(), cmp_by_value);
	return name_score_vec;
}


bool FrequencyRanking::CheckIfIgnore(bstr_t key) {
	std::vector<bstr_t>::iterator it;
	it = std::find(ignores.begin(), ignores.end(), key);
	if (it != ignores.end()) {
		return true;
	}
	else {
		return false;
	}
}

// 数据类接口实现

ULONG64& FrequencyRanking2Data::size() {
	return cb;
}
bstr_t& FrequencyRanking2Data::content() {
	return word;
}
FrequencyRanking2Data::FrequencyRanking2Data() {
	cb = 0;
	word = L"";
}
FrequencyRanking2Data::FrequencyRanking2Data(bstr_t in, ULONG64 cnt) {
	word = in; // 引用计数++
	cb = cnt;
}
FrequencyRanking2Data::FrequencyRanking2Data(const FrequencyRanking2Data& src) {
	word = src.word;
	cb = 1;
}

//比较函数实现
bool CompFrequencyRanking2Data( FrequencyRanking2Data& l,  FrequencyRanking2Data& r) {
	return l.size() > r.size();
}

//新版比较类实现
FrequencyRanking2::FrequencyRanking2() {
	ignores = NULL;
	bakup = new std::map<bstr_t, std::map<bstr_t, ULONG64> >;
}

FrequencyRanking2::FrequencyRanking2(std::vector<bstr_t> ignore) {
	ignores = new std::vector<bstr_t>;
	for (auto i : ignore) {
		ignores->push_back(i);
	}
	bakup = new std::map<bstr_t, std::map<bstr_t, ULONG64> >;
}
FrequencyRanking2::~FrequencyRanking2() {
	if(ignores)
		delete ignores;
	delete bakup;
}
FrequencyRanking2::FrequencyRanking2(const FrequencyRanking2& src) {
	this->bakup = src.bakup;

	ignores = new std::vector<bstr_t>;
	for (auto i : *src.ignores) {
		ignores->push_back(i);
	}
	bakup = new std::map<bstr_t, std::map<bstr_t, ULONG64> >;
	for (auto i : *src.bakup) {
		bakup->insert(i);
	}
}

bool FrequencyRanking2::CheckIfIgnore(bstr_t key) {
	if (ignores == NULL) { return false; }
	std::vector<bstr_t>::iterator it;
	it = std::find(ignores->begin(), ignores->end(), key);
	if (it != ignores->end()) {
		return true;
	}
	else {
		return false;
	}
}

// 只完成存储的过程
OPRESULT FrequencyRanking2::Insert(Article* inobj) {
	bstr_t title;
	bstr_t left;
	bstr_t year;

	inobj->Getyear(&year);
	inobj->Gettitle(&title);
	CString t(title.GetBSTR());
	int idx = t.Find(L" ");

	while (1 && (idx = t.Find(_T(" "))) >= 0) {
		left = t.Left(idx);
		t = t.Right(t.GetLength() - 1 - idx);
		if (CheckIfIgnore(left)) { continue; }

		if (!bakup->count(year)) {
			std::map<bstr_t, ULONG64>*tmp = new std::map<bstr_t, ULONG64>;
			bakup->insert(std::make_pair(year,*tmp));
		}
		
		if (bakup->at(year).count(left)) { bakup->at(year).at(left) += 1; }
		else { bakup->at(year)[left] = 1; }
	}
	left = t;
	if (CheckIfIgnore(left)) { return 0; }
	if (!bakup->count(year)) {
		std::map<bstr_t, ULONG64>* tmp = new std::map<bstr_t, ULONG64>;
		bakup->at(year) = *tmp;
	}

	if (bakup->at(year).count(left)) { bakup->at(year).at(left) += 1; }
	else { bakup->at(year)[left] = 1; }
}

bool CompCb(const PAIR& lhs, const PAIR& rhs) {
	return lhs.second > rhs.second;
}

std::vector<FrequencyRanking2Data> FrequencyRanking2::Get(bstr_t year) {
	std::vector<PAIR> mid(bakup->at(year).begin(), bakup->at(year).end());

	sort(mid.begin(), mid.end(), CompCb);

	std::vector<FrequencyRanking2Data> result;
	for (int i = 0; i < 10; i++) {
		result.push_back(FrequencyRanking2Data(mid[i].first, mid[i].second));
	}
	return result;
}
