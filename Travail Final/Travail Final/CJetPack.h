// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack : public CMouvement {
private:

	CSprite* m_pSpriteJetPack;

	CBarreVie* m_pBarreDeCarburant;

	CLabel* m_pLblDescription;
	string m_strDescription[8];

	bool m_boSpace,
		m_boShowDescription;
	bool boFleche;
	bool boDecoller;

	unsigned int m_uiTempsPropulsionInitiale;
	int m_iVerticalThrust;

	CVecteur2D* pVecteur;

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

	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		char m_chrMunition[4];
		
		SDL_itoa(ObtenirMunition(), m_chrMunition, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Niveau de carburant : ");
		m_strDescription[0].append(m_chrMunition);
		m_strDescription[0].append("%                        ");
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 5, { 0, 0, 0 })), 0);
	}

public:

	CJetPack(string _strEmplacement, CSprite* _pSpriteJetPack, CBarreVie* _pBarreDeCarburant, SDL_Renderer* _pRenderer) {
		boFleche = true;
		m_boShowDescription = false;
		boDecoller = false;
		pVecteur = new CVecteur2D(0, 0.0f);
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
			char chrtmp[55];
			string strCarburant;
			string strVerticalThrust;

			for (int i = 0; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
				switch (i) {
				case 0:

					for (int j = 22; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strCarburant += chrtmp[j];
					}
					break;
				case 2:

					for (int j = 19; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strVerticalThrust += chrtmp[j];
					}
					break;
				}
			}
			m_pBarreDeCarburant = _pBarreDeCarburant;
			m_pBarreDeCarburant->ModifierPourcentageVie(SDL_atoi(strCarburant.c_str()) / 100);
			m_iVerticalThrust = SDL_atoi(strVerticalThrust.c_str());
		}

		FichierDescription.close();

		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 5, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 503, 346, pSDLSurface->w, pSDLSurface->h });

		m_pSpriteJetPack = _pSpriteJetPack;

		m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);

		m_uiTempsPropulsionInitiale = 0;
	}

	~CJetPack() {

		delete m_pSpriteJetPack;
		delete m_pBarreDeCarburant;
		delete m_pLblDescription;
		delete pVecteur;
	}

	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) {

		*_boStable = false;

		m_pSpriteJetPack->DefinirActif(true);
		// Barre de carburant vide
		if (m_pBarreDeCarburant->ObtenirPourcentage() <= 0) {
			m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
			*_boStable = false;
		}
		else {

			switch (_pEvent->type) {

			case SDL_KEYDOWN:

				switch (_pEvent->key.keysym.scancode) {

				case SDL_SCANCODE_RIGHT:
					m_pSpriteJetPack->DefinirEtage(0);
					pVecteur->ModifierComposantX(2);
					*_boStable = false;
					boFleche = true;
					break;

				case SDL_SCANCODE_LEFT:
					m_pSpriteJetPack->DefinirEtage(1);
					pVecteur->ModifierComposantX(-2);
					*_boStable = false;
					boFleche = true;
					break;

				case SDL_SCANCODE_SPACE:
 					m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);
					
					if (!m_boSpace) {
						if (boDecoller)
							pVecteur->ModifierComposantY(-m_iVerticalThrust);
						else {
							pVecteur->ModifierComposantY(-100);
							boDecoller = true;
						}
					}
					m_uiTempsPropulsionInitiale++;
					if (boFleche)
  						boFleche = boFleche;
					if (m_uiTempsPropulsionInitiale >= 3) {
						m_pSpriteJetPack->DefinirPositionDeBouclage(4, 6);
					}
					else
					{
						m_pSpriteJetPack->DefinirPositionDeBouclage(0, 6);
					}
					m_boSpace = true;
					*_boStable = false;
					break;
				}
				break;

			case SDL_KEYUP:

				switch (_pEvent->key.keysym.scancode) {

				case SDL_SCANCODE_RIGHT:
					*_boStable = false;
					pVecteur->ModifierComposantX(0);
					boFleche = false;
					break;

				case SDL_SCANCODE_LEFT:
					pVecteur->ModifierComposantX(0);
					*_boStable = false;
					boFleche = false;
					break;

				case SDL_SCANCODE_SPACE:
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
					pVecteur->ModifierComposantY(0);
					m_boSpace = false;
					m_uiTempsPropulsionInitiale = 0;
					*_boStable = false;
					break;
				}
				break;
			}

			if (m_boSpace) {
				m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);

					if (boDecoller)
						pVecteur->ModifierComposantY(-m_iVerticalThrust);
					else {
						pVecteur->ModifierComposantY(-100);
						boDecoller = true;
					
				}
				m_uiTempsPropulsionInitiale++;
				if (boFleche)
					boFleche = boFleche;
				if (m_uiTempsPropulsionInitiale >= 3) {
					m_pSpriteJetPack->DefinirPositionDeBouclage(4, 6);
				}
				else
				{
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 6);
				}
				m_boSpace = true;
				*_boStable = false;
			}
		}
	}

	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, _RectPlayerDestination);
		if (m_pSpriteJetPack->IsActif())
			m_pBarreDeCarburant->ShowBarre(_pRenderer, { _RectPlayerDestination.x, _RectPlayerDestination.y + _RectPlayerDestination.h + 2, 40, 6 });
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

	bool IsActive(void) {

		return boDecoller;
	}

	CVecteur2D* ObtenirVecteur(void) {

		return pVecteur;
	}
};