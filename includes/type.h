#pragma once
#include <vector>
#include "tube.h"

/**
 * @brief Cette classe modélise une type de tube.
 * 
 * Un type contient les tubes qui lui sont rattaché ainsi qu'à sa cohorte.
 */
class Type
{
public:
	Type(Cohort& cohort);
	~Type();

	/**
	 * @brief Récupérer tous les tubes du type.
	 *
	 * @return Le vecteur contenant les tubes.
	 */
	std::vector<Tube>& getTubes();

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
	Cohort& m_cohort;

	/**
	 * @brief L'identifiant du type.
	 * 
	 * Il est unique dans la cohorte du type mais pas dans l'ensemble de l'instance.
	 */
	int m_id;

	/**
	 * @brief Liste des tubes rattachés au type.
	 */
	std::vector<Tube> m_tubes;
};