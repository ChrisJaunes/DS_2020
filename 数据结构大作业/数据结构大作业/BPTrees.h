#pragma once
#include<vector>
#include<algorithm>
#ifndef BPTree_H
#define BPTree_H
#define NULL 0
#define m 15 //internal的限制
#define n 50 //leaf的限制
//E--Article类 KEY--字符类型(eg宽字符)
//B+树默认阶为15，也可自定义
//树叶默认record数目m=15（vector只是为了省事）
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
	BPNode* parent;
	std::vector<KEY> key;
	std::vector<BPNode*> pointer;
	int num;//现存的数量
	BPNode* nextNode;
	BPNode* prevNode;
	BPNode<E, KEY>()
	{
		parent = nullptr; nextNode = nullptr; prevNode = nullptr;
		num = 0;
		key.reserve(m);
		pointer.reserve(m + 1);
		for (int i = 0; i < m; i++)
		{
			this->key.push_back(NULL);
			this->pointer.push_back(nullptr);
		}
		pointer.push_back(nullptr);
	}
	BPNode<E, KEY>(int x)
	{
		parent = nullptr; nextNode = nullptr; prevNode = nullptr;
		num = 0;
		pos = 0;
		key.reserve(x - 1);
		pointer.reserve(x);
		for (int i = 0; i < x - 1; i++)
		{
			this->key.push_back(NULL);
			this->pointer.push_back(nullptr);
		}
		pointer.push_back(nullptr);
	}
	~BPNode<E, KEY>()
	{
		delete parent;
		delete nextNode;
		delete prevNode;
	}
	virtual bool isLeaf() = 0;
	//virtual void add(E, KEY) = 0;
	void findpos(KEY, int);
};

template<typename E, typename KEY>
class BPInternal :public BPNode<E, KEY>
{
public:
	BPInternal<E, KEY>() : BPNode<E, KEY>(m) {}
	~BPInternal<E, KEY>() {}
	void add(E, KEY, BPNode<E, KEY>*);
	//void add_pointer(E, KEY, BPNode<E, KEY>*);
	void split(KEY, BPInternal<E, KEY>*, BPInternal<E, KEY>*, KEY, BPNode<E, KEY>*);
	BPNode<E, KEY>* find(KEY);
	virtual bool isLeaf() { return false; }
};

