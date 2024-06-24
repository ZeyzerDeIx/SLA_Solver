#pragma once

#include <iostream>
#include <list>
#include <functional>
#include "random.h"

// Concept pour vérifier si T est un pointeur
template<typename T>
concept IsPointer = std::is_pointer_v<T>;

/**
 * @brief Classe représentant un arbre générique.
 * 
 * Cette classe permet de représenter un arbre générique avec des noeuds de type T.
 * \note
 * Il n'y a pas de distinction entre un noeud et un arbre. Chaque noeud est un arbre en soit.
 */
template<typename T>
class Tree
{
public:
	/**
	 * @brief Constructeur par défaut.
	 * 
	 * Construit un arbre vide.
	 */
	Tree(): m_root(nullptr) {}

	/**
	 * @brief Constructeur par défaut (spé pointeur).
	 * 
	 * Initialise m_value à nullptr.
	 * 
	 * @pre La valeur de m_value doit être un pointeur.
	 */
	Tree() requires(IsPointer<T>):
		m_value(nullptr),
		m_root(nullptr)
	{}


	/**
	 * @brief Constructeur avec une valeure.
	 * 
	 * Construit un noeud avec une valeure spécifiée.
	 * 
	 * @param value La valeur du noeud.
	 * @param root La racine de l'arbre.
	 */
	Tree(T value, Tree<T>* root = nullptr):
		m_value(value),
		m_root(root)
	{}

	/**
	 * @brief Constructeur de copie.
	 *
	 * @param[in] other L'arbre à copier.
	 * @param root La racine de l'arbre, si null copie celle de l'arbre source.
	 */
	Tree(const Tree<T>& other, Tree<T>* root = nullptr):
		m_value(other.m_value),
		m_root(root == nullptr ? other.m_root : root)
	{
		for(const Tree<T>& node : other.m_nodes)
			m_nodes.emplace_back(node, this);
	}

	/**
	 * @brief Constructeur de déplacement.
	 *
	 * @param other L'arbre à utiliser.
	 * 
	 * \note
	 * L'objet déplacé n'est plus utilisable après cela.
	 */
	Tree(Tree<T>&& other):
		m_value(std::move(other.m_value)),
		m_root(std::move(other.m_root)),
		m_nodes(std::move(other.m_nodes))
	{
		for(Tree<T>& node : m_nodes)
			node.setRoot(this);
	}

	/**
	 * @brief Opérateur d'affectation par déplacement.
	 *
	 * @param other L'élément à utiliser (déplacer).
	 *
	 * @return Une référence au résultat du déplacement.
	 * 
	 * \note
	 * L'objet déplacé n'est plus utilisable après cela.
	 */
	Tree& operator=(Tree&& other) noexcept
	{
        if (this != &other)
        {
            // Transférer les ressources de other vers this
            m_value = std::move(other.m_value);
            m_root = std::move(other.m_root);
            m_nodes = std::move(other.m_nodes);
            for(Tree<T>& node : m_nodes)
				node.setRoot(this);
        }
        return *this;
    }

	/**
	 * @brief Ajoute un nouveau noeud à l'arbre.
	 * 
	 * Cette méthode ajoute un nouveau noeud à l'arbre en tant qu'enfant du noeud courant.
	 * 
	 * @param value La valeur du nouveau noeud à ajouter.
	 * @return Une référence à l'élément inséré.
	 */
	Tree<T>& addNode(const T& value) { return m_nodes.emplace_back(value, this); }

	/**
	 * @brief Déplace un noeud depuis un autre arbre.
	 * 
	 * Cette méthode ajoute un nouveau noeud à l'arbre en tant qu'enfant du noeud courant et le retire de son ancien parent.
	 * 
	 * @param node Le noeud à déplacer.
	 * @return Une référence à l'élément inséré.
	 */
	Tree<T>& moveNode(Tree<T>& node)
	{
		auto nodeIt = find_if(
			node.getRoot()->getNodes().begin(),
			node.getRoot()->getNodes().end(),
			[&node](auto& tree){return &tree == &node;}
		);
		return moveNode(nodeIt);
	}
	/**
	 * @brief Déplace un noeud depuis un autre arbre.
	 * 
	 * Cette méthode ajoute un nouveau noeud à l'arbre en tant qu'enfant du noeud courant et le retire de son ancien parent.
	 * 
	 * @param nodeIt Un iterateur sur le noeud à déplacer.
	 * @return Une référence à l'élément inséré.
	 */
	Tree<T>& moveNode(std::list<Tree<T>>::iterator nodeIt)
	{
		nodeIt->setRoot(this);
		m_nodes.splice(m_nodes.end(), nodeIt->getRoot()->getNodes(), nodeIt);

		return m_nodes.back();
	}

