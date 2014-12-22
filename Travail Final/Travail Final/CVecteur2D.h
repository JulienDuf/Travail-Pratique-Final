//
// Classe représentant une vecteur 2D
// Créé par Julien Dufresne (dufresne_julien@hotmail.ca) le 18 novembre 2014
// Ajout de ObtenirNorme par Gabriel Beaudry.
//

class CVecteur2D {

private:

	double m_dComposanteX, // La composante en x du vecteur.
		m_dComposanteY; // La composante en y du vecteur.

public:

	// Constructeur par defaut...
	CVecteur2D() {}

	// Constructeur...
	// En entrée:
	// Param1: La composante en x du vecteur.
	// Param2: La composante en y du vecteur.
	CVecteur2D(double _dX, double _dY) {

		m_dComposanteX = _dX;
		m_dComposanteY = _dY;
	}

	// Constructeur...
	// En entrée:
	// Param1: La norme du vecteur.
	// Param2: L'orientation du vecteur.
	CVecteur2D(double _dNorme, float _dOrientation) {

		ModifierVecteur(_dNorme, _dOrientation);		// L'orientation commence à l'angle 0 et va dans le sens horaire au lieu du sens antihoraire.  
														// 90 degré pointera donc vers le bas et 270 degrés vers le haut. Ça fait chier. -Nico
	}

	// Redéfinition de l'opérateur += d'un vecteur.
	// En entrée:
	// Param1: L'autre vecteur à additionner.
	void operator += (CVecteur2D _VecteurB) {

		m_dComposanteX += _VecteurB.m_dComposanteX;
		m_dComposanteY += _VecteurB.m_dComposanteY;

	}

	void operator -= (CVecteur2D _VecteurB) {
		
		m_dComposanteX -= _VecteurB.m_dComposanteX;
		m_dComposanteY -= _VecteurB.m_dComposanteY;
	}

	CVecteur2D operator * (double _dScalaire) {

		return CVecteur2D(_dScalaire*m_dComposanteX, _dScalaire*m_dComposanteY);
	}

	double Scalaire(double _dComposanteX, double _dComposanteY) {

		return m_dComposanteX * _dComposanteX + m_dComposanteY * _dComposanteY;
	}

	void ModifierVecteur(float _dNorme, float _dOrientation) {

		m_dComposanteX = _dNorme * (cosf((M_PI / 180) * _dOrientation));
		m_dComposanteY = _dNorme * (sinf((M_PI / 180) * _dOrientation));
	}

	void ModifierComposantX(int _iX) {

		m_dComposanteX = _iX;
	}

	void ModifierComposantY(int _iY) {

		m_dComposanteY = _iY;
	}

	double ObtenirComposanteX(void) {

		return m_dComposanteX;
	}

	double ObtenirComposanteY(void) {

		return m_dComposanteY;
	}

	double ObtenirNorme(void) {

		return sqrt(pow(m_dComposanteX, 2) + pow(m_dComposanteY, 2));
	}

	double ObtenirOrientation(void) {

		return atan(m_dComposanteY / m_dComposanteX);
	}
	void ModifierOrientation(double _dOrientation) {

		if (_dOrientation <= 360) {

			double dNorme = sqrt(pow(m_dComposanteX, 2) + pow(m_dComposanteY, 2)); // Pythagore
			m_dComposanteX = dNorme * (cosf((M_PI / 180) * _dOrientation));
			m_dComposanteY = dNorme * (sinf((M_PI / 180) * _dOrientation));

		}

		else {
			double dNorme = sqrt(pow(m_dComposanteX, 2) + pow(m_dComposanteY, 2)); // Pythagore
			m_dComposanteX = dNorme * (cosf((M_PI / 180) * 90));
			m_dComposanteY = dNorme * (sinf((M_PI / 180) * 90));

		}
	}


};