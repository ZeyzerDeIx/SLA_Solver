#include "solution.h"
#include "random.h"
#include <map>
#include <algorithm>
#include <ranges>

using namespace std;

// Quelques macro pour éviter la duplication de code et rendre le tout plus lisible
#define DISPLAY_SWAP(obj) \
	if (obj != nullptr) \
	{ \
		obj->verbosePrint(); \
		obj->displayLastSwap(); \
	}
#define REVERT_SWAP(obj) \
	if (obj != nullptr) \
		obj->revertSwap(); \
	obj = nullptr;

Solution::Solution(Instance&& instance):
	m_instance(move(instance)),
	m_tubes(m_instance.getAllTubes()),
	m_types(m_instance.getAllTypes()),
	m_swappedTube(nullptr),
	m_swappedType(nullptr)
{}

Solution::Solution(const Solution& other):
	m_instance(other.m_instance),
	m_tubes(m_instance.getAllTubes()),
	m_types(m_instance.getAllTypes()),
	m_swappedTube(nullptr),
	m_swappedType(nullptr)
{}

void getMaxAliquoHelper(map<const City*, unsigned int>& alicoCounts, const Tree<const City*>& tree, int cohortSize)
{
	alicoCounts[tree.getValue()] += max(0,tree.nodeCount()-1) * cohortSize;

	for(const auto& node: tree.getNodes())
		getMaxAliquoHelper(alicoCounts, node, cohortSize);
}

unsigned int Solution::getMaxAliquo() const
{
	// Sert à assoscier à chaque ville un nombre d'aliquotages.
	map<const City*, unsigned int> alicoCounts;
	for(const Cohort* cohortPtr: m_instance.getCohorts())
		for(const Type& type: cohortPtr->getTypes())
			for(const Tube& tube: type.getTubes())
				getMaxAliquoHelper(alicoCounts, tube.getTree(), cohortPtr->getSize());

	unsigned int maxAliquo(0);
	for(const auto& count: ranges::views::values(alicoCounts))
		if(maxAliquo < count) maxAliquo = count;

	return maxAliquo;
}

bool Solution::isBetterThan(const Solution& other)
{
	return getMaxAliquo() < other.getMaxAliquo();
}

void Solution::randomSwapInTube()
{
	Tube* tube = m_tubes[Random::randomNb(0,m_tubes.size()-1)];

	tube->swapRandomNodes();
	m_swappedTube = tube;
}

void Solution::randomSwapInType()
{
	Type* type = m_types[Random::randomNb(0,m_types.size()-1)];
	type->swapRandomNodes();
	m_swappedType = type;
}

void Solution::revertSwap()
{
	REVERT_SWAP(m_swappedTube);
	REVERT_SWAP(m_swappedType);
}

void Solution::displayLastSwap()
{
	DISPLAY_SWAP(m_swappedTube);
	DISPLAY_SWAP(m_swappedType);
}

void Solution::randomMoveInType()
{
	Type* type = m_types[Random::randomNb(0,m_types.size()-1)];
	type->moveRandomNode();
	m_movedTypeHistory.push(type);
}

void Solution::revertMove()
{
	if(m_movedTypeHistory.size() == 0) return;

	m_movedTypeHistory.top()->revertMove();
	m_movedTypeHistory.pop();
}

ostream& operator<<(ostream& os, const Solution& solution)
{
	for(Tube* tubePtr: solution.m_instance.getAllTubes())
		cout << "Cohorte: " << tubePtr->getType().getCohort().getId()  << " Type: " << tubePtr->getType().getId() << " Tube :" << tubePtr->getId() << (tubePtr->getUsedByCohort()?" - Utilisé par la cohorte":"") << endl << tubePtr->getTree() << endl;
	return os;
}