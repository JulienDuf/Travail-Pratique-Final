/*
Classe qui gere les grenades et leur rebondissement
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CGrenade: public CProjectile {
private:

	int m_iAngle; //Angle du lancement de la grenade
	unsigned int m_uiForce; //Force du lancement de la grenade
	unsigned int m_uiMunition;
	CBarrePuissance* m_pBarrePuissance; //Barre de puissance de la grenade
	bool m_boGrenadeLancer;
	bool m_boDirectionRotation;
	CVecteur2D* m_pVecteurVitesseGrenade;

	SDL_Point m_PointRotation;
	SDL_Texture* m_pTextureGrenade; //Texture de la grenade
	SDL_Surface* m_pSurfaceGrenade,
		*m_pSurfaceGrenadeRotation;
	SDL_Rect m_RectDestinationGrenade; //Position de la grenade

	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY); // Procédure déterminant
	SDL_Surface* (*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle); // Rotation

	CLabel* m_pLblDescription; // La descripton du missile.
	string m_strDescription[8];
	TTF_Font* m_pFont;
	bool m_boShowDescription;

	SDL_Surface* BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(m_pFont, _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 300, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);

		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(m_pFont, _strTexte[i].c_str(), { 0, 0, 0 });
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
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 7, { 0, 0, 0 })), 0);
	}

public:

	// Constructeur de CGrenade
	CGrenade(string _strEmplacement, CGestionaire<TTF_Font*>* _pGestionnaireFont, SDL_Renderer* _pRenderer, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)){

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

		strEmplacement.append("Armes et Packs\\DescriptionGrenade.txt");
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

		m_pFont = _pGestionnaireFont->ObtenirDonnee("pFontDescription");

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 7, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 0, 0, pSDLSurface->w, pSDLSurface->h });

		m_iAngle = 0;
		m_uiForce = 0;

		m_pSurfaceGrenade = _pGestionnaireSurface->ObtenirDonnee("pSurfaceGrenade");
		m_pSurfaceGrenadeRotation = m_pSurfaceGrenade;

		m_RectDestinationGrenade = { 0, 0, 0, 0 };
		m_RectDestinationGrenade.w = m_pSurfaceGrenade->w;
		m_RectDestinationGrenade.h = m_pSurfaceGrenade->h;
		m_pBarrePuissance = new CBarrePuissance(_pGestionnaireTexture);

		m_pMapDestruction = _MapDestruction;
		m_pCollisionMap = _CollisionMap;
		m_pRotation = _Rotation;

		m_PointRotation = { m_RectDestinationGrenade.w, m_RectDestinationGrenade.h / 2 };

		m_boGrenadeLancer = false;
	}
	
	void ReactionColision(int iX, int iY) {
	
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
	
		if (m_boGrenadeLancer) {
			if (m_boDirectionRotation)
				m_iAngle = m_iAngle + 1 % 360;
			else {
				m_iAngle--;
				if (m_iAngle <= -360)
					m_iAngle = 0;
			}
			m_pSurfaceGrenadeRotation = m_pRotation(m_pSurfaceGrenade, m_iAngle);
			SDL_Texture* pTextureTMP = SDL_CreateTextureFromSurface(_pRenderer, m_pSurfaceGrenadeRotation);
			SDL_RenderCopy(_pRenderer, pTextureTMP, NULL, &m_RectDestinationGrenade);
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

		if (!m_boGrenadeLancer) {

			switch (_pEvent->type) {

			case SDL_KEYDOWN:

				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					m_iAngle = m_pBarrePuissance->ObtenirAngle();

					if (m_iAngle <= 90 || m_iAngle >= 270)
						m_boDirectionRotation = false;
					else
						m_boDirectionRotation = true;

					m_uiForce = (m_pBarrePuissance->ObtenirForce() + 3) * 100;
					m_boGrenadeLancer = true;
					m_pVecteurVitesseGrenade = new CVecteur2D((float)m_uiForce, (float)m_iAngle);

					m_pBarrePuissance->ObtenirPosition(&m_RectDestinationGrenade.x, &m_RectDestinationGrenade.y);
				}
				break;
			}
		}
	}

	void DefinirActif(bool _boActif) {
		
	}

	CSprite* ObtenirSprite(string _strNom) { 
		
		return nullptr; 
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

	CVecteur2D* ObtenirVecteurVitesse() {

		return m_pVecteurVitesseGrenade;
	}

	SDL_Rect* ObtenirRectDestination() {

		return &m_RectDestinationGrenade;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSurfaceGrenadeRotation;
	}

	void DefinirAngle(double _dAngle) {


	}

	void DefinirPosition(SDL_Rect _Rect) {

		m_RectDestinationGrenade = _Rect;
	}

	bool EstLancer() {

		return m_boGrenadeLancer;
	}
};