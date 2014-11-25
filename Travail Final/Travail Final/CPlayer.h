
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

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(string _strEmplacementFichier, SDL_Rect _RectDestination, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {


		string strEmplacementFichier = _strEmplacementFichier;

		strEmplacementFichier.append("Personnage\\Course.png");
		m_pSpriteCourse = new CSprite(IMG_Load(strEmplacementFichier.c_str()), _RectDestination, 9, 50, true, false, 2);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Saut.png");
		m_pSpriteSaut = new CSprite(IMG_Load(strEmplacementFichier.c_str()), _RectDestination, 9, 50, false, false, 2);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Parachute.png");
		m_pSpriteParachute = new CSprite(IMG_Load(strEmplacementFichier.c_str()), _RectDestination, 24, 10, true, true, 1);

		strEmplacementFichier = _strEmplacementFichier;
		strEmplacementFichier.append("Personnage\\Repos.png");
		m_pSpriteRepos = new CSprite(IMG_Load(strEmplacementFichier.c_str()), _RectDestination, 1, 50, true, false, 2);


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

		m_RectHitboxPieds.x = 0;
		m_RectHitboxPieds.y = 92;
		m_RectHitboxPieds.w = 58;
		m_RectHitboxPieds.h = 19;

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
					m_pSpriteRepos->DefinirAnimation(0);
					m_pSpriteRepos->DefinirActif(false);
				}

				
				break;
			case SDL_SCANCODE_LEFT:

				if (!m_pSpriteCourse->IsActif()) {
					m_pSpriteCourse->DefinirAnimation(1);
					m_pSpriteCourse->DefinirActif(true);
					m_pSpriteRepos->DefinirAnimation(1);
					m_pSpriteRepos->DefinirActif(false);
				}
				break;

			case SDL_SCANCODE_SPACE:
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
		m_pSpriteCourse->ModifierAnnimation();
		m_pSpriteCourse->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteParachute->ModifierAnnimation();
		m_pSpriteParachute->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteSaut->ModifierAnnimation();
		m_pSpriteSaut->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteRepos->ModifierAnnimation();
		m_pSpriteRepos->Render(_pRenderer, m_RectPlayerDestination);
	}

	void ModifierRectDestination(SDL_Rect _RectDestination) {

		m_RectPlayerDestination = _RectDestination;
	}
	
	void AjouterAPositionY(int _iY) {

		m_RectPlayerDestination.x += _iY;
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

	SDL_Rect ObtenirRectDestination(void) {

		return m_RectPlayerDestination;
	}

};