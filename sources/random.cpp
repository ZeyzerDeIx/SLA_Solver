#include "random.h"

using namespace std;

int Random::randomNb(int a, int b)
{
    // Générateur de nombres aléatoires
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(a, b);

    // Récupérer un élément aléatoire
	return dis(gen);
}