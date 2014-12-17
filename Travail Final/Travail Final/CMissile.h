// 420-202-RE : Travail final
// Classe qui représente un missile ayant un angle et une force.
// 16 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
// Ajout d'une description(BlitText, loading de la description, ShowDescriptoin et UpdateDescription)

class CMissile : public CProjectile {

private:

	bool m_boMissileLancer; // Si le missile est lancé
	bool m_boExplosion; // Si il y a une explosion en cours.
	bool m_boShowDescription; // Si on affiche la description
	int m_iAngle; // L'angle de départ du missile.
	unsigned int m_uiRayon; // Rayon d'explosion du missile.
	unsigned int m_uiForce; // Force de départ du missile.
	unsigned int m_uiMunition; // Le nombre de missiles disponible.
	CVecteur2D* m_pVecteurVitesseMissile;

	SDL_Surface* m_pSDLSurfaceMissile; // La surface du missile.
	SDL_Surface* m_pSDLSurfaceMissileRotation; // La surface du missile.
	SDL_Texture* m_pTextureBazooka; // La texture d bazooka.
	SDL_Rect m_RectDestinationMissile; // La destination du missile dans la fenêtre.
	SDL_Rect m_RectDestinationExplosion; // La destination de l'explosion.
	SDL_Rect RayonExplosion;
	SDL_Rect m_RectDestinationBazooka; // La destination du bazooka.
	CBarrePuissance* m_pBarrePuissance; // La barre déterminant l'angle et la puissance du missile.
	CSprite* m_pSpriteExplosion; // L'animation de l'explosion.
	CSprite* m_pSpritePlayer; // Le sprite du player.

	CLabel* m_pLblDescription; // Le label descripton du missile.
	string m_strDescription[8]; // Le texte de la description du missile.

	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	SDL_Surface* (*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle); // Rotation

	// Fonction tranformant un tableau de string en surface
	// En entrée:
	// Param1: La tableau de string
	// Param2: Le nombre d'élément dans le tableau.
	// Param3: La couleur du texte.
	// En sortie:
	// Le texte dnas une surface.
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

	// Procédure mettant à jour le nombre de minution dans le description et dans le tablau de string.
	// En entrée:
	// Param1: Le renderer de la fenêtre où sera affiché les descrptions.
	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		char chr[3];
		SDL_itoa(m_uiMunition, chr, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Munition : ");
		m_strDescription[0].append(chr);
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 8, { 0, 0, 0 })), 0);
	}

