
// 420-202-RE : Travail final
// Classe qui représente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
// Modifier le 25 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Finalisation des mouvements.
//Fin des modifications.
class CPlayer {

private:

	bool m_BoDeplacement;				// Si le joueur se déplace.

	CSprite* m_pSpriteCourse;			// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de course.
	CSprite* m_pSpriteSaut;				// pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de saut.
	CSprite* m_pSpriteParachute;		// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de chute.
	CSprite* m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est au repos.

	CBarreVie* m_pBarreVie;				// La barre de vie du personnage.

	SDL_Rect m_RectPlayerDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectParachuteDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectSource;				// Affiche si le joueur est ves le gauche ou vers le droite.
	SDL_Rect m_RectHitboxCorpsGauche;
	SDL_Rect m_RectHitboxCorpsDroite;
	SDL_Rect m_RectHitboxPieds;
	SDL_Rect m_RectHitboxPiedsParachute;

	string m_strName;					// Chaine de caractères qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.

	bool m_boStable;					// Si le joueur est stable

	CVecteur2D* VecteurVitesse;			// Vitesse.

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(string _strEmplacementFichier, SDL_Rect _RectDestination, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_boStable = false;

		string strEmplacementFichier = _strEmplacementFichier;

		strEmplacementFichier.append("Personnage\\Courses.png");
		m_pSpriteCourse = new CSprite(IMG_Load(strEmplacementFichier.c_str()), IMG_LoadTexture(_pRenderer, strEmplacementFichier.c_str()), _RectDestination, 9, 50, true, false, 2);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Saut.png");
		m_pSpriteSaut = new CSprite(IMG_Load(strEmplacementFichier.c_str()), IMG_LoadTexture(_pRenderer, strEmplacementFichier.c_str()), _RectDestination, 9, 50, false, false, 2);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Parachute.png");
		m_pSpriteParachute = new CSprite(IMG_Load(strEmplacementFichier.c_str()), IMG_LoadTexture(_pRenderer, strEmplacementFichier.c_str()), _RectDestination, 24, 10, true, true, 1);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Repo.png");
		m_pSpriteRepos = new CSprite(IMG_Load(strEmplacementFichier.c_str()), IMG_LoadTexture(_pRenderer, strEmplacementFichier.c_str()), _RectDestination, 1, 50, true, false, 2);


		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = m_pSpriteCourse->ObtenirRectSource().w;
		m_RectPlayerDestination.h = m_pSpriteCourse->ObtenirRectSource().h;

		m_RectParachuteDestination = _RectDestination;
		m_RectParachuteDestination.w = m_pSpriteParachute->ObtenirRectSource().w;
		m_RectParachuteDestination.h = m_pSpriteParachute->ObtenirRectSource().h;


		m_RectHitboxCorpsGauche.x = 79;
		m_RectHitboxCorpsGauche.y = 3;
		m_RectHitboxCorpsGauche.w = 52;
		m_RectHitboxCorpsGauche.h = 74;

		m_RectHitboxCorpsDroite.x = 119;
		m_RectHitboxCorpsDroite.y = 3;
		m_RectHitboxCorpsDroite.w = 52;
		m_RectHitboxCorpsDroite.h = 74;

		m_RectHitboxPieds.x = 0;
		m_RectHitboxPieds.y = 40;
		m_RectHitboxPieds.w = 40;
		m_RectHitboxPieds.h = 15;

		m_RectHitboxPiedsParachute.x = 0;
		m_RectHitboxPiedsParachute.y = 92;
		m_RectHitboxPiedsParachute.w = 58;
		m_RectHitboxPiedsParachute.h = 19;

		m_pToolList = new CListeDC<CTools*>();

		m_pToolList->AjouterFin(new CMissile(_strEmplacementFichier, _pRenderer, _MapDestruction, _CollisionObjetMap, _Physique, NULL));

		m_BoDeplacement = false;

		m_pBarreVie = new CBarreVie(_strEmplacementFichier, { _RectDestination.x, _RectDestination.y - 2, 0, 0 }, _pRenderer);

		VecteurVitesse = 0;

	}

	// Procédure qui affiche le joueur.
	// Paramètre: _pSDLRenderer, Rendeur de la fenêtre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ReactToEvent(SDL_Event* _pSDLEvent) {
		


