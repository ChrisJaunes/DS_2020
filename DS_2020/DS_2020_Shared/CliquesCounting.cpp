#include "pch.h"
#include "CliquesCounting.h"
#include <fstream>

#define STR bstr_t
#define W ULONG64


void CliquesCounting::initial() {

	std::ofstream file(DS_F5_TEST_OUT,std::ios::trunc);
	file.close();
	/*
	std::ifstream readfile;
	readfile.open(DS_F5_TEST_OUT);
	std::string text; 
	std::string::size_type pos1, pos2, i;
	while (!readfile.eof() && std::getline(readfile, text, '\n')) {
		pos2 = text.find(" ");
		pos1 = 0;
		W x1 = 0, x2 = 0;
		for (i = pos1; i < pos2; i++)
			x1 = x1 * 10 + (text[i] - '0');
		for (i = pos2 + 1; i < text.length(); i++)
			x2 = x2 * 10 + (text[i] - '0');
		CliquesCount[x1] = x2;
	}
	readfile.close();
	*/
}

void CliquesCounting::writefile_f5result() {
	std::ofstream os;
	os.open(DS_F5_TEST_OUT);

	for (auto &it : CliquesCount)
		os << it.first << " " << it.second << "\n";
	
	os.close();
}

void CliquesCounting::readfile_f5result() {
	std::ifstream readfile;
	readfile.open(DS_F5_TEST_OUT);

	CliquesCount.clear();
	
	std::string text;
	std::string::size_type pos1, pos2, i;
	while (!readfile.eof() && std::getline(readfile, text, '\n')) {
		pos2 = text.find(" ");
		pos1 = 0;

		//std::string mp1 = text.substr(pos1, pos2 - pos1);
		//std::string mp2 = text.substr(pos2);

		W x1 = 0, x2 = 0;
		for (i = pos1; i < pos2; i++)
			x1 = x1 * 10 + (text[i] - '0');
		for (i = pos2 + 1; i < text.length(); i++)
			x2 = x2 * 10 + (text[i] - '0');
		CliquesCount[x1] = x2;
	}
	readfile.close();
	//return ret;
}

void CliquesCounting::set_ms(DblpBptMs* Dp) {
	ms = Dp;
}

void CliquesCounting::InsertObject()
{
	for (auto it = ms->author_bpt.begin(); it != ms->author_bpt.end(); ++it)
	{
		auto off = *it;
		void* value = nullptr; size_t value_sz = 0;
		ms->author_bpt.valueFromFileBlock(off, value, value_sz);
		Author author = Author::deserialize((wchar_t*)value);
		
		Insert(&author);

#ifdef TEST_DEBUG
		ins_cnt++;
		printf("%d\n", ins_cnt);
#endif		
	}
}
OPRESULT CliquesCounting::Insert(Author* st) {

	std::vector<STR>* Collaborators = nullptr;
	*Collaborators = st->GetCollaboratorsNoWeight().second;

	CliquesCount[1]++;

	std::vector<Author* > Q; 
	Q.push_back(st);

	W l = 0, r = Collaborators->size()-1, ret = 0;
	while (l <= r)
	{
		W mid = (l + r) >> 1;
		if (st->GetName().second < Collaborators->at(mid)) {
			ret = mid;
			r = mid - 1;
		}
		else l = mid + 1;
	}

	for (W i = ret; i < Collaborators->size(); i++)
	{
		Q[1]= &(ms->getAuthorByName(Collaborators->at(i)).second);
		Counting(Q, 2, i+1);
	}
	return 0;
}

//≈–∂œx «∑Ò‘⁄cot÷–
bool CliquesCounting::Check(std::vector<STR>* cot,STR x) {

	std::vector<STR>::iterator it = find(cot->begin(), cot->end(), x);

	if (it != cot->end()) return true;
	return false;
	
	/* 
	l = 0, r = cot->size() - 1, ret = 0;
	while (l <= r)
	{
		uint16_t mid = (l + r) >> 1;
		if (cot->at(mid) >= x) {
			ret = mid;
			r = mid - 1;
		}
		else l = mid + 1;
	}
	if (cot->at(ret) != x) return false;
	return true;
	*/
}

OPRESULT CliquesCounting::Counting(std::vector<Author* > Clique, W Size, W st) {

	std::vector<STR>* Collaborators = nullptr;
	*Collaborators = Clique[0]->GetCollaboratorsNoWeight().second;

	CliquesCount[Size]++;

	for (W i = st; i < Collaborators->size(); i++)
	{
		Author now;
		now = ms->getAuthorByName(Collaborators->at(i)).second;
		
		STR name = now.GetName().second;
		bool bk = true;
		for (W j = 0; j < Size; j++)
		{
			std::vector<STR>* cot = nullptr;
			*cot = Clique[j]->GetCollaboratorsNoWeight().second;

			if (!Check(cot, name)) {
				bk = false;
				break;
			}
		}

		if (bk) {
			Clique[Size] = &now;
			Counting(Clique, Size + 1, i + 1);
		}
	}
	return 0;
}
/*
W CliquesCounting::GetSize() {
	return MaxSize;
}
*/
std::map<W, W> CliquesCounting::GetResult() 
{
	return CliquesCount;
}