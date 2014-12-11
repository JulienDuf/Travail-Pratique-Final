/*
Classe qui gere les grenades et leur rebondissement
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CGrenade: public CProjectile {
private:

	int m_iAngle; //Angle du lancement de la grenade
	int m_iForce; //Force du lancement de la grenade
	unsigned int m_uiMunition;
	CBarrePuissance* m_pBarrePuissance; //Barre de puissance de la grenade
	CVecteur2D* m_pVecteurVitesseGrenade;

	SDL_Texture* m_pTexture; //Texture de la grenade
	SDL_Surface* m_pSurface;
	SDL_Rect m_RectDestinationGrenade; //Position de la grenade

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
	//constructeur
	CGrenade(string _strEmplacement, CGestionaire<TTF_Font*>* _pGestionnaireFont, SDL_Renderer* _pRenderer){

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

		//Initialisation de la texture
		string strSourceImage = _strEmplacement;
		strSourceImage.append("\\Armes et Packs\\Grenade.png");
		m_pTexture = IMG_LoadTexture(_pRenderer, strSourceImage.c_str());

		//initialisation des vecteurs de la grenade
		m_iAngle = 0;
		m_iForce = 0;
		//m_pBarrePuissance = new CBarrePuissance(_strSourceImage);

		//initialisation de la position de la grenade
		m_RectDestinationGrenade.h = 24;
		m_RectDestinationGrenade.w = 33;
		m_RectDestinationGrenade.y = 0;
		m_RectDestinationGrenade.x = 0;
	}
	
	void ReactionColision(int iX, int iY) {
	
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}
	
	void ReactToEvent(SDL_Event* _pEvent) {}

	void DefinirActif(bool _boActif) {}

	CSprite* ObtenirSprite(string _strNom) { return nullptr; }

	unsigned int ObtenirMunition() { return 0; }

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

		return m_pSurface;
	}

	void DefinirAngle(double _dAngle) {


	}

	void DefinirPosition(SDL_Rect _Rect) {

		m_RectDestinationGrenade = _Rect;
	}
};