// 420-202-RE : Travail final
// Classe qui représente une barre pouvant dertéminer une puissance de lancer.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CBarrePuissance {

private:

	bool m_BoBarreActive; // Si la barre est active.
	double m_dAngle; // L'angle de la barre.
	int m_iForce; // La force affiché par la barre.

	SDL_Point m_PointRotation; // Le point de rotation.
	SDL_Texture* m_pSDLTextureBarrre; // La texture de la barre.
	SDL_Rect m_RectDestinationBarre, // La destination de la barre dans la fenêtre.
		m_RectSourceBarre;

public:

	// Constructeur de la barre de puissance...
	// En entrée:
	// Param1: L'emplacement des fichiers.
	// Param2: Le renderer de la fenêtre.
	CBarrePuissance(CGestionaire<SDL_Texture*>* _pGestionnaireTexture) {

		m_BoBarreActive = false;

		m_iForce = 0;

		m_pSDLTextureBarrre = _pGestionnaireTexture->ObtenirDonnee("pTextureBarrePuissance");

		m_RectDestinationBarre = { 0, 0, 0, 0 };
		SDL_QueryTexture(m_pSDLTextureBarrre, NULL, NULL, &m_RectDestinationBarre.w, &m_RectDestinationBarre.h);

		m_RectDestinationBarre.w /= 4;

		m_RectSourceBarre = { 0, 0, m_RectDestinationBarre.w, m_RectDestinationBarre.h };

		m_PointRotation = { 0, m_RectDestinationBarre.h / 2};
	}

	// Procédure affichant la barre de puissance...
	// En entrée:
	// Param1: Le renderer de la fenêtre.
	void AfficherBarre(SDL_Renderer* _pRenderer, SDL_Rect _RectBarreDestination) {

		m_RectDestinationBarre.x = _RectBarreDestination.x + _RectBarreDestination.w;
		m_RectDestinationBarre.y = _RectBarreDestination.y;

		SDL_RenderCopyEx(_pRenderer, m_pSDLTextureBarrre, &m_RectSourceBarre, &m_RectDestinationBarre, m_dAngle, &m_PointRotation, SDL_FLIP_VERTICAL);
	}

	void ModifierForceBarre(int _iForce) {

		m_iForce = _iForce;
		m_RectSourceBarre.x = m_iForce * m_RectDestinationBarre.w;
	}

	void ReactToEvent(SDL_Event* _pEvent) {

		int iAngleTmp;

		switch (_pEvent->type) {

		case SDL_KEYDOWN:

			switch (_pEvent->key.keysym.scancode) {

			case SDL_SCANCODE_RIGHT:

				m_iForce = (m_iForce + 1) % 4;
				m_RectSourceBarre.x = m_iForce * m_RectDestinationBarre.w;
				break;

			case SDL_SCANCODE_LEFT:

				m_iForce--;
				if (m_iForce < 0)
					m_iForce = 3;
				m_RectSourceBarre.x = m_iForce * m_RectDestinationBarre.w;
				break;

			case SDL_SCANCODE_DOWN:

				iAngleTmp = m_dAngle;
				iAngleTmp = (iAngleTmp + 5) % 360;
				m_dAngle = iAngleTmp;

				break;

			case SDL_SCANCODE_UP:

				m_dAngle -= 5;
				if (m_dAngle == -5)
					m_dAngle = 355;

				break;

			}

			break;

		}
	}

	void DefenirBoActif(bool _boActif) {

		m_BoBarreActive = _boActif;
	}

	// Accesseurs...

	double ObtenirAngle(void) {

		return m_dAngle;
	}

	int ObtenirForce(void) {

		return m_iForce;
	}

	void ObtenirPosition(int* _iX, int* _iY) {

		*_iX = m_RectDestinationBarre.x;
		*_iY = m_RectDestinationBarre.y;
	}

	void DefenirPositionBarre(SDL_Rect _RectDestination) {

		m_RectDestinationBarre.x = _RectDestination.x;
		m_RectDestinationBarre.y = _RectDestination.y;
	}

};