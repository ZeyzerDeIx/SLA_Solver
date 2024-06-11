#include "cohort.h"
#include "instance.h"

using namespace std;

Cohort::Cohort(int id, int size):
	m_instance(nullptr),
	City(id),
	m_size(size)
{}

Cohort::Cohort(Instance& instance, Cohort& other):
	m_instance(&instance),
	City(other.m_id,other.m_demandes),
	m_size(other.m_size)
{
	for(Type& type: other.m_types)
	{
		// On ajoute un nouveau type qui est une copie partielle du type actuel de other. Ce type ne contient pas les tubes c'est pourquoi on en récupère une référence.
		Type& newType = addType(Type(*this, type.getId()));
		cout << "New type a pour id: " << newType.getId() << " et pour adresse: " << &newType << endl;
		// Maintenant on copie les tubes.
		for(Tube& tube: type.getTubes())
		{
			// Pareil ici on ajoute le nouveau tube qui est une copie partielle et on récupère une référence.
			Tube& newTube = newType.addTube(Tube(newType, tube.getVolume()));
			// Avec cette référence on copie l'arbre de répartition du tube.
			newTube.deepTreeCopy(tube.getTree());
		}
	}
}

const Instance& Cohort::getInstance() const {return *m_instance;}
void Cohort::setInstance(Instance& instance) {m_instance=&instance;}

int Cohort::getSize() {return m_size;}

vector<Type>& Cohort::getTypes() {return m_types;}
Type& Cohort::addType(Type type)
{
	m_types.push_back(type);
	return m_types.back();
}

bool Cohort::isCohort() {return true;}

ostream& Cohort::print(ostream& os) const
{
	os << "Cohort: id: " << m_id << " size: " << m_size << endl;
	os << "Demandes:" << endl;
	for(int i=0 ; i<m_demandes.size() ; i++)
		os << "Type " << i << " : " << m_demandes[i] << endl;
	os << "Types:" << endl;
	for(const Type& type: m_types)
		os << type << endl;
	return os;
}

ostream& operator<<(ostream& os, const Cohort& cohort)
{
	return cohort.print(os);
}

bool Cohort::operator==(const int& x) {return x == m_id;}