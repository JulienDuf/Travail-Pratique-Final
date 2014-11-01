
// Inclusions...
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#include <iostream>
using namespace std;
#include "CArbreAVL.h"
#include "CListeDC.h"
#include "CControl.h"
#include "CLabel.h"
#include "CMenu.h"
#include "CWindow.h"
#include "CButton.h"
#include "CLabelLeftRight.h"
#include "CEvenement.h"

// Variables...
bool boExecution; // Variable de la boucle principale du programme.

SDL_Event* pEvent; // Les événemens du programme;

SDL_Surface* pSDLSurfaceBouton; // La surface d'un bouton.

CWindow* pWindowJeu; // La fenêtre de jeu.

TTF_Font* pFontBouton; // La font du texte des boutons.

SDL_Color CouleurTexte; // La couleur du texte.

CMenu* pMenuPrincipal; // Le menu principal du jeu.
CMenu* pMenuNouvellePartie; // Le menu créant une nouvelle partie.

CButton* pBtnNouvellePartie; // Le bouton pour aller vers le menu Nouvelle partie.
CButton* pBtnQuitter; // Le bouton pour quitter le jeu.

//
// Procédure initialisant les librairies et variables.
//
void Start(char* _strApplicationFilename){

	string strEmplacement;

	// Trouve l'emplacement du dossier debug.
	string strApplicationPath(_strApplicationFilename);
	int i = strApplicationPath.length();
	while (strApplicationPath[--i] != '\\');
	strApplicationPath.resize(++i);

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pSDLSurfaceBouton = IMG_Load(strEmplacement.c_str());

	strEmplacement = strApplicationPath;
	strEmplacement.append("ARCADECLASSIC.TTF");
	pFontBouton = TTF_OpenFont(strEmplacement.c_str(), 14);

	CouleurTexte = { 0, 0, 0, 0 };

	pWindowJeu = new CWindow(1024, 768); // Créé la fenêtre.

	pBtnNouvellePartie = new CButton("Nouvelle Partie", pFontBouton, CouleurTexte, pSDLSurfaceBouton, {50,50,1200,40}, 3, 2, pWindowJeu->ObtenirRenderer());
	pBtnQuitter = new CButton("Quitter", pFontBouton, CouleurTexte, pSDLSurfaceBouton, { 50, 150, 480, 40 }, 3, 0, pWindowJeu->ObtenirRenderer());


	pMenuPrincipal = new CMenu(true, 2, pBtnNouvellePartie, pBtnQuitter); // Crée le menu principal.
	pMenuNouvellePartie = new CMenu(false, 0); // Créé le menu nouvelle partie.

	pWindowJeu->AjouterMenu(2, pMenuPrincipal, pMenuNouvellePartie);

	boExecution = true;
	pEvent = new SDL_Event(); // Créé le pointeur.

}

//
// Procédure détialisant les librairies et détruisants les variables.
//
void Close(void) {

	delete pEvent;

	SDL_FreeSurface(pSDLSurfaceBouton);

	delete pBtnNouvellePartie;
	delete pBtnQuitter;

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

			// Selon les types d'événements.
			switch (pEvent->type) {

			// Si un des boutons de la souris est enfoncés.
			case SDL_MOUSEBUTTONDOWN:

				break;
			}
		}
	}

	Close(); // Déinitialisation
	return 0;
}