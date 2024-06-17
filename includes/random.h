#pragma once

#include <random>
#include <climits>

namespace Random
{
	/**
	 * @brief Récupérer un nombre aléatoire entre a et b.
	 *
	 * @param[in] a Borne inférieure.
	 * @param[in] b Borne suppérieure.
	 *
	 * @return Une nombre aléatoire entre a et b.
	 */
	int randomNb(int a = 0, int b = INT_MAX);
}