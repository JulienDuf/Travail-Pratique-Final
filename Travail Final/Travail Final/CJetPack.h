// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack {
private:

	CLabel* m_pDescription;
	CSprite* m_pSpriteJetPack;
	CBarreVie* m_pBarreDeCarburant;

	bool m_boSpace;

	unsigned int m_uiDepart;

	void BlitText(string _strTexte[], int _iNombreElementTableau, TTF_Font* _Font, SDL_Color _Couleur, SDL_Surface* _pSurfaceToBlitIn) {

		SDL_Surface* pSDLSurface; // Variable temporaire...
		SDL_Rect RectTmp = { 0, 0, _pSurfaceToBlitIn->w - 2, 0};

		for (int i = 0; i < _iNombreElementTableau; i++) {
			pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[i].c_str(), _Couleur); // Créé la surface contenant le texte.
			RectTmp.y = pSDLSurface->h * (i);
			SDL_BlitSurface(pSDLSurface, NULL, _pSurfaceToBlitIn, &RectTmp);
			SDL_FreeSurface(pSDLSurface);
		}
	}

public:

	CJetPack(string _strEmplacement, CGestionaire<TTF_Font*>* _pGestionnaireFont, CSprite* _pSpriteJetPack, CBarreVie* _pBarreDeCarburant, SDL_Renderer* _pRenderer) {

		string strDescription[8];
		string strEmplacement(_strEmplacement);
		int i = strEmplacement.length();

		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		string strEmplacementSurface(strEmplacement);
		strEmplacementSurface.append("Armes et Packs\\SurfaceLabel.png");
		SDL_Surface* pSDLSurface = IMG_Load(strEmplacementSurface.c_str());

		strEmplacement.append("Armes et Packs\\DescriptionJetPack.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);
		if (FichierDescription.is_open()) {
			for (int i = 0; i < 8; i++) {
				char chrtmp[75];
				FichierDescription.getline(chrtmp, 75);
				strDescription[i] = chrtmp;
			}
		}

		

		m_pDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 500, 500, 500, 500 });
		m_pSpriteJetPack = _pSpriteJetPack;
		m_pBarreDeCarburant = _pBarreDeCarburant;
		m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
		m_uiDepart = 0;
	}

	bool ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse) {

		if (m_pBarreDeCarburant->ObtenirPourcentage() <= 0) {
			m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
			return false;
		}
		
		switch (_pEvent->key.keysym.scancode) {
		case SDL_SCANCODE_RIGHT:
			if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(0);
					_pVecteurVitesse->ModifierComposantX(10);
					return false;
			}
				break;
		case SDL_SCANCODE_LEFT:
			if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(1);
					_pVecteurVitesse->ModifierComposantX(-10);
					return false;
			}
				break;
		case SDL_SCANCODE_SPACE:
			if (_pEvent->key.type == SDL_KEYDOWN) {
				m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);
				m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.005);
				_pVecteurVitesse->ModifierComposantY(-50);
				m_uiDepart++;
				if (m_uiDepart >= 3) {
					m_pSpriteJetPack->DefinirPositionDeBouclage(4, 6);
				}
				else
				{
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 6);
				}	
				m_boSpace = true;
				return false;
			}
			else
			{
				m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
				m_boSpace = false;
				m_uiDepart = 0;
				return false;
			}
			break;
		}
		return false;
	}

	void ShowJetPack(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, _RectPlayerDestination);
		if (m_pSpriteJetPack->IsActif())
			m_pBarreDeCarburant->ShowBarre(_pRenderer, { _RectPlayerDestination.x, _RectPlayerDestination.y + _RectPlayerDestination.h + 2, 40, 6 });
		m_pDescription->ShowControl(_pRenderer);
	}

	void DefinirActif(bool _boActif) {
		m_pSpriteJetPack->DefinirActif(_boActif);
	}

	CSprite* ObtenirSprite() {
		return m_pSpriteJetPack;
	}
};