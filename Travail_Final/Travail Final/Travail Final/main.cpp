
// Inclusions...
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#include <iostream>
using namespace std;
#include "CArbreAVL.h"
#include "CListeDC.h"
#include "CControl.h"
#include "CButton.h"
#include "CLabel.h"
#include "CMenu.h"
#include "CWindow.h"
#include "CLabelLeftRight.h"
#include "CEvenement.h"

// Variables...
bool boExecution; // Variable de la boucle principale du programme.

SDL_Event* pEvent; // Les événemens du programme;

CWindow* pWindowJeu; // La fenêtre de jeu.

SDL_Texture* pFlecheGauche;
SDL_Texture* pFlecheDroite;

TTF_Font* pFontBouton; // La font du texte des boutons.

SDL_Color CouleurTexte; // La couleur du texte.

CMenu* pMenuPrincipal; // Le menu principal du jeu.
CMenu* pMenuNouvellePartie; // Le menu créant une nouvelle partie.

CButton* pBtnNouvellePartie; // Le bouton pour aller vers le menu Nouvelle partie.
CButton* pBtnQuitter; // Le bouton pour quitter le jeu.
CButton* pBtnDebutPartie; // Le bouton pour commencer la partie.
CButton* pBtnRetour; // Le bouton pour retourn au menu principal.

CLabel* pLblNombreEquipe; // Le label étant écrit le mot " Nombre d'équipe ".
CLabel* pLblNombreJoueurEquipe; // Le label étant écrit le mot " Nombre de joueur par équipe ".
CLabel* pLblDescriptionMap; // Le label ou est inscrit la description de la map.

CLabelLeftRight* pLblLRChoixNbrEquipe; // Le labelLesftRight ou on choisi le nombre d'équipe.
CLabelLeftRight* pLblLRChoixNbrJoueurEquipe; // Le labelLesftRight ou on choisi le nombre de joueur par équipe.
CLabelLeftRight* pLblLRChoixMap; // Le labelLesftRight ou on choisi la map pour le jeu.


void ClickBoutonNouvellePartie(void) {

	pMenuNouvellePartie->DefinirboShow(true);
	pMenuPrincipal->DefinirboShow(false);
}

void ClickBoutonRetour(void) {

	pMenuNouvellePartie->DefinirboShow(false);
	pMenuPrincipal->DefinirboShow(true);
}

void ClickBoutonDebutPartie(void) {

	pMenuNouvellePartie->DefinirboShow(false);
	pMenuPrincipal->DefinirboShow(false);
}

void ClickBoutonQuitter(void) {

	boExecution = false;
}


