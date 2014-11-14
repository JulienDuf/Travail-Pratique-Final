/*
Classe permettant de cr�� des noeuds qui pointes sur les enfants et le parent et qui contient un �l�ment comparable.
Cr��e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 22 avril 2014.
*/
template <class T>
class CNoeudArbreAVL {
private:

	// Donn�es membres...

	T m_Element; // �l�ment dans le noeud.
	string m_strElementID; // L'ID de l'�l�ment.
	CNoeudArbreAVL<T>* m_pParent; // Pointeur sur le parent du noeud
	CNoeudArbreAVL<T>* m_pEG; // Pointeur sur l'enfant plus petit du noeud.
	CNoeudArbreAVL<T>* m_pED; // Pointeur sur l'enfant plus grand du noeud.
	int iIndiceEquilibre; // Entier repr�sentant l'�quilibre de mon arbre.


public:

	// Constructeur...

	CNoeudArbreAVL(T _Element, string _strID, CNoeudArbreAVL<T>* _pParent, int _iIndiceEquilibre) {
		m_strElementID = _strID;
		m_Element = _Element;
		m_pParent = _pParent;
		m_pEG = nullptr;
		m_pED = nullptr;
		int iIndiceEquilibre = _iIndiceEquilibre;
	}

	// M�thodes...

	void DefinirParent(CNoeudArbreAVL<T>* _pParent) {
		m_pParent = _pParent;
	}

	void DefinirEG(CNoeudArbreAVL<T>* _pEG) {
		m_pEG = _pEG;
	}

	void DefinirED(CNoeudArbreAVL<T>* _pED) {
		m_pED = _pED;
	}

	void DefinirElement(T _Element) {
		m_Element = _Element;
	}

	void DefinirIndiceEquilibre(int _i) {
		iIndiceEquilibre = _i;
	}

	// Accesseurs...

	CNoeudArbreAVL<T>* ObtenirParent() {
		return m_pParent;
	}

	CNoeudArbreAVL<T>* ObtenirEG() {
		return m_pEG;
	}

	CNoeudArbreAVL<T>* ObtenirED() {
		return m_pED;
	}

	int ObtenirIE() {
		return iIndiceEquilibre;
	}

	T ObtenirElement() {
		return m_Element;
	}

	string ObtenirID() {

		return m_strElementID;
	}
};