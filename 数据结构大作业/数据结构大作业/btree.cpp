#include "pch.h"
#include "btree.h"

// CNode
CNode::CNode() {
	setType(LEAF);
	setKeyNum(0);
}

CNode::~CNode() {
	setKeyNum(0);
}

int CNode::getKeyIndex(KeyType key)const
{
	int left = 0;
	int right = getKeyNum() - 1;
	int current;
	while (left != right)
	{
		current = (left + right) / 2;
		KeyType currentKey = getKeyValue(current);
		if (key > currentKey)
		{
			left = current + 1;
		}
		else
		{
			right = current;
		}
	}
	return left;
}

// CInternalNode
CInternalNode::CInternalNode() :CNode() {
	setType(INTERNAL);
}

CInternalNode::~CInternalNode() {

}

void CInternalNode::clear()
{
	for (int i = 0; i <= m_KeyNum; ++i)
	{
		m_Childs[i]->clear();
		delete m_Childs[i];
		m_Childs[i] = NULL;
	}
}

void CInternalNode::split(CNode* parentNode, int childIndex)
{
	CInternalNode* newNode = new CInternalNode();//���Ѻ���ҽڵ�
	newNode->setKeyNum(MINNUM_KEY);
	int i;
	for (i = 0; i < MINNUM_KEY; ++i)// �����ؼ��ֵ�ֵ
	{
		newNode->setKeyValue(i, m_KeyValues[i + MINNUM_CHILD]);
	}
	for (i = 0; i < MINNUM_CHILD; ++i) // �������ӽڵ�ָ��
	{
		newNode->setChild(i, m_Childs[i + MINNUM_CHILD]);
	}
	setKeyNum(MINNUM_KEY);  //�����������Ĺؼ��ָ���
	((CInternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_KEY], newNode);
}

void CInternalNode::insert(int keyIndex, int childIndex, KeyType key, CNode* childNode) {
	int i;
	for (i = getKeyNum(); i > keyIndex; --i)//�����ڵ��е�childIndex������йؼ��ֵ�ֵ������ָ�������һλ
	{
		setChild(i + 1, m_Childs[i]);
		setKeyValue(i, m_KeyValues[i - 1]);
	}
	if (i == childIndex)
	{
		setChild(i + 1, m_Childs[i]);
	}
	setChild(childIndex, childNode);
	setKeyValue(keyIndex, key);
	setKeyNum(m_KeyNum + 1);
}

void CInternalNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
	// �ϲ�����
	insert(MINNUM_KEY, MINNUM_KEY + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i)
	{
		insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, childNode->getKeyValue(i - 1), ((CInternalNode*)childNode)->getChild(i));
	}
	//���ڵ�ɾ��index��key
	parentNode->removeKey(keyIndex, keyIndex + 1);
	delete ((CInternalNode*)parentNode)->getChild(keyIndex + 1);
}

void CInternalNode::removeKey(int keyIndex, int childIndex)
{
	for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i)
	{
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void CInternalNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:  // �����ֵܽ���
	{
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	}
	break;
	case RIGHT:  // �����ֵܽ���
	{
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	}
	break;
	default:
		break;
	}
}

int CInternalNode::getChildIndex(KeyType key, int keyIndex)const
{
	if (key == getKeyValue(keyIndex))
	{
		return keyIndex + 1;
	}
	else
	{
		return keyIndex;
	}
}

// CLeafNode
CLeafNode::CLeafNode() :CNode() {
	setType(LEAF);
	setLeftSibling(NULL);
	setRightSibling(NULL);
}

CLeafNode::~CLeafNode() {

}

void CLeafNode::clear()
{
	for (int i = 0; i < m_KeyNum; ++i)
	{
		// if type of m_Datas is pointer
		//delete m_Datas[i];
		//m_Datas[i] = NULL;
	}
}

void CLeafNode::insert(KeyType key, const DataType& data)
{
	int i;
	for (i = m_KeyNum; i >= 1 && m_KeyValues[i - 1] > key; --i)
	{
		setKeyValue(i, m_KeyValues[i - 1]);
		setData(i, m_Datas[i - 1]);
	}
	setKeyValue(i, key);
	setData(i, data);
	setKeyNum(m_KeyNum + 1);
}