// Procédure qui transforme les deux images d'une map en une image.
// Param1: L'emplacement de l'image.
// Param2: Le nom du dossier ou sont les images.
// Param3: La texture ou sera la texture finale.
void PutMapToTexture(string _strEmplacement, string _NomDossier, SDL_Texture** _SDLTextureStockage) {

	// Variables temporaires...
	string strEmplacement;
	SDL_Surface* pSDLSurfaceTmp;
	SDL_Surface* pSDLSurfaceCopie;
	SDL_Rect RectTmp = { 0, 0, 0, 0 };

	// Ajoute le nom du dossier Maps dans la variable d'emplacement des images.
	_strEmplacement.append("Maps\\");

	// Téléchargement des images
	strEmplacement = _strEmplacement;
	strEmplacement.append(_NomDossier + "\\background.jpg");
	pSDLSurfaceCopie = IMG_Load(strEmplacement.c_str());
	strEmplacement = _strEmplacement;
	strEmplacement.append(_NomDossier + "\\map.png");
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

	// Création de la fenêtre...
	pWindowJeu = new CWindow(1366, 768);
	pWindowJeu->GetSize(&iW, &iH);
	
	// Chargement de font du texte des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("ARCADECLASSIC.TTF");
	pFontBouton = TTF_OpenFont(strEmplacement.c_str(), 30);

	// Chargements de la texture pour le bouton droit d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheDroite.png");
	pFlecheDroite = IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str());

	// Chargements de la texture pour le bouton gauche d'un LabelLeftRight.
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\FlecheGauche.png");
	pFlecheGauche = IMG_LoadTexture(pWindowJeu->ObtenirRenderer(), strEmplacement.c_str());

	CouleurTexte = { 0, 0, 0, 0 }; // Met la couleur noir.

	// Créé les boutons avec l'emplacement de l'image des boutons...
	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pBtnNouvellePartie = new CButton("Nouvelle    Partie", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 250, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonNouvellePartie);
	pBtnQuitter = new CButton("Quitter", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 320, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonQuitter);
	pBtnDebutPartie = new CButton("Debuter     la      partie", pFontBouton, CouleurTexte, strEmplacement.c_str(), { 790, 530, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonDebutPartie);
	pBtnRetour = new CButton("Retour", pFontBouton, CouleurTexte, strEmplacement.c_str(), { 790, 600, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer(), ClickBoutonRetour);

	// Création des labels...
	pLblNombreEquipe = new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre d equipes", pFontBouton, CouleurTexte, { 180, 520, 231, 32 });
	pLblNombreJoueurEquipe = new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre de joueurs par equipe", pFontBouton, CouleurTexte, { 100, 620, 407, 32 });

	// Créations des lableLeftRight...
	pLblLRChoixNbrEquipe = new CLabelLeftRight(pFontBouton, CouleurTexte, { 270, 550, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pFlecheGauche, {225, 554, 35, 35}, 4, 1, NULL), new CButton(pFlecheDroite, {325, 554, 35, 35}, 4, 1, NULL), 5, "2", "3", "4", "5", "6");
	pLblLRChoixNbrJoueurEquipe = new CLabelLeftRight(pFontBouton, CouleurTexte, { 270, 650, 50, 50 }, pWindowJeu->ObtenirRenderer(), new CButton(pFlecheGauche, {225, 654, 35, 35 }, 4, 1, NULL), new CButton(pFlecheDroite, { 325, 654, 35, 35 }, 4, 1, NULL), 3, "4", "5", "6");
	pLblLRChoixMap = new CLabelLeftRight({120, 50, 623, 367}, new CButton(pFlecheGauche, { 20, 199, 80, 80 }, 4, 1, NULL), new CButton(pFlecheDroite, { 760, 199, 80, 80 }, 4, 1, NULL), 0);
	 
	// Met les map en une texture...
	PutMapToTexture(strApplicationPath, "arcaderoom", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "country", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "desert", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "easterisland", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "farm", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "pirates", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;

	PutMapToTexture(strApplicationPath, "snow", &pTextureTmp);
	pLblLRChoixMap->AjouterControl(1, pTextureTmp);
	pTextureTmp = nullptr;
	//......


	// Création des menus...
	pMenuPrincipal = new CMenu(true, 2, pBtnNouvellePartie, pBtnQuitter); // Crée le menu principal.
	pMenuNouvellePartie = new CMenu(false, 7, pBtnDebutPartie, pBtnRetour, pLblNombreJoueurEquipe, pLblNombreEquipe, pLblLRChoixNbrEquipe, pLblLRChoixNbrJoueurEquipe, pLblLRChoixMap); // Créé le menu nouvelle partie.

	// Ajoue des menus dans la fenêtre.
	pWindowJeu->AjouterMenu(2, pMenuPrincipal, pMenuNouvellePartie);

	boExecution = true;
	pEvent = new SDL_Event(); // Créé le pointeur.

}

//étruisants les variables.
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

	// Boucle principale de l'application.
	while (boExecution) {

		pWindowJeu->Rafraichir();

		// Tant qu'il y a des événements à gérer.
		while (SDL_PollEvent(pEvent)) {

			pMenuPrincipal->ReactToEvent(pEvent);
			pMenuNouvellePartie->ReactToEvent(pEvent);

		}
	}

	Close(); // Déinitialisation
	return 0;
}