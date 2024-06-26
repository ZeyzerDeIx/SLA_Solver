#include "type.h"
#include "cohort.h"
#include "instance.h"
#include "random.h"
#include <ranges>
#include <algorithm>

using namespace std;

Type::Type(const Cohort& cohort, int id):
	m_cohort(cohort),
	m_id(id)
{}
Type::~Type() {}

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
	return getRandomTube().getTree().getRandomNode();
}

Tube& Type::getRandomTube()
{
	int rdm = Random::randomNb(0, m_tubes.size()-1);
	auto it = next(m_tubes.begin(), rdm);
	return *it;
}

void Type::swapNodes(CityTree& a, CityTree& b, bool saveSwap)
{
	if(&a == nullptr or &b == nullptr) return;
	const City* temp = a.getValue();
	a.setValue(b.getValue());
	b.setValue(temp);

	if(saveSwap) m_swapHistory.push({&a,&b});
}

void Type::swapNodes(CityTree& a, CityTree& b, CityTree& c, bool saveSwap)
{
	if(&a == nullptr or &b == nullptr or &c == nullptr) return;

	int maxFreeze = m_cohort.getInstance().getMaxFreeze();

	const City* temp = a.getValue();
	a.setValue(b.getValue());
	b.setValue(temp);

	if(a.getDepth() != maxFreeze) moveNode(&c, &a);
	else moveNode(&c, a.getRoot());

	if(saveSwap) m_swapHistory.push({&a,&b,true,&c});
}

void Type::swapNodes(CityTree& a, CityTree& b, CityTree& c, CityTree& d, bool saveSwap)
{
	swapNodes(a,c, false);
	swapNodes(b,d, false);

	if(saveSwap) m_swapHistory.push({&a,&b,false,&c,&d});
}

void Type::swapRandomNodes()
{
	// Première chose, on récupère deux tube aléatoire ayant des noeuds dans leur arbre.
	Tube *tube1(nullptr), *tube2(nullptr);

	vector<Tube*> shufTubes;
	for(Tube& tube : m_tubes)
		shufTubes.push_back(&tube);
	random_device rd;
	mt19937 g(rd());
	shuffle(shufTubes.begin(), shufTubes.end(), g);

	for(auto it = shufTubes.begin(); it != shufTubes.end();)
	{
		if((*it)->getTree().nodeCount() == 0)
			it = shufTubes.erase(it);
		else if(tube1 == nullptr)
			(tube1 = *it), ++it;
		else {(tube2 = *it); break;}
	}

	// Si aucun tube n'a été trouvé pour tube2, alors aucun swap n'est possible
	if(tube2 == nullptr) return;

	CityTree *node1(nullptr), *node2(nullptr), *node3(nullptr), *node4(nullptr);

	switch(Random::randomNb(1,3))
	{
	case 1:
		do
		{
			node1 = &tube1->getTree().getRandomNode();
			node2 = &tube2->getTree().getRandomNode();
		} while(!checkVolumes(*tube1, *node1, *tube2, *node2));

		swapNodes(*node1, *node2);
		break;
	case 2:
		do
		{
			node1 = &tube1->getTree().getRandomNode();
			node2 = &tube2->getTree().getRandomNode();
			node3 = &tube2->getTree().getRandomNode();
		} while(node2 == node3 or !checkVolumes(*tube1, *node1, *tube2, *node2, *node3));

		swapNodes(*node1, *node2, *node3);
		break;
	case 3:
		do
		{
			node1 = &tube1->getTree().getRandomNode();
			node2 = &tube1->getTree().getRandomNode();
			node3 = &tube2->getTree().getRandomNode();
			node4 = &tube2->getTree().getRandomNode();
		} while(node1 == node2 or node3 == node4 or
				!checkVolumes(*tube1, *node1, *node2, *tube2, *node3, *node4));

		swapNodes(*node1, *node2);
		break;
	}
}

void Type::revertSwap()
{
	if(m_swapHistory.empty()) return;

	ArchivedSwap& archive = m_swapHistory.top();

	if(archive.d == nullptr)
		swapNodes(*archive.a, *archive.b, false);
	else
		swapNodes(*archive.a, *archive.b, *archive.c, *archive.d, false);

	if(archive.movedSomething) revertMove();

	m_swapHistory.pop();
}

void Type::moveNode(CityTree* toMove, CityTree* a, CityTree* b)
{
	if(toMove == a or toMove == b or a == b) return;

	CityTree* root = toMove->getRoot();
	list<CityTree>& rootList = root->getNodes();
	list<CityTree>& aList = a->getNodes();

	// tmTRoot pour toMoveTrueRoot, la racine là plus lointaine de toMove
	CityTree* tmTRoot = root->getTrueRoot();

	// aTRoot pour aTrueRoot
	CityTree* aTRoot = a->getTrueRoot();

	ArchivedMove archive = {aTRoot,CityTree(*aTRoot), tmTRoot,  CityTree(*tmTRoot)};

	// Si le noeud à déplacer possèdes des successeurs.
	if(toMove->nodeCount() != 0)
	{
		// On fait remonter le premier élément pour remplacer celui que l'on déplace.
		CityTree& moved = root->moveNode(toMove->getNodes().begin());
		// Les successeurs du noeud à déplacer sont déplacés vers le noeud que l'on vient de remonter.
		moved.moveNodes(*toMove);
	}
	
	// On déplace effectivement le noeud.
	&a->moveNode(*toMove);

	// Si b existe, on le rattache toMove.
	if(b != nullptr)
		&toMove->moveNode(*b);

	// On enregistre le déplacement.
	m_moveHistory.push(archive);
}

