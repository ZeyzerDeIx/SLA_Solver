#pragma once

#include "cohort.h"

/**
 * @brief Cette classe modélise une instance.
 * 
 * Elle contient 2 vecteur de pointeurs
 * - Un sur les villes, cela inclu les cohortes par polymorphisme. Les villes sont dynamiquement allouées et libérées par l'instance.
 * - Un sur les cohortes, ici il ne s'agit que de pointer les cohortes de la liste précédentes.
 */
class Instance
{
public:
	/**
	 * @brief Constructeur.
	 *
	 * @param cities Vecteur de pointeurs sur les villes de l'instance.
	 * @param cohorts Vecteur de pointeurs sur les cohortes de l'instance.
	 * @param maxFreeze Le nombre maximum de congélations par tube.
	 */
	Instance(std::vector<City*> &cities, std::vector<Cohort*> &cohorts, int maxFreeze);
	/**
	 * @brief Constructeur de copie.
	 *
	 * @param[in] other L'instance à copier.
	 */
	Instance(const Instance& other);
	/**
	 * @brief Destructeur de l'objet.
	 * 
	 * Détruit les villes pour libérer la mémoire.
	 */
	~Instance();

	/**
	 * @brief Récupérer les villes.
	 *
	 * @return Une référence à un vecteur de pointeurs sur les villes.
	 */
	const std::vector<City*>& getCities();
	/**
	 * @brief Récupérer les cohortes.
	 *
	 * @return Une référence à un vecteur de pointeurs sur les cohortes.
	 */
	const std::vector<Cohort*>& getCohorts();

	/**
	 * @brief Récupérer le nombre maximum de congélations de l'instance.
	 *
	 * @return Le nombre max de congélation.
	 */
	int getMaxFreeze();

	/**
	 * @brief Permet d'afficher l'instance sur un flux (cout, fichier en écriture, etc.)
	 *
	 * @param os Le flux sur lequel écrire.
	 * @param[in] instance L'instance.
	 *
	 * @return Une référence au flux utilisé.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Instance& instance);
	
private:
	/**
	 * @brief Contient toutes les villes de l'instance.
	 * 
	 * Il s'agit d'un vecteur de pointeurs pour permettre le polymorphisme et donc l'inclusion des cohortes dans la liste.
	 */
	std::vector<City*> m_cities;

	/**
	 * @brief Ce vecteur permet uniquement d'accéder plus vite aux cohortes.
	 *  Il ne contient pas de nouvelles informations.
	 */
	std::vector<Cohort*> m_cohorts;

	/**
	 * @brief Le nombre maximum de congélations par tube.
	 */
	int m_maxFreeze;
};