
// 420-202-RE : Travail final
// Classe qui représente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CPlayer {

private:

	bool m_BoDeplacement;				// Si le joueur se déplace.

	CSprite* m_pSpriteCourse;			// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de course.
	CSprite* m_pSpriteSaut;				// pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de saut.
	CSprite* m_pSpriteParachute;		// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de chute.
	CSprite* m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est au repos.

	SDL_Rect m_RectPlayerDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectSource;				// Affiche si le joueur est ves le gauche ou vers le droite.
	SDL_Rect m_RectHitboxCorpsGauche;
	SDL_Rect m_RectHitboxCorpsDroite;
	SDL_Rect m_RectHitboxPieds;

	string m_strName;					// Chaine de caractères qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.

<<<<<<< HEAD
=======
	void(*m_pProcedureCollision)(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision);
	
>>>>>>> origin/Branche-Julien

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(CSprite* _pSpriteCourse, CSprite* _pSpriteSaut, CListeDC<CTools*>* _pToolList, string _strName, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision)) {

		m_pSpriteCourse = _pSpriteCourse;
		m_pSpriteSaut = _pSpriteSaut;

		m_strName = _strName;

		m_pToolList = _pToolList;

	}

<<<<<<< HEAD
	CPlayer(string _strEmplacementFichier, SDL_Rect _RectDestination, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Renderer* _pRenderer) {
=======
	CPlayer(string _strEmplacementFichier, SDL_Rect _RectDestiantion, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {
>>>>>>> origin/Branche-Julien

		string strEmplacementFichier = _strEmplacementFichier;

		strEmplacementFichier.append("Personnage\\Course.png");
		m_pSpriteCourse = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 9, 50, true, false);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Saut.png");
		m_pSpriteSaut = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 9, 50, false, false);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Parachute.png");
		m_pSpriteParachute = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 9, 50, true, true);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Repos.png");
		m_pSpriteRepos = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 1, 50, true, false);


		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = m_pSpriteCourse->ObtenirRectSource().w;
		m_RectPlayerDestination.h = m_pSpriteCourse->ObtenirRectSource().h;


		m_RectHitboxCorpsGauche.x = 79;
		m_RectHitboxCorpsGauche.y = 3;
		m_RectHitboxCorpsGauche.w = 52;
		m_RectHitboxCorpsGauche.h = 74;

		m_RectHitboxCorpsDroite.x = 119;
		m_RectHitboxCorpsDroite.y = 3;
		m_RectHitboxCorpsDroite.w = 52;
		m_RectHitboxCorpsDroite.h = 74;

		m_RectHitboxPieds.x = 80;
		m_RectHitboxPieds.y = 78;
		m_RectHitboxPieds.w = 95;
		m_RectHitboxPieds.h = 84;

		m_pToolList = new CListeDC<CTools*>();

		m_pToolList->AjouterFin(new CMissile(_strEmplacementFichier, _pRenderer, _MapDestruction, _CollisionObjetMap, _Physique, NULL));

		m_BoDeplacement = false;

	}

	// Procédure qui affiche le joueur.
	// Paramètre: _pSDLRenderer, Rendeur de la fenêtre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ReactToEvent(SDL_Event* _pSDLEvent) {
		


		switch (_pSDLEvent->type) {
		case SDL_KEYDOWN:
			switch (_pSDLEvent->key.keysym.scancode) {
			case SDL_SCANCODE_RIGHT:

				if (!m_pSpriteCourse->IsActif()) {
					m_pSpriteCourse->DefinirAnimation(0);
					m_pSpriteCourse->DefinirActif(true);
				}
				
				break;
			case SDL_SCANCODE_LEFT:

				if (!m_pSpriteCourse->IsActif()) {
					m_pSpriteCourse->DefinirAnimation(1);
					m_pSpriteCourse->DefinirActif(true);
				}
				break;

			case SDL_SCANCODE_UP:
				m_pSpriteRepos->DefinirActif(false);
				m_pSpriteCourse->DefinirActif(false);
				m_pSpriteSaut->DefinirActif(true);

				
				break;
			}
			break;
		case SDL_KEYUP:
			m_pSpriteCourse->DefinirActif(false);
			if (!m_pSpriteSaut->IsActif())
				m_pSpriteRepos->DefinirActif(true);

			break;

		}

	}
	// Procédure qui sert à afficher le joueur.
	//Paramètre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {
		m_pSpriteCourse->Render(_pRenderer);
		if (m_pSpriteParachute->IsActif())
			m_pSpriteParachute->ModifierAnnimation();
		m_pSpriteParachute->Render(_pRenderer);
		m_pSpriteSaut->Render(_pRenderer);
	}


	

	CSprite* ObtenirSpriteCourse(void) {

		return m_pSpriteCourse;

	}

	CSprite* ObtenirSpriteSaut(void) {

		return m_pSpriteSaut;

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


};