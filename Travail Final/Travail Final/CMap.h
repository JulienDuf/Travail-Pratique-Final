
// 420-202-RE : Travail final
// Classe qui représente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Fin de code le 9 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
// Modifiée le 13 novembre 2014 par Gabriel Beaudry (gabriel.bdry@gmail.com) :
// Ajout d'une boussole(Vent) et modification de comentaire

#define BLANC32BIT 4294967295
#define TRANSPARENCE32BIT 16777215

class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui représente l'arrière plan de la carte de jeu.
	SDL_Surface* m_pSDLSurfaceMap;					// Pointeur de Surface SDL qui pointe sur la surface qui représente l'avant plan de la carte de jeu.
	SDL_Rect m_RectPositionImages;					// Rect représentant la position de la map dans l'écran.
	CListeDC<CPack*>* m_pPackList;					// Liste des packs présents dans la map.
	CVent* m_pVent;									// Classe qui donne la force et la direction du vent.
	//CComboBox* m_pComboBoxChoixOutils;            // Le ComboBox où le joueur choisi son outils pour son tour.

	CVecteur2D* m_VecteurGravite;					// La gravité de la map.
	int m_iVentMax;									// Le vent max de la map.

public:

	// Constructeur de CMap...
	// Param1: _strEmplacementMap, emplacement de la map dans l'arborescence.
	// Param2: _RectPositionImages, la position de la map dans l'écran.
	// Param3: _pSDLTextureMap, pointe sur la texture qui représente l'avant plan de la carte de jeu.
	// Param4: _pVent, Classe qui donne la force et la direction du vent.
	// Param5: Renderer.
	CMap(string _strEmplacementMap, SDL_Rect _RectPositionImages, CVent* _pVent, SDL_Renderer* _pRenderer) {
		
		// Variables temporaires...
		string strTmp[5];
		char chrTmp[50];
		string strTampo;
		ifstream FichierMap;
		string strEmplacement;
		int iNombreMines;

		m_pVent = _pVent;

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
			m_VecteurGravite = new CVecteur2D(SDL_atof(strTampo.c_str()), 90);
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

		// Affiche le vent
		m_pVent->ShowVent(_pSDLRenderer);

		// Affiche tous les packs...
		m_pPackList->AllerDebut();
		for (int i = 0; i < m_pPackList->ObtenirCompte(); i++) {

			m_pPackList->ObtenirElementCurseur()->ShowPack(_pSDLRenderer);
			m_pPackList->AllerSuivantCurseur();
		}
			
	}

	SDL_Surface* ObtenirSurfaceMap(void) {

		return m_pSDLSurfaceMap;

	}

	SDL_Surface* ObtenirMap(void) {

		return m_pSDLSurfaceMap;
	}

	CVecteur2D* ObtenirGravite(void) {

		return m_VecteurGravite;
	}
};