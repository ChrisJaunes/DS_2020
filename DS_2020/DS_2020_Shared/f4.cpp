#include"pch.h"
#include"f4.h"
#include<fstream>

void f4func::initial() {
	std::ifstream readfile;
	readfile.open(DS_F4_HASHTABLE);
	std::string text; std::string::size_type pos1, pos2;
	while (!readfile.eof() && std::getline(readfile, text, '\n')) {
		pos2 = text.find(" ");
		pos1 = 0;
		this->hashtable[text.substr(pos1, pos2 - pos1)] = text.substr(pos2);
	}
	readfile.close();
}
void f4func::writefile_hashtable() {
	std::ofstream os1, os2;
	os1.open(DS_F4_HASHTABLE);
	os2.open(DS_F4_HASHTABLE_INFO);
	std::string str; int last_size = 0;
	std::unordered_map<std::string, std::string>::iterator it = this->hashtable.begin();
	while (it != this->hashtable.end()) {
		std::string s1 = last_size + "\n";
		str = it->first + " " + s1;
		os1 << str;
		os2 << it->second + "\n";
		last_size = (int)(last_size + it->second.size() + 1);
		it++;
	}
	os1.close();
	os2.close();
}
void f4func::writefile_title(int&p, std::vector<std::string>&x) {
	std::ofstream os;
	os.open("title.txt");
	int i = 0;
	while (i < x.size()) {
		os << x[i] + "\n";
		++i;
	}
	os.close();
}
void f4func::writefile_info_to_title(std::vector<int>&x) {
	FILE*fp = fopen("info_to_title.dat", "wb+");
	int tempsize = 0;
	for (int i = 0; i < x.size(); i++) {
		fwrite(&tempsize, 4, 1, fp);
		tempsize += x[i] + 2;
	}
	fclose(fp);
}
//wstring f4func::BSTRtoWString(bstr_t& str) {
//	return (wchar_t*)str;
//}
//bstr_t f4func::WstringtoBSTR(wstring& str) {
//	return (_bstr_t)str.c_str();
//}
//const wchar_t* f4func::WstringtoWchar(std::wstring&str) {
//	return str.c_str();
//}

int f4func::check(std::string &x) {
	if (x.size() <= 3)
		if (((x[0] >= '0'&&x[0] <= '9') || (x[0] >= 'A'&&x[0] <= 'Z')) && (x[x.size() - 1] >= 'A'&&x[x.size() - 1] <= 'Z'))
			return 1;
		else //if (x == "very" || x == "much" || x == "many" || x == "with")
			return -1;
	else
		return 1;
}
void f4func::insert(std::string&s, std::string & num) {
	std::vector<std::string> sepp = f4func::split(s, " ~><+.,-/:\"\'();&[]{}?!\\");
	for (int i = 0; i < sepp.size(); i++) {
		if (sepp[i][0] >= 'A'&&sepp[i][0] <= 'Z')
			if (sepp[i][1] >= 'a'&&sepp[i][1] <= 'z')
				sepp[i][0] += 32;
		if (f4func::check(sepp[i]) > 0)
			f4func::hashtable[sepp[i]] += ("+" + num);
	}
}
bool f4func::insert(Info& info) {
	bstr_t title;
	if (info.GetProperty(MYSTR(L"title")).empty()) {
		//fail
		return 1;
	}
	else {
		title = info.GetProperty(MYSTR(L"title")).at(0);
	}
	return 0;
}
//单字符串分割
void f4func::str_split(const std::string& s, const std::string& sep, std::vector<std::string>& v)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(sep);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.emplace_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + 1;
		pos2 = s.find(sep, pos1);
	}
	if (pos1 != s.length())
		v.emplace_back(s.substr(pos1));
}
//多分隔符分割
std::vector<std::string> f4func::split(const std::string &s, const std::string &seperator) {
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;
	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
std::vector<std::string> f4func::intersect(std::vector<std::string>&a, std::vector<std::string>&b) {
	std::vector<std::string> v;
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(v));//求交集 
	return v;
}
bool f4func::keyword_search(std::string&str, std::vector<std::string> &titles) {
	//获取关键字集
	std::vector<std::string> keywords = this->split(str, " -\'");
	if (keywords.size() == 0)
		return false;
	//获取一个数据块的内容
	FILE*fp = fopen(DS_F4_HASHTABLE_INFO, "r+");
	//获取交集的逻辑地址集vstr1
	std::vector<std::string> vstr1, vstr2, temp;
	char s[200000];//存逻辑地址集
	std::string p = this->hashtable[keywords[0]];//数据块位置
	if (p == "") return 0;
	int pos = std::stoi(p);
	fseek(fp, pos, SEEK_SET);
	fgets(s, 200000, fp);
	//char*转换为string
	std::string s1(s, s + strlen(s));
	s1.pop_back();
	this->str_split(s1, "+", vstr1);
	for (int i = 1; i < keywords.size(); ++i) {

		p = this->hashtable[keywords[i]];//数据块位置
		if (p == "") return 0;
		pos = std::stoi(p);
		fseek(fp, pos, SEEK_SET);
		fgets(s, 200000, fp);
		//char*转换为string
		std::string s1(s, s + strlen(s));
		s1.pop_back();
		this->str_split(s1, "+", vstr2);
		vstr1 = this->intersect(vstr1, vstr2);
		if (vstr1.size() == 1)
			break;
		vstr2.clear();
	}
	fclose(fp);
	if (vstr1.size() <= 1)//没有满足所给关键字的title
		return 0;
	else {
		//去重
		vstr1.erase(unique(vstr1.begin(), vstr1.end()), vstr1.end());
		//获取交集的实际地址集pos
		std::vector<int>position;
		fp = fopen(DS_F4_INFO_TO_TITLE, "rb+");
		int pos0;//偏移量
		int res;//实际地址
		for (int i = 0; i < vstr1.size(); i++) {
			if (vstr1[i] == "")
				continue;
			pos0 = std::stoi(vstr1[i]);
			fseek(fp, pos0 * 4, SEEK_SET);
			fread(&res, 4, 1, fp);
			position.emplace_back(res);
		}
		fclose(fp);
		fp = fopen(DS_F4_TITLE, "r+");
		for (int i = 0; i < position.size(); i++) {
			//获取title内容
			fseek(fp, position[i], SEEK_SET);
			fgets(s, 200000, fp);//获取title
			std::string s1(s, s + strlen(s));
			s1.pop_back();
			titles.emplace_back(s1);
		}
		return 1;
	}
}