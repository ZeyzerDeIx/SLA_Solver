#pragma once
#include <list>
#include <stack>
#include "tube.h"

// Un alias commode pour rendre le code plus concis.
using CityTree = Tree<const City*>;


/**
 * @brief Cette structure permet d'archiver toutes les information nécessaires à l'annulation d'un swap.
 */
struct ArchivedSwap
{
	CityTree* a;
	CityTree* b;
	bool movedSomething = false;
	CityTree* c = nullptr;
	CityTree* d = nullptr;
};

/**
 * @brief Cette structure permet d'archiver toutes les information nécessaires à l'annulation d'un déplacement.
 */
struct ArchivedMove
{
	/**
	 * @brief destTRoot pour destinationTrueRoot, la racine la plus lointaine de la destination avant déplacement.
	 */
	CityTree* destTRoot;
	CityTree destTRootSave;
	/**
	 * @brief formerTRoot pour formerTrueRoot, la racine la plus lointaine du noeud avant déplacement.
	 */
	CityTree* formerTRoot;
	CityTree formerTRootSave;
};
std::ostream& operator<<(std::ostream& os, const ArchivedMove& move);


/**
 * @brief Cette classe modélise une type de tube.
 * 
 * Un type contient les tubes qui lui sont rattaché ainsi qu'à sa cohorte.
 */
class Type
{
public:
	Type(const Cohort& cohort, int id);
	~Type();

	int getId() const;

	const Cohort& getCohort() const;

	/**
	 * @brief Récupérer la liste des tubes du type.
	 *
	 * @return Une référence à liste des tubes.
	 */
	std::list<Tube>& getTubes();
	/**
	 * @brief Récupérer la liste (const) des tubes du type.
	 *
	 * @return Une référence constante à la liste des tubes.
	 * 
	 * \note
	 * Variante constante.
	 */
	const std::list<Tube>& getTubes() const;

	/**
	 * @brief Permet d'ajouter un tube au type.
	 *
	 * @param[in] tube Le tube à ajouter.
	 * 
	 * @return Une référence au tube ajouté.
	 */
	Tube& addTube(Tube tube);

	/**
	 * @brief Récupérer un noeud aléatoire d'un tube aléatoire du type.
	 *
	 * @return Le noeud aléatoire.
	 */
	Tree<const City*>& getRandomNode();

	/**
	 * @brief Récupérer un tube aléatoire du type.
	 *
	 * @return Le tube aléatoire.
	 */
	Tube& getRandomTube();

	/**
	 * @brief Échange les valeures de deux noeuds dans les tubes.
	 *
	 * @param a Premier noeud.
	 * @param b Second noeud.
	 * @param[in] saveSwap Si le swap doit être sauvegardé dans l'historique des swap. Typiquement faux en cas de revert.
	 */
	void swapNodes(CityTree& a, CityTree& b, bool saveSwap = true);
	/**
	 * @brief Échange les valeures d'un noeud avec deux autres dans deux tubes.
	 * 
	 * Le noeud solitaire est échangé normalement avec l'un des deux autres et le dernier est inséré si possible en tant que successeurs de celui qui vient d'être échangé. Si cela est impossible, il sera alors successeur de la même racine.
	 * 
	 * \note
	 * Les noeuds 2 et 3 doivent provenir du même tube.
	 *
	 * @param a Premier noeud.
	 * @param b Deuxième noeud.
	 * @param c Troisième noeud.
	 * @param[in] saveSwap Si le swap doit être sauvegardé dans l'historique des swap. Typiquement faux en cas de revert.
	 */
	void swapNodes(CityTree& a, CityTree& b, CityTree& c, bool saveSwap = true);
	/**
	 * @brief Échange les valeures de deux noeud avec deux autres dans deux tubes différents.
	 * 
	 * \note
	 * Les noeuds 1 et 2 doivent provenir du même tube et il en va de même pour 3 et 4.
	 *
	 * @param a Premier noeud.
	 * @param b Deuxième noeud.
	 * @param c Troisième noeud.
	 * @param d Quatrième noeud.
	 * @param[in] saveSwap Si le swap doit être sauvegardé dans l'historique des swap. Typiquement faux en cas de revert.
	 */
	void swapNodes(CityTree& a, CityTree& b, CityTree& c, CityTree& d, bool saveSwap = true);

	/**
	 * @brief Swap deux noeuds aléatoire entre deux tubes aléatoires en vérifiant que leur volumes le permettent.
	 */
	void swapRandomNodes();

	/**
	 * @brief Annule le dernier swap.
	 */
	void revertSwap();

