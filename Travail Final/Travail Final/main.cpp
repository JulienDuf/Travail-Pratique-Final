
// Inclusions...
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

#include "CArbreAVL.h"
#include "CGestionaire.h"

CGestionaire<SDL_Texture*>* pGestionnaireTexture; // Le gestionaire des textures.
CGestionaire<SDL_Surface*>* pGestionnaireSurface; // Le gestionaire des surfaces.
CGestionaire<TTF_Font*>* pGestionnaireFont; // Le gestionaire des fonts.

#include "CListeDC.h"
#include "CTimer.h"
#include "CVecteur2D.h"
#include "CSprite.h"
#include "CControl.h"

CGestionaire<CControl*>* pGestionnaireControl; // Le gestionaire des controls.

#include "CButton.h"
#include "CLabel.h"
#include "CLabelLeftRight.h"
#include "CTemporaryLabel.h"
#include "CMenu.h"


CGestionaire<CMenu*>* pGestionnaireMenu; // Le gestionaire des menus.

#include "CScrollBar.h"
#include "CBarrePuissance.h"
#include "CProjectile.h"
#include "CMouvement.h"
#include "CDeplacement.h"
#include "CMissile.h"
#include "CGrenade.h"
#include "CMelee.h"
#include "CBarreVie.h"
#include "CJetPack.h"
#include "CPlayer.h"
#include "CToolBar.h"
#include "CPack.h"
#include "CHealthPack.h"
#include "CMine.h"
#include "CTeam.h"
#include "CVent.h"
#include "CMap.h"
#include "CGame.h"
#include "CWindow.h"

// Variables...
bool boExecution; // Variable de la boucle principale du programme.
bool boPause; // Variable pour savoir si le menu est en pause, ou non.

string strEmplacementFichier; // L'emplacement de nos fichiers.

SDL_Event* pEvent; // Les événemens du programme;

CWindow* pWindowJeu; // La fenêtre de jeu.

SDL_Color CouleurTexte; // La couleur du texte.

// Fonction convertissant en angle de degré à radian.
// En entrée:
// Param1: L'angle à convertir.
// En sortie: L'angle converti.
float DegtoRad(float _fAngle) {

	return (M_PI / 180) * _fAngle;
}

// Pointeurs de fonctions pour les classes [

// Fontion effectuant un rotation sur une surface selon un angle.
// En entrée: 
// Param1: La surface qui doit être tourné.
// Param2: L'angle de la rotation.
// En sortie: La surface avec la rotaiton.
SDL_Surface* Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle) {

	SDL_Surface * pNouvelleSurface = SDL_CreateRGBSurface(_pSurfaceRotation->flags, _pSurfaceRotation->w, _pSurfaceRotation->h, _pSurfaceRotation->format->BitsPerPixel, _pSurfaceRotation->format->Rmask, _pSurfaceRotation->format->Gmask, _pSurfaceRotation->format->Bmask, _pSurfaceRotation->format->Amask);


	double sin = sinf(DegtoRad(_fAngle));
	double cos = cosf(DegtoRad(_fAngle));

	for (int y = 0; y < _pSurfaceRotation->h; y++)
	for (int x = 0; x < _pSurfaceRotation->w; x++) {

		int RotX = (x - _pSurfaceRotation->w / 2) * cos - (y - _pSurfaceRotation->h / 2) * sin + _pSurfaceRotation->w / 2;
		int RotY = (x - _pSurfaceRotation->w / 2) * sin + (y - _pSurfaceRotation->h / 2) * cos + _pSurfaceRotation->w / 2;

		if ((RotX >= 0 && RotY >= 0 && (RotX < _pSurfaceRotation->w && RotY < _pSurfaceRotation->h)))
			((unsigned int*)pNouvelleSurface->pixels)[y * pNouvelleSurface->w + x] = ((unsigned int*)_pSurfaceRotation->pixels)[RotY * _pSurfaceRotation->w + RotX];
	}

	return pNouvelleSurface;
}

