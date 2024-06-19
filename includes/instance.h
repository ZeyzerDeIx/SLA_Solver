#pragma once

#include "cohort.h"
#include <memory>

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
	 * @brief Constructeur par défaut.
	 */
	Instance();
	/**
	 * @brief Constructeur.
	 *
	 * @param cities Vecteur de pointeurs sur les villes de l'instance.
	 * @param cohorts Vecteur de pointeurs sur les cohortes de l'instance.
	 * @param maxFreeze Le nombre maximum de congélations par tube.
	 */
	Instance(std::list<std::unique_ptr<City>> &cities, std::list<Cohort*> &cohorts, int maxFreeze);
	/**
	 * @brief Constructeur de copie.
	 *
	 * @param[in] other L'instance à copier.
	 */
	Instance(const Instance& other);

	/**
	 * @brief Constructeur de déplacement.
	 *
	 * @param other L'instance à déplacer (référence à une rvalue).
	 */
	Instance(Instance&& other) noexcept;

	/**
	 * @brief Récupérer les villes.
	 *
	 * @return Une référence à un vecteur de pointeurs sur les villes.
	 */
	const std::list<std::unique_ptr<City>>& getCities() const;

	/**
	 * @brief Récupérer les cohortes.
	 *
	 * @return Une référence à un vecteur de pointeurs sur les cohortes.
	 */
	std::list<Cohort*>& getCohorts();
	/**
	 * @brief Récupérer les cohortes (const).
	 *
	 * @return Une référence à un vecteur de pointeurs sur les cohortes.
	 * 
	 * \note
	 * Variante constante.
	 */
	const std::list<Cohort*>& getCohorts() const;

	/**
	 * @brief Récupérer le nombre maximum de congélations de l'instance.
	 *
	 * @return Le nombre max de congélation.
	 */
	int getMaxFreeze() const;

	/**
	 * @brief Récupérer une liste de tous les tubes contenus dans l'instance.
	 *
	 * @return Vecteur de pointeur sur les tubes.
	 */
	std::vector<Tube*> getAllTubes() const;

	/**
	 * @brief Récupérer une liste de tous les types contenus dans l'instance.
	 *
	 * @return Vecteur de pointeur sur les types.
	 */
	std::vector<Type*> getAllTypes() const;

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
	std::list<std::unique_ptr<City>> m_cities;

	/**
	 * @brief Ce vecteur permet uniquement d'accéder plus vite aux cohortes.
	 *  Il ne contient pas de nouvelles informations.
	 */
	std::list<Cohort*> m_cohorts;

	/**
	 * @brief Le nombre maximum de congélations par tube.
	 */
	int m_maxFreeze;
};