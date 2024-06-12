#include "parser.h"
#include <string>
#include <chrono>

using namespace std;

int main()
{
    cout << endl << "DEBUT DE L'EXECUTION" << endl << endl;
    auto start = chrono::steady_clock::now(); // Point A

    Instance instance = Parser::parseInstance("instance2.txt");
    Solution solution = Parser::parseSolution("solution.txt", instance);    

    auto end = chrono::steady_clock::now();   // Point B
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Exécuté en : " << elapsed_seconds.count()*1000.f << " ms" << endl << endl;
    return 0;
}
