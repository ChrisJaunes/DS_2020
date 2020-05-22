#pragma once
#ifndef f4
#define f4
#include"pch.h"
#include"config.h"
#include"Info.h"
#include<fstream>
class f4func {
public:
	std::unordered_map<std::string, std::string> hashtable;

	f4func() {}
	void initial();
	void writefile_hashtable();
	void writefile_title(int&, std::vector<std::string>&);
	void writefile_info_to_title(std::vector<int>&);
	int check(std::string &x);
	void insert(std::string &s, std::string &num);
	bool insert(Info&);
	void str_split(const std::string& s, const std::string& sep, std::vector<std::string>& res);
	std::vector<std::string> split(const std::string &s, const std::string &seperator);
	std::vector<std::string> intersect(std::vector<std::string>&a, std::vector<std::string>&b);
	bool keyword_search(std::string&str, std::vector<std::string>&titles);
};
#endif
