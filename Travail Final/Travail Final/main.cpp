
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
#include "CListeDC.h"
#include "Pointeur Fonction.h"
#include "CTimer.h"
#include "CVecteur2D.h"
#include "CSprite.h"
#include "CControl.h"
#include "CButton.h"
#include "CLabel.h"
#include "CMenu.h"
#include "CGestionaire.h"
#include "CScrollBar.h"
#include "CToolBar.h"
#include "CBarrePuissance.h"
#include "CTool.h"
#include "CMissile.h"
#include "CGrenade.h"
#include "CMelee.h"
#include "CBarreVie.h"
#include "CJetPack.h"
#include "CPlayer.h"
#include "CPack.h"
#include "CHealthPack.h"
#include "CMine.h"
#include "CTeam.h"
#include "CVent.h"
#include "CMap.h"
#include "CGame.h"
#include "CWindow.h"
#include "CLabelLeftRight.h"

// Variables...
bool boExecution; // Variable de la boucle principale du programme.

string strEmplacementFichier; // L'emplacement de nos fichiers.

SDL_Event* pEvent; // Les �v�nemens du programme;

CWindow* pWindowJeu; // La fen�tre de jeu.

CGestionaire<SDL_Texture*>* pGestionaireTexture; // Le gestionaire des textures.
CGestionaire<SDL_Surface*>* pGestionaireSurface; // Le gestionaire des surfaces.
CGestionaire<TTF_Font*>* pGestionaireFont; // Le gestionaire des fonts.
CGestionaire<CControl*>* pGestionaireControl; // Le gestionaire des controls.
CGestionaire<CMenu*>* pGestionaireMenu; // Le gestionaire des menus.

SDL_Surface* pSurfaceGabarie; // Le gabarie pour les destructions.

SDL_Color CouleurTexte; // La couleur du texte.

CTimer* pTimerPhysique;

// Fonction convertissant en angle de degr� � radian.
// En entr�e:
// Param1: L'angle � convertir.
// En sortie: L'angle converti.
float DegtoRad(float _fAngle) {

	return (M_PI / 180) * _fAngle;
}


// Pointeurs de fonctions pour les classes [



// Fonction qui retourne la position d'une collision.
// Param�tre: _pPlayer, joueur pour lequel on v�rifie les collisions avec la carte de jeu
// Param�tre: _RectPlayer, rectangle qui encadre l'endroit ou l'on veut v�rifier les collisions sur la map. (ex: Prochain rectangle destination d'un player.)
// Param�tre: _pboCollisionCorps, pointeur de bool�enne qui indique si il y a eu une collision au corps.
// Param�tre: _pboCollisionPieds, pointeur de bool�enne qui indique si il y a eu une collision au pieds.
// Param�tre: _puiXCorps, pointeur d'entier non-sign� qui indique la position en x de la collision dans le rectangle du joueur entre le corps et la carte de jeu s'il y a lieu.
// Param�tre: _puiYPieds, pointeur d'entier non-sign� qui indique la position en y de la collision dans le rectangle du joueur entre les pieds et la carte de jeu s'il y a lieu.
// Retour: Un bool�en qui indique si il y a eu une collision.
bool VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _pboCollisionCorps, bool* _pboCollisionPieds, unsigned int* _puiXCorps, unsigned int* _puiYPieds) {

	SDL_Surface* pTmpSDLSurfaceMap = pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirSurfaceMap();	// pointeur temporaire de surface qui pointe sur la surface de la carte de jeu.
	SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropri�e du joueur.

	SDL_Rect TmpSDLRectPlayerDestination = _RectPlayer;						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affich� sur la map.
	SDL_Rect TmpSDLRectPlayerSource;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropri�e du joueur pour la situation.

	SDL_Rect TmpSDLRectPlayerHitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du pieds.
	SDL_Rect TmpSDLRectPlayerHitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du corps.

	if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...

		pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de course.
	}

	else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

		pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de saut.
	}

	else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

		pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds appropri� pour le sprite de parachute.
	}

	else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

		pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de repos.
	}
	else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

		pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
	}

	if (_pPlayer->ObtenirSpriteCourse()->ObtenirAnimation())					// Si le jueur est orient� vers la droite...
		TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
	else																		// Sinon...
		TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	


	*_pboCollisionCorps = false;
	*_pboCollisionPieds = false;

	*_puiXCorps = 0;
	*_puiYPieds = 0;

	// V�rification de la collision des pieds du joueur avec la carte de jeu...

	for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxPieds.h && !*_pboCollisionPieds; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

		for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxPieds.w && !*_pboCollisionPieds; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

			if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfaceMap->w] != 0) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfacePlayer->w] != 0)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents des pieds du joueur...

				*_puiYPieds = TmpSDLRectPlayerHitboxPieds.y + y;		// Dans le rectangle destination, on prend la position en y de la collision pour la stocker.

				*_pboCollisionPieds = true;				// On confirme la collision aux pieds.


			}

		}

	}
	
	// V�rification de la collision du corps du joueur avec la carte de jeu.

	for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxCorps.h && !*_pboCollisionCorps; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

		for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxCorps.w && !*_pboCollisionCorps; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

			if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfaceMap->w] != 0) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfacePlayer->w] != 0)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du corps du joueur...

				*_puiXCorps = TmpSDLRectPlayerHitboxCorps.x + x;		// Dans le rectangle destination, on prend la position en x de la collision pour la stocker.

				*_pboCollisionCorps = true;

			}

		}

	}
	
	return (*_pboCollisionPieds || *_pboCollisionCorps);			// On retourne vrai d�s qu'il y a eu une collision.


}


