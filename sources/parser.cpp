#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <queue>
#include <array>
#include <iostream>
#include <algorithm>

using namespace std;

istringstream newLine(ifstream& file)
{
	string line;
	getline(file, line);
	return istringstream(move(line));
}

Instance Parser::parseInstance(const string& fileLocation)
{
	// Ouvrir le fichier en lecture
	ifstream file("data/instances/"+fileLocation);
	if (!file.is_open())
		throw runtime_error("Impossible d'ouvrir le fichier : " + fileLocation);
	
	// Nombre de villes
	istringstream iss = newLine(file);
	int cityCount;
	iss >> cityCount;

	// Nombre de cohortes, inutile, on skip
	newLine(file);

	// Identifiants des cohortes
	iss = newLine(file);
	queue<int> cohortIds;
	for(int id ; iss >> id;) cohortIds.push(id);

	// Taille des cohortes
	iss = newLine(file);
	vector<Cohort*> cohorts;
	for(int size; iss >> size; cohortIds.pop())
		cohorts.push_back(new Cohort(cohortIds.front(), size));

	// Nombre de types
	iss = newLine(file);
	int typeCount;
	iss >> typeCount;

	// Nombre de tubes
	iss = newLine(file);
	int tubeCount;
	iss >> tubeCount;
	for(Cohort* cohortPtr : cohorts)
	{
		vector<Type>& types = cohortPtr->getTypes();
		for(int i=0 ; i<typeCount ; i++)
		{
			types.emplace_back(*cohortPtr);
			iss = newLine(file);
			for(int volume ; iss >> volume;)
				types[i].getTubes().emplace_back(types[i], volume);
		}
	}

	vector<City*> cities;
	for(int i=0 ; i<cityCount ; i++)
	{
		// On récupère la demande de la ville
		vector<int> demandes;
		iss = newLine(file);
		for(int demande; iss >> demande;)
			demandes.emplace_back(demande);

		// On regarde si la ville actuelle est une des cohortes
		Cohort* foundCohort = nullptr;
		for(Cohort* cohort: cohorts)
			if(*cohort == i)
				foundCohort = cohort;
		// Si on a trouvé une cohorte, on l'ajoute à la liste des villes et on lui ajoute ses demandes
		if(foundCohort != nullptr)
		{
			cities.push_back(foundCohort);
			foundCohort->setDemandes(move(demandes));
		}
		// Sinon, on créer une nouvelle ville avec les demandes
		else cities.push_back(new City(i,move(demandes)));
	}

	// Nombre max de congélations
	iss = newLine(file);
	int maxFreeze;
	iss >> maxFreeze;

	// Fermer le fichier
	file.close();

	
	Instance instance(cities, cohorts, maxFreeze);

	return instance;
}


void parseTubeTree(Tube& tube, ifstream& file, Instance& instance)
{
	// Nombre d'arc du tube
	int arcCount;
	newLine(file) >> arcCount;

	// On lit tous les arcs pour les traiter par la suite
	queue<array<int, 2>> arcs;
	for(int x,y,i=0 ; i<arcCount ; i++)
		newLine(file) >> x >> y, arcs.push(array<int, 2>{x,y});

	Tree<City*>& tree = tube.getTree();
	tree.setValue(&tube.getType().getCohort());
	while(!arcs.empty())
	{
		array<int, 2>& arc = arcs.front();
		Tree<City*>* node = tree.findNodeMatching(
			[&arc](Tree<City*>& node){
				return node.getValue()->getId() == arc[0];
			});
		
		if(node != nullptr)
			for(City* cityPtr: instance.getCities())
				if(cityPtr->getId() == arc[1])
					node->addNode(cityPtr);
		else arcs.push(arcs.front());

		arcs.pop();
	}
}

Solution Parser::parseSolution(const string& fileLocation, Instance instance)
{
	// Ouvrir le fichier en lecture
	ifstream file("data/solutions/"+fileLocation);
	if (!file.is_open())
		throw runtime_error("Impossible d'ouvrir le fichier : " + fileLocation);

	istringstream iss;
	for(Cohort* cohortPtr: instance.getCohorts())
		for(Type& type: cohortPtr->getTypes())
			for((iss = newLine(file)).ignore(8); Tube& tube: type.getTubes())
				for(int n; iss >> n;)
					if(n == cohortPtr->getId())
						tube.setUsedByCohort(true);

	for(Cohort* cohortPtr: instance.getCohorts())
		for(Type& type: cohortPtr->getTypes())
			for(iss = newLine(file); Tube& tube: type.getTubes())
				parseTubeTree(tube, file, instance);

	// Fermer le fichier
	file.close();

	return Solution(instance);
}