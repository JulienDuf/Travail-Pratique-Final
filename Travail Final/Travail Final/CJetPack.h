// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack : public CDeplacement {
private:

	CSprite* m_pSpriteJetPack;

	CBarreVie* m_pBarreDeCarburant;

	CLabel* m_pLblDescription;
	string m_strDescription[5];
	TTF_Font* m_pFont;

	bool m_boSpace,
		m_boShowDescription;

	unsigned int m_uiTempsPropulsionInitiale;

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

	char m_chrMunition[3];

	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		SDL_itoa(ObtenirMunition(), m_chrMunition, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Niveau de carburant : ");
		m_strDescription[0].append(m_chrMunition);
		m_strDescription[0].append("%                        ");
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 5, { 0, 0, 0 })), 0);
	}

public:

	CJetPack(string _strEmplacement, CGestionaire<TTF_Font*>* _pGestionnaireFont, CSprite* _pSpriteJetPack, CBarreVie* _pBarreDeCarburant, SDL_Renderer* _pRenderer) {

		m_boShowDescription = false;

		m_strDescription;
		string strEmplacement(_strEmplacement);
		int i = strEmplacement.length();

		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionJetPack.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);
		if (FichierDescription.is_open()) {
			for (int i = 0; i < 5; i++) {
				char chrtmp[75];
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
			}
		}

		FichierDescription.close();

		m_pFont = _pGestionnaireFont->ObtenirDonnee("pFontDescription");

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 5, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 503, 346, pSDLSurface->w, pSDLSurface->h });

		m_pSpriteJetPack = _pSpriteJetPack;

		m_pBarreDeCarburant = _pBarreDeCarburant;

		m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);

		m_uiTempsPropulsionInitiale = 0;
	}

	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) {

		m_pSpriteJetPack->DefinirActif(true);
		// Barre de carburant vide
		if (m_pBarreDeCarburant->ObtenirPourcentage() <= 0) {
			m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
			*_boStable = false;
		}
		else
		{
			switch (_pEvent->key.keysym.scancode) {
			case SDL_SCANCODE_RIGHT:
				if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(0);
					_pVecteurVitesse->ModifierComposantX(10);
					_boStable = false;
				}
				break;
			case SDL_SCANCODE_LEFT:
				if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(1);
					_pVecteurVitesse->ModifierComposantX(-10);
					_boStable = false;
				}
				break;
			case SDL_SCANCODE_SPACE:
				if (_pEvent->key.type == SDL_KEYDOWN) {
					m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);
					_pVecteurVitesse->ModifierComposantY(-50);
					m_uiTempsPropulsionInitiale++;
					if (m_uiTempsPropulsionInitiale >= 3) {
						m_pSpriteJetPack->DefinirPositionDeBouclage(4, 6);
					}
					else
					{
						m_pSpriteJetPack->DefinirPositionDeBouclage(0, 6);
					}
					m_boSpace = true;
					_boStable = false;
				}
				else
				{
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
					m_boSpace = false;
					m_uiTempsPropulsionInitiale = 0;
					_boStable = false;
				}
				break;
			}
		}

		_boStable = false;
	}

	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, _RectPlayerDestination);
		if (m_pSpriteJetPack->IsActif())
			m_pBarreDeCarburant->ShowBarre(_pRenderer, { _RectPlayerDestination.x, _RectPlayerDestination.y + _RectPlayerDestination.h + 2, 40, 6 });
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}

	void DefinirActif(bool _boActif) {
		m_pSpriteJetPack->DefinirActif(_boActif);
	}

	CSprite* ObtenirSprite(string _strNom) {
		return m_pSpriteJetPack;
	}

	unsigned int ObtenirMunition() {
		return m_pBarreDeCarburant->ObtenirPourcentage() * 100;
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
};