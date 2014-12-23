//
// Classe représentant un gestionnaire de données. 
// Créé par Julien Dufresne (dufresne_julien@hotmail.ca) le 13 novembre 2014.
//

template <class T>

class CGestionaire {

private:

	// Données membres...

	CArbreAVL<T>* m_pArbreGestionaire; // L'arbre ou est stocké les données

public:

	// Constructeur de CGestionaire
	CGestionaire() {

		m_pArbreGestionaire = new CArbreAVL<T>();

	}

	// Procédure ajoutant un élément à l'arbre.
	// En entré:
	// Param1: L'élément à ajouter.
	// Param2: L'ID de l'élément.
	void AjouterDonnee(T _Donnee, string _strID) {

		m_pArbreGestionaire->Ajouter(_Donnee, _strID);
	}
	
	// Fonction obtenant un élément à l'arbre.
	// En entré:
	// Param1: L'ID de l'élément.
	// Retour: L'élément.
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