public:

	// Constructeur
	// En entrée:
	// Param1: L'emplacement de la description du missile.
	// Param2: Le renderer de la fenêtre.
	// Param3: Le pointeur de fonction sur la procédure qui détruit la map selon un rayon.
	// Param4: LE pointeur de fonction sur la fonction qui rotationne une surface.
	CMissile(string _strEmplacement, SDL_Renderer* _pRenderer, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {

		// Initialisation de variable...
		m_boShowDescription = false;
		m_boMissileLancer = false;
		m_iAngle = 0;
		m_uiForce = 0;
		m_pMapDestruction = _MapDestruction;
		m_pRotation = _Rotation;
		m_pSDLSurfaceMissile = pGestionnaireSurface->ObtenirDonnee("pSurfaceMissile");
		m_pSDLSurfaceMissileRotation = m_pSDLSurfaceMissile;
		m_pTextureBazooka = pGestionnaireTexture->ObtenirDonnee("pTextureBazooka");
		m_RectDestinationMissile = { 0, 0, 0, 0 };
		m_RectDestinationMissile.w = m_pSDLSurfaceMissile->w;
		m_RectDestinationMissile.h = m_pSDLSurfaceMissile->h;
		m_RectDestinationExplosion = { 0, 0, 150, 146 };
		m_RectDestinationBazooka = { 0, 0, 0, 0 };
		SDL_QueryTexture(m_pTextureBazooka, NULL, NULL, &m_RectDestinationBazooka.w, &m_RectDestinationBazooka.h);
		RayonExplosion = { 0, 0, 0, 0 };

		// Création de la description 
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
			string strMunition;
			string strRayon;

			for (int i = 0; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
				switch (i) {
				case 0:

					for (int j = 11; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strMunition += chrtmp[j];
					}
					break;
				case 2:

					for (int j = 20; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strRayon += chrtmp[j];
					}
					break;
				}
			}
			m_uiMunition = SDL_atoi(strMunition.c_str());
			m_uiRayon = SDL_atoi(strRayon.c_str());
		}

		FichierDescription.close();

		// Création du label de description.
		SDL_Surface *pSDLSurface = BlitText(m_strDescription, 8, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface), { 0, 0, pSDLSurface->w, pSDLSurface->h });

		// Création des autres pointeurs.
		m_pBarrePuissance = new CBarrePuissance();
		m_pSpriteExplosion = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceExplosionMissile"), pGestionnaireTexture->ObtenirDonnee("pTextureExplosionMissile"), m_RectDestinationExplosion, 9, 100, false, false, 1);
		m_pSpritePlayer = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfacePlayerBazooka"), pGestionnaireTexture->ObtenirDonnee("pTexturePlayerBazooka"), m_RectDestinationBazooka, 1, 0, true, false, 2);
	}

	// Procédure réagissnat à une collision.
	// Param1: La position en X de la collision.
	// Param2: Le position en Y de la collision.
	bool ReactionExplosion(int iX, int iY) {
	
		// Détruit la map avec la position et la rayon de 50, met le missile inactif.
		m_pMapDestruction(m_uiRayon, iX, iY);
		m_boMissileLancer = false;
		m_boExplosion = true;
		m_RectDestinationExplosion.x = iX - m_RectDestinationExplosion.w / 2;
		m_RectDestinationExplosion.y = iY - m_RectDestinationExplosion.h / 2;
		RayonExplosion = { iX - 50, iY - 50, 100, 100 };
		m_pSpriteExplosion->DefinirActif(true);

		return false;
	}

	// Procédure affichant le missile selon une position.
	// Param1: Le renderer de la fenêtre.
	// Param2: La destination du joueur, pour la barre de puissance.
	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {

		// Si le missile est lancé, on affiche le missile
		if (m_boMissileLancer) {

			SDL_Texture* pTextureTMP = SDL_CreateTextureFromSurface(_pRenderer, m_pSDLSurfaceMissileRotation);
			SDL_RenderCopy(_pRenderer, pTextureTMP, NULL, &m_RectDestinationMissile);
			SDL_DestroyTexture(pTextureTMP);
			SDL_SetRenderDrawColor(_pRenderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(_pRenderer, &m_RectDestinationMissile);
		}

		// Sinon, on affiche la barre de puissance.
		else {

			if (!m_pSpriteExplosion->IsActif()) {

				m_RectDestinationBazooka.x = _RectPlayerDestination.x;
				m_RectDestinationBazooka.y = _RectPlayerDestination.y + (_RectPlayerDestination.h / 2);
				_RectPlayerDestination.x -= 22;
				_RectPlayerDestination.y -= 10;
				m_pBarrePuissance->AfficherBarre(_pRenderer, _RectPlayerDestination);
				m_pSpritePlayer->Render(_pRenderer, _RectPlayerDestination);
				m_pSpritePlayer->ModifierAnnimation();
				SDL_RenderCopyEx(_pRenderer, m_pTextureBazooka, NULL, &m_RectDestinationBazooka, m_pBarrePuissance->ObtenirAngle(), NULL, SDL_FLIP_NONE);
			}

			else {
				m_pSpriteExplosion->Render(_pRenderer, m_RectDestinationExplosion);
				m_pSpriteExplosion->ModifierAnnimation();
				SDL_SetRenderDrawColor(_pRenderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(_pRenderer, &RayonExplosion);
			}
		}
	}

	// Procédure affichant la description du missile.
	// Param1: Le renderer de la fenêtre.
	void ShowDescription(SDL_Renderer* _pRenderer) {
		// Si la description est active
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer); // Met à jour la description avec les minutions.
			m_pLblDescription->ShowControl(_pRenderer); // Affiche.
		}
	}

	// Procédure réagissant au événement dans la fenêtre.
	void ReactToEvent(SDL_Event* _pEvent) {

		// Réaction au événement pour la barre de puissance.
		m_pBarrePuissance->ReactToEvent(_pEvent);

		// Si le missile n'est pas lancé.
		if (!m_boMissileLancer) {

			// Pour tous les types d'événements...
			switch (_pEvent->type) {

			// Si une touche du clavier en enfoncé.
			case SDL_KEYDOWN:

				// Si c'est la space...
				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					// Modifie la force, l'angle et on crée le vecteur vitesse.
					m_iAngle = m_pBarrePuissance->ObtenirAngle();
					m_uiForce = (m_pBarrePuissance->ObtenirForce() + 3) * 100;
					m_pVecteurVitesseMissile = new CVecteur2D((float)m_uiForce, (float)m_iAngle);
					
					// On met le missile actif.
					m_boMissileLancer = true;

					// On modifie la position du missile et le nombre de minution.
					m_pBarrePuissance->ObtenirPosition(&m_RectDestinationMissile.x, &m_RectDestinationMissile.y);
					m_RectDestinationMissile.y -= m_RectDestinationMissile.h;
					m_uiMunition--;
				}
				break;
			}
		}

	}

	// Procédure modifiant la position et l'activité de la description.
	// Param1: Si la description est active.
	//Param2: LA position de la descrition.
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

	void DefinirRotation(int _iVitesseAngulaire) {}

	// Accesseurs...

	CSprite* ObtenirSprite(string _strNom) {
		return m_pSpriteExplosion;
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

	void DefinirExplosion(bool _boExplosion) {

		m_boExplosion = _boExplosion;
	}

	void DestructionProjectile() {

	}

	bool EstLancer() {
		
		return m_boMissileLancer;
	}

	bool ExplosionEnCours(void) {

		return m_boExplosion;
	}

	unsigned int ObtenirMunition() {
		return m_uiMunition;
	}
};