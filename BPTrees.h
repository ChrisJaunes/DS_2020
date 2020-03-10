#pragma once
#include<vector>
#include<algorithm>
#ifndef BPTree_H
#define BPTree_H
#define NULL 0
#define m 5 //internal的限制
#define n 10 //leaf的限制
//T1--Article类 T2--字符类型(eg宽字符)
//B+树默认阶为5，也可自定义
//树叶默认record数目m=10（vector只是为了省事）
//叶节点内部采用二分查找


template<typename T>
int comp(T a, T b)
{
	if (a > b) return 1;
	else if (a == b) return 0;
	else return -1;
}

template<typename T1, typename T2>
class BPNode
{
public:
	BPNode*parent;
	std::vector<T2>* value;
	std::vector<BPNode*>* pointer;
	int num;
	BPNode*next;
	BPNode*pre;
	BPNode<T1, T2>()
	{
		parent = NULL; next = NULL; pre = NULL;
		this->value = new std::vector<T2>;
		this->pointer = new std::vector<BPNode*>;
		num = 0;
		for (int i = 0; i < m - 1; i++)
		{
			this->value->push_back(NULL);
			this->pointer->push_back(NULL);
		}
		pointer->push_back(NULL);
	}
	BPNode<T1, T2>(int x)
	{
		parent = NULL; next = NULL; pre = NULL;
		this->value = new std::vector<T2>;
		this->pointer = new std::vector<BPNode*>;
		num = 0;
		for (int i = 0; i < x; i++)
		{
			this->value->push_back(NULL);
			this->pointer->push_back(NULL);
		}
		pointer->push_back(NULL);
	}
	~BPNode<T1, T2>()
	{
		delete this->value;
		delete this->pointer;
	}
	virtual bool isLeaf() = 0;
	virtual void add(T1, T2) = 0;
};

template<typename T1, typename T2>
class BPInternal :public BPNode<T1, T2>
{
public:
	BPInternal<T1, T2>() : BPNode<T1, T2>(m - 1) {}
	~BPInternal<T1, T2>() {}
	virtual void add(T1, T2);
	BPInternal<T1, T2>* split(BPInternal<T1, T2>*, T2 str);
	BPNode<T1, T2>* find(T2);
	virtual bool isLeaf() { return false; }
};

template<typename T1, typename T2>
class BPLeaf :public BPNode<T1, T2>
{
private:
	std::vector<std::vector<T1>*>* ptr;//放地址
public:
	BPLeaf<T1, T2>() : BPNode<T1, T2>(n)
	{
		ptr = new std::vector<std::vector<T1>*>;
		for (int i = 0; i < n; i++) {
			std::vector<T1>*tmp = new std::vector<T1>;
			ptr->push_back(tmp);
		}
	}
	~BPLeaf<T1, T2>() {
		for (int i = 0; i < n; i++)
			delete ptr->at(i);
		delete ptr;
	}
	virtual bool isLeaf() { return true; }
	virtual void add(T1, T2);
	BPInternal<T1, T2>* split(BPLeaf<T1, T2>*, T2 str);
	bool find(std::vector<T1>* &, T2);
};

template<typename T1, typename T2>
class BPTree
{
private:
	int order;
	BPNode<T1, T2>*root;
public:
	BPTree();//默认阶为5
	~BPTree();
	void insert(T1, T2);
	bool search(std::vector<T1>* &, T2);
};

#endif

using namespace std;
//具体实现
//internal node
template<typename T1, typename T2>
void BPInternal<T1, T2>::add(T1 x, T2 str)
{
	this->num++;
	BPNode<T1, T2>*p = this;
	int i;
	for (i = 0; i < this->num; i++) {
		if (comp(str, p->value->at(i)) == 0) {
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->value->at(0)) < 0)
			break;
		else if (comp(str, p->value->at(i)) < 0 && comp(str, p->value->at(i - 1)) >= 0)
			break;
		else if (i == this->num - 1 && comp(str, p->value->at(this->num - 1)) >= 0)
			break;
	}
	for (int j = this->num - 1; j >= i; j--) {
		p->value->at(j + 1) = p->value->at(j);
		p->pointer->at(j + 1) = p->pointer->at(j);
	}
	this->value->at(i) = str;
}
template<typename T1, typename T2>
BPNode<T1, T2>* BPInternal<T1, T2>::find(T2 str)
{
	int number = this->num;
	BPNode<T1, T2>*p = this;
	for (int i = 0; i < number; i++) {
		if (i == 0 && comp(str, p->value->at(0)) < 0)
			return p->pointer->at(0);
		else if (i == number - 1 && comp(str, p->value->at(number - 1)) >= 0)
			return p->pointer->at(number);
		else if (comp(str, p->value->at(i)) < 0 && comp(str, p->value->at(i - 1)) >= 0)
			return p->pointer->at(i);
	}
}
template<typename T1, typename T2>
BPInternal<T1, T2>* BPInternal<T1, T2>::split(BPInternal<T1, T2>*, T2 str)
{
	
}

