#pragma once

#include <iostream>
#include <utility>
#include "tree.h"
#include "city.h"

//Pour éviter une boucle d'inclusion, type.h et cohort.h sont inclus dans tube.cpp
class Type;
class Cohort;

/**
 * @brief Cette classe modélise un tube.
 */
class Tube
{
public:
	/**
	 * @brief Constructeur.
	 *
	 * @param volume  Le volume contenu dans le tube.
	 */
	Tube(const Type& type, int volume, int id);

	/**
	 * @brief Fait une copie profonde de l'arbre de répartition du tube.
	 * 
	 * La copie s'assure que les pointeurs de l'arbres pointes sur les villes de la bonne instance.
	 *
	 * @param tree L'arbre à copier.
	 */
	void deepTreeCopy(const Tree<const City*>& tree);

	/**
	 * @brief Récupérer le type auquel est rattaché le tube.
	 *
	 * @return Une référence au type.
	 */
	const Type& getType() const;

	/**
	 * @brief Récupérer l'identifiant du tube.
	 *
	 * @return L'identifiant.
	 */
	int getId() const;

	/**
	 * @brief Getter pour le volume contenu dans le tube.
	 *
	 * @return Le volume contenu dans le tube.
	 */
	int getVolume() const;

	/**
	 * @brief Getter pour le volume utilisé du tube.
	 *
	 * @return Le volume utilisé du tube.
	 */
	int getUsedVolume() const;

	/**
	 * @brief Setter pour le volume utilisé du tube.
	 *
	 * @param usedVolume Le nouveau volume utilisé du tube.
	 */
	void setUsedVolume(int usedVolume);

	/**
	 * @brief Calcule et retourne le volume restant dans le tube.
	 *
	 * @return Le volume restant dans le tube.
	 */
	int getRemainingVolume() const;

	/**
	 * @brief Consome du volume.
	 * Met à jour m_usedVolume.
	 *
	 * @param[in] volume Le volume consommé.
	 */
	void consume(int volume);

	/**
	 * @brief Savoir si le tube est utilisé par sa cohorte.
	 *
	 * @return True si le tube est utilisé par sa cohorte, false sinon;
	 */
	bool getUsedByCohort();
	/**
	 * @brief Permet de définir le nouvel état de usedByCohort.
	 *
	 * @param[in] usedbyCohort le nouvel état.
	 */
	void setUsedByCohort(bool usedbyCohort);

	/**
	 * @brief Récupérer l'arbre de répartition du tube.
	 *
	 * @return Une référence à l'abre de répartition.
	 */
	Tree<const City*>& getTree();

	/**
	 * @brief Récupérer l'arbre de répartition du tube (const).
	 *
	 * @return Une référence à l'abre de répartition.
	 * 
	 * \note
	 * Variante constante.
	 */
	const Tree<const City*>& getTree() const;

	/**
	 * @brief Échange les valeures de deux noeuds de l'arbre.
	 *
	 * @param a Premier noeud.
	 * @param b Second noeud.
	 */
	void swapNodes(Tree<const City*>& a, Tree<const City*>& b);

	/**
	 * @brief Swap aléatoirement deux noeuds de l'arbre.
	 */
	void swapRandomNodes();

	/**
	 * @brief Annule le dernier swap.
	 */
	void revertSwap();

	/**
	 * @brief Affiche le dernier swap.
	 */
	void displayLastSwap();

	/**
	 * @brief Permet d'afficher le tube en détails.
	 * 
	 * Cela inclut l'id du tube, de son type et de sa cohorte.
	 */
	void verbosePrint();

	/**
	 * @brief Permet d'afficher le tube sur un flux (cout, fichier en écriture, etc.)
	 *
	 * @param os Le flux sur lequel écrire.
	 * @param[in] tube Le tube.
	 *
	 * @return Une référence au flux utilisé.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Tube& tube);

private:

	/**
	 * @brief Type auquel est rattaché le tube.
	 */
	const Type& m_type;

	/**
	 * @brief Identifiant du tube dans le type.
	 */
	int m_id;

	/**
	 * @brief Le volume contenu dans le tube.
	 */
	int m_volume;

	/**
	 * @brief Le volume utilisé du tube.
	 */
	int m_usedVolume;

	/**
	 * @brief True si le tube est prélevé par la cohorte. False sinon.
	 */
	bool m_usedByCohort;

	/**
	 * @brief Arbre de répartition du tube.
	 * 
	 * Il contient les villes utilisant le tube et la structure de la répartition, c'est-à-dire l'ordre dans lequel sont visités les villes.
	 * 
	 * De cet arbre peuvent être déduis les arcs à enregistrer dans la solution.
	 */
	Tree<const City*> m_tree;

	/**
	 * @brief Les derniers noeuds à avoir été swappé.
	 */
	std::pair<Tree<const City*>*, Tree<const City*>*> m_swappedNodes;
};