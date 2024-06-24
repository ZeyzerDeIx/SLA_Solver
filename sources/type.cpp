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
	CityTree* tmTRoot = root->getTrueRoot();

	// aTRoot pour aTrueRoot
	CityTree* aTRoot = a->getTrueRoot();

	//cout << "toMove\n" << *toMove << endl;
	//cout << "tmTRoot\n" << *tmTRoot << endl;

	//cout << "dest\n" << *a << endl;
	//cout << "destTRoot\n" << *aTRoot << endl;

	ArchivedMove archive = {aTRoot,CityTree(*aTRoot), tmTRoot, CityTree(*tmTRoot)};

	/*cout << "archived version" << endl;
	cout << "tmTRoot\n" << archive.formerTRootSave << endl;

	cout << "destTRoot\n" << archive.destTRootSave << endl;*/

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

	//cout << "moved version" << endl;

	//cout << "toMove\n" << *toMove << endl;
	//cout << "tmTRoot\n" << *tmTRoot << endl;

	//cout << "dest\n" << *a << endl;
	//cout << "destTRoot\n" << *aTRoot << endl;

	//cout << "node moved" << endl;
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

void Type::revertMove()
{
	if(m_moveHistory.size() == 0) return;

	ArchivedMove& archive = m_moveHistory.top();

	//cout << "revert begin" << endl;

	//cout << "destTRoot:\n" << *archive.destTRoot << endl;
	//cout << "formerTRoot:\n" << *archive.formerTRoot << endl;
	//cout << "rdm former node:\n" << *archive.formerTRoot->getRandomNode().getTrueRoot() << endl;

	//cout << "destTRootSave rdm node:\n" << *archive.destTRootSave.getRandomNode().getTrueRoot() << endl;

	*archive.destTRoot = move(archive.destTRootSave);
	*archive.formerTRoot = move(archive.formerTRootSave);
	//cout << "destTRoot:\n" << *archive.destTRoot << endl;
	//cout << "formerTRoot:\n" << *archive.formerTRoot << endl;
	//cout << "rdm former node:\n" << *archive.formerTRoot->getRandomNode().getTrueRoot() << endl;

	m_moveHistory.pop();
	//cout << "revert end" << endl;
}

void Type::verbosePrint()
{
	cout << "Type n°" << m_id << " from cohort n°" << m_cohort.getId() << endl;
}

void Type::displayLastSwap()
{
	cout << "Swapped " << m_swapHistory.top().first->getValue() << " with " << m_swapHistory.top().second->getValue() << endl;
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

		cout << "Destination before move:\n" << archive.destTRootSave << endl;
		cout << "Destination after move:\n" << *archive.destTRoot << endl;
		cout << "Former root before move:\n" << archive.formerTRootSave << endl;
		cout << "Former root after move:\n" << *archive.formerTRoot << endl;

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

bool Type::operator==(const Type& other) {return m_id == other.m_id;}