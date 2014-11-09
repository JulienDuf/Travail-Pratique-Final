
// 420-202-RE : Travail final
// Classe qui représente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Fin de la classe le 9 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

#define BLANC32BIT 4294967295
#define TRANSPARENCE32BIT 16777215

class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui représente l'arrière plan de la carte de jeu.
	SDL_Surface* m_pSDLSurfaceMap;					// Pointeur de Surface SDL qui pointe sur la surface qui représente l'avant plan de la carte de jeu.
	SDL_Rect m_RectPositionImages;					// Rect représentant la position de la map dans l'écran.
	CListeDC<CPack*>* m_pPackList;					// Liste des packs présents dans la map.
	//CComboBox* m_pComboBoxChoixOutils;              // Le comboBox ou le joueur choisi son outils pour son tour.

public:

	// Constructeur...
	// Paramètre: _pSDLTextureBackground, pointe sur la texture qui représente l'arrière plan de la carte de jeu.
	// Paramètre: _pSDLTextureMap, pointe sur la texture qui représente l'avant plan de la carte de jeu.
	// Paramètre: _RectPositionImages, la position de la map dans l'écran.
	// Paramètre: argc, Indique le nombre de packs que l'utilisateur ajoute à la liste de packs dans le constructeur.
	// Paramètre: ..., Pack que l'utilisateur veut ajouter à la liste de packs (Nombre indéfinis, CPack).
	// Retour: rien (Constructeur).
	CMap(SDL_Texture* _pSDLTextureBackground, SDL_Surface* _pSDLSurfaceMap, SDL_Rect _RectPositionImages, int argc, ...) {
		m_pSDLTextureBackground = _pSDLTextureBackground;
		m_pSDLSurfaceMap = _pSDLSurfaceMap;

		m_RectPositionImages = _RectPositionImages;
		
		m_pPackList = new CListeDC<CPack*>();

		if (argc > 0) {

			va_list parametres;
			va_start(parametres, argc);

			for (int i = 0; i < argc; i++) 
				m_pPackList->AjouterFin(va_arg(parametres, CPack*));
			
			va_end(parametres);

			m_pPackList->AllerDebut();
		}

	}
	

	// Procédure qui détruit la carte de jeu à l'endroit approprié.
	// Paramètre: _iRayon, variable entière qui contient la longueur du rayon du cercle dans lequel la carte de jeu sera détruite.
	// Paramètre: _iX, La position en X ou la carte sera détruite.
	// Paramètre: _iY, La position en Y ou la carte sera détruite.
	// Retour: Rien.
	void MapDestruction(int _iRayon, int _iX, int _iY) {

		// Variables représentant la position dans la map.
		int iX =_iX - _iRayon;
		int iY = _iY - _iRayon;

		// Surface du gabarie.
		SDL_Surface* pSDLSurfaceGabarie = nullptr;

		// Selon le rayon mis en paramètre...
		switch (_iRayon) {

		// Si le rayon est de 50 pixels.
		case 50:

			break;
		}

		// Pour toute la grandeur du gabarie...
		for (int y = 0; y < _iRayon * 2; y++) {
			for (int x = 0; x < _iRayon * 2; x++) {

				// Si le point est dans la map...
				if (iX > 0 && iY > 0) {

					// Si le pixel du gabarie n'est pas en blanc, remplace le pixel de la map en transparent.
					if (((unsigned int*)pSDLSurfaceGabarie->pixels)[y * pSDLSurfaceGabarie->w + x] != BLANC32BIT)
						((unsigned int*)m_pSDLSurfaceMap->pixels)[iY * m_pSDLSurfaceMap->w + iX] = TRANSPARENCE32BIT;

					iX++; // Augmente la position en X.
				}
			}

			iY++; // Augmente la position en Y.
			iX = _iX - 50; // Remet la position en X au départ.
		}

	}


	// Procédure qui affiche la carte de jeu.
	// Paramètre: _pSDLRender, Rendeur de la fenêtre dans laquelle on veut afficher la carte de jeu.
	// Retour: Rien.
	void ShowMap(SDL_Renderer* _pSDLRenderer) {

		// Créé une texture de la map.
		SDL_Texture* pSDLTextureMap = SDL_CreateTextureFromSurface(_pSDLRenderer, m_pSDLSurfaceMap);

		// Affiche les maps.
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureBackground, NULL, &m_RectPositionImages);
		SDL_RenderCopy(_pSDLRenderer, pSDLTextureMap, NULL, &m_RectPositionImages);

		//Détruits la texture créée
		SDL_DestroyTexture(pSDLTextureMap);

		// Affiche tous les packs...
		m_pPackList->AllerDebut();
		for (int i = 0; i < m_pPackList->ObtenirCompte(); i++) {

			m_pPackList->ObtenirElementCurseur()->ShowPack(_pSDLRenderer);
			m_pPackList->AllerSuivantCurseur();
		}
			
	}





};