// Procédure détruisant une partie de la map selon un rayon.
// En entrée:
// Param1: Le rayon de la destruction.
// Param2: La position en X de la destruction dans la map.
// Param3: La position en Y de la destruction dans la map.
void MapDestruction(int _iRayonDestruction,int _iX, int _iY) {
	SDL_Rect RectTmp = { _iX - _iRayonDestruction, _iY - _iRayonDestruction, 2 * _iRayonDestruction, 2 * _iRayonDestruction };
	SDL_Surface* pSurfaceMap = pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirSurfaceMap();
	double dX;
	double dY;
	double dRayonPixel;
	for (int j = 0; j <= RectTmp.h; j++) {
		for (int i = 0; i <= RectTmp.w; i++) {
			dX = _iRayonDestruction - i;
			dY = _iRayonDestruction - j;
			dRayonPixel = sqrt(pow(dX, 2) + pow(dY, 2));
			if (dRayonPixel <= _iRayonDestruction) {
				for (int i2 = 0; i2 <= dX * 2; i2++) {
					if (j + RectTmp.y < 768 && j + RectTmp.y >= 0 && i + RectTmp.x < 1366 && i + RectTmp.x >= 0)
						((unsigned int*)pSurfaceMap->pixels)[(j + RectTmp.y) * pSurfaceMap->w + i + RectTmp.x] = 0;
					i++;
				}
			}
		}
	}
	pWindowJeu->ObtenirGame()->ObtenirMap()->PutMapInTexture(pWindowJeu->ObtenirRenderer());
}

// Procédure pour le click sur le bouton nouvelle partie...
void ClickBoutonNouvellePartie(void) {

	pGestionnaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(true);
	pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(false);
}

// Procédure pour le click sur le bouton retour dans le menu nouvelle partie...
void ClickBoutonRetour(void) {

	pGestionnaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(false);
	pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(true);
}

