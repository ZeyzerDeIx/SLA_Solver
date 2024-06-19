#include "type.h"
#include "cohort.h"
#include "instance.h"
#include "random.h"

using namespace std;
// Un alias commode pour rendre le code plus concis.
using CityTree = Tree<const City*>;

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

CityTree& Type::getRandomNode()
{
	int rdm = Random::randomNb(0, m_tubes.size()-1);
	auto it = next(m_tubes.begin(), rdm);
	return it->getTree().getRandomNode();
}

void Type::swapNodes(CityTree& a, CityTree& b)
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
	swapNodes(getRandomNode(),getRandomNode());
}

void Type::revertSwap()
{
	if(m_swappedNodes.first != nullptr)
		swapNodes(*m_swappedNodes.first, *m_swappedNodes.second);
	m_swappedNodes = {nullptr,nullptr};
}

void Type::moveNode(CityTree* toMove, CityTree* a, CityTree* b)
{
	if(toMove == a or toMove == b or a == b) return;

	CityTree* root = toMove->getRoot();
	list<CityTree>& rootList = root->getNodes();
	list<CityTree>& aList = a->getNodes();

	ArchivedMove archive = {nullptr,nullptr,root,nullptr};

	// Si le noeud à déplacer possèdes des successeurs.
	if(toMove->nodeCount() != 0)
	{
		// On fait remonter le premier élément pour remplacer celui que l'on déplace.
		CityTree& moved = root->moveNode(toMove->getNodes().begin());
		// Les successeurs du noeud à déplacer sont déplacés vers le noeud que l'on vient de remonter.
		moved.moveNodes(*toMove);
		// On enregistre moved comme ayant remplacé toMove dans la pile.
		archive.replacement = &moved;
	}
	
	// On déplace effectivement le noeud et on stock sa nouvelle valeur dans la pile d'archive des déplacement.
	archive.moved = &a->moveNode(*toMove);

	// Si b existe, on le rattache toMove et on l'enregistre.
	if(b != nullptr)
		archive.replaced = &toMove->moveNode(*b);

	// On enregistre le déplacement.
	m_moveHistory.push(archive);
}

void Type::moveRandomNode()
{
	CityTree* dest(nullptr);
	CityTree* toMove(nullptr);
	CityTree* b(nullptr);
	for(bool loop = true; loop;)
	{
		// On tire un noeud aléatoire.
		dest = &getRandomNode();
		// Si l'unes des conditions suivante est vrai, alors on recommance la boucle.
		loop =
			// Si le noeud est null.
			dest == nullptr or
			// Si la destination est déjà à la profondeure max.
			dest->getDepth() == m_cohort.getInstance().getMaxFreeze();
	}
	
	for(bool loop = true; loop;)
	{
		// On tire un noeud aléatoire.
		toMove = &getRandomNode();

		if(toMove == nullptr) continue;

		list<CityTree>& nodes = toMove->getNodes();

		// On cherche parmis les successeurs directs de toMove si dest est présent.
		auto found = find_if(nodes.begin(), nodes.end(),
			[&dest](const CityTree& tree){return dest == &tree;}
		);
		// Si l'unes des conditions suivante est vrai, alors on recommance la boucle.
		loop = found != nodes.end();
	}
	
	for(CityTree& node: dest->getNodes())
	{
		// On tire un noeud aléatoire.
		b = &node;

		
		if(b->getMaxDepth() != m_cohort.getInstance().getMaxFreeze())
			break;
	}
	
	//TODO: vérifier que le déplacement est légal.


	CityTree* root(nullptr);
	for(root = toMove->getRoot(); root->getRoot() != nullptr; root = root->getRoot());
	if(root != nullptr) cout << "toMoveRoot:\n" << *root << endl;
	cout << "toMove: " << toMove << endl << *toMove << endl;
	cout << "dest:\n" << *dest << endl;

	moveNode(toMove, dest, b);


	if(root != nullptr) cout << "toMoveRoot:\n" << *root << endl;
	cout << "toMove: " << toMove << endl << *toMove << endl;
	cout << "dest:\n" << *dest << endl;
}

void Type::revertMove()
{
	if(m_moveHistory.size() == 0) return;

	ArchivedMove& archive = m_moveHistory.top();

	// Si il y a eu un noeud remplacé par (et donc successeur de) moved, on le remet à sa place.
	if(archive.replaced != nullptr)
		archive.moved->getRoot()->moveNode(*archive.replaced);

	// On replace l'élément déplacé.
	archive.formerRoot->moveNode(*archive.moved);

	// Si il en avait, on lui redonne ses successeurs.
	if(archive.replacement != nullptr)
	{
		archive.moved->moveNodes(*archive.replacement);
		archive.moved->moveNode(*archive.replacement);
	}

	m_moveHistory.pop();
}

void Type::verbosePrint()
{
	cout << "Type n°" << m_id << " from cohort n°" << m_cohort.getId() << endl;
}

void Type::displayLastSwap()
{
	cout << "Swapped " << m_swappedNodes.first->getValue() << " with " << m_swappedNodes.second->getValue() << endl;
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