#include "pch.h"
#include "Author.h"
#include "CliquesCounting.h"

#define STR bstr_t
#define W int

OPRESULT CliquesCounting::StartCount(Author* st) {
	std::vector<STR>* Collaborators;
	*Collaborators = st->GetCollaboratorsNoWeight().second;

	CliquesCount[1]++;

	std::vector<Author* > Q; 
	Q.push_back(st);

	uint64_t l = 0, r = Collaborators->size - 1 , ret = 0;
	while (l <= r)
	{
		uint16_t mid = (l + r) >> 1;
		if (st->GetName().second < Collaborators->at(mid)) {
			ret = mid;
			r = mid - 1;
		}
		else l = mid + 1;
	}

	for (uint64_t i = ret; i < Collaborators->size; i++)
	{
		Q[1]= &(st->getAuthorByName(Collaborators->at(i)));
		Counting(Q, 2, i+1);
	}
}

//≈–∂œx «∑Ò‘⁄cot÷–
bool CliquesCounting::Check(std::vector<STR>* cot,STR x) {

	std::vector<STR>::iterator it = find(cot->begin(), cot->end(), x);

	if (it != cot->end()) return true;
	return false;
	/* l = 0, r = cot->size - 1, ret = 0;
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
	return true;*/
}

OPRESULT CliquesCounting::Counting(std::vector<Author* > Clique, uint64_t Size, uint64_t st) {
	std::vector<STR>* Collaborators;
	*Collaborators = Clique[0]->GetCollaboratorsNoWeight().second;

	CliquesCount[Size]++;
	MaxSize = (MaxSize < Size) ? Size : MaxSize;

	for (uint64_t i = st; i < Collaborators->size; i++)
	{
		Author now;
		now = now.getAuthorByName(Collaborators->at(i));
		
		STR name = now.GetName().second;
		bool bk = true;
		for (uint64_t j = 0; j < Size; j++)
		{
			std::vector<STR>* cot;
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
}

uint64_t CliquesCounting::GetSize() {
	return MaxSize;
}

std::map<uint64_t, uint64_t> CliquesCounting::GetResult() {
	return CliquesCount;
}