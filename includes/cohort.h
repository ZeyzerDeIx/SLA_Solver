#pragma once

#include "city.h"
#include "type.h"

/**
 * @brief Cette classe modélise une cohorte.
 * 
 * On appel cohorte une ville dont le CHU contient une cohorte de patient sur laquelle faire des prélèvements.
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
	 * @brief Récupérer la taille de la cohorte.
	 * 
	 * Cela correspond au nombre de patients.
	 *
	 * @return La taille.
	 */
	int getSize();

	/**
	 * @brief Récupérer la liste des types de la cohorte.
	 *
	 * @return Une référence au vecteur des types de la cohorte.
	 */
	std::vector<Type>& getTypes();

	/**
	 * @brief Détermine si la ville est une cohorte.
	 *
	 * @return True, car nous sommes dans la classe fille, cohorte.
	 */
	bool isCohort() override;

	/**
	 * @brief Permet d'afficher la cohorte sur un flux (cout, fichier en écriture, etc.)
	 *
	 * @param os Le flux sur lequel écrire.
	 * @param[in] cohort La cohorte.
	 *
	 * @return Une référence au flux utilisé.
	 */
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
	 * @brief Taille de la cohorte.
	 * 
	 * Cela correspond au nombre de patients et donc de tubes.
	 */
	int m_size;

	/**
	 * @brief Types de la cohorte.
	 * 
	 * Ils contiennent les tubes.
	 */
	std::vector<Type> m_types;
};