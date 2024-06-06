#include "solution.h"

using namespace std;

Solution::Solution(Instance instance):
	m_instance(instance),
	m_tubes(m_instance.getAllTubes())
{}