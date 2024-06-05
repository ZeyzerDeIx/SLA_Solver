#include "instance.h"

using namespace std;

Instance::Instance(vector<City*> &cities, vector<Cohort*> &cohorts, int maxFreeze):
	m_cities(cities),
	m_cohorts(cohorts),
	m_maxFreeze(maxFreeze)
{}

Instance::Instance(const Instance& other): m_cities(other.m_cities.size()), m_maxFreeze(other.m_maxFreeze)
{
	vector<Cohort*> cohorts;
    // Copie des villes
    for (size_t i = 0; i < other.m_cities.size(); ++i)
    {
    	if(Cohort* cohortPtr = dynamic_cast<Cohort*>(other.m_cities[i]))
    	{
    		Cohort* newCohortPtr = new Cohort(*(cohortPtr));
    		cohorts.push_back(newCohortPtr);
    		m_cities[i] = newCohortPtr;
    	}
    	else m_cities[i] = new City(*(other.m_cities[i]));
    }
    for(Cohort* cohortPtr: other.m_cohorts)
    	for(Cohort* cohortPtr2: cohorts)
    		if(cohortPtr->getId() == cohortPtr2->getId())
    			m_cohorts.push_back(cohortPtr2);
}

Instance::~Instance()
{
	for (City* cityPtr : m_cities) delete cityPtr;
}

const vector<City*>& Instance::getCities() {return m_cities;}
const vector<Cohort*>& Instance::getCohorts() {return m_cohorts;}

int Instance::getMaxFreeze() {return m_maxFreeze;}

ostream& operator<<(ostream& os, const Instance& instance)
{
	os << "Instance: " << endl << endl;
	for (City* cityPtr : instance.m_cities)
	{
		cityPtr->print(os);
		os << endl;
	}
	return os;
}