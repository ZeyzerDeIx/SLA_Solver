#pragma once

#include <map>
#include "instance.h"

/**
 * @brief Cette classe modélise une solution.
 * 
 * \note
 * En réalité la vrai solution et les algorithmes ont lieu au niveau de l'instance (bien que cela paraisse contre intuitif) et ce pour des raisons de simplicité au niveau de la manipulation des données.
 * <br>
 * La classe en elle même ne sert donc que de wrapper ainsi qu'a compter les aliquotages.
 */
class Solution
{
public:
	Solution(Instance&& instance);

	/**
	 * @brief Retourne le nombre d'aliquotages de la ville en aillant le plus.
	 *
	 * @return Le nombre max d'aliquotages.
	 */
	unsigned int getMaxAliquo() const;

	/**
	 * @brief Détermine si la solution est meilleurs que celle donnée en paramètre.
	 *
	 * @param[in] other L'autre solution.
	 *
	 * @return Vrai si la solution est absolument meilleure que l'autre, faux sinon.
	 */
	bool isBetterThan(const Solution& other);

	friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

private:
	/**
	 * @brief L'instance sur laquelle cette solution travail.
	 * 
	 * Il ne s'agit pas d'une référence car l'instance va être modifiée et fait partie intégrante de la solution.
	 */
	Instance m_instance;

	/**
	 * @brief Liste des tubes de la solution.
	 * 
	 * Il s'agit de pointeurs car ils pointent vers les tubes de l'instance en réalité, l'idée ici c'est de se passer d'une double boucle cohortes types et d'accéder directement aux arbres.
	 */
	std::list<Tube*> m_tubes;
};