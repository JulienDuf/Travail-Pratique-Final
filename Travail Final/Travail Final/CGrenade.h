/*
Classe qui gere les grenades...
Crée par Samuel Rambaud le 25 novembre 2014
Modifiée par Gabriel Beaudry
-Ajout de la description(BlitText, ModifierMunition, Load de la description, ShowDescription)
-Ajout du ReactToEvent et ShowProjectile (Celles que Julien avait fait dans CMissile, modifiées)
-Ajout des méthodes et accesseurs:
DefinirActif
ObtenirSprite
ObtenirMunition
UpdateDescription
ObtenirVecteurVitesse
ObtenirRectDestination
ObtenirSurface
DefinirAngle
DefinirPosition
EstLancer
*/

class CGrenade: public CProjectile {
private:

	bool m_boGrenadeLancer, // Lancée = true
		m_boShowDescription,
		m_boExplosion; // Explosion en cours = true

	int m_iAngle, // Angle du lancement de la grenade
		m_iVitesseRotationAngulaire; // Vitesse de rotation de la grenade en degré/sec.

	unsigned int m_uiForce, // Force du lancement de la grenade
		m_uiMunition,
		m_uiMunitionPrecedente, // Munition precedente, afin de ne pas refaire la texture du label de description à chaque affichage.
		m_uiRayon; // Rayon d'explosion de la grenade.

	CBarrePuissance *m_pBarrePuissance;
	CVecteur2D *m_pVecteurVitesseGrenade;

	CSprite* m_pSpriteExplosion;

	CTimer *m_pTimerExplosion,
		*m_pTimerRotation;

	SDL_Texture *m_pTextureGrenade; //Texture de la grenade
	SDL_Surface *m_pSurfaceGrenade, // Surface de base
		*m_pSurfaceGrenadeRotation; // Surface d'affichage
	SDL_Rect m_RectDestinationGrenade; 

	// Pointeurs de fonciton
	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	SDL_Surface *(*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle); // Rotation de la grenade.

	// La descripton du missile.
	CLabel *m_pLblDescription; 
	string m_strDescription[8];

	// Fonction qui permet de blit du texte dans une Surface de couleur
	// Param1; Tableau de string.
	// Param2: Nombre d'élément dans le tableau.
	// Param3: Couleur de la surface dans laquelle on blit le texte.
	// Retourne la surface finie.
	SDL_Surface *BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

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

	// Procédure permettant de mettre à jour le nombre de munition dans le label de description.
	// Param1; Renderer pour mettre à jour la surface.
	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		char chr[10];
		SDL_itoa(m_uiMunition, chr, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Munition : ");
		m_strDescription[0].append(chr);
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 7, { 0, 0, 0 })), 0);
	}

