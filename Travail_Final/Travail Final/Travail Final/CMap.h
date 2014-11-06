
// 420-202-RE : Travail final
// Classe qui repr�sente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	SDL_Texture* m_pSDLTextureMap;					// Pointeur de texture SDL qui pointe sur la texture qui repr�sente l'avant plan de la carte de jeu.

	CListeDC<CPlayer*>* m_pPlayerList;				// Pointeur de liste diublement cha�n�e qui pointe sur la liste de joueurs.

public:

	// Constructeur...
	// Param�tre: _pSDLTextureBackground, pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	// Param�tre: _pSDLTextureMap, pointe sur la texture qui repr�sente l'avant plan de la carte de jeu.
	// Param�tre: argc, Indique le nombre de joueur que l'utilisateur ajoute � la liste de joueurs dans le constructeur.
	// Param�tre: ..., Joueurs que l'utilisateur veut ajouter � la liste de joueurs (Nombre ind�finis, CPlayer).
	// Retour: rien (Constructeur).
	CMap(SDL_Texture* _pSDLTextureBackground, SDL_Texture* _pSDLTextureMap, int argc, ...) {

		m_pSDLTextureBackground = _pSDLTextureBackground;
		m_pSDLTextureMap = _pSDLTextureMap;

		m_pPlayerList = new CListeDC<CPlayer*>();

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