	/**
	 * @brief Déplacer le noeud de sa position actuelle vers une position entre a et b.
	 * 
	 * Enregistre également le déplacement dans une pile.
	 *
	 * @param toMove Le noeud à déplacer.
	 * @param a L'arbre qui sera la nouvelle racine du noeud déplacé.
	 * @param b L'arbre qui deviendra successeur du noeud déplacé. Par défaut sa valeure est nullptr, cela signifie que toMove est juste inséré dans les successeurs de a.
	 * 
	 * \note
	 * Cette méthode ne vérifie pas la légalité du mouvement demandé. À utiliser avec précautions.
	 */
	void moveNode(CityTree* toMove, CityTree* a, Tree<const City*>* b = nullptr);

	/**
	 * @brief Déplace un noeud choisit aléatoirement vers un autre également aléatoire.
	 */
	void moveRandomNode();

	/**
	 * @brief Vérifie si les nœuds peuvent être déplacés dans le tube de l’autre.
	 *
	 * @param[in] tube1 Le tube 1.
	 * @param[in] node1 Le noeud 1 du tube 1.
	 * @param[in] tube2 Le tube 2.
	 * @param[in] node2 Le noeud 2 du tube 2.
	 *
	 * @return True si cela est possible, false si les volumes ne coïncident pas.
	 */
	bool checkVolumes(const Tube& tube1, const CityTree& node1, const Tube& tube2, const CityTree& node2);

	/**
	 * @brief Vérifie si les nœuds peuvent être déplacés d'un tube à l’autre.
	 *
	 * @param[in] tube1 Le tube 1.
	 * @param[in] node1 Le noeud 1 du tube 1.
	 * @param[in] tube2 Le tube 2.
	 * @param[in] node2 Le noeud 2 du tube 2.
	 * @param[in] node3 Le noeud 3 du tube 2.
	 *
	 * @return True si cela est possible, false si les volumes ne coïncident pas.
	 */
	bool checkVolumes(const Tube& tube1, const CityTree& node1, const Tube& tube2, const CityTree& node2, const CityTree& node3);

	/**
	 * @brief Vérifie si les nœuds peuvent être déplacés d'un tube à l’autre deux à deux.
	 *
	 * @param[in] tube1 Le tube 1.
	 * @param[in] node1 Le noeud 1 du tube 1.
	 * @param[in] node2 Le noeud 2 du tube 1.
	 * @param[in] tube2 Le tube 2.
	 * @param[in] node3 Le noeud 3 du tube 2.
	 * @param[in] node4 Le noeud 4 du tube 2.
	 *
	 * @return True si cela est possible, false si les volumes ne coïncident pas.
	 */
	bool checkVolumes(const Tube& tube1, const CityTree& node1, const CityTree& node2, const Tube& tube2, const CityTree& node3, const CityTree& node4);

	/**
	 * @brief Annule le dernier déplacement de noeud effectué dans le type.
	 * 
	 * \warning
	 * L'ordre des noeuds dans les listes peux ne pas être le même qu'avant le déplacement, en principe cela ne devrait pas poser de problème, mais il est important de le garder à l'esprit en cas de pépin.
	 */
	void revertMove();

	/**
	 * @brief Permet d'afficher le type en détails.
	 * 
	 * Cela inclut l'id du type et de sa cohorte.
	 */
	void verbosePrint();

	/**
	 * @brief Affiche le dernier swap.
	 */
	void displayLastSwap();

	/**
	 * @brief Affiche l'historique des déplacements.
	 */
	void displayMoveHistory();

	/**
	 * @brief Permet d'afficher du type sur un flux (cout, fichier en écriture, etc.)
	 *
	 * @param os Le flux sur lequel écrire.
	 * @param[in] type Le type.
	 *
	 * @return Une référence au flux utilisé.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Type& type);

	/**
	 * @brief Permet la comparaison entre deux type sur la base de leur id.
	 *
	 * @param[in] other L'autre type.
	 *
	 * @return True si les deux types on le même id, false sinon.
	 */
	bool operator==(const Type& other);
	
private:
	/**
	 * @brief Référence à la cohorte du type.
	 */
	const Cohort& m_cohort;

	/**
	 * @brief L'identifiant du type.
	 * 
	 * Il est unique dans la cohorte du type mais pas dans l'ensemble de l'instance.
	 */
	const int m_id;

	/**
	 * @brief Liste des tubes rattachés au type.
	 */
	std::list<Tube> m_tubes;

	/**
	 * @brief L'historique des noeuds swappés.
	 */
	std::stack<ArchivedSwap> m_swapHistory;

	/**
	 * @brief L'historique des déplacement effectués dans le type.
	 * 
	 * Permet de les revert. Chaque ArchivedMove contient toutes les informations nécessaires.
	 */
	std::stack<ArchivedMove> m_moveHistory;
};