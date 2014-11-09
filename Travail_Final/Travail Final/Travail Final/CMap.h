
// 420-202-RE : Travail final
// Classe qui repr�sente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Fin de la classe le 9 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

#define BLANC32BIT 4294967295
#define TRANSPARENCE32BIT 16777215

class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	SDL_Surface* m_pSDLSurfaceMap;					// Pointeur de Surface SDL qui pointe sur la surface qui repr�sente l'avant plan de la carte de jeu.
	SDL_Rect m_RectPositionImages;					// Rect repr�sentant la position de la map dans l'�cran.
	CListeDC<CPack*>* m_pPackList;					// Liste des packs pr�sents dans la map.
	//CComboBox* m_pComboBoxChoixOutils;              // Le comboBox ou le joueur choisi son outils pour son tour.

public:

	// Constructeur...
	// Param�tre: _pSDLTextureBackground, pointe sur la texture qui repr�sente l'arri�re plan de la carte de jeu.
	// Param�tre: _pSDLTextureMap, pointe sur la texture qui repr�sente l'avant plan de la carte de jeu.
	// Param�tre: _RectPositionImages, la position de la map dans l'�cran.
	// Param�tre: argc, Indique le nombre de packs que l'utilisateur ajoute � la liste de packs dans le constructeur.
	// Param�tre: ..., Pack que l'utilisateur veut ajouter � la liste de packs (Nombre ind�finis, CPack).
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
	

	// Proc�dure qui d�truit la carte de jeu � l'endroit appropri�.
	// Param�tre: _iRayon, variable enti�re qui contient la longueur du rayon du cercle dans lequel la carte de jeu sera d�truite.
	// Param�tre: _iX, La position en X ou la carte sera d�truite.
	// Param�tre: _iY, La position en Y ou la carte sera d�truite.
	// Retour: Rien.
	void MapDestruction(int _iRayon, int _iX, int _iY) {

		// Variables repr�sentant la position dans la map.
		int iX =_iX - _iRayon;
		int iY = _iY - _iRayon;

		// Surface du gabarie.
		SDL_Surface* pSDLSurfaceGabarie = nullptr;

		// Selon le rayon mis en param�tre...
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
			iX = _iX - 50; // Remet la position en X au d�part.
		}

	}


	// Proc�dure qui affiche la carte de jeu.
	// Param�tre: _pSDLRender, Rendeur de la fen�tre dans laquelle on veut afficher la carte de jeu.
	// Retour: Rien.
	void ShowMap(SDL_Renderer* _pSDLRenderer) {

		// Cr�� une texture de la map.
		SDL_Texture* pSDLTextureMap = SDL_CreateTextureFromSurface(_pSDLRenderer, m_pSDLSurfaceMap);

		// Affiche les maps.
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureBackground, NULL, &m_RectPositionImages);
		SDL_RenderCopy(_pSDLRenderer, pSDLTextureMap, NULL, &m_RectPositionImages);

		//D�truits la texture cr��e
		SDL_DestroyTexture(pSDLTextureMap);

		// Affiche tous les packs...
		m_pPackList->AllerDebut();
		for (int i = 0; i < m_pPackList->ObtenirCompte(); i++) {

			m_pPackList->ObtenirElementCurseur()->ShowPack(_pSDLRenderer);
			m_pPackList->AllerSuivantCurseur();
		}
			
	}





};