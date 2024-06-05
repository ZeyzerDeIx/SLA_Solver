#include "tube.h"

using namespace std;

Tube::Tube(int volume): m_volume(volume), m_usedVolume(0) {}

int Tube::getVolume() const {return m_volume;}

int Tube::getUsedVolume() const {return m_usedVolume;}
void Tube::setUsedVolume(int usedVolume) {m_usedVolume = usedVolume;}

int Tube::getRemainingVolume() const {return m_volume-m_usedVolume;}

void Tube::consume(int volume)
{
	m_usedVolume += volume;

	//cout << "Tube prélevé de " << volume << endl;
	//cout << "Reste " << *this << endl;
}

ostream& operator<<(ostream& os, const Tube& tube)
{
	os << tube.m_volume-tube.m_usedVolume << "/" << tube.m_volume << endl;
	return os;
}