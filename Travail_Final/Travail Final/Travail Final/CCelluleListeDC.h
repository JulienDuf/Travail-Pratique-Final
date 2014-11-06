/*
CCellule qui représente un élément et pointe sur la cellule suivante et précédante d'une liste.
Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 15 avril 2014
*/
template <class T>
class CCelluleListeDC {
private:

	// Données membres...

	T m_Element; // Élément dans la cellule de la liste.
	CCelluleListeDC<T>* m_pSuivante; // Pointeur de type CCelluleListeDC sur la cellule suivante.
	CCelluleListeDC<T>* m_pPrecedente; // Pointeur de type CCelluleListeDC sur la cellule précédente.

public:

	// Constructeur...

	CCelluleListeDC(T _Element, CCelluleListeDC<T>* _pSuivante, CCelluleListeDC<T>* _pPrecedente) {
		m_Element = _Element;
		m_pSuivante = _pSuivante;
		m_pPrecedente = _pPrecedente;
	}

	// Méthodes...

	/*
	Procédure permettant de définir mon pointeur m_pSuivante.
	Prend un pointeur de type CCelluleListeDC qui remplacera m_pSuivante.
	*/
	void DefinirSuivante(CCelluleListeDC<T>* _pSuivante) {
		m_pSuivante = _pSuivante;
	}

	/*
	Procédure permettant de définir mon pointeur m_pPrécédente.
	Prend un pointeur de type CCelluleListeDC qui remplacera m_pPrécédente.
	*/
	void DefinirPrecedente(CCelluleListeDC<T>* _pPrecedente) {
		m_pPrecedente = _pPrecedente;
	}

	/*
	Procédure qui modifie l'élement dans la cellule
	Prend un nouvel élement
	*/
	void DefinirElement(T _Element) {
		m_Element = _Element;
	}
	// Accesseur...

	// Fonction qui retourne l'élément contenue dans la cellule.
	T ObtenirElement() {
		return m_Element;
	}

	// Fonction qui retourne l'adresse mémoire du pointeur m_pSuivante.
	CCelluleListeDC<T>* ObtenirSuivante() {
		return m_pSuivante;
	}

	// Fonction qui retourne l'adresse mémoire du pointeur m_pPrécédente.
	CCelluleListeDC<T>* ObtenirPrecedente() {
		return m_pPrecedente;
	}
};