// Fontion effectuant un rotation sur une surface selon un angle.
// En entr�e: 
// Param1: La surface qui doit �tre tourn�.
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

// Proc�dure d�truisant une partie de la map selon un rayon.
// En entr�e:
// Param1: Le rayon de la destruction.
// Param2: La position en X de la destruction dans la map.
// Param3: La position en Y de la destruction dans la map.
void MapDestruction(int _iRayon, int _iX, int _iY) {

	int iX, iY;
	SDL_Rect RectSource = { 0, 0, 120, 120 };
	SDL_Surface* pSurfaceMap = pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirSurfaceMap();

	iX = _iX - 60;
	iY = _iY - 60;

	switch (_iRayon) {

	case 45:
		RectSource.x = RectSource.w * 0;
		break;

	case 50:
		RectSource.x = RectSource.w * 1;
		break;

	case 60:
		RectSource.x = RectSource.w * 2;
		break;
	}

	for (int y = 0; y < 120; y++) {

		for (int x = 0; x < 120; x++) {

			if ((iY >= 0 && iX >= 0) && (iY <= 768 & iX <= 1366)) {

				if (((unsigned int*)pSurfaceGabarie->pixels)[y * pSurfaceGabarie->w + RectSource.x + x] != BLANC32BIT)
					((unsigned int*)pSurfaceMap->pixels)[iY * pSurfaceMap->w + iX] = TRANSPARENCE32BIT;
			}
			iX++;

		}
		iY++;
		iX = _iX - 60;

	}

	pWindowJeu->ObtenirGame()->ObtenirMap()->PutMapInTexture(pWindowJeu->ObtenirRenderer());

}

// Fonction effectuant la physique d'un missile.
// En entr�e:
// Param1: Le vecteur de la vitesse du missile.
// Param2: La destination du missile.
// En sortie: Le nouvel angle du missile.
double PhysiqueMissile(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile) {

	_DestinationMissile->x += _VitesseMissile->ObtenirComposanteX() / 35;
	_DestinationMissile->y += (_VitesseMissile->ObtenirComposanteY()) / 35;

	if (pTimerPhysique->IsDone()) {

		*_VitesseMissile += *pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirGravite();

		if (_VitesseMissile->ObtenirComposanteY() < 0 && _VitesseMissile->ObtenirComposanteX() >= 0)
			return (180 / M_PI) * atanf(((-(float)_VitesseMissile->ObtenirComposanteY()) / ((float)_VitesseMissile->ObtenirComposanteX())));

		if (_VitesseMissile->ObtenirComposanteY() >= 0 && _VitesseMissile->ObtenirComposanteX() < 0)
			return 180 + (180 / M_PI) * atanf((((float)_VitesseMissile->ObtenirComposanteY()) / (-(float)_VitesseMissile->ObtenirComposanteX())));

		if (_VitesseMissile->ObtenirComposanteY() < 0 && _VitesseMissile->ObtenirComposanteX() < 0)
			return 180 - (180 / M_PI) * atanf(((-(float)_VitesseMissile->ObtenirComposanteY()) / (-(float)_VitesseMissile->ObtenirComposanteX())));

		if (_VitesseMissile->ObtenirComposanteY() >= 0 && _VitesseMissile->ObtenirComposanteX() >= 0)
			return 360 - (180 / M_PI) * atanf((((float)_VitesseMissile->ObtenirComposanteY()) / ((float)_VitesseMissile->ObtenirComposanteX())));

		pTimerPhysique->Start();
	}
}