// Procédure pour le click sur le bouton début partie...
void ClickBoutonDebutPartie(void) {

	string strTmp = strEmplacementFichier;
	int iNombreEquipe = 0;
	int iNombreJoueur = 0;

	// Load des texture pour la toolbar...
	string strBazookaToolPath = strEmplacementFichier;
	strBazookaToolPath.append("Armes et Packs\\bazookatool.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strBazookaToolPath.c_str()), "BazookaTool");

	string strGrenadaToolPath = strEmplacementFichier;
	strGrenadaToolPath.append("Armes et Packs\\grenadetool.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strGrenadaToolPath.c_str()), "GrenadaTool");

	string strSwordToolPath = strEmplacementFichier;
	strSwordToolPath.append("Armes et Packs\\swordtool.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strSwordToolPath.c_str()), "SwordTool");

	string strJetPackToolPath = strEmplacementFichier;
	strJetPackToolPath.append("Armes et Packs\\jetpacktool.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strJetPackToolPath.c_str()), "JetPackTool");

	pGestionnaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(false);
	pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(false);

	strTmp.append("Maps\\");

	switch (pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->ObtenirElement("PositionLabel")) {

	case 0:
		strTmp.append("arcaderoom\\");
		break;

	case 1:
		strTmp.append("country\\");
		break;

	case 2:
		strTmp.append("desert\\");
		break;

	case 3:
		strTmp.append("easterisland\\");
		break;
		
	case 4:
		strTmp.append("farm\\");
		break;

	case 5:
		strTmp.append("pirates\\");
		break;

	case 6:
		strTmp.append("snow\\");
		break;
	}

	iNombreEquipe = pGestionnaireControl->ObtenirDonnee("pLblLRChoixNbrEquipe")->ObtenirElement("PositionLabel") + 2;
	iNombreJoueur = pGestionnaireControl->ObtenirDonnee("pLblLRChoixNbrJoueurEquipe")->ObtenirElement("PositionLabel") + 4;

	pWindowJeu->CreateGame(strTmp, iNombreEquipe, iNombreJoueur, new CVent(pGestionnaireFont->ObtenirDonnee("pFontBouton"), "250 km/h", CouleurTexte, pGestionnaireTexture->ObtenirDonnee("pFlecheVent"), { 1200, 30, 117, 63 }, 180, pWindowJeu->ObtenirRenderer()), MapDestruction, Rotation);
}

// Procédure pour le click sur le bouton quitter...
void ClickBoutonQuitter(void) {

	boExecution = false;
}

// Procédure pour le click sur le bouton droit du labelLeftRight de choix de la map...
void ClickBoutonDroitChoixMap(void) {

	pGestionnaireControl->ObtenirDonnee("pLblDescriptionMap")->ChangeTexture(true);

}

// Procédure pour le click sur le bouton gauche du labelLeftRight de choix de la map...
void ClickBoutonGaucheChoixMap(void) {

	pGestionnaireControl->ObtenirDonnee("pLblDescriptionMap")->ChangeTexture(false);

}

// Procédure qui résume la partie lorsquelle est sur pause...
void ClickBoutonResumer(void) {
	pGestionnaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(false);
	boPause = false;
}

// Procédure qui quitter une partie en cours et la détruit...
void ClickBoutonQuitterJeu(void) {
	pWindowJeu->FinDePartie();
	pGestionnaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(false);
	boPause = false;
	pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(true);
}

// ]


// Fonction qui met le texte d'un tableau en une seule surface.
// En entrée: 
// Param1: Le tableau contenant le texte à mettre ensemble.
// Param2: Le nombre d'élément du tableau.
// Param3: La font du texte.
// Param4: La couleur du texte.
// En sortie:
// La surface finale.
void BlitTexte(string _strTexte[], int _iNombreElementTableau, TTF_Font* _Font, SDL_Color _Couleur, SDL_Surface* _pSurfaceToBlitIn) {

	SDL_Surface* pSDLSurface; // Variable temporaire...
	SDL_Rect RectTmp = { 0, 0, 0, 0 };
	
	for (int i = 0; i < _iNombreElementTableau; i++) {
		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[i].c_str(), _Couleur); // Créé la surface contenant le texte.
		RectTmp.y = pSDLSurface->h * (i);
		SDL_BlitSurface(pSDLSurface, NULL, _pSurfaceToBlitIn, &RectTmp);
		SDL_FreeSurface(pSDLSurface);
	}
}

// Procédure qui lis les informations d'une map.
// En entrée:
// Param1 : L'emplacement du fichier à lire.
void ReadMapInfo(string _strEmplacement) {

	// Variables temporaires...
	string strTmp[5];
	char chrTmp[50];
	ifstream FichierMap;

	// Ouverture du fichier...
	_strEmplacement.append("\\map.txt");
	FichierMap.open(_strEmplacement);

	// Si le fichier est ouvert...
	if (FichierMap.is_open()) {

		// Pour toutes les ligens de fichier, on les met dans le tableau temporaires.
		for (int i = 0; i < 5; i++) {

			FichierMap.getline(chrTmp, 50);
			strTmp[i].append(chrTmp);

		}

		// Ferme le fichier.
		FichierMap.close();
	}

	SDL_Surface* pSDLSurface = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontBouton"), strTmp[0].c_str(), CouleurTexte);
	pSDLSurface = SDL_CreateRGBSurface(pSDLSurface->flags, 500, pSDLSurface->h * 5, pSDLSurface->format->BitsPerPixel, pSDLSurface->format->Rmask, pSDLSurface->format->Gmask, pSDLSurface->format->Bmask, pSDLSurface->format->Amask);

	// Ajoute ce qui a été lu dans le label.
	BlitTexte(strTmp, 5, pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, pSDLSurface);
	pGestionnaireControl->ObtenirDonnee("pLblDescriptionMap")->AjouterTexture(1, SDL_CreateTextureFromSurface(pWindowJeu->ObtenirRenderer(), pSDLSurface));

}

// Procédure qui transforme les deux images d'une map en une image.
// Param1: L'emplacement de l'image.
// Param2: Le nom du dossier ou sont les images.
// Param3: La texture ou sera la texture finale.
void PutMapToTexture(string _strEmplacement, SDL_Texture** _SDLTextureStockage) {

	// Variables temporaires...
	string strEmplacement;
	SDL_Surface* pSDLSurfaceTmp;
	SDL_Surface* pSDLSurfaceCopie;
	SDL_Rect RectTmp = { 0, 0, 0, 0 };

	// Lis les infos de la map.
	ReadMapInfo(_strEmplacement);

	// Téléchargement des images
	strEmplacement = _strEmplacement;
	strEmplacement.append("\\background.jpg");
	pSDLSurfaceCopie = IMG_Load(strEmplacement.c_str());
	strEmplacement = _strEmplacement;
	strEmplacement.append("\\map.png");
	pSDLSurfaceTmp = IMG_Load(strEmplacement.c_str());

	// Redéfinition du rect et blit des deux surfaces...
	RectTmp.x = (pSDLSurfaceCopie->w - pSDLSurfaceTmp->w) / 2;
	RectTmp.y = pSDLSurfaceCopie->h - pSDLSurfaceTmp->h;
	SDL_BlitSurface(pSDLSurfaceTmp, NULL, pSDLSurfaceCopie, &RectTmp);

	// Création de la texture et suppression des surfaces.
	*_SDLTextureStockage = SDL_CreateTextureFromSurface(pWindowJeu->ObtenirRenderer(), pSDLSurfaceCopie);
	SDL_FreeSurface(pSDLSurfaceCopie);
	SDL_FreeSurface(pSDLSurfaceTmp);
}


//
// Procédure initialisant les librairies et variables.
//
void Start(char* _strApplicationFilename) {

	// Variables temporaires...
	string strEmplacement;
	int iW, iH;
	SDL_Texture* pTextureTmp = nullptr;

	// Trouve l'emplacement du dossier debug.
	string strApplicationPath(_strApplicationFilename);
	int i = strApplicationPath.length();
	while (strApplicationPath[--i] != '\\');
	strApplicationPath.resize(++i);

	// Initialistion des librairies
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	// Création de la fenêtre...
	pWindowJeu = new CWindow(1366, 768);
	pWindowJeu->GetSize(&iW, &iH);
	
	pGestionnaireTexture = new CGestionaire<SDL_Texture*>();
	pGestionnaireSurface = new CGestionaire<SDL_Surface*>();
	pGestionnaireFont = new CGestionaire<TTF_Font*>();
	pGestionnaireControl = new CGestionaire<CControl*>();
	pGestionnaireMenu = new CGestionaire<CMenu*>();

	// Chargement des surfaces des personnages...
	
	// La course...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Course.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceCourse");

	// Le saut...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Saut.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceSaut");

	// Le parachute...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Parachute.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceParachute");

	// Au repos...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Repos.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceRepos");

	// Le jetpack...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\jetpack.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceJetPack");

	// Chargement de la surface du missile...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Missile.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceMissile");

	// Chargement de la surface de la grenade...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Grenade.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceGrenade");

	// L'attaque de mélée...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Melee.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceMelee");

	// Avec le bazooka...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\bazooka.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfacePlayerBazooka");

	// Chargement de la surface des mines...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Mine.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceMine");

	// Chargement de la surface des healthpacks...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\HealthPack.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceHealthPack");

	// Chargement de la surface de l'explosion du missile...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\ExplosionMissile.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceExplosionMissile");

	// Chargement de la surface de l'explosion de la grenade...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\ExplosionGrenade.png");
	pGestionnaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceExplosionGrenade");

	// Chargement de font du texte des boutons et label de description...
	strEmplacement = strApplicationPath;
	strEmplacement.append("BRLNSR.ttf");
	pGestionnaireFont->AjouterDonnee(TTF_OpenFont(strEmplacement.c_str(), 30), "pFontBouton");
	pGestionnaireFont->AjouterDonnee(TTF_OpenFont(strEmplacement.c_str(), 12), "pFontDescription");
	pGestionnaireFont->AjouterDonnee(TTF_OpenFont(strEmplacement.c_str(), 20), "pFontDomage");

	// Chargement des textures des personnages...

	// La course...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Course.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureCourse");

	// Le saut...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Saut.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureSaut");

	// Le parachute...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Parachute.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureParachute");

	// Au repos...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Repos.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureRepos");

	// Le jetpack...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\jetpack.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureJetPack");
	
	// L'attaque de mélée...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Melee.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureMelee");

	// Avec le bazooka...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnege\\bazooka.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTexturePlayerBazooka");

	// Étoile pour reconnaître le joueur actif...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\EtoileJoueurActif.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "EtoileJoueurActif");

	// Chargement de la surface des mines...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Mine.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer() ,strEmplacement.c_str()), "pTextureMine");

	// Chargement de la surface des healthpacks...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\HealthPack.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureHealthPack");

	// Chargement de la texture de la barre de puissance
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\BarrePuissance.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureBarrePuissance");

	// Chargement de la texture de l'explosion du missile...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\ExplosionMissile.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureExplosionMissile");

	// Chargement de la texture de l'explosion du missile...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\ExplosionGrenade.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureExplosionGrenade");

	// Chargement de la texture du bazooka.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\bazooka.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureBazooka");

	// Chargement des textures de la barre de vie...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Barre.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureBarreVie");

	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\ContourBarre.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureContourBarreVie");


	// Chargements de la texture pour le bouton droit d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheDroite.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheDroite");

	// Chargements de la texture pour le bouton gauche d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheGauche.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheGauche");

	// Chargements de la texture pour le bouton gauche d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("FlecheVent.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheVent");

	// Chargement de la texture du menu principale.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Menu\\Menu Principal.png");
	pGestionnaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureMenuPrincipal");

	CouleurTexte = { 0, 0, 0, 0 }; // Met la couleur noir.

	// Créé les boutons avec l'emplacement de l'image des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pGestionnaireControl->AjouterDonnee(new CButton("Nouvelle Partie", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 250, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonNouvellePartie), "pBtnNouvellePartie");
	pGestionnaireControl->AjouterDonnee(new CButton("Quitter", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 350, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonQuitter), "pBtnQuitter");
	pGestionnaireControl->AjouterDonnee(new CButton("Debuter la partie", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 790, 530, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonDebutPartie), "pBtnDebutPartie");
	pGestionnaireControl->AjouterDonnee(new CButton("Retour", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 790, 600, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonRetour), "pBtnRetour");
	pGestionnaireControl->AjouterDonnee(new CButton("Resumer", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 503, 284, 360, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonResumer), "pBtnResumer");
	pGestionnaireControl->AjouterDonnee(new CButton("Quitter", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 503, 444, 360, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonQuitterJeu), "pBtnQuitterGame");

	// Création des labels...
	pGestionnaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre d equipes", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 180, 520, 231, 32 }), "pLblNombreEquipe");
	pGestionnaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre de joueurs par equipe", pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 100, 620, 407, 32 }), "pLblNombreJoueurEquipe");
	pGestionnaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), { 900, 130, 500, 32 * 5 }), "pLblDescriptionMap");

	// Créations des lableLeftRight...
	pGestionnaireControl->AjouterDonnee(new CLabelLeftRight(pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 270, 550, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheGauche"), { 225, 554, 35, 35 }, 4, 1, NULL), new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheDroite"), { 325, 554, 35, 35 }, 4, 1, NULL), 3, "2", "3", "4"), "pLblLRChoixNbrEquipe");
	pGestionnaireControl->AjouterDonnee(new CLabelLeftRight(pGestionnaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 270, 650, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheGauche"), { 225, 654, 35, 35 }, 4, 1, NULL), new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheDroite"), { 325, 654, 35, 35 }, 4, 1, NULL), 3, "4", "5", "6"), "pLblLRChoixNbrJoueurEquipe");
	pGestionnaireControl->AjouterDonnee(new CLabelLeftRight({ 120, 50, 623, 367 }, new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheGauche"), { 20, 199, 80, 80 }, 4, 1, ClickBoutonGaucheChoixMap), new CButton(pGestionnaireTexture->ObtenirDonnee("pFlecheDroite"), { 760, 199, 80, 80 }, 4, 1, ClickBoutonDroitChoixMap), 0), "pLblLRChoixMap");
	 
	// Met les map en une texture...

	PutMapToTexture(strApplicationPath + "Maps\\arcaderoom", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\country", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\desert", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\easterisland", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\farm", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\pirates", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\snow", &pTextureTmp);
	pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;
	//......

	strEmplacementFichier = strApplicationPath;

	// Création des menus...
	pGestionnaireMenu->AjouterDonnee(new CMenu(true, { 0, 0, iW, iH }, pGestionnaireTexture->ObtenirDonnee("pTextureMenuPrincipal"), 2, pGestionnaireControl->ObtenirDonnee("pBtnNouvellePartie"), pGestionnaireControl->ObtenirDonnee("pBtnQuitter")), "pMenuPrincipal"); // Crée le menu principal.
	pGestionnaireMenu->AjouterDonnee(new CMenu(false, { 0, 0, iW, iH }, { 255, 255, 255, 255 }, 8, pGestionnaireControl->ObtenirDonnee("pBtnDebutPartie"), pGestionnaireControl->ObtenirDonnee("pBtnRetour"), pGestionnaireControl->ObtenirDonnee("pLblDescriptionMap"), pGestionnaireControl->ObtenirDonnee("pLblNombreJoueurEquipe"), pGestionnaireControl->ObtenirDonnee("pLblNombreEquipe"), pGestionnaireControl->ObtenirDonnee("pLblLRChoixNbrEquipe"), pGestionnaireControl->ObtenirDonnee("pLblLRChoixNbrJoueurEquipe"), pGestionnaireControl->ObtenirDonnee("pLblLRChoixMap")), "pMenuNouvellePartie"); // Créé le menu nouvelle partie.
	pGestionnaireMenu->AjouterDonnee(new CMenu(false, { 341, 192, 683, 384 }, { 128, 128, 128, 165 }, 2, pGestionnaireControl->ObtenirDonnee("pBtnResumer"), pGestionnaireControl->ObtenirDonnee("pBtnQuitterGame")), "pMenuPause");

	// Ajoue des menus dans la fenêtre.
	pWindowJeu->AjouterMenu(3, pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal"), pGestionnaireMenu->ObtenirDonnee("pMenuNouvellePartie"), pGestionnaireMenu->ObtenirDonnee("pMenuPause"));

	boExecution = true;
	boPause = false;
	pEvent = new SDL_Event(); // Créé le pointeur.

}

// Procédure détruisant les variables.
//
void Close(void) {

	// Détruit le gestionnaire d'événement
	delete pEvent;
	delete pWindowJeu; // Détruit la fenêtre, donc tous les contrôles reliés à la fenêtre.

	// Quitte les librairies...
	SDL_Quit();
	TTF_Quit();
}




int main(int argc, char* argv[]) {

	Start(argv[0]); // Initialisation

	srand(time(NULL));

	// Boucle principale de l'application.
	while (boExecution) {

		pWindowJeu->Rafraichir(boPause);

		// Tant qu'il y a des événements à gérer.
		while (SDL_PollEvent(pEvent)) {

			pGestionnaireMenu->ObtenirDonnee("pMenuPrincipal")->ReactToEvent(pEvent);
			pGestionnaireMenu->ObtenirDonnee("pMenuNouvellePartie")->ReactToEvent(pEvent);
			pGestionnaireMenu->ObtenirDonnee("pMenuPause")->ReactToEvent(pEvent);
			if (pWindowJeu->ObtenirGame() != nullptr && !boPause) {
				pWindowJeu->ObtenirGame()->ReactToEvent(pEvent);
			}

			switch (pEvent->type) {

			case SDL_QUIT:
				boExecution = false;
				break;
			case SDL_KEYDOWN:
				switch (pEvent->key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					if (pEvent->type == SDL_KEYDOWN) {
						if (pWindowJeu->ObtenirGame() != nullptr) {
							pGestionnaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(true);
							boPause = true;
						}
						else
							boExecution = !(pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE);
					}
					break;
				}
				break;

			}
		}
	}

	Close(); // Déinitialisation
	return 0;
}