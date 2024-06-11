#include "type.h"

using namespace std;

Type::Type(const Cohort& cohort, int id):
	m_cohort(cohort),
	m_id(id)
{}
Type::~Type()
{}

int Type::getId() const {return m_id;}

const Cohort& Type::getCohort() const {return m_cohort;}

bool Type::operator==(const Type& other) {return m_id == other.m_id;}

list<Tube>& Type::getTubes() {return m_tubes;}
Tube& Type::addTube(Tube tube)
{
	m_tubes.push_back(tube);
	return m_tubes.back();
}

ostream& operator<<(ostream& os, const Type& type)
{
	os << "Type n° " << type.m_id << " :" << endl;
	cout << "size: " << type.m_tubes.size() << endl;
	for(const Tube& tube: type.m_tubes)
		os << tube << endl;
	return os;
}