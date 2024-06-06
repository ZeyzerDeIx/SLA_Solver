#include "type.h"

using namespace std;

Type::Type(Cohort& cohort): m_cohort(cohort) {}
Type::~Type() {}

bool Type::operator==(const Type& other) {return m_id == other.m_id;}

vector<Tube>& Type::getTubes() {return m_tubes;}

ostream& operator<<(ostream& os, const Type& type)
{
	os << "Type n° " << type.m_id << " :" << endl;
	for(const Tube& tube: type.m_tubes)
		os << tube << endl;
	return os;
}