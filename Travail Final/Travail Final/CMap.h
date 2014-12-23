
// 420-202-RE : Travail final
// Classe qui représente une carte du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Fin de code le 9 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
// Modifiée le 13 novembre 2014 par Gabriel Beaudry (gabriel.bdry@gmail.com) :
// Ajout d'une boussole(Vent) et modification de comentaire
// Modifiée le 29 novembre par Gabriel Beaudry (gabriel.bdry@gmail.com)
// Ajout de la procédure NouveauVent() et réglage de problèmes lors de la lecture du fichier texte

#define BLANC32BIT 4294967295
#define TRANSPARENCE32BIT 16777215

class CMap {

private:

	SDL_Texture* m_pSDLTextureBackground;			// Pointeur de texture SDL qui pointe sur la texture qui représente l'arrière plan de la carte de jeu.
	SDL_Texture* m_pSDLTextureMap;
	SDL_Surface* m_pSDLSurfaceMap;					// Pointeur de Surface SDL qui pointe sur la surface qui représente l'avant plan de la carte de jeu.
	SDL_Rect m_RectPositionImages;					// Rect représentant la position de la map dans l'écran.
	CListeDC<CPack*>* m_pPackList;					// Liste des packs présents dans la map.
	CVent* m_pVent;									// Classe qui donne la force et la direction du vent.

	CVecteur2D* m_VecteurGravite;					// La gravité de la map.
	int m_iVentMax;									// Le vent max de la map.

public:

	// Constructeur de CMap...
	// Param1: _strEmplacementMap, emplacement de la map dans l'arborescence.
	// Param2: _RectPositionImages, la position de la map dans l'écran.
	// Param3: _pSDLTextureMap, pointe sur la texture qui représente l'avant plan de la carte de jeu.
	// Param4: _pVent, Classe qui donne la force et la direction du vent.
	// Param5: Renderer.
	CMap(string _strEmplacementMap, SDL_Rect _RectPositionImages, CVent* _pVent, SDL_Renderer* _pRenderer, void _MapDestruction(int _iRayon, int _iX, int _iY)) {
		
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

		m_pSDLTextureMap = SDL_CreateTextureFromSurface(_pRenderer, m_pSDLSurfaceMap);

		// Ouverture du fichier...
		strEmplacement = _strEmplacementMap;
		strEmplacement.append("map.txt");
		FichierMap.open(strEmplacement);

		m_RectPositionImages.w = m_pSDLSurfaceMap->w;
		m_RectPositionImages.h = m_pSDLSurfaceMap->h;

		if (FichierMap.is_open()) {

			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[20];
			strTampo += chrTmp[21];
			strTampo += chrTmp[22];
			m_VecteurGravite = new CVecteur2D(SDL_atof(strTampo.c_str()), 90.0f);
			FichierMap.getline(chrTmp, 50);
			
			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[23];
			strTampo += chrTmp[24];
			strTampo += chrTmp[25];
			m_iVentMax = SDL_atoi(strTampo.c_str());
			FichierMap.getline(chrTmp, 50);

			FichierMap.getline(chrTmp, 50);
			strTampo = chrTmp[28];
			strTampo += chrTmp[29];
			iNombreMines = SDL_atoi(strTampo.c_str());

			FichierMap.close();
		}

		for (int i = 0; i < iNombreMines; i++) {

			m_pPackList->AjouterFin(new CMine(_strEmplacementMap, _pRenderer, _MapDestruction));

		}

		NouveauVent(pGestionnaireFont->ObtenirDonnee("pFontBouton"), _pRenderer);
	}


	// Procédure qui affiche la carte de jeu.
	// Paramètre: _pSDLRender, Rendeur de la fenêtre dans laquelle on veut afficher la carte de jeu.
	// Retour: Rien.
	void ShowMap(SDL_Renderer* _pSDLRenderer) {

		// Affiche les maps.
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureBackground, NULL, &m_RectPositionImages);
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureMap, NULL, &m_RectPositionImages);

		// Affiche le vent
		m_pVent->ShowVent(_pSDLRenderer);

		// Affiche tous les packs...
		m_pPackList->AllerDebut();
		for (int i = m_pPackList->ObtenirCompte(); i > 0; i--) {

			if (m_pPackList->ObtenirElementCurseur()->GetSpriteExplosion() != nullptr) {
				if (m_pPackList->ObtenirElementCurseur()->IsUse() && !m_pPackList->ObtenirElementCurseur()->GetSpriteExplosion()->IsActif())
					m_pPackList->Retirer(true);
			}
			
			else {
				if (m_pPackList->ObtenirElementCurseur()->IsUse())
					m_pPackList->Retirer(true);
			}

			if (m_pPackList->ObtenirCompte() > 0) {
				m_pPackList->ObtenirElementCurseur()->ShowPack(_pSDLRenderer);
				m_pPackList->AllerSuivantCurseur();
			}
			
		}
			
		
	}

	// Procédure créant un nouveau vent...
	// En entrée:
	// Param1: Le font du texte...
	// Param2: Le renderer de la fenêtre...
	void NouveauVent(TTF_Font* _pFont, SDL_Renderer* _pRenderer) {
		int iAngle = rand() % 360;
		m_pVent->ModifierAngle(iAngle);
		char chr[4];
		int i = rand() % m_iVentMax;
		m_pVent->ModifierVecteurVent(0.20 * i, iAngle);
		SDL_itoa(i, chr, 10);
		string str = chr;
		str.append(" km/h");
		m_pVent->ModifierForce(_pFont, str.c_str(), { 0, 0, 0 }, _pRenderer);
	}
	
	// Procédure créant un healypack...
	void CreateHealthPack(void) {

		m_pPackList->AjouterFin(new CHealthPack());
	}

	// Procédure mettant la map en texture...
	// En entrée: 
	// Param1: Le renderer de la fenêtre...
	void PutMapInTexture(SDL_Renderer* _pRenderer) {

		SDL_DestroyTexture(m_pSDLTextureMap);
		m_pSDLTextureMap = SDL_CreateTextureFromSurface(_pRenderer, m_pSDLSurfaceMap);
	}


	// Accesseurs...

	SDL_Surface* ObtenirSurfaceMap(void) {

		return m_pSDLSurfaceMap;

	}

	CVecteur2D* ObtenirGravite(void) {

		return m_VecteurGravite;
	}
	
	CVecteur2D* ObtenirVecteurVent(void) {

		return m_pVent->ObtenirVecteurVent();
	}

	CListeDC<CPack*>* ObtenirPackList(void) {

		return m_pPackList;
	}
};