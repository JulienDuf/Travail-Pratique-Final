
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

	SDL_Rect m_RectPlayerDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectSource;				// Affiche si le joueur est ves le gauche ou vers le droite.

	string m_strName;					// Chaine de caractères qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.

	void(*m_pProcedureCollision)(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision);


public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
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
		m_pSpriteCourse = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 12, 50);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Saut.png");
		m_pSpriteSaut = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 12, 50);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Parachute.png");
		m_pSpriteParachute = new CSprite(IMG_Load(strEmplacementFichier.c_str()), 12, 50);

		m_pSpriteParachute->DefinirActif(true);

		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = m_pSpriteCourse->ObtenirRectSource().w;
		m_RectPlayerDestination.h = m_pSpriteCourse->ObtenirRectSource().h;

		m_pToolList = new CListeDC<CTools*>();

		m_pProcedureCollision = _ProcedureCollision;

		m_pToolList->AjouterFin(new CMissile(_strEmplacementFichier, _pRenderer, _MapDestruction));

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



				m_pSpriteCourse->ModifierAnnimation('D', 0);
				m_pSpriteCourse->DefinirEnMvt(true);
				break;
			case SDL_SCANCODE_LEFT:



				m_pSpriteCourse->ModifierAnnimation('G', 1);
				m_pSpriteCourse->DefinirEnMvt(true);
				break;

			case SDL_SCANCODE_UP:
				if (m_pSpriteCourse->ObtenirDirection() == 'D') // Si la course se fait vers la droite.
					m_pSpriteSaut->ModifierAnnimation('H', 0); // Alors le saut se fait vers la droite. 0 = la premiere étage d'annimation.
				else if (m_pSpriteCourse->ObtenirDirection() == 'G') // Si la course se fait vers la gauche.
					m_pSpriteSaut->ModifierAnnimation('H', 1); // Alors le saut se fait vers la droite. 0 = la premiere étage d'annimation.
				m_pSpriteSaut->DefinirEnMvt(true);
				break;
			}
			break;
		case SDL_KEYUP:
			m_pSpriteSaut->DefinirActif(false);
			m_pSpriteCourse->DefinirEnMvt(false);
			if (m_pSpriteCourse->IsSteady()) {
				if (m_pSpriteCourse->ObtenirDirection() == 'D')
					m_pSpriteCourse->ModifierAnnimation('N', 2);
				else if (m_pSpriteCourse->ObtenirDirection() == 'G')
					m_pSpriteCourse->ModifierAnnimation('N', 3);
			}


			break;

		}

	}


	


};