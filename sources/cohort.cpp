#include "cohort.h"

using namespace std;

Cohort::Cohort(int id, int size): City(id), m_size(size) {}

int Cohort::getSize() {return m_size;}

vector<Type>& Cohort::getTypes() {return m_types;}

bool Cohort::isCohort() {return true;}

ostream& operator<<(ostream& os, const Cohort& cohort)
{
	os << "Cohort: id: " << cohort.m_id << " size: " << cohort.m_size << endl;
	os << "Demandes:" << endl;
	for(int i=0 ; i<cohort.m_demandes.size() ; i++)
		os << "Type " << i << " : " << cohort.m_demandes[i] << endl;
	os << "Types:" << endl;
	for(const Type& type: cohort.m_types)
		os << type << endl;
	return os;
}

bool Cohort::operator==(const int& x) {return x == m_id;}