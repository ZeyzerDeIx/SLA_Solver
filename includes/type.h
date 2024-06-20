#pragma once
#include <list>
#include <stack>
#include "tube.h"

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
	 * @brief Échange les valeures de deux noeuds dans les tubes.
	 *
	 * @param a Premier noeud.
	 * @param b Second noeud.
	 * @param[in] saveSwap Si le swap doit être sauvegardé dans l'historique des swap. Typiquement faux en cas de revert.
	 */
	void swapNodes(Tree<const City*>& a, Tree<const City*>& b, bool saveSwap = true);

	/**
	 * @brief Swap deux noeuds aléatoire entre deux tubes aléatoires.
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
	void moveNode(Tree<const City*>* toMove, Tree<const City*>* a, Tree<const City*>* b = nullptr);

	/**
	 * @brief Déplace un noeud choisit aléatoirement vers un autre également aléatoire.
	 */
	void moveRandomNode();

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
	 * @brief Cette structure permet d'archiver toutes les information nécessaires à l'annulation d'un déplacement.
	 */
	struct ArchivedMove
	{
		/**
		 * @brief destTRoot pour destinationTrueRoot, la racine la plus lointaine de la destination avant déplacement.
		 */
		Tree<const City*>* destTRoot;
		Tree<const City*> destTRootSave;
		/**
		 * @brief formerTRoot pour formerTrueRoot, la racine la plus lointaine du noeud avant déplacement.
		 */
		Tree<const City*>* formerTRoot;
		Tree<const City*> formerTRootSave;
	};

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
	std::stack<std::pair<Tree<const City*>*, Tree<const City*>*>> m_swapHistory;

	/**
	 * @brief L'historique des déplacement effectués dans le type.
	 * 
	 * Permet de les revert. Chaque ArchivedMove contient toutes les informations nécessaires.
	 */
	std::stack<ArchivedMove> m_moveHistory;
};