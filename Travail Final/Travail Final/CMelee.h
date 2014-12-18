/*
Classe qui gere les armes de melee
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CMelee : public CMouvement {
private:

	float m_fDommage; //Degats de l'arme de melee
	unsigned int m_uiRayon; // Rayon de dégat de l'attaque.
	CSprite* m_pSprite; //sprite du personnage qui utilise l'arme de melee

	CLabel* m_pLblDescription; // La descripton du missile.
	string m_strDescription[8];
	bool m_boShowDescription;
	bool m_boSpace;

	SDL_Surface* BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 235, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);

		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[i].c_str(), { 0, 0, 0 });
			Rect.y = uiH * i;
			SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
		}

		return pSurfaceBlitin;
	}

public:

	// Constructeur de CMelee
	CMelee(string _strEmplacement, CSprite* _pSprite, SDL_Renderer* _pRenderer) {

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
			string strDommage;
			string strRayon;

			for (int i = 0; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
				switch (i) {
				case 0:

					for (int j = 9; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strDommage += chrtmp[j];
					}
					break;
				case 2:

					for (int j = 8; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strRayon += chrtmp[j];
					}
					break;
				}
			}
			m_fDommage = (float)SDL_atoi(strDommage.c_str()) / 100;
			m_uiRayon = SDL_atoi(strRayon.c_str());
		}

		FichierDescription.close();

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 8, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 0, 0, pSDLSurface->w, pSDLSurface->h });

		m_pSprite = _pSprite;
		m_pSprite->DefinirPositionDeBouclage(0, 1);

		m_boSpace = false;
	}


	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) { 
		m_pSprite->DefinirActif(true);
   		if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE && !m_boSpace) {
			m_boSpace = true;
			m_pSprite->DefinirboBoucle(false);
			m_pSprite->DefinirPositionDeBouclage(0, 30);
		}
	}

	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		if (m_pSprite->ModifierAnnimation()) { // Fin du tour...
			m_pSprite->DefinirPositionDeBouclage(0, 1);
			m_pSprite->DefinirActif(true);
			m_pSprite->DefinirboBoucle(true);
			m_boSpace = false;
		}
		_RectPlayerDestination.w += 20;
		_RectPlayerDestination.h += 10;
		_RectPlayerDestination.y -= 5;
		m_pSprite->Render(_pRenderer, _RectPlayerDestination);
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription)
			m_pLblDescription->ShowControl(_pRenderer);
	}

	void DefinirActif(bool _boActif) {

		m_pSprite->DefinirActif(_boActif);
	}

	unsigned int ObtenirMunition() {
		return NULL;
	}

	CSprite* ObtenirSprite(string _strNom) { return m_pSprite; }

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

	bool IsActive(void) {

		return true;
	}

	CVecteur2D* ObtenirVecteur(void) {

		return nullptr;
	}
};