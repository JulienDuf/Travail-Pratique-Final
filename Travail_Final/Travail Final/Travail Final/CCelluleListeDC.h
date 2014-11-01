/*
CCellule qui repr�sente un �l�ment et pointe sur la cellule suivante et pr�c�dante d'une liste.
Cr��e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 15 avril 2014
*/
template <class T>
class CCelluleListeDC {
private:

	// Donn�es membres...

	T m_Element; // �l�ment dans la cellule de la liste.
	CCelluleListeDC<T>* m_pSuivante; // Pointeur de type CCelluleListeDC sur la cellule suivante.
	CCelluleListeDC<T>* m_pPrecedente; // Pointeur de type CCelluleListeDC sur la cellule pr�c�dente.

public:

	// Constructeur...

	CCelluleListeDC(T _Element, CCelluleListeDC<T>* _pSuivante, CCelluleListeDC<T>* _pPrecedente) {
		m_Element = _Element;
		m_pSuivante = _pSuivante;
		m_pPrecedente = _pPrecedente;
	}

	// M�thodes...

	/*
	Proc�dure permettant de d�finir mon pointeur m_pSuivante.
	Prend un pointeur de type CCelluleListeDC qui remplacera m_pSuivante.
	*/
	void DefinirSuivante(CCelluleListeDC<T>* _pSuivante) {
		m_pSuivante = _pSuivante;
	}

	/*
	Proc�dure permettant de d�finir mon pointeur m_pPr�c�dente.
	Prend un pointeur de type CCelluleListeDC qui remplacera m_pPr�c�dente.
	*/
	void DefinirPrecedente(CCelluleListeDC<T>* _pPrecedente) {
		m_pPrecedente = _pPrecedente;
	}

	/*
	Proc�dure qui modifie l'�lement dans la cellule
	Prend un nouvel �lement
	*/
	void DefinirElement(T _Element) {
		m_Element = _Element;
	}
	// Accesseur...

	// Fonction qui retourne l'�l�ment contenue dans la cellule.
	T ObtenirElement() {
		return m_Element;
	}

	// Fonction qui retourne l'adresse m�moire du pointeur m_pSuivante.
	CCelluleListeDC<T>* ObtenirSuivante() {
		return m_pSuivante;
	}

	// Fonction qui retourne l'adresse m�moire du pointeur m_pPr�c�dente.
	CCelluleListeDC<T>* ObtenirPrecedente() {
		return m_pPrecedente;
	}
};