// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack : public CMouvement {

private:

	CSprite* m_pSpriteJetPack; // Le sprite du jetpack...

	CBarreVie* m_pBarreDeCarburant; // La barre de carburant...

	CLabel* m_pLblDescription; // Le label description du jetpack...
	string m_strDescription[8]; // La description du jetpack...

	bool m_boSpace, // Si la barre d'expace est enfoncé...
		m_boShowDescription; // Si on affiche la description...
	bool m_boDecoller; // Si le jectpack est décollé...

	unsigned int m_uiTempsPropulsionInitiale; // Le temps de propulsion du jetpack...
	int m_iVerticalThrust; // La poussée vertical...

	CVecteur2D* m_pVecteur; // Le vecteur du jetpack

	SDL_Surface* BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 300, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
		SDL_FreeSurface(pSurfaceBlitSource);
		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[i].c_str(), { 0, 0, 0 });
			Rect.y = uiH * i;
			SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
			SDL_FreeSurface(pSurfaceBlitSource);
		}

		return pSurfaceBlitin;
	}

	// Procédure mettant ajour les munitions...
	// En entrée: 
	// Param1: Le renderer de la fenêtre...
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

	// Constructeur...
	CJetPack(string _strEmplacement, CSprite* _pSpriteJetPack, CBarreVie* _pBarreDeCarburant, SDL_Renderer* _pRenderer) {
		m_boShowDescription = false;
		m_boDecoller = false;
		m_pVecteur = new CVecteur2D(0, 0.0f);
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

	// Classe réagissant au évémement du programme...
	// En entrée: 
	// Param1: Le gestionnaire d'événement
	// Param2: Le vecteur vitesse du joueur.
	// Param3: Le boléan de stabilité du joueur.
	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* pVecteurVitesse, bool* _boStable) {

		*_boStable = false; // Le joueur n'est pas stable...

		// Le sprite devient actif...
		m_pSpriteJetPack->DefinirActif(true);
		
		// Barre de carburant vide
		if (m_pBarreDeCarburant->ObtenirVie() <= 0) {
			m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
			*_boStable = false;
		}

		// S'il reste du carburant...
		else {

			switch (_pEvent->type) {

			// S'il y a une touche d'enfoncée...
			case SDL_KEYDOWN:

				switch (_pEvent->key.keysym.scancode) {

				// Si c'est la flèche de droite...
				case SDL_SCANCODE_RIGHT:
					m_pSpriteJetPack->DefinirEtage(0);
					m_pVecteur->ModifierComposantX(2);
					*_boStable = false;
					break;

				// Si c'est la flèche de gauche...
				case SDL_SCANCODE_LEFT:
					m_pSpriteJetPack->DefinirEtage(1);
					m_pVecteur->ModifierComposantX(-2);
					*_boStable = false;
					break;

				// Si c'est la barre d'espace..
				case SDL_SCANCODE_SPACE:
 					m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);
					
					if (!m_boSpace) {
						if (m_boDecoller)
							m_pVecteur->ModifierComposantY(-m_iVerticalThrust);
						else {
							m_pVecteur->ModifierComposantY(-100);
							m_boDecoller = true;
						}
					}
					m_uiTempsPropulsionInitiale++;

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

			// Si une touche n'est plus enfoncée...
			case SDL_KEYUP:

				switch (_pEvent->key.keysym.scancode) {

				// Si c'est la flèche de droite...
				case SDL_SCANCODE_RIGHT:
					*_boStable = false;
					m_pVecteur->ModifierComposantX(0);
					break;

				// Si c'est la flèche de gauche...
				case SDL_SCANCODE_LEFT:
					m_pVecteur->ModifierComposantX(0);
					*_boStable = false;
					break;

				// Si c'est la barre d'espace..
				case SDL_SCANCODE_SPACE:
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
					m_pVecteur->ModifierComposantY(0);
					m_boSpace = false;
					m_uiTempsPropulsionInitiale = 0;
					*_boStable = false;
					break;
				}
				break;
			}

			// Si la barre d'espace est enfoncée... On fait comme dans le KEYDOWN...
			if (m_boSpace) {
				m_pBarreDeCarburant->ModifierPourcentageVie(m_pBarreDeCarburant->ObtenirVie() - 0.002);

					if (m_boDecoller)
						m_pVecteur->ModifierComposantY(-m_iVerticalThrust);
					else {
						m_pVecteur->ModifierComposantY(-100);
						m_boDecoller = true;
					
				}
				m_uiTempsPropulsionInitiale++;
				

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

	// Procédure affichant le déplacement...
	// En entrée: 
	// Param1: Le renderer de la fenêtre.
	// Param2: Le rect destination du joueur.
	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		
		// Affiche le sprite...
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, _RectPlayerDestination);
		if (m_pSpriteJetPack->IsActif())
			m_pBarreDeCarburant->ShowBarre(_pRenderer, { _RectPlayerDestination.x, _RectPlayerDestination.y + _RectPlayerDestination.h + 2});
	}

	// Procédure affichant une description...
	// En entrée:
	// Param1: Le renderer de la fenêtre...
	void ShowDescription(SDL_Renderer* _pRenderer) {
		
		// Si elle s'affiche...
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}

	// Procédure modifiant la position et l'activité de la description.
	// Param1: Si la description est active.
	// Param2: La position de la descrition.
	void UpdateDescription(bool _boShow, SDL_Point _PositionDescription) {
		
		m_boShowDescription = _boShow;
		int uiW,
			uiH;
		m_pLblDescription->GetTextureDimension(0, &uiW, &uiH);
		if (_PositionDescription.x + uiW >= 1366)
			m_pLblDescription->SetRectDestinationX(_PositionDescription.x - uiW);
		else
			m_pLblDescription->SetRectDestinationX(_PositionDescription.x);

		if (_PositionDescription.y + uiH >= 768)
			m_pLblDescription->SetRectDestinationY(_PositionDescription.y - uiH);
		else
			m_pLblDescription->SetRectDestinationY(_PositionDescription.y);
	}

	void DefinirActif(bool _boActif) {
		m_pSpriteJetPack->DefinirActif(_boActif);
		m_boDecoller = _boActif;
	}

	// Accesseurs...

	CSprite* ObtenirSprite(string _strNom) {
		return m_pSpriteJetPack;
	}

	unsigned int ObtenirMunition() {
		return m_pBarreDeCarburant->ObtenirVie() * 100;
	}

	bool IsActive(void) {

		return m_boDecoller;
	}

	CVecteur2D* ObtenirVecteur(void) {

		return m_pVecteur;
	}
};