public:

	// Constructeur de CGrenade
	// Param1; Emplacement dans le chemin d'arborescence.
	// Param2: Render pour créer la texture du label.
	// Param3: Procédure de la destruction de la map.
	// Param4: Fonction pour la rotation de la grenade.
	CGrenade(string _strEmplacement, SDL_Renderer* _pRenderer, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)){

		m_boGrenadeLancer = false;
		m_boShowDescription = false;
		m_boExplosion = false;

		// Initialisation de la description de l'arme et des stats en liens avec celle-ci...
		m_strDescription;
		string strEmplacement(_strEmplacement);
		int i = strEmplacement.length();
		
		// Retour à debug...
		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionGrenade.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);

		// Si le fichier s'est ouvert...
		if (FichierDescription.is_open()) {
			char chrtmp[55];
			string strMunition;
			string strRayon;
			string strDelai;
			// Boucle sur toutes les lignes du fichier texte(jusqu'à 8)...
			for (int i = 0; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
				// Switch pour initialiser les stats propres à l'arme...
				switch (i) {
				case 0: // Munition

					for (int j = 11; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strMunition += chrtmp[j];
					}
					break;
				case 2: // Rayon d'explosion

					for (int j = 20; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strRayon += chrtmp[j];
					}
					break;
				case 4: // Délai d'explosion(en seconde ds le fichier texte)

					for (int j = 20; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
						strDelai += chrtmp[j];
					}
					break;
				}
			}
			// Initialisation des stats...
			m_uiMunition = SDL_atoi(strMunition.c_str());
			m_uiMunitionPrecedente = m_uiMunition;
			m_uiRayon = SDL_atoi(strRayon.c_str());
			m_pTimerExplosion = new CTimer(SDL_atoi(strDelai.c_str()) * 1000);
		}

		FichierDescription.close();

		m_pTimerRotation = new CTimer(50);

		SDL_Surface* pSurfacetmp = BlitText(m_strDescription, 7, { 0, 0, 0 });
		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, pSurfacetmp), { 0, 0, 0, 0 });
		SDL_FreeSurface(pSurfacetmp);

		m_pSurfaceGrenadeRotation = m_pSurfaceGrenade = pGestionnaireSurface->ObtenirDonnee("pSurfaceGrenade");

		m_pTextureGrenade = SDL_CreateTextureFromSurface(_pRenderer, m_pSurfaceGrenade);

		m_RectDestinationGrenade = { 0, 0, m_pSurfaceGrenade->w, m_pSurfaceGrenade->h };

		m_pBarrePuissance = new CBarrePuissance();

		m_pMapDestruction = _MapDestruction;
		m_pRotation = _Rotation;

		m_pSpriteExplosion = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceExplosionGrenade"), pGestionnaireTexture->ObtenirDonnee("pTextureExplosionGrenade"), { 0, 0, m_uiRayon, m_uiRayon }, 10, 50, false, false, 1);
	}
	
	// Fontion qui "trigger" l'explosion...
	// En entrée: Aucun, Si grenade, explosion à partir de son centre.
	// Retour: Si l'explosion a eu lieu.
	bool ReactionExplosion(int iX, int iY) {

		// Timer fini...
		if (m_pTimerExplosion->IsDone()) {
			m_boExplosion = true;
			m_pSpriteExplosion->DefinirActif(true);
			m_boGrenadeLancer = false;
			// Initialisation position d'explosion...
			iX = m_RectDestinationGrenade.x + m_RectDestinationGrenade.w / 2;
			iY = m_RectDestinationGrenade.y + m_RectDestinationGrenade.h / 2;
			m_pMapDestruction(m_uiRayon, iX, iY);
			m_RectDestinationGrenade = { iX - m_uiRayon, iY - m_uiRayon, 2*m_uiRayon, 2*m_uiRayon }; // Utilisation du rect pour les dommages
			delete m_pVecteurVitesseGrenade;
			m_pVecteurVitesseGrenade = nullptr;
			m_uiMunition--;
			return true;
		}
		return false;
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
	
		if (m_boGrenadeLancer) {

			if (m_pTimerRotation->IsDone()) {

				m_iAngle += (float)m_iVitesseRotationAngulaire * m_pTimerRotation->ObtenirTimer();
				m_iAngle %= 360;
				m_pSurfaceGrenadeRotation = m_pRotation(m_pSurfaceGrenade, m_iAngle);
				m_pTextureGrenade = SDL_CreateTextureFromSurface(_pRenderer, m_pSurfaceGrenadeRotation);
				m_pTimerRotation->Start();
			}

			SDL_RenderCopy(_pRenderer, m_pTextureGrenade, NULL, &m_RectDestinationGrenade);
			SDL_RenderDrawRect(_pRenderer, &m_RectDestinationGrenade);
		}
		else if (m_boExplosion) {

			m_pSpriteExplosion->Render(_pRenderer, m_RectDestinationGrenade);
			if (m_pSpriteExplosion->ModifierAnnimation()) {

				m_pSpriteExplosion->DefinirActif(false);
				m_RectDestinationGrenade.w = m_pSurfaceGrenade->w;
				m_RectDestinationGrenade.h = m_pSurfaceGrenade->h;
			}
		}
		else {
			m_RectDestinationGrenade = { _RectPlayerDestination.x + _RectPlayerDestination.w / 2 - m_RectDestinationGrenade.w / 2, _RectPlayerDestination.y - 10, m_pSurfaceGrenade->w, m_pSurfaceGrenade->h };
			SDL_RenderCopyEx(_pRenderer, m_pTextureGrenade, nullptr, &m_RectDestinationGrenade, m_pBarrePuissance->ObtenirAngle(), NULL, SDL_FLIP_NONE);
			// Positionnement de la barre de puissance avec rotation autour de la tête...
			m_pBarrePuissance->AfficherBarre(_pRenderer, { _RectPlayerDestination.x + 32, _RectPlayerDestination.y - 2, -12, NULL });
		}
	}

	// Procédure qui permet d'afficher la description de l'arme...
	void ShowDescription(SDL_Renderer* _pRenderer) {
		// Si l'affichage est à true(souris sur l'arme)...
		if (m_boShowDescription) {
			// Réinitialisation de la texture de la description s'il y a eu un changement dans le nombre de munitions...
			if (m_uiMunitionPrecedente != m_uiMunition) {
				MiseajourMunition(_pRenderer);
				m_uiMunitionPrecedente--;
			}
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}

	// Procédure des événements liés à la grenade...
	void ReactToEvent(SDL_Event* _pEvent) {
		
		// événements barre de puissance...
		m_pBarrePuissance->ReactToEvent(_pEvent);

		if (!m_boGrenadeLancer && !m_boExplosion) {

			switch (_pEvent->type) {

			case SDL_KEYDOWN: // Touche enfoncée...

				// Barre d'espace...
				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					// Initialisation de la grenade pour le lancement...
					m_iAngle = m_pBarrePuissance->ObtenirAngle();
					m_uiForce = (m_pBarrePuissance->ObtenirForce() + 3) * 55;
					m_boGrenadeLancer = true;
					m_pVecteurVitesseGrenade = new CVecteur2D((float)m_uiForce, (float)m_iAngle);

					// Signe de la vitesse angulaire...
					if (m_iAngle <= 90 || m_iAngle >= 270) {
						m_RectDestinationGrenade.x += 5;
						m_iVitesseRotationAngulaire = m_uiForce;
					}
					else {
						m_RectDestinationGrenade.x -= 5;
						m_iVitesseRotationAngulaire = -(int)m_uiForce;
					}
					m_iAngle = 0;
					m_pBarrePuissance->Reinitialisation();
					m_pTimerExplosion->Start();
					m_pTimerRotation->Start();
				}
				break;
			}
		}
	}

	// Procédure permettant de mettre à jour l'affichage de la description...
	// En entrée:
	// Param1: _boShow->true: la description affiche
	// Param2: Nouvelle position de la description, ne tient compte que ne x et y
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

	// Positif = vers la gauche...
	void DefinirRotation(int _iVitesseAngulaire) {

		m_iVitesseRotationAngulaire = _iVitesseAngulaire;
	}

	CSprite* ObtenirSprite(string _strNom) {

		return m_pSpriteExplosion;
	}

	unsigned int ObtenirMunition() {

		return m_uiMunition; 
	}

	void ReinitialisationProjectile(void) {
		m_boGrenadeLancer = false;
		m_boExplosion = false;
		delete m_pVecteurVitesseGrenade;
		m_pVecteurVitesseGrenade = nullptr;
		m_pSpriteExplosion->DefinirActif(false);
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

	void DefinirExplosion(bool _boExplosion) {
		
		m_boExplosion = _boExplosion;
	}

	bool EstLancer() {

		return m_boGrenadeLancer;
	}

	bool ExplosionEnCours(void) {

		return m_boExplosion;
	}

	unsigned int ObtenirRayonDommage() {
		return m_uiRayon;
	}

	float ObtenirDommage(void) {
		return 0;
	}
};