		switch (_pSDLEvent->type) {
		case SDL_KEYDOWN:
			switch (_pSDLEvent->key.keysym.scancode) {
			case SDL_SCANCODE_RIGHT:

				if (!m_pSpriteCourse->IsActif()) {					// S'il était au repos et que la flèche droite est appuyer.
					m_pSpriteCourse->DefinirAnimation(0);
					m_pSpriteCourse->DefinirActif(true);
					m_pSpriteRepos->DefinirAnimation(0);
					m_pSpriteRepos->DefinirActif(false);			// Il n'est plus au repos.
					VecteurVitesse->ModifierVecteur(20, 0);
				}

				
				break;
			case SDL_SCANCODE_LEFT:

				if (!m_pSpriteCourse->IsActif()) {					// S'il était au repos et que la flèche gauche est appuyer.
					m_pSpriteCourse->DefinirAnimation(1);
					m_pSpriteCourse->DefinirActif(true);
					m_pSpriteRepos->DefinirAnimation(1);
					m_pSpriteRepos->DefinirActif(false);			// Il n'est plus au repos.
					VecteurVitesse->ModifierVecteur(20, 180);
				}
				break;

			case SDL_SCANCODE_SPACE:								// Saut = Espace
				m_pSpriteRepos->DefinirActif(false);
				m_pSpriteCourse->DefinirActif(false);
				m_pSpriteSaut->DefinirActif(true);
				VecteurVitesse->ModifierVecteur(20, 90);

				
				break;
			}
			break;
		case SDL_KEYUP:
			VecteurVitesse->ModifierVecteur(0, 0);
			m_boStable = true;
			m_pSpriteCourse->DefinirActif(false);					// Le sprite ne court plus.
			if (!m_pSpriteSaut->IsActif())
				m_pSpriteRepos->DefinirActif(true);

			break;

		}

	}

	// Procédure qui sert à afficher le joueur.
	//Paramètre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {
		m_pSpriteCourse->ModifierAnnimation();
		m_pSpriteCourse->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteParachute->ModifierAnnimation();
		m_pSpriteParachute->Render(_pRenderer, m_RectParachuteDestination);

		m_pSpriteSaut->ModifierAnnimation();
		m_pSpriteSaut->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteRepos->ModifierAnnimation();
		m_pSpriteRepos->Render(_pRenderer, m_RectPlayerDestination);

		if (!m_pSpriteParachute->IsActif())
			m_pBarreVie->ShowBarre(_pRenderer);
	}
															// Accesseur ... 



	void ModifierRectDestination(SDL_Rect _RectDestination) {

		m_RectPlayerDestination = _RectDestination;

		m_pBarreVie->ModifierPositionBarre(m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2);

	}

	void ModifierRectDestinationParachute(SDL_Rect _RectDestination) {

		m_RectParachuteDestination = _RectDestination;
	}
	
	void AjouterAPositionY(int _iY) {

		m_RectPlayerDestination.x += _iY;
	}

	void ModifierStabiliteJoueur(bool _boStable) {

		m_boStable = _boStable;
	}

	CSprite* ObtenirSpriteCourse(void) {

		return m_pSpriteCourse;

	}

	CSprite* ObtenirSpriteSaut(void) {

		return m_pSpriteSaut;

	}

	CSprite* ObtenirSpriteRepos(void) {

		return m_pSpriteRepos;

	}

	CSprite* ObtenirSpriteParachute(void) {

		return m_pSpriteParachute;

	}

	SDL_Rect ObtenirHitboxCorpsDroite(void) {

		return m_RectHitboxCorpsDroite;

	}

	SDL_Rect ObtenirHitboxCorpsGauche(void) {

		return m_RectHitboxCorpsGauche;

	}

	SDL_Rect ObtenirHitboxPieds(void) {

		return m_RectHitboxPieds;

	}

	SDL_Rect ObtenirHitboxPiedsParachute(void) {

		return m_RectHitboxPiedsParachute;

	}

	SDL_Rect ObtenirRectDestination(void) {

		return m_RectPlayerDestination;
	}

	SDL_Rect ObtenirRectDestinationParachute(void) {

		return m_RectParachuteDestination;
	}

	bool IsStable(void) {

		return m_boStable;
	}

	CVecteur2D* ObtenirVecteurVitesse(void) {
		return VecteurVitesse;
	}
};