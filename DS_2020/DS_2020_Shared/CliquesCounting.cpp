#include "pch.h"
#include "CliquesCounting.h"
#include <fstream>

void CliquesCounting::pre_C(int lim) {
	//memset(C, 0, sizeof(C));
	//C[1][0] = 1; C[1][1] = 1;
	fz(C[1][0], 1); fz(C[1][1], 1);
	for (int i = 2; i <= lim; i++)
	{
		//C[i][0] = 1;
		fz(C[i][0], 1);
		for (int j = 1; j <= i; j++)
		{
			C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
			//printf("C(%d,%d)=%lld\n", i, j, f[i][j]);
		}
	}
}
CliquesCounting::CliquesCounting() {
	//CliquesCount.clear();
	for (int i = 0; i < 300; i++) fz(CC_test[i], 0);
	max_size = 0;
	ms = nullptr;
	ins_cnt = 0;
	pre_C(295);
};
CliquesCounting::CliquesCounting(DblpBptMs* Dp) {
	CliquesCount.clear();
	ms = Dp;
	ins_cnt = 0;
	pre_C(295);
};
void CliquesCounting::initial() {

	std::ofstream file(DS_F5_TEST_OUT, std::ios::trunc);
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
void CliquesCounting::writefile_f5result(int x) {
	std::ofstream os;
	os.open(DS_F5_TEST_OUT);

	os << x << "\n";
	/*//for (auto &it : CC_test)
	for (auto &it : CliquesCount)
	os << it.first << " " << it.second << "\n";*/
	for (int i = 1; i < 300; i++)
	{
		if (CC_test[i] == 0) break;
		os << i << " " << CC_test[i] << "\n";
	}
	os.close();
}
int CliquesCounting::readfile_f5result(int flag) {
	std::ifstream readfile;
	readfile.open(DS_F5_TEST_OUT);

	CliquesCount.clear();

	std::string text;
	std::string::size_type pos1, pos2, i;

	int ret = 0;
	std::getline(readfile, text, '\n');
	for (i = 0; i < text.length(); i++)
		ret = ret * 10 + (text[i] - '0');

	while (!readfile.eof() && std::getline(readfile, text, '\n')) {
		pos2 = text.find(" ");
		pos1 = 0;

		//std::string mp1 = text.substr(pos1, pos2 - pos1);
		//std::string mp2 = text.substr(pos2);

		MYW x1 = 0, x2 = 0;
		for (i = pos1; i < pos2; i++)
			x1 = x1 * 10 + (text[i] - '0');
		for (i = pos2 + 1; i < text.length(); i++)
			x2 = x2 * 10 + (text[i] - '0');
		CliquesCount[x1] = x2;
	}
	readfile.close();
	return ret;
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

		MYW x1 = 0, x2 = 0;
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
	int ret = readfile_f5result(0);
	//initial();
	for (auto it = ms->author_bpt.begin(); it != ms->author_bpt.end(); ++it)
	{
		if (ret <= ins_cnt)
		{
			auto off = *it;
			void* value = nullptr; size_t value_sz = 0;
			ms->author_bpt.valueFromFileBlock(off, value, value_sz);
			Author author = Author::deserialize((wchar_t*)value);

			Insert(&author);
			free(value);
		}

#ifdef TEST_DEBUG
		ins_cnt++;
		printf("%d\n", ins_cnt);
		if (ret <= ins_cnt)
		{
			initial();
			writefile_f5result(ins_cnt);
		}
#endif		
	}
}

/*
void CliquesCounting::CountDir() {
int ret = readfile_f5result(0);
for (auto it = ms->info_bpt.begin(); it != ms->info_bpt.end(); ++it)
{
if (ret <= ins_cnt) {
auto off = *it;
void* value = nullptr; size_t value_sz = 0;
ms->info_bpt.valueFromFileBlock(off, value, value_sz);
Info _info = Info::deserialize((wchar_t*)value);

auto authors = _info.GetProperty(L"author");
int w = authors.size();
for (int i = 1; i <= w; i++) {
CliquesCount[i] = CliquesCount[i] + C[w][i];
}
free(value);
}
#ifdef TEST_DEBUG
ins_cnt++;
//printf("%d\n", ins_cnt);
if (ins_cnt % 50000 == 0 && ret<=ins_cnt)
{
printf("%d\n", ins_cnt);
initial();
writefile_f5result(ins_cnt);
}
//if (ins_cnt - ret >= 400000) break;
#endif
}
initial();
writefile_f5result(ins_cnt);
}
*/
void CliquesCounting::insert(Info& temp) {

	auto authors = temp.GetProperty(L"author");
	int w = (int)authors.size();
	for (int i = 1; i <= w; i++) {
		//CliquesCount[i] = (CliquesCount[i]+C[w][i]);
		CC_test[i] = (CC_test[i] + C[w][i]);
	}
	max_size = (w > max_size) ? w : max_size;
#ifdef TEST_DEBUG
	ins_cnt++;
	//printf("%d\n", ins_cnt);
	if (ins_cnt % 10000 == 0)
	{
		printf("%d %d\n", ins_cnt, max_size);
		initial();
		writefile_f5result(ins_cnt);
	}
	//if (ins_cnt - ret >= 400000) break;
#endif
}

bool b_Comp(MYSTR str_x, MYSTR str_y) {
	std::string x = (char*)str_x;
	std::string y = (char*)str_y;
	return x < y;
}
OPRESULT CliquesCounting::Insert(Author* st) {

	/*std::vector<STR>* Collaborators = nullptr;
	*Collaborators = st->GetCollaboratorsNoWeight().second;*/
	std::vector<MYSTR> Collaborators = st->GetCollaboratorsNoWeight().second;

	CliquesCount[1]++;

	std::vector<Author* > Q;
	Q.push_back(st);

	MYW l = 1, r = Collaborators.size() - 1, ret = -1;
	while (l <= r)
	{
		MYW mid = (l + r) >> 1;
		if (b_Comp(st->GetName().second , Collaborators[mid])){
		//if (((char*)st->GetName().second) < ((char*)Collaborators[mid])) {
			ret = mid;
			r = mid - 1;
		}
		else l = mid + 1;
	}
	if (ret != -1) {
		for (MYW i = ret; i < Collaborators.size(); i++)
		{
			Author temp = ms->getAuthorByName(Collaborators[i]).second;
			Q.push_back(&temp);
			Counting(Q, 2, i + 1);
			Q.pop_back();
		}
	}
	return 0;
}

//≈–∂œx «∑Ò‘⁄cot÷–
bool CliquesCounting::Check(std::vector<MYSTR>* cot, MYSTR x) {

	std::vector<MYSTR>::iterator it = find(cot->begin(), cot->end(), x);

	if (it != cot->end()) return true;
	return false;

}

OPRESULT CliquesCounting::Counting(std::vector<Author* > Clique, MYW Size, MYW st) {

	/*std::vector<STR>* Collaborators = nullptr;
	*Collaborators = Clique[0]->GetCollaboratorsNoWeight().second;*/
	std::vector<MYSTR> Collaborators = Clique[0]->GetCollaboratorsNoWeight().second;

	CliquesCount[Size]++;

	for (MYW i = st; i < Collaborators.size(); i++)
	{
		Author now;
		now = ms->getAuthorByName(Collaborators[i]).second;

		MYSTR name = now.GetName().second;
		bool bk = true;
		for (MYW j = 1; j < Size; j++)
		{
			/*std::vector<STR>* cot = nullptr;
			*cot = Clique[j]->GetCollaboratorsNoWeight().second;*/
			std::vector<MYSTR> cot = Clique[j]->GetCollaboratorsNoWeight().second;

			if (!Check(&cot, name)) {
				bk = false;
				break;
			}
		}

		if (bk) {
			Clique.push_back(&now);
			Counting(Clique, Size + 1, i + 1);
			Clique.pop_back();
		}
	}
	return 0;
}
/*
W CliquesCounting::GetSize() {
return MaxSize;
}
*/
std::map<MYW, MYW> CliquesCounting::GetResult()
{
	return CliquesCount;
}
int CliquesCounting::getcnt() {
	return ins_cnt;
}

std::map<MYW, MYSTR> CliquesCounting::GetResult_2()
{
	std::ifstream readfile;
	readfile.open(DS_F5_TEST_OUT);

	std::map<MYW, MYSTR> ret;

	std::string text;
	std::string::size_type pos1, pos2, i;
	while (!readfile.eof() && std::getline(readfile, text, '\n')) {
		pos2 = text.find(" ");
		pos1 = 0;

		//std::string mp1 = text.substr(pos1, pos2 - pos1);
		//std::string mp2 = text.substr(pos2);
		MYW x1 = 0;
		for (i = pos1; i < pos2; i++)
			x1 = x1 * 10 + (text[i] - '0');
		ret[x1]=text.substr(pos2).c_str();
	}
	readfile.close();
	//return ret;
	return ret;
}