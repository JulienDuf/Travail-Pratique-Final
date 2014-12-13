// 420-202-RE : Travail final
// Classe qui repr�sente un missile ayant un angle et une force.
// 16 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CMissile : public CProjectile {

private:

	bool m_boMissileLancer; // Si le missile est lanc�
	int m_iAngle; // L'angle de d�part du missile.
	unsigned int m_uiForce; // Force de d�part du missile.
	unsigned int m_uiMunition; // Le nombre de missiles disponible.
	CVecteur2D* m_pVecteurVitesseMissile;

	SDL_Point  m_PointRotation; // Le poitn de rotation.
	SDL_Surface* m_pSDLSurfaceMissile; // La surface du missile.
	SDL_Surface* m_pSDLSurfaceMissileRotation; // La surface du missile.
	SDL_Rect m_RectDestinationMissile; // La destination du missile dans la fen�tre.
	CBarrePuissance* m_pBarrePuissance; // La barre d�terminant l'angle et la puissance du missile.

	CLabel* m_pLblDescription; // La descripton du missile.
	string m_strDescription[8];
	bool m_boShowDescription;

	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY); // Proc�dure d�terminant
	SDL_Surface* (*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle); // Rotation

	SDL_Surface* BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 300, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);

		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[i].c_str(), { 0, 0, 0 });
			Rect.y = uiH * i;
			SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
		}

		return pSurfaceBlitin;
	}

	char chr[3];

	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		SDL_itoa(m_uiMunition, chr, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Munition : ");
		m_strDescription[0].append(chr);
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 8, { 0, 0, 0 })), 0);
	}

public:

	CMissile(string _strEmplacement, SDL_Renderer* _pRenderer, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {

		m_boShowDescription = false;

		m_strDescription;
		string strEmplacement(_strEmplacement);
		string strMunition;
		int i = strEmplacement.length();

		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionMissile.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);
		if (FichierDescription.is_open()) {
			char chrtmp[55];
			FichierDescription.getline(chrtmp, 75);
			for (int i = 11; chrtmp[i] != -52; i++) {
				strMunition += chrtmp[i];
			}
			m_uiMunition = SDL_atoi(strMunition.c_str());
			m_strDescription[0] = chrtmp;
			for (int i = 1; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
			}
		}

		FichierDescription.close();

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 8, { 0, 0, 0 });

		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 0, 0, pSDLSurface->w, pSDLSurface->h });

		m_iAngle = 0;
		m_uiForce = 0;

		m_pSDLSurfaceMissile = pGestionnaireSurface->ObtenirDonnee("pSurfaceMissile");
		m_pSDLSurfaceMissileRotation = m_pSDLSurfaceMissile;

		m_RectDestinationMissile = { 0, 0, 0, 0 };
		m_RectDestinationMissile.w = m_pSDLSurfaceMissile->w;
		m_RectDestinationMissile.h = m_pSDLSurfaceMissile->h;
		m_pBarrePuissance = new CBarrePuissance();

		m_pMapDestruction = _MapDestruction;
		m_pCollisionMap = _CollisionMap;
		m_pRotation = _Rotation;

		m_PointRotation = { m_RectDestinationMissile.w, m_RectDestinationMissile.h / 2 };

		m_boMissileLancer = false;
	}

	void ReactionColision(int iX, int iY) {
	
		m_pMapDestruction(50, iX, iY);
		m_boMissileLancer = false;

		delete m_pVecteurVitesseMissile;
		m_pVecteurVitesseMissile = nullptr;
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {

		if (m_boMissileLancer) {

			SDL_Texture* pTextureTMP = SDL_CreateTextureFromSurface(_pRenderer, m_pSDLSurfaceMissileRotation);
			SDL_RenderCopy(_pRenderer, pTextureTMP, NULL, &m_RectDestinationMissile);
			SDL_DestroyTexture(pTextureTMP);
			
		}
		else {
			_RectPlayerDestination.x -= 22;
			_RectPlayerDestination.y -= 10;
			m_pBarrePuissance->AfficherBarre(_pRenderer, _RectPlayerDestination);
		}
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}

	void ReactToEvent(SDL_Event* _pEvent) {

		m_pBarrePuissance->ReactToEvent(_pEvent);

		if (!m_boMissileLancer) {

			switch (_pEvent->type) {

			case SDL_KEYDOWN:

				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					m_iAngle = m_pBarrePuissance->ObtenirAngle();
					m_uiForce = (m_pBarrePuissance->ObtenirForce() + 3) * 100;
					m_boMissileLancer = true;
					m_pVecteurVitesseMissile = new CVecteur2D((float)m_uiForce, (float)m_iAngle);

					m_pBarrePuissance->ObtenirPosition(&m_RectDestinationMissile.x, &m_RectDestinationMissile.y);
					m_RectDestinationMissile.y -= m_RectDestinationMissile.h;
				}
				break;
			}
		}

	}

	void DefinirActif(bool _boActif) {

	}

	unsigned int ObtenirMunition() {
		return m_uiMunition;
	}

	void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) {

		m_boShowDescription = _boShow;
		int uiW, 
			uiH;
		m_pLblDescription->GetTextureDimension(0, &uiW, &uiH);
		if (_RectPositionDescription.x + uiW >= 1366) 
			m_pLblDescription->SetRectDestinationX(_RectPositionDescription.x - uiW);
		else
			m_pLblDescription->SetRectDestinationX(_RectPositionDescription.x);

		if (_RectPositionDescription.y + uiH >= 768) 
			m_pLblDescription->SetRectDestinationY(_RectPositionDescription.y - uiH);
		else
			m_pLblDescription->SetRectDestinationY(_RectPositionDescription.y);
	}

	CSprite* ObtenirSprite(string _strNom) {
		return nullptr;
	}

	CVecteur2D* ObtenirVecteurVitesse() {

		return m_pVecteurVitesseMissile;
	}

	SDL_Rect* ObtenirRectDestination() {

		return &m_RectDestinationMissile;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSDLSurfaceMissileRotation;
	}

	void DefinirAngle(double _dAngle) {

		m_iAngle = _dAngle;

		m_pSDLSurfaceMissileRotation = m_pRotation(m_pSDLSurfaceMissile, m_iAngle);

	}

	void DefinirPosition(SDL_Rect _Rect) {

		m_RectDestinationMissile = _Rect;
	}

	bool EstLancer() {
		
		return m_boMissileLancer;
	}
};