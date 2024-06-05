#pragma once

#include <string>
#include "instance.h"

/**
 * @brief Ce namespace contient les méthodes nécessaire au parsing.
 * 
 * Elles permettent de construire l'instance.
 */
namespace Parser
{
	/**
	 * @brief Parse le fichier donnée en entrée pour créer une instance.
	 *
	 * @param[in]  fileLocation  L'emplacement du fichier instance.
	 * 
	 * @return L'instance générée sous forme de rvalue.
	 */
	Instance parseInstance(const std::string& fileLocation);
}