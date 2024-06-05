#pragma once

#include "cohort.h"

/**
 * @brief Cette classe modélise une instance.
 */
class Instance
{
public:
	/**
	 * @brief Constructeur.
	 *
	 * @param cities Les villes de l'instance.
	 * @param maxFreeze Le nombre maximum de congélations par tube.
	 */
	Instance(std::vector<City*> &cities, std::vector<Cohort*> &cohorts, int maxFreeze);
	Instance(const Instance& other);
	~Instance();

	const std::vector<City*>& getCities();
	const std::vector<Cohort*>& getCohorts();
	int getMaxFreeze();

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