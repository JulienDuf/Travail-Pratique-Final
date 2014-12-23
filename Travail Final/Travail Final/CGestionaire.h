//
// Classe repr�sentant un gestionnaire de donn�es. 
// Cr�� par Julien Dufresne (dufresne_julien@hotmail.ca) le 13 novembre 2014.
//

template <class T>

class CGestionaire {

private:

	// Donn�es membres...

	CArbreAVL<T>* m_pArbreGestionaire; // L'arbre ou est stock� les donn�es

public:

	// Constructeur de CGestionaire
	CGestionaire() {

		m_pArbreGestionaire = new CArbreAVL<T>();

	}

	// Proc�dure ajoutant un �l�ment � l'arbre.
	// En entr�:
	// Param1: L'�l�ment � ajouter.
	// Param2: L'ID de l'�l�ment.
	void AjouterDonnee(T _Donnee, string _strID) {

		m_pArbreGestionaire->Ajouter(_Donnee, _strID);
	}
	
	// Fonction obtenant un �l�ment � l'arbre.
	// En entr�:
	// Param1: L'ID de l'�l�ment.
	// Retour: L'�l�ment.
	T ObtenirDonnee(string _strID) {

		return m_pArbreGestionaire->ObtenirElement(_strID);
	}

	void Destruction() {
		m_pArbreGestionaire->ParcoursDestructeur();
	}

	void DestructionFont(void) {
		m_pArbreGestionaire->ParcoursDestructionFont();
	}

	void DestructionTexture(void) {
		m_pArbreGestionaire->ParcoursDestructionTexture();
	}

	void DestructionSurface(void) {
		m_pArbreGestionaire->ParcoursDestructionSurface();
	}
};