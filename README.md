# SLA Solver
Il s'agit d'un programme c++ ayant pour objectif d'améliorer des solutions (dans el contexte d'un problème de théorie des graphes) par le biais de méthodes de voisinnage.

## Dépendances
Aucune dépendance pour le moment.

## Documentation
La documentation est faite en Doxygen. Installer DoxygenWizard et téléchargez Graphviz, il suffit d'éxécuter le Doxyfile avec DoxygenWizard pour générer un site web statique comprenant toute la doc.
Notez qu'il faut d'abord renseigner la localisation de Graphviz (à installer par vous même) à DoxygenWizard dans "Expert->Dot->DOT_PATH".

## Nota Bene
Les types de tubes sont des objets à part entière et non juste une énumération, quand on utilise un objet Type, il faut avoir conscience qu'il est rattaché à une cohorte et sera différent de son équivalent (même id)	dans une autre cohorte. Chaque type contient donc les tubes auquel il est assoscié.