	/**
	 * @brief Déplace tous les noeuds d'un autre arbre.
	 * 
	 * Cette méthode ajoute des nouveaux noeud à l'arbre en tant qu'enfants du noeud courant et les retire à leur ancien parent.
	 * 
	 * @param target Le noeud dont sont extraits les enfants.
	 */
	void moveNodes(Tree<T>& target)
	{
		// On récupère les successeurs de l'élément que l'on a remonté.
		std::list<Tree<T>>& nodeList = target.getNodes();
		// On met à jour leur racines.
		for (auto it = nodeList.begin(); it != nodeList.end(); ++it)
			it->setRoot(this);
		// On déplace.
		m_nodes.splice(m_nodes.end(), nodeList);
	}

	/**
	 * @brief Récupère un noeud de l'arbre par son index.
	 * 
	 * Cette méthode permet de récupérer un noeud de l'arbre en fonction de son index dans le vecteur
	 * de noeuds de l'arbre. L'index doit être valide, sinon un comportement indéfini peut se produire.
	 * 
	 * @param index L'index du noeud à récupérer.
	 * @return Une référence constante vers le noeud correspondant à l'index spécifié.
	 */
	Tree<T>& getNode(size_t index) { return m_nodes.at(index); }

	/**
	 * @brief Récupère la liste des nœuds de l'arbre.
	 *
	 * @return Une référence au vecteur des nœuds de l'arbre. Chaque nœud est de type Tree<T>.
	 */
	std::list<Tree<T>>& getNodes() {return m_nodes;}

	/**
	 * @brief Récupère la liste (const) des nœuds de l'arbre.
	 *
	 * @return Une référence au vecteur des nœuds de l'arbre. Chaque nœud est de type Tree<T>.
	 * 
	 * \note
	 * Variante constante.
	 */
	const std::list<Tree<T>>& getNodes() const {return m_nodes;}


	/**
	 * @brief Récupère le nombre de noeuds de l'arbre.
	 *
	 * @return Le nombre de noeuds.
	 */
	int nodeCount() const { return m_nodes.size(); }

	/**
	 * @brief Récupère le nombre de noeuds total de l'arbre incluant les descendants.
	 *
	 * @return Le nombre total de noeuds.
	 */
	int allNodesCount() const
	{
		int count = nodeCount();
		for(const Tree& node: m_nodes)
			count += node.allNodesCount();
		return count;
	}

	/**
	 * @brief Récupère la valeur de l'objet.
	 *
	 * @return La valeur de l'objet.
	 */
	T getValue() const { return m_value; }

	/**
	 * @brief Définit la valeur de l'objet.
	 *
	 * @param value La nouvelle valeur à assigner.
	 */
	void setValue(T value) { m_value = value; }

	/**
	 * @brief Récupère un vecteur de toutes les valeurs de l'arbre.
	 * 
	 * Cette méthode parcourt l'arbre entier et collecte toutes les valeurs dans un vecteur.
	 * 
	 * @return Un vecteur contenant toutes les valeurs de l'arbre.
	 */
	std::list<T> getAllValues() const
	{
		std::list<T> values;
		getAllValuesHelper(values);
		return std::move(values);
	}

	/**
	 * @brief Permet de trouver un noeud/arbre contenant la valeure donnée en entrée.
	 *
	 * @param[in] value La valeure recherchée.
	 *
	 * @return Un pointeur sur le noeud trouver, nullptr si aucun noeud avec la bonne valeure n'a été trouvé.
	 */
	Tree* findNodeContaining(T value) const
	{
		if(value = m_value) return this;

		Tree* found = nullptr;
		for(Tree& tree: m_nodes)
			if((found = tree.findNodeContaining(value)) != nullptr)
				return found;

		return nullptr;
	}

	/**
	 * @brief Permet de trouver un noeud/arbre correspondant à une condition donnée en entrée.
	 *
	 * @param[in] predicate La condition de recherche sous forme de lambda.
	 *
	 * @return Un pointeur sur le noeud trouvé, nullptr si aucun noeud correspondant n'a été trouvé.
	 */
	Tree* findNodeMatching(const std::function<bool(Tree&)>& predicate)
	{
		if (predicate(*this)) return this;

		Tree* found = nullptr;
		for (Tree& tree : m_nodes)
			if ((found = tree.findNodeMatching(predicate)) != nullptr)
				return found;

		return nullptr;
	}

	/**
	 * @brief Récupérer un noeud aléatoire de l'abre.
	 * 
	 * Ne peut pas retourner l'arbre lui-même, seulement des noeuds descendants.
	 *
	 * @return Le noeud aléatoire.
	 */
	Tree<T>& getRandomNode()
	{
		return getNodeAtIndex(Random::randomNb(1, allNodesCount()-1));
	}

