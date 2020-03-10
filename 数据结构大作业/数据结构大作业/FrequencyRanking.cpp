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
		if (top10.size() < 10) {
			if (!top10.count(year)) {
				std::map<bstr_t, uint64_t> *tmp = new std::map<bstr_t, uint64_t>;
				top10[year] = *tmp;
			}
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
		if (top10.size() < 10) {
			if (!top10.count(year)) {
				std::map<bstr_t, uint64_t> *tmp = new std::map<bstr_t, uint64_t>;
				top10[year] = *tmp;
			}
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