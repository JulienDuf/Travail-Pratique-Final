// 420-202-RE : Travail final
// Classe qui représente une barre pouvant dertéminer une puissance de lancer.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CBarrePuissance {

private:

	int m_iAngle; // L'angle de la barre.
	unsigned int m_uiForce; // La force affiché par la barre.
	bool m_boActive; // La barre est active.

	SDL_Point m_PointRotation; // Le point de rotation.
	SDL_Texture* m_pSDLTextureBarrre; // La texture de la barre.
	SDL_Rect m_RectDestinationBarre, // La destination de la barre dans la fenêtre.
		m_RectSourceBarre;

public:

	// Constructeur de la barre de puissance...
	// En entrée:
	// Param1: L'emplacement des fichiers.
	// Param2: Le renderer de la fenêtre.
	CBarrePuissance() {

		m_uiForce = 0;
		m_boActive = true;

		m_pSDLTextureBarrre = pGestionnaireTexture->ObtenirDonnee("pTextureBarrePuissance");

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

		if (m_boActive) {
			m_RectDestinationBarre.x = _RectBarreDestination.x + _RectBarreDestination.w;
			m_RectDestinationBarre.y = _RectBarreDestination.y;
			SDL_RenderCopyEx(_pRenderer, m_pSDLTextureBarrre, &m_RectSourceBarre, &m_RectDestinationBarre, m_iAngle, &m_PointRotation, SDL_FLIP_NONE);
		}
	}

	void ModifierForceBarre(int _iForce) {

		m_uiForce = _iForce;
		m_RectSourceBarre.x = m_uiForce * m_RectDestinationBarre.w;
	}

	void ReactToEvent(SDL_Event* _pEvent) {

		switch (_pEvent->type) {

		case SDL_KEYDOWN:

			switch (_pEvent->key.keysym.scancode) {

			case SDL_SCANCODE_RIGHT:

				m_uiForce = (m_uiForce + 1) % 4;
				m_RectSourceBarre.x = m_uiForce * m_RectDestinationBarre.w;
				break;

			case SDL_SCANCODE_LEFT:

				if (m_uiForce == 0)
					m_uiForce = 3;
				else
					m_uiForce--;
				m_RectSourceBarre.x = m_uiForce * m_RectDestinationBarre.w;
				break;

			case SDL_SCANCODE_DOWN:

				m_iAngle = (m_iAngle + 5) % 360;
				break;

			case SDL_SCANCODE_UP:

				m_iAngle -= 5;
				if (m_iAngle < 0) {
					m_iAngle = 355;
				}
				break;

			}

			break;

		}
	}

	void Reinitialisation() {
		m_RectSourceBarre.x = 0;
		m_uiForce = 0;
		m_iAngle = 0;
	}

	// Accesseurs...

	double ObtenirAngle(void) {

		return m_iAngle;
	}

	int ObtenirForce(void) {

		return m_uiForce;
	}

	void ObtenirPosition(int* _iX, int* _iY) {

		*_iX = m_RectDestinationBarre.x;
		*_iY = m_RectDestinationBarre.y;
	}

	void DefenirPositionBarre(SDL_Rect _RectDestination) {

		m_RectDestinationBarre.x = _RectDestination.x;
		m_RectDestinationBarre.y = _RectDestination.y;
	}

	void ModidierActivite(bool _boActive) {

		m_boActive = _boActive;
	}
};