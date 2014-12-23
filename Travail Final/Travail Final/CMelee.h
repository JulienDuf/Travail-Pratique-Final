/*
Classe qui gere les armes de melee
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CMelee : public CProjectile {
private:

	float m_fDommage; //Degats de l'arme de melee
	unsigned int m_uiRayon; // Rayon de dégat de l'attaque.
	CSprite* m_pSprite; //sprite du personnage qui utilise l'arme de melee
	SDL_Rect m_RectDestinationMelee; // La destination de l'arme de melee.

	CLabel* m_pLblDescription; // La descripton du missile.
	string m_strDescription[8]; // La description du melee
	bool m_boShowDescription; // Si on affiche la description...
	bool m_boSpace; // Si la barre espace est enfoncé...
	bool m_boFinUtilisation; // Si la mélée est en fon d'utilisation...
	
	SDL_Surface* BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 235, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
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

public:

	// Constructeur de CMelee
	CMelee(string _strEmplacement, CSprite* _pSprite, SDL_Renderer* _pRenderer) {

		m_boShowDescription = false;
		m_boFinUtilisation = false;

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

		m_RectDestinationMelee = { 0, 0, m_pSprite->ObtenirRectSource().w, m_pSprite->ObtenirRectSource().h };
	}

	// Procédure réagissant aux événements reliés à l'arme de mélée
	// En entrée:
	// Param1: Les gestionnaires des événements...
	void ReactToEvent(SDL_Event* _pEvent) { 
		
		// Le sprite jetpack devient actif...
		m_pSprite->DefinirActif(true);
   		
		// Si la barre d'espace est enfoncé et qu'elle ne l'a pas été avant et qu'il n'est pas en fin d'utilisation...
		if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE && !m_boSpace && !m_boFinUtilisation) {
			// Initialisation des variables...
			m_boSpace = true;
			m_pSprite->DefinirboBoucle(false);
			m_pSprite->DefinirPositionDeBouclage(0, 30);
		}
	}

	// Procédure affichant le tool...
	// En entrée:
	// Param1: Le renderer de la fenêtre...
	// Param2: La destination du joueur...
	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		
		// Si le sprite est fini et que l'arme n'est pas en fin d'utilisation...
		if (m_pSprite->ModifierAnnimation() && !m_boFinUtilisation) {
			m_pSprite->DefinirPositionDeBouclage(0, 1);
			m_pSprite->DefinirActif(true);
		}
		// Réinitialisation de la position de l'arme...
		m_RectDestinationMelee.x = _RectPlayerDestination.x;
		m_RectDestinationMelee.y = _RectPlayerDestination.y - 5;

		// Petite modification si l'arme est vers la gauche...
		if (m_pSprite->ObtenirEtage() == 1)
			m_RectDestinationMelee.x -= 25;

		// Affiche de sprite...
		m_pSprite->Render(_pRenderer, m_RectDestinationMelee);
	}

	// Procédure affichant la description du mélée...
	// En entrée:
	// Param1: Le renderer de la fenêtre...
	void ShowDescription(SDL_Renderer* _pRenderer) {
		
		// Si la description doit s'afficher...
		if (m_boShowDescription)
			m_pLblDescription->ShowControl(_pRenderer);
	}

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

	// Fonction réagissant à une fin d'utilisation...
	// En entrée:
	// Variables inutiles pour cette classe...
	bool ReactionExplosion(int iX, int iY) {

		m_boSpace = false;
		m_boFinUtilisation = true;

		return false;
	}
	
	void DefinirActif(bool _boActif) {

		m_pSprite->DefinirActif(_boActif);
		m_boSpace = false;
	}
	
	void DefinirExplosion(bool _boExplosion) {

		m_boFinUtilisation = _boExplosion;

	}

	bool ExplosionEnCours() {

		return m_boFinUtilisation;
	}

	// Accesseurs...

	unsigned int ObtenirMunition() {
		return 1000;
	}

	CSprite* ObtenirSprite(string _strNom) {

		return m_pSprite;
	}

	unsigned int ObtenirRayonDommage() {

		return m_uiRayon;
	}

	bool EstLancer() {

		return m_boSpace;
	}

	SDL_Rect* ObtenirRectDestination() {

		return &m_RectDestinationMelee;
	}

	float ObtenirDommage(void) {

		return m_fDommage;
	}
	
	
	// Méthodes inutiles pour cette classe.......

	CVecteur2D* ObtenirVecteurVitesse(void) {

		return nullptr;
	}

	void DefinirRotation(int _iVitesseAngulaire) {

	}

	void ReinitialisationProjectile(void) {

	}

	SDL_Surface* ObtenirSurface() {

		return nullptr;
	}

	void DefinirAngle(double _dAngle) {

	}

	void DefinirPosition(SDL_Rect _Rect) {

	}

};