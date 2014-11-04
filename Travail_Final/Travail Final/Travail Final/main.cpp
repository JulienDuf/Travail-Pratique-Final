
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
CButton* pBtnDebutPartie; // Le bouton pour commencer la partie.
CButton* pBtnRetour; // Le bouton pour retourn au menu principal.

CLabel* pLblNombreEquipe; // Le label �tant �crit le mot " Nombre d'�quipe ".
CLabel* pLblNombreJoueurEquipe; // Le label �tant �crit le mot " Nombre de joueur par �quipe ".
CLabel* pLblDescriptionMap; // Le label ou est inscrit la description de la map.

CLabelLeftRight* pLblLRChoixNbrEquipe; // Le labelLesftRight ou on choisi le nombre d'�quipe.
CLabelLeftRight* pLblLRChoixNbrJoueurEquipe; // Le labelLesftRight ou on choisi le nombre de joueur par �quipe.
CLabelLeftRight* pLblLRChoixMap; // Le labelLesftRight ou on choisi la map pour le jeu.


//
// Proc�dure initialisant les librairies et variables.
//
void Start(char* _strApplicationFilename){

	string strEmplacement; //
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

	pWindowJeu = new CWindow(1366, 768); // Cr�� la fen�tre.
	pWindowJeu->GetSize(&iW, &iH);

	strEmplacement = strApplicationPath;
	strEmplacement.append("Bouton\\BoutonMettreTexte.png");
	pBtnNouvellePartie = new CButton("Nouvelle    Partie", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 250, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer());
	pBtnQuitter = new CButton("Quitter", pFontBouton, CouleurTexte, strEmplacement.c_str(), { (iW - 500) / 2, 320, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer());
	pBtnDebutPartie = new CButton("D�buter la parite", pFontBouton, CouleurTexte, strEmplacement.c_str(), { 700, 700, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer());
	pBtnRetour = new CButton("Retour", pFontBouton, CouleurTexte, strEmplacement.c_str(), { 700, 770, 500, 60 }, 3, 0, pWindowJeu->ObtenirRenderer());

	pLblNombreEquipe = new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre d'�quipe", pFontBouton, CouleurTexte, { 100, 600, 150, 30 });
	pLblNombreJoueurEquipe = new CLabel(pWindowJeu->ObtenirRenderer(), "Nombre joueur par �quipe", pFontBouton, CouleurTexte, { 100, 600, 200, 30 });
	pLblDescriptionMap = new CLabel(pWindowJeu->ObtenirRenderer(), " ", pFontBouton, CouleurTexte, {700, 100, 500, 500});

	pMenuPrincipal = new CMenu(true, 2, pBtnNouvellePartie, pBtnQuitter); // Cr�e le menu principal.
	pMenuNouvellePartie = new CMenu(false, 5, pBtnDebutPartie, pBtnRetour, pLblDescriptionMap, pLblNombreJoueurEquipe, pLblNombreEquipe); // Cr�� le menu nouvelle partie.

	pWindowJeu->AjouterMenu(2, pMenuPrincipal, pMenuNouvellePartie);

	boExecution = true;
	pEvent = new SDL_Event(); // Cr�� le pointeur.

}

//�truisants les variables.
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

			CControl* pReacttmp;

			switch (pMenuPrincipal->ReactToEvent(pEvent, )) {
			case MouseButtonDown:
				break;

			}
			
		}
	}

	Close(); // D�initialisation
	return 0;
}