template<typename E, typename KEY>
class BPLeaf :public BPNode<E, KEY>
{
private:
	std::vector<std::vector<E>> value;//放地址
public:
	BPLeaf<E, KEY>() : BPNode<E, KEY>(n)
	{
		value.reserve(n);
		for (int i = 0; i < n; i++) {
			std::vector<E> tmp;
			tmp.reserve(10);
			value.push_back(tmp);
		}
	}
	~BPLeaf<E, KEY>() {}
	virtual bool isLeaf() { return true; }
	void add(E, KEY);
	void split(KEY, BPLeaf<E, KEY>* &, BPLeaf<E, KEY>*, KEY, E);
	bool find(std::vector<E> &, KEY);
	BPLeaf<E, KEY>* next() {
		return (BPLeaf<E, KEY>*)this->nextNode;
	}
	void getvalue(std::vector<KEY> Key, std::vector<std::vector<E>> Value) {
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
	bool search(std::vector<E> &, KEY);
	BPLeaf<E, KEY>* begin();//第一个BPLeaf
	BPLeaf<E, KEY>* end();//最后一个BPLeaf
};

#endif

using namespace std;
//具体实现

//bpnode
//return第一个>=key的位置
template<typename E, typename KEY>
void BPNode<E, KEY>::findpos(KEY str, int pos)
{
	int start = 0; int end = this->num - 1;
	int pos = -1;
	while (start <= end)
	{
		int mid = start + (end - start) / 2;
		if (comp(this->key.at(mid), str) == 0)
		{
			pos = mid + 1;
			return;
		}
		else if (comp(this->key.at(mid), str) < 0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (pos == -1)
		pos = start;
	return;
}
//internal node
//internode没满的添加
template<typename E, typename KEY>
void BPInternal<E, KEY>::add(E x, KEY str, BPNode<E, KEY>*node_new)
{
	this->num++;
	BPNode<E, KEY>*p = this;
	int i;
	/*原始版找位置
	for (i = 0; i < this->num; i++) {
		if (comp(str, p->key.at(i)) == 0) {
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->key.at(0)) < 0)
			break;
		else if (comp(str, p->key.at(i)) < 0 && comp(str, p->key.at(i - 1)) >= 0)
			break;
		else if (i == this->num - 1 && comp(str, p->key.at(this->num - 1)) >= 0)
			break;
	}*/
	p->findpos(str, i);
	for (int j = this->num - 1; j >= i; j--) {
		p->key.at(j + 1) = p->key.at(j);
		p->pointer.at(j + 1) = p->pointer.at(j);
	}
	p->key.at(i) = str;
	p->pointer.at(i) = node_new;
	node_new->parent = this;
}
template<typename E, typename KEY>
BPNode<E, KEY>* BPInternal<E, KEY>::find(KEY str)
{
	int number = this->num;
	BPNode<E, KEY>*p = this;
	for (int i = 0; i < number; i++) {
		if (i == 0 && comp(str, p->key.at(0)) < 0)
			return p->pointer.at(0);
		else if (i == number - 1 && comp(str, p->key.at(number - 1)) >= 0)
			return p->pointer.at(number);
		else if (comp(str, p->key.at(i)) < 0 && comp(str, p->key.at(i - 1)) >= 0)
			return p->pointer.at(i);
	}
}
template<typename E, typename KEY>
void BPInternal<E, KEY>::split(KEY k, BPInternal<E, KEY>*node_new, BPInternal<E, KEY>*node, KEY str, BPNode<E, KEY>*newchild)
{
	node_new->parent = node->parent;
	int i;//x插入位置
	BPNode<E, KEY>*temp1 = node_new;//new node
	BPNode<E, KEY>*temp2 = node;//old node
	temp2->findpos(str, i);
	/*费内存版
	std::vector<KEY>key_v;
	std::vector<BPNode<E, KEY>*>node_v;
	key_v.assign(temp2->key.begin(), temp2->key.begin() + i);
	node_v.assign(temp2->pointer.begin(), temp2->pointer.begin() + i);
	key_v.at(i) = str;
	node_v.at(i) = newchild;
	key_v.assign(temp2->key.begin() + 1 + i, temp2->key.end());
	node_v.assign(temp2->pointer.begin(), temp2->pointer.end());
	for (int j = m - 1; j > m / 2; j--) {
		temp2->key.at(j) = NULL;
		temp2->pointer.at(j) = nullptr;
		temp1->key.at(j-m/2)=
	}*/
	/*原始版
	for (i = 0; i < m; i++) {
		if (comp(str, temp2->key.at(i)) <= 0)
			break;
		if (i == m - 1) {
			i++;
			break;
		}
	}*/
	node->num = m - m / 2;
	node_new->num = m / 2;
	int up = (m - 1) / 2;
	if (i <= up) {
		k = temp2->key.at(up);
		for (int j = up + 1; j < up + 1 + m / 2; j++) {
			temp1->key.at(j - up - 1) = temp2->key.at(j);
			temp2->key.at(j) = NULL;
		}//copy key
		for (int j = up + 1; j <= up + 1 + m / 2; j++) {
			temp1->pointer.at(j - up - 1) = temp2->pointer.at(j);
			temp2->pointer.at(j) = nullptr;
		}//copy pointer
		for (int j = up; j > i; j--) {
			temp2->key.at(j) = temp2->key.at(j - 1);
			temp2->pointer.at(j + 1) = temp2->pointer.at(j);
		}//move key and pointer
		temp2->key.at(i) = str;
		temp2->pointer.at(i + 1) = newchild;
	}
	else if (i == up + 1) {
		k = str;
		temp1->pointer.at(0) = newchild;
		for (int j = 0; j < m / 2; j++) {
			temp1->key.at(j) = temp2->key.at(j + up + 1);
			temp2->key.at(j + up + 1) = NULL;
			temp1->pointer.at(j + 1) = temp2->pointer.at(j + up + 2);
			temp2->pointer.at(j + up + 2) = nullptr;
		}
	}
	else {//此处尝试vector指针，有待测试
		k = temp2->key.at(up + 1);
		temp2->key.at(up + 1) = NULL;
		for (int j = i; j < m; j++) {
			temp1->key.at(j - up - 1) = temp2->key.at(j);
			temp2->key.at(j) = NULL;
			temp1->pointer.at(j - up) = temp2->pointer.at(j + 1);
			temp2->pointer.at(j + 1) = nullptr;
		}
		temp1->pointer.at(i - up - 1) = temp2->pointer.at(i);
		temp2->pointer.at(i) = nullptr;
		temp1->key.at(i - up - 2) = str;
		temp1->pointer.at(i - up - 2) = newchild;
		for (int j = up + 2; j < i; j++) {
			temp1->key.at(j - up - 2) = temp2->key.at(j);
			temp2->key.at(j) = NULL;
			temp1->pointer.at(j - up - 2) = temp2->pointer.at(j);
			temp2->pointer.at(j) = nullptr;
		}
	}
}

//leaf node
template<typename E, typename KEY>
void BPLeaf<E, KEY>::add(E x, KEY str)
{
	BPNode<E, KEY>*p = this;
	int number = ++this->num;
	int i;
	for (i = 0; i < number; i++) {
		if (comp(str, p->key.at(i)) == 0) {
			std::vector<E> temp = this->value.at(i);
			temp.push_back(x);
			this->num--;
			return;
		}
		if (i == 0 && comp(str, p->key.at(0)) < 0)
			break;
		else if (comp(str, p->key.at(i)) < 0 && comp(str, p->key.at(i - 1)) >= 0)
			break;
		else if (i == number - 1 && comp(str, p->key.at(number - 1)) >= 0)
			break;
	}
	for (int j = number - 2; j >= i; j--) {
		p->key.at(j + 1) = p->key.at(j);
		this->value.at(j + 1) = this->value.at(j);
	}
	p->key.at(i) = str;
	std::vector<E> temp;
	temp.push_back(x);
	this->value.at(i) = temp;
}
template<typename E, typename KEY>
bool BPLeaf<E, KEY>::find(vector<E> &p, KEY str)//二分查找
{
	int start = 0; int end = this->num - 1;
	BPNode<E, KEY>*pp = this;
	while (start <= end)
	{
		int mid = start + (end - start) / 2;
		if (comp(pp->key.at(mid), str) == 0)
		{
			p = this->value.at(mid);
			return true;
		}
		else if (comp(pp->key.at(mid), str) < 0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	p = NULL;
	return false;
}
//internode孩子没满的情况,leaf_new是函数外新建的
template<typename E, typename KEY>
void BPLeaf<E, KEY>::split(KEY k, BPLeaf<E, KEY>* &leaf_new, BPLeaf<E, KEY>*leaf, KEY str, E x)
{
	//leaf_new->parent = leaf->parent;
	int i;//x插入位置
	BPNode<E, KEY>*lfnode = (BPNode<E, KEY>*)leaf;
	for (i = 0; i < n; i++) {
		if (comp(str, lfnode->key.at(i)) <= 0)
			break;
		if (i == n - 1) {
			i++;
			break;
		}
	}
	BPNode<E, KEY>*temp1 = leaf_new;
	BPNode<E, KEY>*temp2 = leaf;
	leaf->num = n / 2 + 1;
	leaf_new->num = n - n / 2;
	if (i <= n / 2) {
		for (int j = n / 2; j < n; j++) {
			leaf_new->value.at(j - n / 2) = leaf->value.at(j);
			std::vector<E>*t = leaf->value.at(j);
			t->clear();
			temp1->key.at(j - n / 2) = temp2->key.at(j);
			temp2->key.at(j) = NULL;
		}
		for (int j = leaf->num - 2; j >= i; j--) {
			temp2->key.at(j + 1) = temp2->key.at(j);
			leaf->value.at(j + 1) = leaf->value.at(j);
		}
		temp2->key.at(i) = str;
		std::vector<E> temp = leaf->value.at(i);
		temp.push_back(x);
		leaf->value.at(i) = temp;
	}
	else {//此处尝试vector指针，有待测试
		for (int j = n / 2 + 1; j < n; j++) {
			if (j == i) {
				temp1->key.at(j - n / 2 - 1) = str;
				std::vector<E>*temp = &leaf_new->value.at(j - n / 2 - 1);
				temp->push_back(x);
			}
			leaf_new->value.at(j - n / 2 - 1) = leaf->value.at(j);
			std::vector<E>*t = &leaf->value.at(j);
			t->clear();
			temp1->key.at(j - n / 2 - 1) = temp2->key.at(j);
			temp2->key.at(j) = NULL;
		}
		if (i == n) {
			temp1->key.at(n - n / 2 - 1) = str;
			std::vector<E>*temp = &leaf_new->value.at(n - n / 2 - 1);
			temp->push_back(x);
		}
		if (i == n / 2 + 1) k = str;
		else k = temp1->key.at(0);
	}
	//处理leaf连成串
	if (leaf->nextNode == nullptr)
		leaf->nextNode = leaf_new;
	else {
		leaf_new->nextNode = leaf->nextNode;
		leaf->nextNode = leaf_new;
	}
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
	//one leaf or just two leaves
	if (root->num == 0) {
		BPNode<E, KEY>* curr = root->pointer.at(0);
		if (curr == nullptr) {
			BPLeaf<E, KEY>* lf = new BPLeaf<E, KEY>;
			lf->add(x, str);
			lf->parent = root;
			root->pointer.at(0) = lf;
			return;
		}
		else if (curr->num < n &&curr->num>0) {
			BPLeaf<E, KEY>* lf = root->pointer.at(0);
			lf->add(x, str);
			return;
		}
		else {
			BPLeaf<E, KEY>* lf_new = new BPLeaf<E, KEY>;
			BPLeaf<E, KEY>* lf = root->pointer.at(0);
			KEY k;
			lf->split(k, lf_new, lf, str, x);
			root->pointer.at(1) = lf_new;
			root->key.at(0) = k;
			root->num++;
			lf->nextNode = lf_new;
			return;
		}
	}
	else {
		BPNode<E, KEY>* curr = root;
		while (curr->isLeaf() == false) {
			auto p = (BPInternal<E, KEY>*)curr;
			curr = p->find(str);
		}
		BPLeaf<E, KEY>*leaf = (BPLeaf<E, KEY>*)curr;
		if (leaf->num < n)
			leaf->add(x, str);
		else {
			BPLeaf<E, KEY>* lf_new = new BPLeaf<E, KEY>;
			BPInternal<E, KEY>*internode_new = nullptr;
			BPInternal<E, KEY>*internode_new = nullptr;
			KEY k;//leaf传上去的key值key_leaf,internode传上去的key值key_internode
			int pos;//顶上去的key的位置
			leaf->split(k, lf_new, leaf, str, x);
			lf_new->parent = leaf->parent;
			int mark = 0;//leaf
			//upupup
			while (true) {
				if (curr->parent == nullptr) {
					curr->parent = new BPInternal<E, KEY>;
					curr->parent->pointer.at(0) = curr;
					curr->parent->pointer.at(1) = internode_new;
					root = curr->parent;
				}
				curr = curr->parent;
				//curr->findpos(k, pos);
				BPInternal<E, KEY>*internode = (BPInternal<E, KEY>*)curr;
				if (curr->num < m) {
					if (mark == 0)
						internode->add(k, str, lf_new);
					else
						internode->add(k, str, internode_new);
					break;
				}
				else {
					if (mark == 0) {
						internode->split(k, internode_new, internode, k, lf_new);
						mark = 1;
					}
					else {
						BPInternal<E, KEY>*temp = new BPInternal<E, KEY>;
						internode->split(k, temp, internode, k, internode_new);
						internode_new = temp;
						//internode_new->parent = internode->parent;
					}
				}
			}
		}
	}
}
template<typename E, typename KEY>
bool BPTree<E, KEY>::search(vector<E> &val, KEY str)
{
	BPNode<E, KEY>* curr = root;
	if (root->num == 0) curr = curr->pointer.at(0);
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
	BPNode<E, KEY>* curr = root;
	while (curr->isLeaf() == false) {
		curr = curr->pointer.at(0);
	}
	return (BPLeaf<E, KEY>*)curr;
}
template<typename E, typename KEY>
BPLeaf<E, KEY>* BPTree<E, KEY>::end()
{
	BPNode<E, KEY>* curr = root;
	while (curr->isLeaf() == false) {
		curr = curr->pointer.at(curr->num);
	}
	return (BPLeaf<E, KEY>*)curr;
}