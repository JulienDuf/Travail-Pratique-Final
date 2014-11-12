
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
	SDL_Surface* m_pSurfaceGabarie;					// Surface ou est stock� les gabaries pour la destruction.
	//CComboBox* m_pComboBoxChoixOutils;              // Le comboBox ou le joueur choisi son outils pour son tour.

	int m_iGravite;									// La gravit� de la map.
	int m_iVentMax;									// Le vent max de la map.

public:

	// Constructeur...
	// Param�tre: _pSDLTextureMap, pointe sur la texture qui repr�sente l'avant plan de la carte de jeu.
	// Param�tre: _RectPositionImages, la position de la map dans l'�cran.
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
		strEmplacement.append("background");
		m_pSDLTextureBackground = IMG_LoadTexture(_pRenderer, strEmplacement.c_str());

		strEmplacement = _strEmplacementMap;
		strEmplacement.append("map");
		m_pSDLSurfaceMap = IMG_Load(strEmplacement.c_str());

		// Ouverture du fichier...
		strEmplacement.append("\\map.txt");
		FichierMap.open(strEmplacement);

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
	

	// Proc�dure qui d�truit la carte de jeu � l'endroit appropri�.
	// Param�tre: _iRayon, variable enti�re qui contient la longueur du rayon du cercle dans lequel la carte de jeu sera d�truite.
	// Param�tre: _iX, La position en X ou la carte sera d�truite.
	// Param�tre: _iY, La position en Y ou la carte sera d�truite.
	// Retour: Rien.
	void MapDestruction(int _iRayon, int _iX, int _iY) {

		// Variables repr�sentant la position dans la map.
		int iX =_iX - _iRayon;
		int iY = _iY - _iRayon;

		SDL_Rect RectSource; // Source du gabarie.

		// Selon le rayon mis en param�tre...
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
			iX = _iX - _iRayon; // Remet la position en X au d�part.
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