
// Inclusions...
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;
#include "CArbreAVL.h"
#include "CListeDC.h"
#include "CTimer.h"
#include "CSprite.h"
#include "CControl.h"
#include "CButton.h"
#include "CLabel.h"
#include "CMenu.h"
#include "CBarrePuissance.h"
#include "CTool.h"
#include "CMissile.h"
#include "CPack.h"
#include "CHealthPack.h"
#include "CMine.h"
#include "CPlayer.h"
#include "CTeam.h"
#include "CVent.h"
#include "CMap.h"
#include "CGame.h"
#include "CWindow.h"
#include "CLabelLeftRight.h"
#include "CGestionaire.h"



// Variables...
bool boExecution; // Variable de la boucle principale du programme.

string strEmplacementFichier; // L'emplacement de nos fichiers.

SDL_Event* pEvent; // Les �v�nemens du programme;

CWindow* pWindowJeu; // La fen�tre de jeu.

CGestionaire<SDL_Texture*>* pGestionaireTexture; // Le gestionaire des textures.
CGestionaire<TTF_Font*>* pGestionaireFont; // Le gestionaire des fonts.
CGestionaire<CControl*>* pGestionaireControl; // Le gestionaire des controls.
CGestionaire<CMenu*>* pGestionaireMenu; // Le gestionaire des menus.

SDL_Color CouleurTexte; // La couleur du texte.

CGame* pGame; // La partie.

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

	pWindowJeu->CreateGame(strTmp, strEmplacementFichier, iNombreEquipe, iNombreJoueur, NULL, new CVent(pGestionaireFont->ObtenirDonnee("pFontBouton"), "250 km/h", CouleurTexte, pGestionaireTexture->ObtenirDonnee("pFlecheVent"), {1200, 30 , 117, 63}, 180, pWindowJeu->ObtenirRenderer()), pWindowJeu->ObtenirRenderer());
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
void Start(char* _strApplicationFilename){

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

	pGame = nullptr;

	// Cr�ation de la fen�tre...
	pWindowJeu = new CWindow(1366, 768);
	pWindowJeu->GetSize(&iW, &iH);
	
	pGestionaireTexture = new CGestionaire<SDL_Texture*>();
	pGestionaireFont = new CGestionaire<TTF_Font*>();
	pGestionaireControl = new CGestionaire<CControl*>();
	pGestionaireMenu = new CGestionaire<CMenu*>();

	// Chargement de font du texte des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("calibri.ttf");
	pGestionaireFont->AjouterDonnee(TTF_OpenFont(strEmplacement.c_str(), 30), "pFontBouton");

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


	// Cr�ation des labels...
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre d equipes", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 180, 520, 231, 32 }), "pLblNombreEquipe");
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre de joueurs par equipe", pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 100, 620, 407, 32 }), "pLblNombreJoueurEquipe");
	pGestionaireControl->AjouterDonnee(new CLabel(pWindowJeu->ObtenirRenderer(), { 900, 130, 500, 32 * 5 }), "pLblDescriptionMap");

	// Cr�ations des lableLeftRight...
	pGestionaireControl->AjouterDonnee(new CLabelLeftRight(pGestionaireFont->ObtenirDonnee("pFontBouton"), CouleurTexte, { 270, 550, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheGauche"), { 225, 554, 35, 35 }, 4, 1, NULL), new CButton(pGestionaireTexture->ObtenirDonnee("pFlecheDroite"), { 325, 554, 35, 35 }, 4, 1, NULL), 5, "2", "3", "4", "5", "6"), "pLblLRChoixNbrEquipe");
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


	// Ajoue des menus dans la fen�tre.
	pWindowJeu->AjouterMenu(2, pGestionaireMenu->ObtenirDonnee("pMenuPrincipal"), pGestionaireMenu->ObtenirDonnee("pMenuNouvellePartie"));

	boExecution = true;
	pEvent = new SDL_Event(); // Cr�� le pointeur.

}

//�truisants les variables.
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

			switch (pEvent->type) {

			case SDL_QUIT:
				boExecution = false;
				break;

			case SDL_KEYDOWN:
				boExecution = !(pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE);
				break;
			
			}
		}
	}

	Close(); // D�initialisation
	return 0;
}