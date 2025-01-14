#include "instance.h"

using namespace std;

Instance::Instance() {}

Instance::Instance(list<unique_ptr<City>> &cities, list<Cohort*> &cohorts, int maxFreeze):
	m_cities(move(cities)),
	m_cohorts(cohorts),
	m_maxFreeze(maxFreeze)
{}

Instance::Instance(const Instance& other): m_cities(), m_maxFreeze(other.m_maxFreeze)
{
	list<Cohort*> cohorts;
    // Copie des villes
    for (const auto& cityPtr: other.m_cities)
    {
    	if(Cohort* cohortPtr = dynamic_cast<Cohort*>(cityPtr.get()))
    	{
    		Cohort* newCohortPtr = new Cohort(*this,*cohortPtr);
    		cohorts.push_back(newCohortPtr);
    		m_cities.push_back(unique_ptr<City>(newCohortPtr));
    	}
    	else m_cities.push_back(make_unique<City>(*cityPtr));
    }

    for(Cohort* cohortPtr: other.m_cohorts)
    	for(Cohort* cohortPtr2: cohorts)
    		if(cohortPtr->getId() == cohortPtr2->getId())
    			m_cohorts.push_back(cohortPtr2);
}

Instance::Instance(Instance&& other) noexcept :
    m_cities(move(other.m_cities)),
    m_cohorts(move(other.m_cohorts)),
    m_maxFreeze(other.m_maxFreeze)
{}


const list<unique_ptr<City>>& Instance::getCities() const {return m_cities;}
list<Cohort*>& Instance::getCohorts() {return m_cohorts;}
const list<Cohort*>& Instance::getCohorts() const {return m_cohorts;}

int Instance::getMaxFreeze() const {return m_maxFreeze;}

vector<Tube*> Instance::getAllTubes() const
{
	vector<Tube*> tubes;
	for(Cohort* cohortPtr: m_cohorts)
		for(Type& type: cohortPtr->getTypes())
			for(Tube& tube: type.getTubes())
				tubes.push_back(&tube);

	return tubes;
}

vector<Type*> Instance::getAllTypes() const
{
	vector<Type*> types;
	for(Cohort* cohortPtr: m_cohorts)
		for(Type& type: cohortPtr->getTypes())
			types.push_back(&type);

	return types;
}

ostream& operator<<(ostream& os, const Instance& instance)
{
	os << "Instance: " << endl << endl;
	for (const auto& cityPtr : instance.m_cities)
	{
		cityPtr->print(os) << endl;
	}
	return os;
}