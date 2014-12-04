// 420-202-RE : Travail final
// Classe qui représente un missile ayant un angle et une force.
// 16 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CMissile : public CTools {

private:

	bool m_boMissileLancer; // Si le missile est lancé
	double m_dAngle; // L'angle de départ du missile.
	int m_iForce; // Force de départ du missile.
	int m_iNombreMissiles; // Le nombre de missiles disponible.
	CVecteur2D* VecteurVitesse;

	SDL_Point  m_PointRotation; // Le poitn de rotation.
	SDL_Surface* m_pSDLSurfaceMissile; // La surface du missile.
	SDL_Surface* m_pSDLSurfaceMissileRotation; // La surface du missile.
	SDL_Rect m_RectDestinationMissile; // La destination du missile dans la fenêtre.
	CBarrePuissance* m_pBarrePuissance; // La barre déterminant l'angle et la puissance du missile.

	CLabel* m_pLblDescription; // La descripton du missile.

	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY); // Procédure déterminant 
	double(*m_pPhysiqueMissile)(CVecteur2D* _ForceMissile, SDL_Rect* _DestinationMissile);
	SDL_Surface* (*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle);

public:

	CMissile(string _strEmplacement, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _PhysiqueMissile(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {

		m_dAngle = 0;
		m_iForce = 0;

		m_pSDLSurfaceMissile = _pGestionnaireSurface->ObtenirDonnee("pSurfaceMissile");

		m_RectDestinationMissile = { 0, 0, 0, 0 };
		m_RectDestinationMissile.w = m_pSDLSurfaceMissile->w;
		m_RectDestinationMissile.h = m_pSDLSurfaceMissile->h;
		m_pBarrePuissance = new CBarrePuissance(_pGestionnaireTexture);

		m_pMapDestruction = _MapDestruction;
		m_pCollisionMap = _CollisionMap;
		m_pPhysiqueMissile = _PhysiqueMissile;
		m_pRotation = _Rotation;

		m_PointRotation = { m_RectDestinationMissile.w, m_RectDestinationMissile.h / 2 };

		m_boMissileLancer = false;
	}

	void ShowTools(SDL_Renderer* _pRenderer) {

		if (m_boMissileLancer) {

			int iX, iY;

			m_pPhysiqueMissile(VecteurVitesse, &m_RectDestinationMissile);
			SDL_Texture* pTextureTMP = SDL_CreateTextureFromSurface(_pRenderer, m_pSDLSurfaceMissileRotation);
			SDL_RenderCopy(_pRenderer, pTextureTMP, NULL, &m_RectDestinationMissile);
			SDL_DestroyTexture(pTextureTMP);

			m_pCollisionMap(m_pSDLSurfaceMissileRotation, m_RectDestinationMissile, &iX, &iY);

			if (iX != 0 && iY != 0) {

				m_pMapDestruction(50, iX, iY);

				m_boMissileLancer = false;
			}
		}

		m_pBarrePuissance->AfficherBarre(_pRenderer);
	}

	void ReactToEvent(SDL_Event* _pEvent) {

		m_pBarrePuissance->ReactToEvent(_pEvent);

		if (!m_boMissileLancer) {

			switch (_pEvent->type) {

			case SDL_KEYDOWN:

				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					m_dAngle = m_pBarrePuissance->ObtenirAngle();
					m_iForce = (m_pBarrePuissance->ObtenirForce() + 3) * 100;
					m_boMissileLancer = true;
					float fAngle = m_dAngle;
					VecteurVitesse = new CVecteur2D((float)m_iForce, fAngle);

					m_pBarrePuissance->ObtenirPosition(&m_RectDestinationMissile.x, &m_RectDestinationMissile.y);
					m_pBarrePuissance->DefenirBoActif(false);
				}
				break;
			}
		}

	}

	void ModifierAngle(float _fAngle) {

		m_dAngle = _fAngle;

		m_pSDLSurfaceMissileRotation = m_pRotation(m_pSDLSurfaceMissile, m_dAngle);

	}

};