//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un label.
// Créé le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification fait par Julien Dufresne (dufresne_julien@hotmai.ca) le 28 octobre 2014
// Modifiée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
// Fin du codage de la classe.
//

class CLabel : public CControl {
private:

	SDL_Texture* m_pSDLTextureLabel; // La texture d'un label.
	SDL_Rect m_RectPosition; // La postion/destination qu'on veut l'afficher.

public:

	// Constructeurs de CLabel...
	// En entrée:
	// Param1: Le renderer de destination du Control.
	// Param2: Le texte qui sera présent dans le label.
	// Param3: La font du texte qui sera présent dans le label.
	// Param4: La couleur du texte qui sera présent dans le label.
	// Param5: La position du label dans la fenêtre.
	CLabel(SDL_Renderer* _pRenderer, string _strTexte, TTF_Font* _Font, SDL_Color _Couleur, SDL_Rect _RectPosition) {

		SDL_Surface* pSDLSurface; // Variable temporaire...

		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte.c_str(), _Couleur); // Créé la surface contenant le texte.

		// Met la surface en texture.
		m_pSDLTextureLabel = SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface); // Merci Xavier !!
		SDL_FreeSurface(pSDLSurface); // Détruit la surface.

		// Définie le rect de position...
		m_RectPosition = _RectPosition;
	}

	// Constructeur de CLabel...
	// En entrée:
	// Param1: La texture qui sera présente dans le label.
	// Param2: La position du label dans la fenêtre.
	CLabel(SDL_Texture* _pTextureLabel, SDL_Rect _RectPosition) {

		// Défini la texture...
		m_pSDLTextureLabel = _pTextureLabel;

		// Défini le rect de position...
		m_RectPosition = _RectPosition;
	}

	~CLabel() {

		SDL_DestroyTexture(m_pSDLTextureLabel);
	}

	// Procédure permettant d'afficher le label...
	// En entrée:
	// Param1: Le renderer de destination du Control.
	void ShowControl(SDL_Renderer* _pRenderer) {

		// Met la texture dans le renderer.
		SDL_RenderCopy(_pRenderer, m_pSDLTextureLabel, NULL, &m_RectPosition);

	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	// Sortie: Si le control à réagit
	void ReactToEvent(SDL_Event* _pSDLEvent) {
	

	}

};