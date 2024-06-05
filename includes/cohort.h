#pragma once

#include "city.h"
#include "tube.h"

using Type = std::vector<Tube>;

/**
 * @brief Cette classe modélise une cohorte.
 */
class Cohort: public City
{
public:
	/**
	 * @brief Constructeur.
	 *
	 * @param[in] id L'identifiant de la cohorte.
	 * @param[in] size La taille de la cohorte.
	 */
	Cohort(int id, int size);
	/**
	 * @brief Définit les tubes associés à chaque type dans la cohorte.
	 *
	 * @param tubes Les tubes de la cohortes par type.
	 */
	void setTubes(std::vector<std::vector<Tube>> tubes);

	/**
	 * @brief Dtermine si la ville est une cohorte.
	 *
	 * @return True, car nous sommes dans la classe fille, cohorte.
	 */
	bool isCohort() override;

	std::vector<std::vector<Tube>>& getTubes();

	friend std::ostream& operator<<(std::ostream& os, const Cohort& cohort);

	/**
	 * @brief Permet de comparer l'id de la cohorte avec un int.
	 *
	 * @param[in] x L'entier avec lequel est comparé la cohorte.
	 *
	 * @return True si l'id de la cohorte est égale à x, false sinon.
	 */
	bool operator==(const int& x);
	
private:

	/**
	 * @brief Affiche la cohorte.
	 *
	 * @param os Le flux de sortie.
	 */
	void print(std::ostream& os) const override;
	/**
	 * @brief Taille de la cohorte.
	 * 
	 * Cela correspond au nombre de patients et donc de tubes.
	 */
	int m_size;
	/**
	 * @brief Tubes de la cohorte par id de type.
	 * 
	 * Il s'agit d'un vecteur de vecteurs, le contenant est le type.
	 * Si l'on appel m_tubes[2][4] cela correspond au 5ème tube du 3ème type.
	 */
	std::vector<Type> m_tubes;
};