/*
Classe qui gere les armes de melee
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CMelee : public CTool {
private:

	float m_fDommage; //Degats de l'arme de melee
	SDL_Surface* m_pSurfaceSprite; //Surface SDL qui deviendra le sprite
	CSprite* m_pSprite; //sprite du personnage qui utilise l'arme de melee
	SDL_Rect m_RectDestinationArme;

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

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 235, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);

		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(m_pFont, _strTexte[i].c_str(), { 0, 0, 0 });
			Rect.y = uiH * i;
			SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
		}

		return pSurfaceBlitin;
	}

public:

	// Constructeur de CMelee
	CMelee(string _strEmplacement, CGestionaire<TTF_Font*>* _pGestionnaireFont, SDL_Renderer* _pRenderer){

		m_boShowDescription = false;

		m_strDescription;
		string strEmplacement(_strEmplacement);
		string strDommage;
		int i = strEmplacement.length();

		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionMelee.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);
		if (FichierDescription.is_open()) {
			char chrtmp[55];
			FichierDescription.getline(chrtmp, 75);
			for (int i = 10; chrtmp[i] != -52; i++) {
				strDommage += chrtmp[i];
			}
			m_fDommage = SDL_atoi(strDommage.c_str()) / 100;
			m_strDescription[0] = chrtmp;
			for (int i = 1; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
			}
		}

		FichierDescription.close();

		m_pFont = _pGestionnaireFont->ObtenirDonnee("pFontDescription");

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 8, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 0, 0, pSDLSurface->w, pSDLSurface->h });
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSprite->Render(_pRenderer, m_RectDestinationArme);
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription)
			m_pLblDescription->ShowControl(_pRenderer);
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
};