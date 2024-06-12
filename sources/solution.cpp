#include "solution.h"

using namespace std;

Solution::Solution(Instance&& instance):
	m_instance(move(instance)),
	m_tubes(m_instance.getAllTubes())
{}


ostream& operator<<(ostream& os, const Solution& solution)
{
	for(Tube* tubePtr: solution.m_instance.getAllTubes())
		cout << "Cohorte: " << tubePtr->getType().getCohort().getId()  << " Type: " << tubePtr->getType().getId() << " Tube :" << tubePtr->getId() << (tubePtr->getUsedByCohort()?" - Utilisé par la cohorte":"") << endl << tubePtr->getTree() << endl;
	return os;
}