void Type::moveRandomNode()
{
	CityTree* dest(nullptr);
	CityTree* toMove(nullptr);
	CityTree* b(nullptr);

	int maxFreeze = m_cohort.getInstance().getMaxFreeze();


	auto canBeMoved = [](CityTree* toMove)
	{
		return toMove != nullptr;
	};
	
	Tube* toMoveTube = nullptr;
	do
	{
		toMoveTube = &getRandomTube();
		toMove = &toMoveTube->getTree().getRandomNode();
	} while (!canBeMoved(toMove));

	auto canBeDest = [&](Tube* tube, CityTree* dest)
	{
		if(dest == nullptr or dest->getDepth() == maxFreeze)
			return false;

		if(toMoveTube != tube and tube->getRemainingVolume() < dest->getValue()->getDemande(tube->getType()))
			return false;
		return true;
	};

	Tube* destTube = nullptr;
	int limit = 0;
	do
	{
		destTube = &getRandomTube();
		dest = &destTube->getTree().getRandomNode();
	} while (!canBeDest(destTube, dest) && ++limit != 100);

	if(limit == 100)
		return;
	
	for(CityTree& node: dest->getNodes())
	{
		b = &node;

		if(b->getMaxDepth() != maxFreeze) break;
		b = nullptr;
	}

	moveNode(toMove, dest, b);
}

// Ces macros permettent juste de rendre le code plus lisible.
#define demandeOf(node) node.getValue()->getDemande(*this)
#define volumeOf(tube) tube.getRemainingVolume()
bool Type::checkVolumes(const Tube& tube1, const CityTree& node1, const Tube& tube2, const CityTree& node2)
{
	return
		demandeOf(node1) <= volumeOf(tube2) + demandeOf(node2) and 
		demandeOf(node2) <= volumeOf(tube1) + demandeOf(node1);
}

bool Type::checkVolumes(const Tube& tube1, const CityTree& node1, const Tube& tube2, const CityTree& node2, const CityTree& node3)
{
	return
		demandeOf(node1) <= volumeOf(tube2) + demandeOf(node2) + demandeOf(node3) and
		demandeOf(node2) + demandeOf(node3) <= volumeOf(tube1) + demandeOf(node1);
}

bool Type::checkVolumes(const Tube& tube1, const CityTree& node1, const CityTree& node2, const Tube& tube2, const CityTree& node3, const CityTree& node4)
{
	return
		demandeOf(node1) + demandeOf(node2) <= volumeOf(tube2) + demandeOf(node3) + demandeOf(node4) and
		demandeOf(node3) + demandeOf(node4) <= volumeOf(tube1) + demandeOf(node1) + demandeOf(node2);
}

void Type::revertMove()
{
	if(m_moveHistory.empty()) return;

	ArchivedMove& archive = m_moveHistory.top();

	*archive.destTRoot = move(archive.destTRootSave);
	*archive.formerTRoot = move(archive.formerTRootSave);

	m_moveHistory.pop();
}

void Type::verbosePrint()
{
	cout << "Type n°" << m_id << " from cohort n°" << m_cohort.getId() << endl;
}

void Type::displayLastSwap()
{
	ArchivedSwap& archive = m_swapHistory.top();
	cout << "Swapped " << archive.a->getValue() << " with " << archive.b->getValue() << endl;
	if(archive.movedSomething) cout << m_moveHistory.top() << endl;
}

void Type::displayMoveHistory()
{
	stack<ArchivedMove> movesBuffer;
	while(!m_moveHistory.empty())
	{
		movesBuffer.push(move(m_moveHistory.top()));
		m_moveHistory.pop();
	}

	while(!movesBuffer.empty())
	{
		ArchivedMove& archive = movesBuffer.top();

		cout << archive << endl;

		m_moveHistory.push(move(archive));
		movesBuffer.pop();
	}
}

ostream& operator<<(ostream& os, const Type& type)
{
	os << "Type n° " << type.m_id << " :" << endl;
	cout << "size: " << type.m_tubes.size() << endl;
	for(const Tube& tube: type.m_tubes)
		os << tube << endl;
	return os;
}

ostream& operator<<(ostream& os, const ArchivedMove& move)
{
	os << "Destination before move:\n" << move.destTRootSave << endl;
	os << "Destination after move:\n" << *move.destTRoot << endl;
	os << "Former root before move:\n" << move.formerTRootSave << endl;
	os << "Former root after move:\n" << *move.formerTRoot << endl;
	return os;
}

bool Type::operator==(const Type& other) {return m_id == other.m_id;}