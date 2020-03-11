#pragma once
#include<vector>
#include<algorithm>
#ifndef BPTree_H
#define BPTree_H
#define NULL 0
#define m 5 //internal的限制
#define n 10 //leaf的限制
//E--Article类 KEY--字符类型(eg宽字符)
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

template<typename E, typename KEY>
class BPNode
{
public:
	BPNode*parent;
	std::vector<KEY>* key;
	std::vector<BPNode*>* pointer;
	int num;
	BPNode*nextNode;
	BPNode*prevNode;
	BPNode<E, KEY>()
	{
		parent = NULL; nextNode = NULL; prevNode = NULL;
		this->key = new std::vector<KEY>;
		this->pointer = new std::vector<BPNode*>;
		num = 0;
		for (int i = 0; i < m - 1; i++)
		{
			this->key->push_back(NULL);
			this->pointer->push_back(NULL);
		}
		pointer->push_back(NULL);
	}
	BPNode<E, KEY>(int x)
	{
		parent = NULL; nextNode = NULL; prevNode = NULL;
		this->key = new std::vector<KEY>;
		this->pointer = new std::vector<BPNode*>;
		num = 0;
		for (int i = 0; i < x; i++)
		{
			this->key->push_back(NULL);
			this->pointer->push_back(NULL);
		}
		pointer->push_back(NULL);
	}
	~BPNode<E, KEY>()
	{
		delete this->key;
		delete this->pointer;
	}
	virtual bool isLeaf() = 0;
	virtual void add(E, KEY) = 0;
};

template<typename E, typename KEY>
class BPInternal :public BPNode<E, KEY>
{
public:
	BPInternal<E, KEY>() : BPNode<E, KEY>(m - 1) {}
	~BPInternal<E, KEY>() {}
	virtual void add(E, KEY);
	BPInternal<E, KEY>* split(BPInternal<E, KEY>*, KEY str);
	BPNode<E, KEY>* find(KEY);
	virtual bool isLeaf() { return false; }
};

template<typename E, typename KEY>
class BPLeaf :public BPNode<E, KEY>
{
private:
	std::vector<std::vector<E>*>* value;//放地址
public:
	BPLeaf<E, KEY>() : BPNode<E, KEY>(n)
	{
		value = new std::vector<std::vector<E>*>;
		for (int i = 0; i < n; i++) {
			std::vector<E>*tmp = new std::vector<E>;
			value->push_back(tmp);
		}
	}
	~BPLeaf<E, KEY>() {
		for (int i = 0; i < n; i++)
			delete value->at(i);
		delete value;
	}
	virtual bool isLeaf() { return true; }
	virtual void add(E, KEY);
	BPInternal<E, KEY>* split(BPLeaf<E, KEY>*, KEY str);
	bool find(std::vector<E>* &, KEY);
	BPLeaf<E, KEY>* next() {
		return (BPLeaf<E, KEY>*)this->nextNode;
	}
	void getvalue(std::vector<KEY>* Key, std::vector<std::vector<E>*>* Value) {
		Key = this->key;
		Value = this->value;
	}
};

template<typename E, typename KEY>
class BPTree
{
private:
	int order;
	BPNode<E, KEY>*root;
public:
	BPTree();//默认阶为5
	~BPTree();
	void insert(E, KEY);
	bool search(std::vector<E>* &, KEY);
	BPLeaf<E, KEY>* begin();//第一个BPLeaf
	BPLeaf<E, KEY>* end();//最后一个BPLeaf
};

#endif

using namespace std;
//具体实现
//internal node
template<typename E, typename KEY>
void BPInternal<E, KEY>::add(E x, KEY str)
{
	this->num++;
	BPNode<E, KEY>*p = this;
	int i;
	for (i = 0; i < this->num; i++) {
		if (comp(str, p->key->at(i)) == 0) {
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->key->at(0)) < 0)
			break;
		else if (comp(str, p->key->at(i)) < 0 && comp(str, p->key->at(i - 1)) >= 0)
			break;
		else if (i == this->num - 1 && comp(str, p->key->at(this->num - 1)) >= 0)
			break;
	}
	for (int j = this->num - 1; j >= i; j--) {
		p->key->at(j + 1) = p->key->at(j);
		p->pointer->at(j + 1) = p->pointer->at(j);
	}
	this->key->at(i) = str;
}
template<typename E, typename KEY>
BPNode<E, KEY>* BPInternal<E, KEY>::find(KEY str)
{
	int number = this->num;
	BPNode<E, KEY>*p = this;
	for (int i = 0; i < number; i++) {
		if (i == 0 && comp(str, p->key->at(0)) < 0)
			return p->pointer->at(0);
		else if (i == number - 1 && comp(str, p->key->at(number - 1)) >= 0)
			return p->pointer->at(number);
		else if (comp(str, p->key->at(i)) < 0 && comp(str, p->key->at(i - 1)) >= 0)
			return p->pointer->at(i);
	}
}
template<typename E, typename KEY>
BPInternal<E, KEY>* BPInternal<E, KEY>::split(BPInternal<E, KEY>*, KEY str)
{

}

