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
	/**
	 * @brief Constructeur de solution.
	 *
	 * @param instance Une référence à une rvalue de l'instance à déplacer.
	 */
	Solution(Instance&& instance);

	/**
	 * @brief Constructeur de copie.
	 *
	 * @param[in] other La solution à copier.
	 */
	Solution(const Solution& other);

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

	/**
	 * @brief Échange aléatoirement deux noeuds dans un tube.
	 * 
	 * Dans l'arbre de répartition d'un unique tube choisit aléatoirement.
	 */
	void randomSwapInTube();

	/**
	 * @brief Annule le dernier swap effectué dans un tube.
	 */
	void revertSwapInTube();
	/**
	 * @brief Échange aléatoirement deux noeuds dans un type.
	 * 
	 * Dans les arbres de répartition des tubes d'un type choisit aléatoirement.
	 */
	void randomSwapInType();

	/**
	 * @brief Affiche le dernier swap éffectué.
	 */
	void displayLastSwap();

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
	std::vector<Tube*> m_tubes;

	/**
	 * @brief Pointeur vers le dernier tube à avoir été swap.
	 */
	Tube* m_swappedTube;
};