//leaf node
template<typename T1, typename T2>
void BPLeaf<T1, T2>::add(T1 x, T2 str)
{
	BPNode<T1, T2>*p = this;
	int number = ++this->num;
	int i;
	for (i = 0; i < number; i++) {
		if (comp(str, p->value->at(i)) == 0) {
			std::vector<T1>*temp = this->ptr->at(i);
			temp->push_back(x);
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->value->at(0)) < 0)
			break;
		else if (comp(str, p->value->at(i)) < 0 && comp(str, p->value->at(i - 1)) >= 0)
			break;
		else if (i == number - 1 && comp(str, p->value->at(number - 1)) >= 0) 
			break;
	}
	for (int j = number - 2; j >= i; j--) {
		this->value->at(j + 1) = this->value->at(j);
		this->ptr->at(j + 1) = this->ptr->at(j);
	}
	this->value->at(i) = str;
	auto temp = new std::vector<T1>;
	this->ptr->at(i) = temp;
	temp->push_back(x);
}
template<typename T1, typename T2>
bool BPLeaf<T1, T2>::find(vector<T1>* &p, T2 str)//二分查找
{
	int start = 0; int end = this->num - 1;
	BPNode<T1, T2>*pp = this;
	while (start <= end)
	{
		int mid = start + (end - start) / 2;
		if (comp(pp->value->at(mid), str) == 0)
		{
			p = this->ptr->at(mid);
			return true;
		}
		else if (comp(pp->value->at(mid), str) < 0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	p = NULL;
	return false;
}
template<typename T1, typename T2>
BPInternal<T1, T2>* BPLeaf<T1, T2>::split(BPLeaf<T1, T2>*, T2 str)
{

}

//b+tree
template<typename T1, typename T2>
BPTree<T1, T2>::BPTree()
{
	order = 5;
	root = new BPInternal<T1, T2>;
}
template<typename T1, typename T2>
BPTree<T1, T2>::~BPTree()
{
	delete root;
}
template<typename T1, typename T2>
void BPTree<T1, T2>::insert(T1 x, T2 str)
{
	if (root->num == 0) {
		BPNode<T1, T2>* curr = root->pointer->at(0);
		if (curr == NULL) {
			BPLeaf<T1, T2>* lf = new BPLeaf<T1, T2>;
			lf->add(x, str);
			lf->parent = root;
			//T1 test = x;
			root->pointer->at(0) = lf;
			return;
		}
		else if (curr->num < n &&curr->num>0) {
			BPNode<T1, T2>* lf = root->pointer->at(0);
			lf->add(x, str);
			return;
		}
		else {
			
		}
	}
	else {
		BPNode<T1, T2>* curr = root;
		while (curr->isLeaf() == false)
		{
			auto p = (BPInternal<T1, T2>*)curr;
			curr = p->find(str);
		}
		BPLeaf<T1, T2>*leaf = (BPLeaf<T1, T2>*)curr;
		if (leaf->num < n)
			leaf->add(x, str);
		else {

		}
	}
}
template<typename T1, typename T2>
bool BPTree<T1, T2>::search(vector<T1>* &ptr, T2 str)
{
	BPNode<T1, T2>* curr = root;
	if (root->num == 0) curr = curr->pointer->at(0);
	else {
		while (curr->isLeaf() == false)
		{
			auto p = (BPInternal<T1, T2>*)curr;
			curr = p->find(str);
		}
	}
	BPLeaf<T1, T2>*leaf = (BPLeaf<T1, T2>*)curr;
	if (leaf->find(ptr, str) == true)
		return true;
	else
		return false;
}