void CLeafNode::split(CNode* parentNode, int childIndex)
{
	CLeafNode* newNode = new CLeafNode();//���Ѻ���ҽڵ�
	setKeyNum(MINNUM_LEAF);
	newNode->setKeyNum(MINNUM_LEAF + 1);
	newNode->setRightSibling(getRightSibling());
	setRightSibling(newNode);
	newNode->setLeftSibling(this);
	int i;
	for (i = 0; i < MINNUM_LEAF + 1; ++i)// �����ؼ��ֵ�ֵ
	{
		newNode->setKeyValue(i, m_KeyValues[i + MINNUM_LEAF]);
	}
	for (i = 0; i < MINNUM_LEAF + 1; ++i)// ��������
	{
		newNode->setData(i, m_Datas[i + MINNUM_LEAF]);
	}
	((CInternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_LEAF], newNode);
}

void CLeafNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
	// �ϲ�����
	for (int i = 0; i < childNode->getKeyNum(); ++i)
	{
		insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
	}
	setRightSibling(((CLeafNode*)childNode)->getRightSibling());
	//���ڵ�ɾ��index��key��
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

void CLeafNode::removeKey(int keyIndex, int childIndex)
{
	for (int i = keyIndex; i < getKeyNum() - 1; ++i)
	{
		setKeyValue(i, getKeyValue(i + 1));
		setData(i, getData(i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void CLeafNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:  // �����ֵܽ���
	{
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), ((CLeafNode*)siblingNode)->getData(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, getKeyValue(0));
	}
	break;
	case RIGHT:  // �����ֵܽ���
	{
		insert(siblingNode->getKeyValue(0), ((CLeafNode*)siblingNode)->getData(0));
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
	break;
	default:
		break;
	}
}

int CLeafNode::getChildIndex(KeyType key, int keyIndex)const
{
	return keyIndex;
}


CBPlusTree::CBPlusTree() {
	m_Root = NULL;
	m_DataHead = NULL;
}

CBPlusTree::~CBPlusTree() {
	clear();
}

bool CBPlusTree::insert(KeyType key, const DataType& data) {
	// �Ƿ��Ѿ�����
	if (search(key))
	{
		return false;
	}
	// �ҵ����Բ����Ҷ�ӽ�㣬���򴴽��µ�Ҷ�ӽ��
	if (m_Root == NULL)
	{
		m_Root = new CLeafNode();
		m_DataHead = (CLeafNode*)m_Root;
		m_MaxKey = key;
	}
	if (m_Root->getKeyNum() >= MAXNUM_KEY) // ���������������
	{
		CInternalNode* newNode = new CInternalNode();  //�����µĸ��ڵ�
		newNode->setChild(0, m_Root);
		m_Root->split(newNode, 0);    // Ҷ�ӽ�����
		m_Root = newNode;  //���¸��ڵ�ָ��
	}
	if (key > m_MaxKey)  // ��������ֵ
	{
		m_MaxKey = key;
	}
	recursive_insert(m_Root, key, data);
	return true;
}

void CBPlusTree::recursive_insert(CNode* parentNode, KeyType key, const DataType& data)
{
	if (parentNode->getType() == LEAF)  // Ҷ�ӽ�㣬ֱ�Ӳ���
	{
		((CLeafNode*)parentNode)->insert(key, data);
	}
	else
	{
		// �ҵ��ӽ��
		int keyIndex = parentNode->getKeyIndex(key);
		int childIndex = parentNode->getChildIndex(key, keyIndex); // ���ӽ��ָ������
		CNode* childNode = ((CInternalNode*)parentNode)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAXNUM_LEAF)  // �ӽ������������з���
		{
			childNode->split(parentNode, childIndex);
			if (parentNode->getKeyValue(childIndex) <= key)   // ȷ��Ŀ���ӽ��
			{
				childNode = ((CInternalNode*)parentNode)->getChild(childIndex + 1);
			}
		}
		recursive_insert(childNode, key, data);
	}
}

void CBPlusTree::clear()
{
	if (m_Root != NULL)
	{
		m_Root->clear();
		delete m_Root;
		m_Root = NULL;
		m_DataHead = NULL;
	}
}

bool CBPlusTree::search(KeyType key)
{
	return recursive_search(m_Root, key);
}

bool CBPlusTree::recursive_search(CNode *pNode, KeyType key)const
{
	if (pNode == NULL)  //���ڵ�ָ���Ƿ�Ϊ�գ���ýڵ��Ƿ�ΪҶ�ӽڵ�
	{
		return false;
	}
	else
	{
		int keyIndex = pNode->getKeyIndex(key);
		int childIndex = pNode->getChildIndex(key, keyIndex); // ���ӽ��ָ������
		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex))
		{
			return true;
		}
		else
		{
			if (pNode->getType() == LEAF)   //���ýڵ��Ƿ�ΪҶ�ӽڵ�
			{
				return false;
			}
			else
			{
				return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key);
			}
		}
	}
}