// Proc�dure d�terminant la position d'une collision entre un objet et la map, si il y en a une.
// En entr�e:
// Param1: La surface de l'objet.
// Param2: La destination de l'objet.
// Param3: La position en X qui sera retourn�.
// Param4: La position en Y qui sera retourn�.
void CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY) {

	*_iX = 0;
	*_iY = 0;

	unsigned int ix, iy;

	SDL_Surface* pSDLSurfaceMap = pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirSurfaceMap();

	for (int y = _RectDestination.y; y < _RectDestination.y + _RectDestination.h; y++) {
		for (int x = _RectDestination.x; x < _RectDestination.x + _RectDestination.w; x++) {

			if (x >= 0 && x <= 1366 && y >= 0 && y <= 768) {

				ix = ((unsigned int*)pSDLSurfaceMap->pixels)[y * pSDLSurfaceMap->w + x];

				if ((((unsigned int*)pSDLSurfaceMap->pixels)[y * pSDLSurfaceMap->w + x] != 0) && (((unsigned int*)_pSDLSurface->pixels)[(y - _RectDestination.y) * _pSDLSurface->w + (x - _RectDestination.x)] != 0)) {

					if (*_iX == 0) {
						*_iX = x;
						*_iY = y;
					}
				}
			}
		}
	}

}

// Proc�dure pour le click sur le bouton nouvelle partie...
void ClickBoutonNouvellePartie(void) {

	pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(true);
	pGestionaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(false);
}

// Proc�dure pour le click sur le bouton retour dans le menu nouvelle partie...
void ClickBoutonRetour(void) {

	pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(false);
	pGestionaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(true);
}

