#include "solution.h"
#include <map>
#include <algorithm>

using namespace std;

Solution::Solution(Instance&& instance):
	m_instance(move(instance)),
	m_tubes(m_instance.getAllTubes())
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
	for(const auto& alicoCount: alicoCounts)
		if(maxAliquo < alicoCount.second)
			maxAliquo = alicoCount.second;

	return maxAliquo;
}

ostream& operator<<(ostream& os, const Solution& solution)
{
	for(Tube* tubePtr: solution.m_instance.getAllTubes())
		cout << "Cohorte: " << tubePtr->getType().getCohort().getId()  << " Type: " << tubePtr->getType().getId() << " Tube :" << tubePtr->getId() << (tubePtr->getUsedByCohort()?" - Utilisé par la cohorte":"") << endl << tubePtr->getTree() << endl;
	return os;
}