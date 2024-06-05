#pragma once

#include <string>
#include <vector>
#include <iostream>

/**
 * @brief Cette classe modélise une ville.
 */
class City
{
public:
	/**
	 * @brief Constructeur.
	 *
	 * @param[in] id L'identifiant de la ville.
	 */
	City(int id);
	/**
	 * @brief Constructeur.
	 *
	 * @param[in] id L'identifiant de la ville.
	 * @param[in] demandes Les demandes de la ville.
	 */
	City(int id, std::vector<int> demandes);

	/**
	 * @brief Permet de récupérer la liste des demandes de la ville.
	 *
	 * @return Les demandes.
	 */
	const std::vector<int>& getDemandes();

	/**
	 * @brief Définit les demandes de la ville.
	 *
	 * @param[in] demandes Les demandes.
	 */
	void setDemandes(std::vector<int> demandes);

	/**
	 * @brief Retourne l'identifiant de la ville.
	 *
	 * @return L'id.
	 */
	int getId();

	/**
	 * @brief Dtermine si la ville est une cohorte.
	 *
	 * @return False, car nous sommes dans la classe mère.
	 */
	virtual bool isCohort();

	/**
	 * @brief Affiche la ville.
	 *
	 * @param os Le flux de sortie.
	 */
	virtual void print(std::ostream& os) const;

	friend std::ostream& operator<<(std::ostream& os, const City& city);
	friend std::ostream& operator<<(std::ostream& os, const City* city);
	
protected:
	/**
	 * @brief L'identifiant de la ville.
	 */
	int m_id;
	/**
	 * @brief Demandes de la ville par type de tube.
	 */
	std::vector<int> m_demandes;
};