#include "cohort.h"

using namespace std;

Cohort::Cohort(int id, int size): City(id), m_size(size) {}

void Cohort::setTubes(vector<vector<Tube>> tubes) {m_tubes = tubes;}

bool Cohort::isCohort() {return true;}

vector<vector<Tube>>& Cohort::getTubes() {return m_tubes;}

ostream& operator<<(ostream& os, const Cohort& cohort)
{
	cohort.print(os);
	return os;
}

bool Cohort::operator==(const int& x)
{
	return x == m_id;
}



void Cohort::print(ostream& os) const
{
	os << "Cohort: id: " << m_id << " size: " << m_size << endl;
	os << "Demandes:" << endl;
	for(int i=0 ; i<m_demandes.size() ; i++)
		os << "Type " << i << " : " << m_demandes[i] << endl;
	os << "Tubes:" << endl;
	for(int i=0 ; i<m_tubes.size() ; i++)
		for(int j=0 ; j<m_tubes[i].size(); j++)
			os << "Type " << i << ", tube " << j << ": " << m_tubes[i][j];
}