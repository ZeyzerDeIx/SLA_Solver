#include "type.h"
#include "cohort.h"
#include "random.h"

using namespace std;

Type::Type(const Cohort& cohort, int id):
	m_cohort(cohort),
	m_id(id),
	m_swappedNodes{nullptr,nullptr}
{}
Type::~Type()
{}

int Type::getId() const {return m_id;}

const Cohort& Type::getCohort() const {return m_cohort;}

list<Tube>& Type::getTubes() {return m_tubes;}
const list<Tube>& Type::getTubes() const {return m_tubes;}

Tube& Type::addTube(Tube tube)
{
	m_tubes.push_back(tube);
	return m_tubes.back();
}

void Type::swapNodes(Tree<const City*>& a, Tree<const City*>& b)
{
	if(&a == nullptr or &b == nullptr) return;
	const City* temp = a.getValue();
	a.setValue(b.getValue());
	b.setValue(temp);

	m_swappedNodes.first = &a;
	m_swappedNodes.second = &b;
}

void Type::swapRandomNodes()
{
	auto rdmNode = [this]() -> Tree<const City*>&
	{
		int rdm = Random::randomNb(0, m_tubes.size()-1);
		auto it = next(m_tubes.begin(), rdm);
		return it->getTree().getRandomNode();
	};
	swapNodes(rdmNode(),rdmNode());
}

void Type::revertSwap()
{
	if(m_swappedNodes.first != nullptr)
		swapNodes(*m_swappedNodes.first, *m_swappedNodes.second);
	m_swappedNodes = {nullptr,nullptr};
}

void Type::verbosePrint()
{
	cout << "Type n°" << m_id << " from cohort n°" << m_cohort.getId() << endl;
}

void Type::displayLastSwap()
{
	std::cout << "Swapped " << m_swappedNodes.first->getValue() << " with " << m_swappedNodes.second->getValue() << std::endl;
}

ostream& operator<<(ostream& os, const Type& type)
{
	os << "Type n° " << type.m_id << " :" << endl;
	cout << "size: " << type.m_tubes.size() << endl;
	for(const Tube& tube: type.m_tubes)
		os << tube << endl;
	return os;
}

bool Type::operator==(const Type& other) {return m_id == other.m_id;}