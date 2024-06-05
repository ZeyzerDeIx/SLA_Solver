#pragma once

#include <iostream>
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
	Tube(int volume);

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

	friend std::ostream& operator<<(std::ostream& os, const Tube& tube);

private:
	/**
	 * @brief Le volume contenu dans le tube.
	 */
	int m_volume;

	/**
	 * @brief Le volume utilisé du tube.
	 */
	int m_usedVolume;
};