
// 420-202-RE : Travail final
// Classe qui repr�sente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CPlayer {

private:

	bool m_BoDeplacement;				// Si le joueur se d�place.

	CSprite* m_pSpriteCourse;			// Pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est en �tat de course.
	CSprite* m_pSpriteSaut;				// pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est en �tat de saut.
	CSprite* m_pSpriteParachute;		// Pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est en �tat de chute.
	CSprite* m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est au repos.

	SDL_Rect m_RectPlayerDestination;   // La destination du joueur dans le fen�tre.
	SDL_Rect m_RectSource;				// Affiche si le joueur est ves le gauche ou vers le droite.

	string m_strName;					// Chaine de caract�res qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.

	void(*m_pProcedureCollision)(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision);


public:

	// Constructeur...
	// Param�tre: _pSpriteCourse, pointe sur le sprite qui repr�sente le joueur qui est en �tat de course.
	// Param�tre: _pSpriteSaut, pointe sur le sprite qui repr�sente le joueur qui est en �tat de saut.
	// Param�tre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Param�tre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(CSprite* _pSpriteCourse, CSprite* _pSpriteSaut, CListeDC<CTools*>* _pToolList, string _strName, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision)) {

		m_pSpriteCourse = _pSpriteCourse;
		m_pSpriteSaut = _pSpriteSaut;

		m_strName = _strName;

		m_pToolList = _pToolList;

		m_pProcedureCollision = _ProcedureCollision;

	}

	CPlayer(string _strEmplacementFichier, SDL_Rect _RectDestination, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Renderer* _pRenderer) {

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

		m_pToolList = new CListeDC<CTools*>();

		m_pProcedureCollision = _ProcedureCollision;

		m_pToolList->AjouterFin(new CMissile(_strEmplacementFichier, _pRenderer, _MapDestruction));

		m_BoDeplacement = false;

	}

	// Proc�dure qui affiche le joueur.
	// Param�tre: _pSDLRenderer, Rendeur de la fen�tre dans laquelle on veut afficher le joueur.
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
	// Proc�dure qui sert � afficher le joueur.
	//Param�tre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {
		m_pSpriteCourse->Render(_pRenderer);
		if (m_pSpriteParachute->IsActif())
			m_pSpriteParachute->ModifierAnnimation();
		m_pSpriteParachute->Render(_pRenderer);
		m_pSpriteSaut->Render(_pRenderer);
	}


	


};