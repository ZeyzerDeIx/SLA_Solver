#include "type.h"
#include "cohort.h"
#include "instance.h"
#include "random.h"

using namespace std;
// Un alias commode pour rendre le code plus concis.
using CityTree = Tree<const City*>;

Type::Type(const Cohort& cohort, int id):
	m_cohort(cohort),
	m_id(id)
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

void Type::swapNodes(CityTree& a, CityTree& b, bool saveSwap)
{
	if(&a == nullptr or &b == nullptr) return;
	const City* temp = a.getValue();
	a.setValue(b.getValue());
	b.setValue(temp);

	if(saveSwap) m_swapHistory.push({&a,&b});
}

void Type::swapRandomNodes()
{
	swapNodes(getRandomNode(),getRandomNode());
}

void Type::revertSwap()
{
	if(m_swapHistory.size() == 0) return;

	swapNodes(*m_swapHistory.top().first, *m_swapHistory.top().second, false);
	m_swapHistory.pop();
}

void Type::moveNode(CityTree* toMove, CityTree* a, CityTree* b)
{
	if(toMove == a or toMove == b or a == b) return;

	CityTree* root = toMove->getRoot();
	list<CityTree>& rootList = root->getNodes();
	list<CityTree>& aList = a->getNodes();

	// tmTRoot pour toMoveTrueRoot, la racine là plus lointaine de toMove
	CityTree* tmTRoot = root;
	while (tmTRoot->getRoot() != nullptr)
		tmTRoot = tmTRoot->getRoot();

	// aTRoot pour aTrueRoot
	CityTree* aTRoot = a;
	while (aTRoot->getRoot() != nullptr)
		aTRoot = aTRoot->getRoot();

	cout << "tmTRoot:\n" << *tmTRoot << endl;
	cout << "aTRoot:\n" << *aTRoot << endl;

	ArchivedMove archive = {tmTRoot,CityTree(*tmTRoot), aTRoot, CityTree(*aTRoot)};

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

		// Si le pointeur est null, on reboucle de suite.
		if(toMove == nullptr) continue;

		list<CityTree>& nodes = toMove->getNodes();

		// On cherche parmis les successeurs directs de toMove si dest est présent.
		auto found = find_if(nodes.begin(), nodes.end(),
			[&dest](const CityTree& tree){return dest == &tree;}
		);
		// Si dest est successeur, alors on reboucle car cela reviendrait à faire un simple swap (en plus de provoquer des problèmes)
		loop = found != nodes.end();
	}
	
	for(CityTree& node: dest->getNodes())
	{
		// On tire un noeud aléatoire.
		b = &node;

		if(b->getMaxDepth() != m_cohort.getInstance().getMaxFreeze())
			break;
	}

	moveNode(toMove, dest, b);
}

void Type::revertMove()
{
	if(m_moveHistory.size() == 0) return;

	ArchivedMove& archive = m_moveHistory.top();

	cout << "archive.destTRoot\n" << *archive.destTRoot << endl;
	cout << "archive.formerTRoot\n" << *archive.formerTRoot << endl;
	*archive.destTRoot = move(archive.destTRootSave);
	*archive.formerTRoot = move(archive.formerTRootSave);
	cout << "archive.destTRoot\n" << *archive.destTRoot << endl;
	cout << "archive.formerTRoot\n" << *archive.formerTRoot << endl;

	m_moveHistory.pop();
}

void Type::verbosePrint()
{
	cout << "Type n°" << m_id << " from cohort n°" << m_cohort.getId() << endl;
}

void Type::displayLastSwap()
{
	cout << "Swapped " << m_swapHistory.top().first->getValue() << " with " << m_swapHistory.top().second->getValue() << endl;
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