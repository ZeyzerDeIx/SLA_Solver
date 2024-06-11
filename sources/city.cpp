#include "city.h"

using namespace std;

City::City(int id): m_id(id) {}
City::City(int id, list<int> demandes): m_id(id), m_demandes(demandes) {}

const list<int>& City::getDemandes() {return m_demandes;}
void City::setDemandes(list<int> demandes) {m_demandes = demandes;}

int City::getId() const {return m_id;}

bool City::isCohort() {return false;}

ostream& City::print(ostream& os) const
{
	os << "City: id: " << m_id << endl;
	os << "Demandes:" << endl;
	for(int i=0 ; int demande: m_demandes)
		os << "Type " << i++ << " : " << demande << endl;
	return os;
}

ostream& operator<<(ostream& os, const City& city)
{
	return city.print(os);
}
ostream& operator<<(ostream& os, const City* city)
{
	if(city == nullptr)
		return os << "nullptr";
	return os << city->getId();
}