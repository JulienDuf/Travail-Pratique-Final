
// 420-202-RE : Travail final
// Classe qui repr�sente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	SDL_Surface* m_pSDLSurfaceMap;					// Pointeur de Surface SDL qui pointe sur la surface qui repr�sente l'avant plan de la carte de jeu.

	CArbreAVL<CControl*>* m_pArbreControl;

public:

	// Constructeur...
	// Param�tre: _pSDLTextureBackground, pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	// Param�tre: _pSDLTextureMap, pointe sur la texture qui repr�sente l'avant plan de la carte de jeu.
	// Param�tre: argc, Indique le nombre de joueur que l'utilisateur ajoute � la liste de joueurs dans le constructeur.
	// Param�tre: ..., Joueurs que l'utilisateur veut ajouter � la liste de joueurs (Nombre ind�finis, CPlayer).
	// Retour: rien (Constructeur).
	CMap(SDL_Texture* _pSDLTextureBackground, SDL_Surface* _pSDLSurfaceMap, int argc, ...) {
		CPlayer* pPlayerTmp;
		m_pSDLTextureBackground = _pSDLTextureBackground;
		m_pSDLSurfaceMap = _pSDLSurfaceMap;

		m_pArbreControl = new CArbreAVL<CControl*>();

		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			m_pPlayerList->AllerDebut();
			for (int i = 0; i < argc; i++) {
				pPlayerTmp = va_arg(parametres, CPlayer*);
				
				m_pPlayerList->AjouterFin(pPlayerTmp);
				m_pPlayerList->AllerSuivantCurseur();
			}
			va_end(parametres);
		}

	}
	

	// Proc�dure qui d�truit la carte de jeu � l'endroit appropri�.
	// Param�tre: _iRayon, variable enti�re qui contient la longueur du rayon du cercle dans lequel la carte de jeu sera d�truite.
	// Param�tre: _SDLRectPosition, Rectangle SDL qui indique l'endroit dans lequel la carte de jeu sera d�truite selon le rayon.
	// Retour: Rien.
	void MapDestruction(int _iRayon, SDL_Rect _SDLRectPosition) {




	}


	// Proc�dure qui affiche la carte de jeu.
	// Param�tre: _pSDLRender, Rendeur de la fen�tre dans laquelle on veut afficher la carte de jeu.
	// Retour: Rien.
	void ShowMap(SDL_Renderer* _pSDLRenderer) {




	}





};