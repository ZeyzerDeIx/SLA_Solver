#pragma once

#include <iostream>
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
	Tube(Type& type, int volume);

	/**
	 * @brief Récupérer le type auquel est rattaché le tube.
	 *
	 * @return Une référence au type.
	 */
	Type& getType();

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
	Tree<City*>& getTree();

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
	Type& m_type;

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
	Tree<City*> m_tree;
};