#include "pch.h"
#include "Author.h"
#include "CliquesCounting.h"

OPRESULT CliquesCounting::StartCount(Author FirstOne) {
	std::vector<Author> Collaborators = FirstOne.GetCollaborator().second;
	CliquesCount[1]++;
	std::vector<Author> Q; Q.push_back(FirstOne);
	uint64_t l = 0, r = Collaborators.size - 1 , st = 0;
	while (l <= r)
	{
		uint16_t mid = (l + r) >> 1;
		if (FirstOne.GetName() < Collaborators[mid].GetName()) {
			st = mid;
			r = mid - 1;
		}
		else l = mid + 1;
	}
	for (uint64_t i = st; i < Collaborators.size; i++)
	{
		Q[1]=Collaborators[i];
		Counting(Q, 2, Collaborators, i+1);
	}
}
OPRESULT CliquesCounting::Counting(std::vector<Author> Clique, uint64_t Size, std::vector<Author> Collaborators, uint64_t st) {
	CliquesCount[Size]++;
	MaxSize = (MaxSize < Size) ? Size : MaxSize;
	for (uint64_t i = st; i < Collaborators.size; i++)
	{
		Author now = Collaborators[i];
		bool bk = true;
		for (uint64_t j = 0; j < Size; j++)
		{
			uint16_t l = 0, r = Clique[j].GetCollaborator().second.size - 1, st;
			//这里是想判断能否在集团中加入新成员
			//所以要遍历集团中的点 并找到其合作关系的vector来查找
			//所以觉得每次GetCollaborator都for一遍对应文章会很慢
			while (l <= r)
			{
				uint16_t mid = (l + r) >> 1;
				if (Clique[j].GetCollaborator().second[mid].GetName() >= now.GetName()) {
					st = mid;
					r = mid - 1;
				}
				else l = mid + 1;
			}
			if (Clique[st].GetCollaborator().second[mid].GetName() != now.GetName()) {
				bk = false;
				break;
			}
		}
		if (bk) {
			Q[Size] = now;
			Counting(Q, Size + 1, Collaborators, i + 1);
		}
	}
}
uint64_t CliquesCounting::GetSize() {
	return MaxSize;
}
uint64_t CliquesCounting::GetResult(uint64_t id) {
	return CliquesCount[id];
}