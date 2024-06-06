#include "cohort.h"

using namespace std;

Tube::Tube(Type& type, int volume):
	m_type(type),
	m_volume(volume),
	m_usedVolume(0),
	m_usedByCohort(false)
{}

Type& Tube::getType() {return m_type;}

int Tube::getVolume() const {return m_volume;}

int Tube::getUsedVolume() const {return m_usedVolume;}
void Tube::setUsedVolume(int usedVolume) {m_usedVolume = usedVolume;}

int Tube::getRemainingVolume() const {return m_volume-m_usedVolume;}

void Tube::consume(int volume) {m_usedVolume += volume;}

bool Tube::getUsedByCohort() {return m_usedByCohort;}
void Tube::setUsedByCohort(bool usedbyCohort) {m_usedByCohort = usedbyCohort;}

Tree<City*>& Tube::getTree() {return m_tree;}

ostream& operator<<(ostream& os, const Tube& tube)
{
	return os << tube.m_volume-tube.m_usedVolume << "/" << tube.m_volume;
}