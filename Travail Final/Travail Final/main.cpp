
// Inclusions...
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_image.h>
#include <iostream>
using namespace std;
#include "CControl.h"
#include "CLabel.h"
#include "CMenu.h"
#include "CWindow.h"
#include "CButton.h"

// Variables
bool boExecution; // Variable de la boucle principale du programme.

SDL_Event* pEvent; // Les événemens du programme;

//
// Procédure initialisant les librairies et variables.
//
void Start(void) {

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	boExecution = true;
	pEvent = new SDL_Event(); // Créé le pointeur.
}

//
// Procédure détialisant les librairies et détruisants les variables.
//
void Close(void) {

	delete pEvent;

	SDL_Quit();
	TTF_Quit();
}


int main(int argc, char* argv[]) {

	Start(); // Initialisation

	// Boucle principale de l'application.
	while (boExecution) {

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