void CBPlusTree::print()const
{
	printInConcavo(m_Root, 10);
}

void CBPlusTree::printInConcavo(CNode *pNode, int count) const {
	if (pNode != NULL)
	{
		int i, j;
		for (i = 0; i < pNode->getKeyNum(); ++i)
		{
			if (pNode->getType() != LEAF)
			{
				printInConcavo(((CInternalNode*)pNode)->getChild(i), count - 2);
			}
			for (j = count; j >= 0; --j)
			{
				cout << "-";
			}
			cout << pNode->getKeyValue(i) << endl;
		}
		if (pNode->getType() != LEAF)
		{
			printInConcavo(((CInternalNode*)pNode)->getChild(i), count - 2);
		}
	}
}

void CBPlusTree::printData()const
{
	CLeafNode* itr = m_DataHead;
	while (itr != NULL)
	{
		for (int i = 0; i < itr->getKeyNum(); ++i)
		{
			//cout << itr->getData(i) << " ";
		}
		cout << endl;
		itr = itr->getRightSibling();
	}
}

bool CBPlusTree::remove(KeyType key)
{
	if (!search(key))  //������
	{
		return false;
	}
	if (m_Root->getKeyNum() == 1)//�����������
	{
		if (m_Root->getType() == LEAF)
		{
			clear();
			return true;
		}
		else
		{
			CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
			CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key);
	return true;
}

// parentNode�а����ļ�ֵ��>MINNUM_KEY
void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key)
{
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex); // ���ӽ��ָ������
	if (parentNode->getType() == LEAF)// �ҵ�Ŀ��Ҷ�ӽڵ�
	{
		if (key == m_MaxKey && keyIndex > 0)
		{
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		parentNode->removeKey(keyIndex, childIndex);  // ֱ��ɾ��
		// �����ֵ���ڲ�����д��ڣ�ҲҪ��Ӧ���滻�ڲ����
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead)
		{
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else // �ڽ��
	{
		CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //����key���������ڵ�
		if (pChildNode->getKeyNum() == MINNUM_KEY)                       // �����ؼ��ִﵽ����ֵ��������ز���
		{
			CNode *pLeft = childIndex > 0 ? ((CInternalNode*)parentNode)->getChild(childIndex - 1) : NULL;                       //���ֵܽڵ�
			CNode *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex + 1) : NULL;//���ֵܽڵ�
			// �ȿ��Ǵ��ֵܽ���н�
			if (pLeft && pLeft->getKeyNum() > MINNUM_KEY)// ���ֵܽ��ɽ�
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MINNUM_KEY)//���ֵܽ��ɽ�
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//�����ֵܽڵ㶼���ɽ裬���Ǻϲ�
			else if (pLeft)                    //�����ֵܺϲ�
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight)                   //�����ֵܺϲ�
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key);
	}
}

void CBPlusTree::changeKey(CNode *pNode, KeyType oldKey, KeyType newKey)
{
	if (pNode != NULL && pNode->getType() != LEAF)
	{
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex < pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex))  // �ҵ�
		{
			pNode->setKeyValue(keyIndex, newKey);
		}
		else   // ������
		{
			changeKey(((CInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
		}
	}
}

bool CBPlusTree::update(KeyType oldKey, KeyType newKey)
{
	if (search(newKey)) // �����º�ļ��Ƿ��Ѿ�����
	{
		return false;
	}
	else
	{
		DataType dataValue;
		remove(oldKey, dataValue);
		if (dataValue == INVALID_INDEX)
		{
			return false;
		}
		else
		{
			return insert(newKey, dataValue);
		}
	}
}

void CBPlusTree::remove(KeyType key, DataType& dataValue)
{
	if (!search(key))  //������
	{
		dataValue = INVALID_INDEX;
		return;
	}
	if (m_Root->getKeyNum() == 1)//�����������
	{
		if (m_Root->getType() == LEAF)
		{
			dataValue = ((CLeafNode*)m_Root)->getData(0);
			clear();
			return;
		}
		else
		{
			CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
			CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key, dataValue);
}

void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue)
{
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex); // ���ӽ��ָ������
	if (parentNode->getType() == LEAF)// �ҵ�Ŀ��Ҷ�ӽڵ�
	{
		if (key == m_MaxKey && keyIndex > 0)
		{
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		dataValue = ((CLeafNode*)parentNode)->getData(keyIndex);
		parentNode->removeKey(keyIndex, childIndex);  // ֱ��ɾ��
		// �����ֵ���ڲ�����д��ڣ�ҲҪ��Ӧ���滻�ڲ����
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead)
		{
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else // �ڽ��
	{
		CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //����key���������ڵ�
		if (pChildNode->getKeyNum() == MINNUM_KEY)                       // �����ؼ��ִﵽ����ֵ��������ز���
		{
			CNode *pLeft = childIndex > 0 ? ((CInternalNode*)parentNode)->getChild(childIndex - 1) : NULL;                       //���ֵܽڵ�
			CNode *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex + 1) : NULL;//���ֵܽڵ�
			// �ȿ��Ǵ��ֵܽ���н�
			if (pLeft && pLeft->getKeyNum() > MINNUM_KEY)// ���ֵܽ��ɽ�
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MINNUM_KEY)//���ֵܽ��ɽ�
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//�����ֵܽڵ㶼���ɽ裬���Ǻϲ�
			else if (pLeft)                    //�����ֵܺϲ�
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight)                   //�����ֵܺϲ�
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key, dataValue);
	}
}