// Proc�dure pour le click sur le bouton d�but partie...
void ClickBoutonDebutPartie(void) {

	string strTmp = strEmplacementFichier;
	int iNombreEquipe = 0;
	int iNombreJoueur = 0;

	// Load des texture pour la toolbar...
	string strBazookaToolPath = strEmplacementFichier;
	strBazookaToolPath.append("Armes et Packs\\bazookatool.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strBazookaToolPath.c_str()), "BazookaTool");

	string strGrenadaToolPath = strEmplacementFichier;
	strGrenadaToolPath.append("Armes et Packs\\grenadetool.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strGrenadaToolPath.c_str()), "GrenadaTool");

	string strSwordToolPath = strEmplacementFichier;
	strSwordToolPath.append("Armes et Packs\\swordtool.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strSwordToolPath.c_str()), "SwordTool");

	string strJetPackToolPath = strEmplacementFichier;
	strJetPackToolPath.append("Armes et Packs\\jetpacktool.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strJetPackToolPath.c_str()), "JetPackTool");

	pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie")->DefinirboShow(false);
	pGestionaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(false);

	strTmp.append("Maps\\");

	switch (pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->ObtenirElement("PositionLabel")) {

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

	iNombreEquipe = pGestionaireControl->ObtenirDonnee("pLblLRChoixNbrEquipe")->ObtenirElement("PositionLabel") + 2;
	iNombreJoueur = pGestionaireControl->ObtenirDonnee("pLblLRChoixNbrJoueurEquipe")->ObtenirElement("PositionLabel") + 4;

	pWindowJeu->CreateGame(strTmp, pGestionaireSurface, pGestionaireTexture, iNombreEquipe, iNombreJoueur, new CVent(pGestionaireFont->ObtenirDonnee("pFontBouton"), "250 km/h", CouleurTexte, pGestionaireTexture->ObtenirDonnee("pFlecheVent"), { 1200, 30, 117, 63 }, 180, pWindowJeu->ObtenirRenderer()),  VerifierCollisionJoueurMap, MapDestruction, CollisionObjetMap, PhysiqueMissile, pWindowJeu->ObtenirRenderer());
}

// Proc�dure pour le click sur le bouton quitter...
void ClickBoutonQuitter(void) {

	boExecution = false;
}

// Proc�dure pour le click sur le bouton droit du labelLeftRight de choix de la map...
void ClickBoutonDroitChoixMap(void) {

	pGestionaireControl->ObtenirDonnee("pLblDescriptionMap")->ChangeTexture(true);

}

// Proc�dure pour le click sur le bouton gauche du labelLeftRight de choix de la map...
void ClickBoutonGaucheChoixMap(void) {

	pGestionaireControl->ObtenirDonnee("pLblDescriptionMap")->ChangeTexture(false);

}

// Proc�dure qui r�sume la partie lorsquelle est sur pause...
void ClickBoutonResumer(void) {
	pGestionaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(false);
}

// Proc�dure qui quitter une partie en cours et la d�truit...
void ClickBoutonQuitterJeu(void) {
	pWindowJeu->FinDePartie();
	pGestionaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(false);
	pGestionaireMenu->ObtenirDonnee("pMenuPrincipal")->DefinirboShow(true);
}

// ]


// Fonction qui met le texte d'un tableau en une seule surface.
// En entr�e: 
// Param1: Le tableau contenant le texte � mettre ensemble.
// Param2: Le nombre d'�l�ment du tableau.
// Param3: La font du texte.
// Param4: La couleur du texte.
// En sortie:
// La surface finale.
SDL_Surface* BlitTexte(string _strTexte[], int _iNombreElementTableau, TTF_Font* _Font, SDL_Color _Couleur) {

	SDL_Surface* pSDLSurface; // Variable temporaire...
	SDL_Surface* pSDLSurfaceTmp;
	SDL_Rect RectTmp = { 0, 0, 0, 0 };

	if (_iNombreElementTableau > 0) {
		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[0].c_str(), _Couleur); // Cr�� la surface contenant le texte.
		pSDLSurfaceTmp = SDL_CreateRGBSurface(pSDLSurface->flags, 500, pSDLSurface->h * _iNombreElementTableau, pSDLSurface->format->BitsPerPixel, pSDLSurface->format->Rmask, pSDLSurface->format->Gmask, pSDLSurface->format->Bmask, pSDLSurface->format->Amask);

		SDL_BlitSurface(pSDLSurface, NULL, pSDLSurfaceTmp, &RectTmp);
		RectTmp.y = pSDLSurface->h;
		SDL_FreeSurface(pSDLSurface);

		for (int i = 1; i < _iNombreElementTableau; i++) {

			pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[i].c_str(), _Couleur); // Cr�� la surface contenant le texte.
			SDL_BlitSurface(pSDLSurface, NULL, pSDLSurfaceTmp, &RectTmp);
			RectTmp.y = pSDLSurface->h * (i + 1);
			SDL_FreeSurface(pSDLSurface);
		}

		return pSDLSurfaceTmp;
	}

	return nullptr;
}

// Proc�dure qui lis les informations d'une map.
// En entr�e:
// Param1 : L'emplacement du fichier � lire.
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

	// Ajoute ce qui a �t� lu dans le label.
	SDL_Surface* pSDLSurface = BlitTexte(strTmp, 5, pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte);
	pGestionaireControl->ObtenirDonnee("pLblDescriptionMap")->AjouterTexture(1, SDL_CreateTextureFromSurface(pWindowJeu->ObtenirRenderer(), pSDLSurface));

}

// Proc�dure qui transforme les deux images d'une map en une image.
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

	// T�l�chargement des images
	strEmplacement = _strEmplacement;
	strEmplacement.append("\\background.jpg");
	pSDLSurfaceCopie = IMG_Load(strEmplacement.c_str());
	strEmplacement = _strEmplacement;
	strEmplacement.append("\\map.png");
	pSDLSurfaceTmp = IMG_Load(strEmplacement.c_str());

	// Red�finition du rect et blit des deux surfaces...
	RectTmp.x = (pSDLSurfaceCopie->w - pSDLSurfaceTmp->w) / 2;
	RectTmp.y = pSDLSurfaceCopie->h - pSDLSurfaceTmp->h;
	SDL_BlitSurface(pSDLSurfaceTmp, NULL, pSDLSurfaceCopie, &RectTmp);

	// Cr�ation de la texture et suppression des surfaces.
	*_SDLTextureStockage = SDL_CreateTextureFromSurface(pWindowJeu->ObtenirRenderer(), pSDLSurfaceCopie);
	SDL_FreeSurface(pSDLSurfaceCopie);
	SDL_FreeSurface(pSDLSurfaceTmp);
}


//
// Proc�dure initialisant les librairies et variables.
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

	// Cr�ation de la fen�tre...
	pWindowJeu = new CWindow(1366, 768);
	pWindowJeu->GetSize(&iW, &iH);
	
	pGestionaireTexture = new CGestionaire<SDL_Texture*>();
	pGestionaireSurface = new CGestionaire<SDL_Surface*>();
	pGestionaireFont = new CGestionaire<TTF_Font*>();
	pGestionaireControl = new CGestionaire<CControl*>();
	pGestionaireMenu = new CGestionaire<CMenu*>();

	pTimerPhysique = new CTimer(20);

	// Chargement du gabarie...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Gabarie.png");
	pSurfaceGabarie = IMG_Load(strEmplacement.c_str());

	// Chargement des surfaces des personnages...
	
	// La course...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Course.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceCourse");

	// Le saut...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Saut.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceSaut");

	// Le parachute...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Parachute.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceParachute");

	// Au repos...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Repos.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceRepos");

	// Le jetpack...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\jetpack.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceJetPack");

	// Chargement de la surface du missile...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Missile.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceMissile");

	// Chargement de la surface des mines...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\Mine.png");
	pGestionaireSurface->AjouterDonnee(IMG_Load(strEmplacement.c_str()), "pSurfaceMine");


	// Chargement de font du texte des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("calibri.ttf");
	pGestionaireFont->AjouterDonnee(TTF_OpenFont(strEmplacement.c_str(), 30), "pFontBouton");


	// Chargement des textures des personnages...

	// La course...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Course.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureCourse");

	// Le saut...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Saut.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureSaut");

	// Le parachute...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Parachute.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureParachute");

	// Au repos...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Repos.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureRepos");

	// Le jetpack...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\jetpack.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureJetPack");
	

	// Chargement de la texture de la barre de puissance
	strEmplacement = strApplicationPath;
	strEmplacement.append("Armes et Packs\\BarrePuissance.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureBarrePuissance");

	// Chargement des textures de la barre de vie...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\Barre.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureBarreVie");

	strEmplacement = strApplicationPath;
	strEmplacement.append("Personnage\\ContourBarre.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pTextureContourBarreVie");


	// Chargements de la texture pour le bouton droit d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheDroite.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheDroite");

	// Chargements de la texture pour le bouton gauche d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheGauche.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheGauche");

	// Chargements de la texture pour le bouton gauche d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("FlecheVent.png");
	pGestionaireTexture->AjouterDonnee(IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str()), "pFlecheVent");

	CouleurTexte = { 0, 0, 0, 0 }; // Met la couleur noir.

	// Cr�� les boutons avec l'emplacement de l'image des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pGestionaireControl->AjouterDonnee(new CButton("Nouvelle Partie", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 250, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonNouvellePartie), "pBtnNouvellePartie");
	pGestionaireControl->AjouterDonnee(new CButton("Quitter", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 320, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonQuitter), "pBtnQuitter");
	pGestionaireControl->AjouterDonnee(new CButton("Debuter la partie", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 790, 530, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonDebutPartie), "pBtnDebutPartie");
	pGestionaireControl->AjouterDonnee(new CButton("Retour", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 790, 600, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonRetour), "pBtnRetour");
	pGestionaireControl->AjouterDonnee(new CButton("Resumer", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 503, 324, 360, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonResumer), "pBtnResumer");
	pGestionaireControl->AjouterDonnee(new CButton("Quitter", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, strEmplacement.c_str(), { 503, 484, 360, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonQuitterJeu), "pBtnQuitterGame");

	// Cr�ation des labels...
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre d equipes", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 180, 520, 231, 32 }), "pLblNombreEquipe");
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre de joueurs par equipe", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 100, 620, 407, 32 }), "pLblNombreJoueurEquipe");
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), { 900, 130, 500, 32 * 5 }), "pLblDescriptionMap");

	// Cr�ations des lableLeftRight...
	pGestionaireControl->AjouterDonnee(new CLabelLeftRight(pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 270, 550, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheGauche"), { 225, 554, 35, 35 }, 4, 1, NULL), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheDroite"), { 325, 554, 35, 35 }, 4, 1, NULL), 3, "2", "3", "4"), "pLblLRChoixNbrEquipe");
	pGestionaireControl->AjouterDonnee(new CLabelLeftRight(pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 270, 650, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheGauche"), { 225, 654, 35, 35 }, 4, 1, NULL), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheDroite"), { 325, 654, 35, 35 }, 4, 1, NULL), 3, "4", "5", "6"), "pLblLRChoixNbrJoueurEquipe");
	pGestionaireControl->AjouterDonnee(new CLabelLeftRight({ 120, 50, 623, 367 }, new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheGauche"), { 20, 199, 80, 80 }, 4, 1, ClickBoutonGaucheChoixMap), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheDroite"), { 760, 199, 80, 80 }, 4, 1, ClickBoutonDroitChoixMap), 0), "pLblLRChoixMap");
	 
	// Met les map en une texture...

	PutMapToTexture(strApplicationPath + "Maps\\arcaderoom", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\country", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\desert", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\easterisland", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\farm", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\pirates", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath + "Maps\\snow", &pTextureTmp);
	pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")->AjouterTexture(1, pTextureTmp);
	pTextureTmp = nullptr;
	//......

	strEmplacementFichier = strApplicationPath;

	// Cr�ation des menus...
	pGestionaireMenu->AjouterDonnee(new CMenu(true, { 0, 0, iW, iH }, pWindowJeu->ObtenirRenderer(), 2, pGestionaireControl->ObtenirDonnee("pBtnNouvellePartie"), pGestionaireControl->ObtenirDonnee("pBtnQuitter")), "pMenuPrincipal"); // Cr�e le menu principal.
	pGestionaireMenu->AjouterDonnee(new CMenu(false, { 0, 0, iW, iH }, pWindowJeu->ObtenirRenderer(), 8, pGestionaireControl->ObtenirDonnee("pBtnDebutPartie"), pGestionaireControl->ObtenirDonnee("pBtnRetour"), pGestionaireControl->ObtenirDonnee("pLblDescriptionMap"), pGestionaireControl->ObtenirDonnee("pLblNombreJoueurEquipe"), pGestionaireControl->ObtenirDonnee("pLblNombreEquipe"), pGestionaireControl->ObtenirDonnee("pLblLRChoixNbrEquipe"), pGestionaireControl->ObtenirDonnee("pLblLRChoixNbrJoueurEquipe"), pGestionaireControl->ObtenirDonnee("pLblLRChoixMap")), "pMenuNouvellePartie"); // Cr�� le menu nouvelle partie.
	pGestionaireMenu->AjouterDonnee(new CMenu(false, { 433, 134, 500, 500 }, pWindowJeu->ObtenirRenderer(), 2, pGestionaireControl->ObtenirDonnee("pBtnResumer"), pGestionaireControl->ObtenirDonnee("pBtnQuitterGame")), "pMenuPause");

	// Ajoue des menus dans la fen�tre.
	pWindowJeu->AjouterMenu(3, pGestionaireMenu->ObtenirDonnee("pMenuPrincipal"), pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie"), pGestionaireMenu->ObtenirDonnee("pMenuPause"));

	boExecution = true;
	pEvent = new SDL_Event(); // Cr�� le pointeur.

}

// Proc�dure d�truisant les variables.
//
void Close(void) {

	// D�truit le gestionnaire d'�v�nement
	delete pEvent;
	delete pWindowJeu; // D�truit la fen�tre, donc tous les contr�les reli�s � la fen�tre.

	// Quitte les librairies...
	SDL_Quit();
	TTF_Quit();
}




int main(int argc, char* argv[]) {

	Start(argv[0]); // Initialisation

	srand(time(NULL));

	// Boucle principale de l'application.
	while (boExecution) {

		pWindowJeu->Rafraichir();

		// Tant qu'il y a des �v�nements � g�rer.
		while (SDL_PollEvent(pEvent)) {

			pGestionaireMenu->ObtenirDonnee("pMenuPrincipal")->ReactToEvent(pEvent);
			pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie")->ReactToEvent(pEvent);
			pGestionaireMenu->ObtenirDonnee("pMenuPause")->ReactToEvent(pEvent);
			if (pWindowJeu->ObtenirGame() != nullptr) {
				pWindowJeu->ObtenirGame()->ReactToEvent(pEvent);
			}

			switch (pEvent->type) {

			case SDL_QUIT:
				boExecution = false;
				break;

			case SDL_KEYDOWN:
				switch (pEvent->key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					if (pWindowJeu->ObtenirGame() != nullptr) {
						pGestionaireMenu->ObtenirDonnee("pMenuPause")->DefinirboShow(true);
					}
					else
						boExecution = !(pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE);
					break;
				case SDL_SCANCODE_T:
					//pWindowJeu->ObtenirGame()->ChangerTour(pGestionaireFont->ObtenirDonnee("pFontBouton"), pWindowJeu->ObtenirRenderer());
					pWindowJeu->ObtenirGame()->ReverseShowToolBar();
					break;
				}
				break;

			}
		}
	}

	Close(); // D�initialisation
	return 0;
}