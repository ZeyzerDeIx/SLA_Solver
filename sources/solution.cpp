#include "solution.h"
#include <map>
#include <algorithm>
#include <ranges>
#include <random>
#include <climits>

using namespace std;

/**
 * @brief Récupérer un nombre aléatoire entre a et b.
 * 
 * \note
 * Je ne suis pas certains qu'il soit pertinent de ranger cette fonction ici, cependant elle n'est utilisée nul part ailleurs dans le code et je ne sais pas trop où la mettre sinon, à toute personne lisant ceci, sentez vous libre de la déplacer.
 *
 * @param[in] a Borne inférieure.
 * @param[in] b Borne suppérieure.
 *
 * @return Une nombre aléatoire entre a et b.
 */
int randomNb(int a = 0, int b = INT_MAX)
{
    // Générateur de nombres aléatoires
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(a, b);

    // Récupérer un élément aléatoire
	return dis(gen);
}

Solution::Solution(Instance&& instance):
	m_instance(move(instance)),
	m_tubes(m_instance.getAllTubes()),
	m_swappedTube(nullptr)
{}

Solution::Solution(const Solution& other):
	m_instance(other.m_instance),
	m_tubes(m_instance.getAllTubes()),
	m_swappedTube(nullptr)
{}

void getMaxAliquoHelper(map<const City*, unsigned int>& alicoCounts, const Tree<const City*>& tree, int cohortSize)
{
	alicoCounts[tree.getValue()] += max(0,tree.nodeCount()-1) * cohortSize;

	for(const auto& node: tree.getNodes())
		getMaxAliquoHelper(alicoCounts, node, cohortSize);
}

unsigned int Solution::getMaxAliquo() const
{
	// Sert à assoscier à chaque ville un nombre d'aliquotages.
	map<const City*, unsigned int> alicoCounts;
	for(const Cohort* cohortPtr: m_instance.getCohorts())
		for(const Type& type: cohortPtr->getTypes())
			for(const Tube& tube: type.getTubes())
				getMaxAliquoHelper(alicoCounts, tube.getTree(), cohortPtr->getSize());

	unsigned int maxAliquo(0);
	for(const auto& count: ranges::views::values(alicoCounts))
		if(maxAliquo < count) maxAliquo = count;

	return maxAliquo;
}

bool Solution::isBetterThan(const Solution& other)
{
	return getMaxAliquo() < other.getMaxAliquo();
}

void Solution::randomSwapInTube()
{
	Tube* tube = m_tubes[randomNb(0,m_tubes.size()-1)];
	Tree<const City*>& tree = tube->getTree();

	tree.swapNodes(tree.getRandomNode(), tree.getRandomNode());
	m_swappedTube = tube;
}

void Solution::revertSwapInTube()
{
	if(m_swappedTube != nullptr)
		m_swappedTube->getTree().revertSwap();
	m_swappedTube = nullptr;
}

void Solution::randomSwapInType()
{}

void Solution::displayLastSwap()
{
	m_swappedTube->verbosePrint();
	m_swappedTube->getTree().displayLastSwap();
}

ostream& operator<<(ostream& os, const Solution& solution)
{
	for(Tube* tubePtr: solution.m_instance.getAllTubes())
		cout << "Cohorte: " << tubePtr->getType().getCohort().getId()  << " Type: " << tubePtr->getType().getId() << " Tube :" << tubePtr->getId() << (tubePtr->getUsedByCohort()?" - Utilisé par la cohorte":"") << endl << tubePtr->getTree() << endl;
	return os;
}