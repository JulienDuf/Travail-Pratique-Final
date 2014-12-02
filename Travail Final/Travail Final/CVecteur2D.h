//
// Classe repr�sentant une vecteur 2D
// Cr�� par Julien Dufresne (dufresne_julien@hotmail.ca) le 18 novembre 2014
//

class CVecteur2D {

private:

	int m_iComposanteX, // La composante en x du vecteur.
		m_iComposanteY; // La composante en y du vecteur.

public:

	// Constructeur...
	// En entr�e:
	// Param1: La composante en x du vecteur.
	// Param2: La composante en y du vecteur.
	CVecteur2D(int _iX, int _iY) {

		m_iComposanteX = _iX;
		m_iComposanteY = _iY;
	}

	// Constructeur...
	// En entr�e:
	// Param1: La norme du vecteur.
	// Param2: L'orientation du vecteur.
	CVecteur2D(float _fNorme, float _fOrientation) {

		ModifierVecteur(_fNorme, _fOrientation);		// L'orientation commence � l'angle 0 et va dans le sens horaire au lieu du sens antihoraire.  
														// 90 degr� pointera donc vers le bas et 270 degr�s vers le haut. �a fait chier. -Nico
	}

	// Red�finition de l'op�rateur += d'un vecteur.
	// En entr�e:
	// Param1: L'autre vecteur � additionner.
	void operator += (CVecteur2D _VecteurB) {

		m_iComposanteX += _VecteurB.m_iComposanteX;
		m_iComposanteY += _VecteurB.m_iComposanteY;

	}
	void ModifierVecteur(float _fNorme, float _fOrientation) {

		m_iComposanteX = _fNorme * (cosf((M_PI / 180) * _fOrientation));
		m_iComposanteY = _fNorme * (sinf((M_PI / 180) * _fOrientation));
	}

	void ModifierComposantX(int _iX) {

		m_iComposanteX = _iX;
	}

	void ModifierComposantY(int _iY) {

		m_iComposanteY = _iY;
	}

	int ObtenirComposanteX(void) {

		return m_iComposanteX;
	}

	int ObtenirComposanteY(void) {

		return m_iComposanteY;
	}


};