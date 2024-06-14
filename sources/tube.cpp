#include "cohort.h"
#include "instance.h"

using namespace std;

Tube::Tube(const Type& type, int volume, int id):
	m_type(type),
	m_id(id),
	m_volume(volume),
	m_usedVolume(0),
	m_usedByCohort(false)
{}

void deepTreeCopyHelper(const Tree<const City*>& treeSrc, Tree<const City*>& treeDst, const list<unique_ptr<City>>& cities)
{
	for(const auto& nextTree: treeSrc.getNodes())
		deepTreeCopyHelper(nextTree, treeDst.addNode(nullptr), cities);
	treeDst.setValue(nullptr);
	if(treeSrc.getValue() == nullptr) return;

	for(const auto& cityPtr: cities)
		if(cityPtr->getId() == treeSrc.getValue()->getId())
			treeDst.setValue(cityPtr.get());
}
void Tube::deepTreeCopy(const Tree<const City*>& tree)
{
	const auto& cities = m_type.getCohort().getInstance().getCities();
	deepTreeCopyHelper(tree, m_tree, cities);
}

const Type& Tube::getType() const {return m_type;}

int Tube::getId() const {return m_id;}

int Tube::getVolume() const {return m_volume;}

int Tube::getUsedVolume() const {return m_usedVolume;}
void Tube::setUsedVolume(int usedVolume) {m_usedVolume = usedVolume;}

int Tube::getRemainingVolume() const {return m_volume-m_usedVolume;}

void Tube::consume(int volume) {m_usedVolume += volume;}

bool Tube::getUsedByCohort() {return m_usedByCohort;}
void Tube::setUsedByCohort(bool usedbyCohort) {m_usedByCohort = usedbyCohort;}

Tree<const City*>& Tube::getTree() {return m_tree;}
const Tree<const City*>& Tube::getTree() const {return m_tree;}

void Tube::verbosePrint()
{
	cout << "Tube n°" << m_id << " from type n°" << m_type.getId() << " from cohort n°" << m_type.getCohort().getId() << endl;
}

ostream& operator<<(ostream& os, const Tube& tube)
{
	return os << tube.m_volume-tube.m_usedVolume << "/" << tube.m_volume;
}