	Tree<T>& getNodeAtIndex(int index)
	{
		return *getNodeAtIndexHelper(index);
	}

	/**
	 * @brief Récupère la racine (le parent) du noeud.
	 *
	 * @return Un pointeur vers la racine.
	 */
	Tree<T>* getRoot() {return m_root;}

	/**
	 * @brief Mettre à jour la valeure de la racine (parent) du noeud.
	 *
	 * @param root Un pointeur vers la nouvelle racine.
	 */
	void setRoot(Tree<T>* root) {m_root = root;}

	/**
	 * @brief Récupère la profondeur du noeud.
	 *
	 * Cette méthode calcule la profondeur du noeud dans l'arbre en partant de la racine.
	 *
	 * @return La profondeur du noeud courrant. Si le noeud est racine, retourne 1.
	 */
	int getDepth() const
	{
		return m_root != nullptr ? m_root->getDepth()+1 : 0;
	}

	/**
	 * @brief Récupère la profondeur du noeud le plus profond du noeud courrant.
	 *
	 * @return La profondeur max.
	 */
	int getMaxDepth() const
	{
		int maxDepth = getDepth();

		if(nodeCount() == 0) return maxDepth;

		for(int nodeMaxDepth = 0; const Tree<T>& node : m_nodes)
			maxDepth = std::max(node.getMaxDepth(), maxDepth);

		return maxDepth;
	}

	/**
	 * @brief Récupérer la racine la plus lointaine.
	 *
	 * @return Un pointeur vers la vraie racine.
	 */
	Tree<T>* getTrueRoot()
	{
		Tree<T>* trueRoot(this);
		while (trueRoot->getRoot() != nullptr)
			trueRoot = trueRoot->getRoot();
		return trueRoot;
	}


	/**
	 * @brief Surcharge de l'opérateur de sortie pour l'affichage de l'arbre.
	 * 
	 * Cette fonction surcharge l'opérateur de sortie << pour permettre l'affichage de l'arbre.
	 * 
	 * @param os Le flux de sortie.
	 * @param tree L'arbre à afficher.
	 * @return Le flux de sortie modifié.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Tree<T>& tree)
	{
		return tree.print(os);
	}

private:
	T m_value; /**< La valeur du noeud. */
	std::list<Tree<T>> m_nodes; /**< Les noeuds enfants. */
	Tree<T>* m_root; /**< La racine (le noeud parent). */

	/**
	 * @brief Fonction auxiliaire récursive pour collecter les valeurs de l'arbre.
	 * 
	 * @param values Référence au vecteur où les valeurs seront collectées.
	 */
	void getAllValuesHelper(std::list<T>& values) const
	{
		if(m_value != nullptr) values.push_back(m_value);
		for(const Tree<T>& node : m_nodes)
			node.getAllValuesHelper(values);
	}

	/**
	 * @brief Fonction auxiliaire récursive pour atteindre le noeud ayant le bon index via un parcours en profondeur.
	 *
	 * @param index Une référence à l'index, décrémenté à chaque étape.
	 *
	 * @return Le noeud correspondant à l'index demandé si il est trouvé, nullptr sinon.
	 */
	Tree<T>* getNodeAtIndexHelper(int& index)
	{
		if(index == 0) return this;

		for(Tree<T>* found; Tree& node: m_nodes)
			if((found = node.getNodeAtIndexHelper(--index)) != nullptr)
				return found;

		return nullptr;
	}
	/**
	 * @brief Affiche l'arbre avec un décalage d'indentation sur un flux de sortie.
	 * 
	 * Cette méthode affiche l'arbre à partir de ce noeud en utilisant un décalage d'indentation
	 * spécifié. Chaque niveau de l'arbre est décalé de manière proportionnelle à son niveau dans
	 * la hiérarchie.
	 * 
	 * @param os Le flux de sortie sur lequel afficher l'arbre.
	 * @param indent Le niveau d'indentation initial (par défaut 0).
	 * @return Le flux de sortie modifié.
	 */
	std::ostream& print(std::ostream& os, int indent = 0) const
	{
		// Afficher la valeur du noeud avec un décalage d'indentation
		for(int i=1; i<indent ; i++) os << " | ";
		os << (indent?" |___":"\\_") << m_value << std::endl;
		
		// Afficher les enfants avec un décalage d'indentation accru
		for(const Tree<T>& node : m_nodes)
			node.print(os, indent + 1);

		if(m_nodes.size() == 0) return os;
		return os;
	}
};