//leaf node
template<typename E, typename KEY>
void BPLeaf<E, KEY>::add(E x, KEY str)
{
	BPNode<E, KEY>*p = this;
	int number = ++this->num;
	int i;
	for (i = 0; i < number; i++) {
		if (comp(str, p->key->at(i)) == 0) {
			std::vector<E>*temp = this->value->at(i);
			temp->push_back(x);
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->key->at(0)) < 0)
			break;
		else if (comp(str, p->key->at(i)) < 0 && comp(str, p->key->at(i - 1)) >= 0)
			break;
		else if (i == number - 1 && comp(str, p->key->at(number - 1)) >= 0)
			break;
	}
	for (int j = number - 2; j >= i; j--) {
		this->key->at(j + 1) = this->key->at(j);
		this->value->at(j + 1) = this->value->at(j);
	}
	this->key->at(i) = str;
	auto temp = new std::vector<E>;
	this->value->at(i) = temp;
	temp->push_back(x);
}
template<typename E, typename KEY>
bool BPLeaf<E, KEY>::find(vector<E>* &p, KEY str)//二分查找
{
	int start = 0; int end = this->num - 1;
	BPNode<E, KEY>*pp = this;
	while (start <= end)
	{
		int mid = start + (end - start) / 2;
		if (comp(pp->key->at(mid), str) == 0)
		{
			p = this->value->at(mid);
			return true;
		}
		else if (comp(pp->key->at(mid), str) < 0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	p = NULL;
	return false;
}
template<typename E, typename KEY>
BPInternal<E, KEY>* BPLeaf<E, KEY>::split(BPLeaf<E, KEY>*, KEY str)
{

}

//b+tree
template<typename E, typename KEY>
BPTree<E, KEY>::BPTree()
{
	order = 5;
	root = new BPInternal<E, KEY>;
}
template<typename E, typename KEY>
BPTree<E, KEY>::~BPTree()
{
	delete root;
}
template<typename E, typename KEY>
void BPTree<E, KEY>::insert(E x, KEY str)
{
	if (root->num == 0) {
		BPNode<E, KEY>* curr = root->pointer->at(0);
		if (curr == NULL) {
			BPLeaf<E, KEY>* lf = new BPLeaf<E, KEY>;
			lf->add(x, str);
			lf->parent = root;
			//E test = x;
			root->pointer->at(0) = lf;
			return;
		}
		else if (curr->num < n &&curr->num>0) {
			BPNode<E, KEY>* lf = root->pointer->at(0);
			lf->add(x, str);
			return;
		}
		else {

		}
	}
	else {
		BPNode<E, KEY>* curr = root;
		while (curr->isLeaf() == false)
		{
			auto p = (BPInternal<E, KEY>*)curr;
			curr = p->find(str);
		}
		BPLeaf<E, KEY>*leaf = (BPLeaf<E, KEY>*)curr;
		if (leaf->num < n)
			leaf->add(x, str);
		else {

		}
	}
}
template<typename E, typename KEY>
bool BPTree<E, KEY>::search(vector<E>* &val, KEY str)
{
	BPNode<E, KEY>* curr = root;
	if (root->num == 0) curr = curr->pointer->at(0);
	else {
		while (curr->isLeaf() == false)
		{
			auto p = (BPInternal<E, KEY>*)curr;
			curr = p->find(str);
		}
	}
	BPLeaf<E, KEY>*leaf = (BPLeaf<E, KEY>*)curr;
	if (leaf->find(val, str) == true)
		return true;
	else
		return false;
}
template<typename E, typename KEY>
BPLeaf<E, KEY>* BPTree<E, KEY>::begin()
{

}
template<typename E, typename KEY>
BPLeaf<E, KEY>* BPTree<E, KEY>::end()
{

}