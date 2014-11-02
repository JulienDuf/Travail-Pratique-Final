
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

SDL_Event* pEvent; // Les �v�nemens du programme;

CWindow* pWindowJeu; // La fen�tre de jeu.

TTF_Font* pFontBouton; // La font du texte des boutons.

SDL_Color CouleurTexte; // La couleur du texte.

CMenu* pMenuPrincipal; // Le menu principal du jeu.
CMenu* pMenuNouvellePartie; // Le menu cr�ant une nouvelle partie.

CButton* pBtnNouvellePartie; // Le bouton pour aller vers le menu Nouvelle partie.
CButton* pBtnQuitter; // Le bouton pour quitter le jeu.

//
// Proc�dure initialisant les librairies et variables.
//
void Start(char* _strApplicationFilename){

	string strEmplacement;
	int iW, iH;

	// Trouve l'emplacement du dossier debug.
	string strApplicationPath(_strApplicationFilename);
	int i = strApplicationPath.length();
	while (strApplicationPath[--i] != '\\');
	strApplicationPath.resize(++i);

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	strEmplacement = strApplicationPath;
	strEmplacement.append("ARCADECLASSIC.TTF");
	pFontBouton = TTF_OpenFont(strEmplacement.c_str(), 30);

	CouleurTexte = { 0, 0, 0, 0 };

	pWindowJeu = new CWindow(1024, 768); // Cr�� la fen�tre.
	pWindowJeu->GetSize(&iW, &iH);

	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pBtnNouvellePartie = new CButton("Nouvelle    Partie", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 250, 1500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer());
	pBtnQuitter = new CButton("Quitter", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 320, 1500, 60 }, 3, 2, pWindowJeu->ObtenirRenderer());


	pMenuPrincipal = new CMenu(true, 2, pBtnNouvellePartie, pBtnQuitter); // Cr�e le menu principal.
	pMenuNouvellePartie = new CMenu(false, 0); // Cr�� le menu nouvelle partie.

	pWindowJeu->AjouterMenu(2, pMenuPrincipal, pMenuNouvellePartie);

	boExecution = true;
	pEvent = new SDL_Event(); // Cr�� le pointeur.

}

//
// Proc�dure d�tialisant les librairies et d�truisants les variables.
//
void Close(void) {

	delete pEvent;
	delete pWindowJeu;

	SDL_Quit();
	TTF_Quit();
}


int main(int argc, char* argv[]) {

	Start(argv[0]); // Initialisation

	// Boucle principale de l'application.
	while (boExecution) {

		pWindowJeu->Rafraichir();

		// Tant qu'il y a des �v�nements � g�rer.
		while (SDL_PollEvent(pEvent)) {

			// Selon les types d'�v�nements.
			switch (pEvent->type) {

			// Si un des boutons de la souris est enfonc�s.
			case SDL_MOUSEBUTTONDOWN:

				break;

			case SDL_KEYDOWN:
				if (pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					boExecution = false;
				break;
			}
		}
	}

	Close(); // D�initialisation
	return 0;
}