vector<DataType> CBPlusTree::select(KeyType compareKey, int compareOpeartor)
{
	vector<DataType> results;
	if (m_Root != NULL)
	{
		if (compareKey > m_MaxKey)   // �Ƚϼ�ֵ����B+�������ļ�ֵ
		{
			if (compareOpeartor == LE || compareOpeartor == LT)
			{
				for (CLeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
				{
					for (int i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else if (compareKey < m_DataHead->getKeyValue(0))  // �Ƚϼ�ֵС��B+������С�ļ�ֵ
		{
			if (compareOpeartor == BE || compareOpeartor == BT)
			{
				for (CLeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
				{
					for (int i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else  // �Ƚϼ�ֵ��B+����
		{
			SelectResult result;
			search(compareKey, result);
			switch (compareOpeartor)
			{
			case LT:
			case LE:
			{
				CLeafNode* itr = m_DataHead;
				int i;
				while (itr != result.targetNode)
				{
					for (i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
				for (i = 0; i < result.keyIndex; ++i)
				{
					results.push_back(itr->getData(i));
				}
				if (itr->getKeyValue(i) < compareKey ||
					(compareOpeartor == LE && compareKey == itr->getKeyValue(i)))
				{
					results.push_back(itr->getData(i));
				}
			}
			break;
			case EQ:
			{
				if (result.targetNode->getKeyValue(result.keyIndex) == compareKey)
				{
					results.push_back(result.targetNode->getData(result.keyIndex));
				}
			}
			break;
			case BE:
			case BT:
			{
				CLeafNode* itr = result.targetNode;
				if (compareKey < itr->getKeyValue(result.keyIndex) ||
					(compareOpeartor == BE && compareKey == itr->getKeyValue(result.keyIndex))
					)
				{
					results.push_back(itr->getData(result.keyIndex));
				}
				int i;
				for (i = result.keyIndex + 1; i < itr->getKeyNum(); ++i)
				{
					results.push_back(itr->getData(i));
				}
				itr = itr->getRightSibling();
				while (itr != NULL)
				{
					for (i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
			}
			break;
			default:  // ��Χ��ѯ
				break;
			}
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}

vector<DataType> CBPlusTree::select(KeyType smallKey, KeyType largeKey)
{
	vector<DataType> results;
	if (smallKey <= largeKey)
	{
		SelectResult start, end;
		search(smallKey, start);
		search(largeKey, end);
		CLeafNode* itr = start.targetNode;
		int i = start.keyIndex;
		if (itr->getKeyValue(i) < smallKey)
		{
			++i;
		}
		if (end.targetNode->getKeyValue(end.keyIndex) > largeKey)
		{
			--end.keyIndex;
		}
		while (itr != end.targetNode)
		{
			for (; i < itr->getKeyNum(); ++i)
			{
				results.push_back(itr->getData(i));
			}
			itr = itr->getRightSibling();
			i = 0;
		}
		for (; i <= end.keyIndex; ++i)
		{
			results.push_back(itr->getData(i));
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}

void CBPlusTree::search(KeyType key, SelectResult& result)
{
	recursive_search(m_Root, key, result);
}

void CBPlusTree::recursive_search(CNode* pNode, KeyType key, SelectResult& result)
{
	int keyIndex = pNode->getKeyIndex(key);
	int childIndex = pNode->getChildIndex(key, keyIndex); // ���ӽ��ָ������
	if (pNode->getType() == LEAF)
	{
		result.keyIndex = keyIndex;
		result.targetNode = (CLeafNode*)pNode;
		return;
	}
	else
	{
		return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key, result);
	}
}