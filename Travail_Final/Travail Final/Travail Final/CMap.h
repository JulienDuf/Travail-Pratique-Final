
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
	SDL_Surface* m_pSurfaceGabarie;					// Surface ou est stocké les gabaries pour la destruction.
	//CComboBox* m_pComboBoxChoixOutils;              // Le comboBox ou le joueur choisi son outils pour son tour.

	int m_iGravite;									// La gravité de la map.
	int m_iVentMax;									// Le vent max de la map.

public:

	// Constructeur...
	// Paramètre: _pSDLTextureMap, pointe sur la texture qui représente l'avant plan de la carte de jeu.
	// Paramètre: _RectPositionImages, la position de la map dans l'écran.
	// Retour: rien (Constructeur).
	CMap(string _strEmplacementMap, SDL_Rect _RectPositionImages, SDL_Surface* _pSurfaceGabarie, SDL_Renderer* _pRenderer) {
		
		// Variables temporaires...
		string strTmp[5];
		char chrTmp[50];
		string strTampo;
		ifstream FichierMap;
		string strEmplacement;
		int iNombreMines;
		
		m_pSurfaceGabarie = _pSurfaceGabarie;

		m_RectPositionImages = _RectPositionImages;
		
		m_pPackList = new CListeDC<CPack*>();

		strEmplacement = _strEmplacementMap;
		strEmplacement.append("background.jpg");
		m_pSDLTextureBackground = IMG_LoadTexture(_pRenderer, strEmplacement.c_str());

		strEmplacement = _strEmplacementMap;
		strEmplacement.append("map.png");
		m_pSDLSurfaceMap = IMG_Load(strEmplacement.c_str());

		// Ouverture du fichier...
		strEmplacement.append("\\map.txt");
		FichierMap.open(strEmplacement);

		m_RectPositionImages.w = m_pSDLSurfaceMap->w;
		m_RectPositionImages.h = m_pSDLSurfaceMap->h;

		if (FichierMap.is_open()) {

			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[21];
			m_iGravite = SDL_atoi(strTampo.c_str());
			FichierMap.getline(chrTmp, 50);

			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[24];
			strTampo += chrTmp[25];
			strTampo += chrTmp[26];
			m_iVentMax = SDL_atoi(strTampo.c_str());
			FichierMap.getline(chrTmp, 50);

			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[21];
			iNombreMines = SDL_atoi(strTampo.c_str());

			FichierMap.close();
		}

		for (int i = iNombreMines; i > 0; i--) {


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

		SDL_Rect RectSource; // Source du gabarie.

		// Selon le rayon mis en paramètre...
		switch (_iRayon) {

		// Si le rayon est de 50 pixels.
		case 45:
			RectSource = { 0, 0, m_pSurfaceGabarie->h, m_pSurfaceGabarie->h };
			break;

		// Si le rayon est de 50 pixels.
		case 50:
			RectSource = { 1 * m_pSurfaceGabarie->h, 0, m_pSurfaceGabarie->h, m_pSurfaceGabarie->h };
			break;

		// Si le rayon est de 60 pixels.
		case 60:
			RectSource = { 2 * m_pSurfaceGabarie->h, 0, m_pSurfaceGabarie->h, m_pSurfaceGabarie->h };
			break;
		}

		// Pour toute la grandeur du gabarie...
		for (int y = 0; y < _iRayon * 2; y++) {
			for (int x = 0; x < _iRayon * 2; x++) {

				// Si le point est dans la map...
				if (iX > 0 && iY > 0) {

					// Si le pixel du gabarie n'est pas en blanc, remplace le pixel de la map en transparent.
					if (((unsigned int*)m_pSurfaceGabarie->pixels)[y * m_pSurfaceGabarie->w + x] != BLANC32BIT)
						((unsigned int*)m_pSDLSurfaceMap->pixels)[iY * m_pSDLSurfaceMap->w + iX] = TRANSPARENCE32BIT;

					iX++; // Augmente la position en X.
				}
			}

			iY++; // Augmente la position en Y.
			iX = _iX - _iRayon; // Remet la position en X au départ.
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

	// Fonction qui retourne la position d'une collision.
	// Paramètre: _pSDLSurfacecollision, pointe vers la surface avec laquelle on veut vérifier les collisions avec la map.
	// Paramètre: _SDLRectCollision, rectangle qui encadre l'endroit ou l'on veut vérifier les collisions sur la map. (ex: rectangle destination d'un sprite.)
	// Paramètre: _SDLRectSource, rectangle qui encadre la position sur la surface ou l'on veut vérifier les collisions (ex : rectangle source d'un sprite.)
	// Paramètre: _uiXMap, position en x dans la map ou la collision a lieu.
	// Paramètre: _uiYMap, position en y dans la map ou la collision a lieu.
	// Paramètre: _uiXRectCollision, Position en x dans le rectangle de collision ou la collision a eu lieu.
	// Paramètre: _uiYRectCollision, position en y dans le rectangle de collision ou la collision a eu lieu.
	// Retour: Rien, mais les positions en x et en y de la collision seront stockés dans les 4 paramètres écrits plus haut.
	void VerifierCollisionMap(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision,SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision) {

		_uiXMap = 0;
		_uiYMap = 0;
		_uiXRectCollision = 0;
		_uiYRectCollision = 0;

		for (int y = _SDLRectCollision.h; y > 0; y--) {				// On parcours les pixels dans le rectangle collision en y à l'envers.

			for (int x = _SDLRectCollision.w; x > 0; x--) {			// On parcours les pixels dans le rectangle collision en x à l'envers.

				if ((((unsigned int*)m_pSDLSurfaceMap->pixels)[_SDLRectCollision.x + x * y] != TRANSPARENCE32BIT) && (((unsigned int*)_pSDLSurfaceCollision->pixels)[_SDLRectSource.x + x * y] != TRANSPARENCE32BIT)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du joueur...

					_uiXMap = _SDLRectCollision.x + x;		// On stocke la position de la collision dans les variables adéquates.
					_uiYMap = _SDLRectCollision.y + y;
					_uiXRectCollision = x;
					_uiYRectCollision = y;

				}

			}

		}


	}


};