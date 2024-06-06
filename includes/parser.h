#pragma once

#include <string>
#include "instance.h"
#include "solution.h"

/**
 * @brief Ce namespace contient les méthodes nécessaire au parsing.
 * 
 * Elles permettent de construire l'instance et la solution.
 */
namespace Parser
{
	/**
	 * @brief Parse le fichier donné en entrée pour créer une instance.
	 *
	 * @param[in]  fileLocation  L'emplacement du fichier instance.
	 * 
	 * @return L'instance générée.
	 */
	Instance parseInstance(const std::string& fileLocation);

	/**
	 * @brief Parse le fichier de donné en entrée pour créer une solution de l'instance donnée.
	 *
	 * @param[in] fileLocation L'emplacement du fichier solution.
	 * @param[in] instance L'instance associée à la solution.
	 *
	 * @return La solution générée.
	 */
	Solution parseSolution(const std::string& fileLocation, Instance instance);
}