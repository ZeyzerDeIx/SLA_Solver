#include "parser.h"
#include <string>
#include <chrono>

using namespace std;
#define LIMIT 100'000

int main()
{
	cout << endl << "DEBUT DE L'EXECUTION" << endl << endl;
	auto start = chrono::steady_clock::now(); // Point A

	Instance instance = Parser::parseInstance("instance.txt");
	Solution solution = Parser::parseSolution("solution1.txt", instance);

	int aliquoBase = solution.getMaxAliquo();
	//cout << instance << endl << solution << endl;
	cout << "Max aliquo base: " << aliquoBase << endl;
	int limit = 0 ;
	for(; aliquoBase <= solution.getMaxAliquo() && limit < LIMIT; limit++)
	{
		solution.revertSwapInTube();
		solution.randomSwapInTube();
	}
	if(limit != LIMIT)
	{
		cout << "Max aliquo fin: " << solution.getMaxAliquo() << endl;
		cout << "Nombre d'essais " << limit << endl;
		solution.displayLastSwap();
	}
	else cout << "Echec de la mission" << endl;


	auto end = chrono::steady_clock::now();   // Point B
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "Exécuté en : " << elapsed_seconds.count()*1000.f << " ms" << endl << endl;
	return 0;
}
