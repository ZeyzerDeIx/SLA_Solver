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
	list<Cohort*> cohorts;
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
		list<Type>& types = cohortPtr->getTypes();
		for(int i=0 ; i<typeCount ; i++)
		{
			types.emplace_back(*cohortPtr, i);
			iss = newLine(file);
			for(int volume ; iss >> volume;)
				types.back().getTubes().emplace_back(types.back(), volume);
		}
	}

	list<unique_ptr<City>> cities;
	for(int i=0 ; i<cityCount ; i++)
	{
		// On récupère la demande de la ville
		list<int> demandes;
		iss = newLine(file);
		for(int demande; iss >> demande;)
			demandes.emplace_back(demande);

		// On regarde si la ville actuelle est une des cohortes
		Cohort* foundCohort = nullptr;
		for(Cohort* cohortPtr: cohorts)
			if(*cohortPtr == i)
				foundCohort = cohortPtr;
		// Si on a trouvé une cohorte, on l'ajoute à la liste des villes et on lui ajoute ses demandes
		if(foundCohort != nullptr)
		{
			cities.push_back(unique_ptr<City>(foundCohort));
			foundCohort->setDemandes(move(demandes));
		}
		// Sinon, on créer une nouvelle ville avec les demandes
		else cities.push_back(make_unique<City>(i,move(demandes)));
	}

	// Nombre max de congélations
	iss = newLine(file);
	int maxFreeze;
	iss >> maxFreeze;

	// Fermer le fichier
	file.close();

	
	Instance instance(cities, cohorts, maxFreeze);
	for(Cohort* cohortPtr: cohorts)
		cohortPtr->setInstance(instance);

	return instance;
}


void parseTubeTree(Tube& tube, ifstream& file, Instance& instance)
{
	cout << "Parsing tube: " << tube << endl;
	// Nombre d'arc du tube
	int arcCount;
	newLine(file) >> arcCount;
	cout << "Arc count: " << arcCount << endl;

	// On lit tous les arcs pour les traiter par la suite
	queue<array<int, 2>> arcs;
	for(int x,y,i=0 ; i<arcCount ; i++)
		newLine(file) >> x >> y, arcs.push(array<int, 2>{x,y});

	Tree<const City*>& tree = tube.getTree();
	cout << "cohort: " << endl << tube.getType().getCohort() << endl;
	tree.setValue(&tube.getType().getCohort());
	cout << "tree value: " << tree.getValue() << endl;
	while(!arcs.empty())
	{
		array<int, 2>& arc = arcs.front();
		cout << arc[0] << " -> " << arc[1] << endl;
		Tree<const City*>* node = tree.findNodeMatching(
			[&arc](const auto& node)
			{
				return node.getValue()->getId() == arc[0];
			}
		);
		
		if(node != nullptr)
			for(const auto& cityPtr: instance.getCities())
				if(cityPtr->getId() == arc[1])
					node->addNode(cityPtr.get());
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
	int n=0;
	for(Cohort* cohortPtr: instance.getCohorts())
		for(Type& type: cohortPtr->getTypes())
			for(Tube& tube: type.getTubes())
				for((iss = newLine(file)).ignore(8); iss >> n;)
					if(n == cohortPtr->getId())
						tube.setUsedByCohort(true);

	for(Cohort* cohortPtr: instance.getCohorts())
		for(Type& type: cohortPtr->getTypes())
			for(Tube& tube: type.getTubes())
				parseTubeTree(tube, file, instance);

	// Fermer le fichier
	file.close();

	for(Tube* tubePtr: instance.getAllTubes())
			cout << tubePtr->getTree() << endl;

	return Solution(instance);
}