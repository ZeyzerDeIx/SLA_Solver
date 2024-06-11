#include "city.h"

using namespace std;

City::City(int id): m_id(id) {}
City::City(int id, vector<int> demandes): m_id(id), m_demandes(demandes) {}

const vector<int>& City::getDemandes() {return m_demandes;}
void City::setDemandes(vector<int> demandes) {m_demandes = demandes;}

int City::getId() const {return m_id;}

bool City::isCohort() {return false;}

ostream& City::print(ostream& os) const
{
	os << "City: id: " << m_id << endl;
	os << "Demandes:" << endl;
	for(int i=0 ; i<m_demandes.size() ; i++)
		os << "Type " << i << " : " << m_demandes[i] << endl;
	return os;
}

ostream& operator<<(ostream& os, const City& city)
{
	return city.print(os);
}
ostream& operator<<(ostream& os, const City* city)
{
	if(city == nullptr)
		os << "nullptr";
	else os << city;
	return os;
}