#pragma once

#include <map>
#include "instance.h"

/**
 * @brief Cette classe modélise une solution.
 * 
 * \note
 * En réalité la vrai solution et les algorithmes ont lieu au niveau de l'instance (bien que cela paraisse contre intuitif) et ce pour des raisons de simplicité au niveau de la manipulation des données.
 * <br>
 * La classe en elle même ne sert donc que de wrapper ainsi qu'a compter les alicotages.
 */
class Solution
{
public:
	Solution(Instance instance);

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
	/**
	 * @brief Nombre total d'alicotages de la solution.
	 * 
	 * \note
	 * Je ne suis pas certains de la pertinence de cette donnée, à voir si on la garde.
	 */
	int m_totalAlicoCount;
	/**
	 * @brief Nombre d'alicotage de chaque ville sous forme de map.
	 */
	std::map<City*, int> m_citiesAlicotCount;
};