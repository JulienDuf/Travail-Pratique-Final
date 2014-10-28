//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant une fenêtre.
// Créé le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification fait par Julien Dufresne (dufresne_julien@hotmai.ca) le 28 octobre 2014
// Fin du codage de la classe.
//
class CLabel {
private :
	SDL_Texture* m_pSDLTextureLabel; // La texture d'un label.
	SDL_Rect m_RectPosition; // La postion/destination qu'on veut l'afficher.

public:

	// Constructeurs de Menu...

	// En entrée:
	// Param1: Le renderer de destination du menu.
	// Param2: Le texte qui sera présent dans le label
	// Param3: La font du texte qui sera présent dans le label.
	// Param4: La couleur du texte qui sera présent dans le label.
	// Param5: La position du label dans la fenêtre.
	CLabel(SDL_Renderer* _pRenderer, string _strTexte, TTF_Font* _Font, SDL_Color _Couleur, SDL_Rect _RectPosition) {

		SDL_Surface* pSDLSurface; // Variable temporaire...

		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte.c_str(), _Couleur); // Créé la surface contenant le texte.

		// Définie le rect de position...
		m_RectPosition = _RectPosition;
		m_RectPosition.w = pSDLSurface->w;
		m_RectPosition.h = pSDLSurface->h;

		// Met la surface en texture.
		SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface);
		SDL_FreeSurface(pSDLSurface); // Détruit la surface.
	}

	// En entrée:
	// Param1: La texture qui sera présente dans le label
	// Param2: La position du label dans la fenêtre.
	CLabel(SDL_Texture* _pTextureLabel, SDL_Rect _RectPosition) {

		int iw, ih; // Variables temporaires...

		// Défini la texture...
		m_pSDLTextureLabel = _pTextureLabel;

		// Défini le rect de position...
		m_RectPosition = _RectPosition;
		SDL_QueryTexture(m_pSDLTextureLabel, NULL, NULL, &iw, &ih);
		m_RectPosition.w = iw;
		m_RectPosition.h = ih;
	}

	// Porcédure permettant d'afficher le label...
	// En entrée:
	// Param1: Le renderer de destination du menu.
	void ShowControl(SDL_Renderer* _pRenderer) {

		// Met la texture dans le renderer.
		SDL_RenderCopy(_pRenderer, m_pSDLTextureLabel, NULL, &m_RectPosition);

	}

};