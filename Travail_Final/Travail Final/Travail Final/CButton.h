//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un boutton.
// Créé le 29 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//
class CButton {
private:

	SDL_Texture* m_pSDLTextureButton; // La texture du button.
	SDL_Rect m_pSDLRectPositionButton; // Position qu'on veut l'afficher.
	unsigned int m_uiNombreButton; // Le nombre de button dans le sprite.
	SDL_Rect m_pSDLRectSource; // La source du sprite.
	unsigned int m_uiButtonState; // À quel sprite on est rendu.

public:

	CButton(char* _chrTexte, TTF_Font* _pSDLFont, SDL_Color _SDLColor, SDL_Surface* _pSDLSurface, SDL_Rect _SDLRectPosition, unsigned int _uiNombreButton, unsigned int _uiState, SDL_Renderer* _pSDLRenderer) {
		
		// Nombre de boutton dans le sprite.
		m_uiNombreButton = _uiNombreButton;
		// Rend le text en surface.
		SDL_Surface* pSDLSurfaceTmp; // Surface temporaire.

		pSDLSurfaceTmp = TTF_RenderText_Blended(_pSDLFont, _chrTexte, _SDLColor); // Avoir la surface du texte.
		
		SDL_Rect SDLRectText = {0,0,0,0}; // Rect du texte.
		
		// La destination est le meme que la source.
		m_pSDLRectPositionButton = _SDLRectPosition;
		m_pSDLRectSource = m_pSDLRectPositionButton;
		// Largeur d'une image.
		m_pSDLRectSource.w = m_pSDLRectSource.w / _uiNombreButton;
		m_pSDLRectPositionButton.w = m_pSDLRectSource.w;

		// Pour centrer le texte sur les bouttons.

		SDLRectText.x = (m_pSDLRectSource.w - pSDLSurfaceTmp->w) / 2;
		SDLRectText.y = (m_pSDLRectSource.w - pSDLSurfaceTmp->h) / 2;

		// Faire un for...
		for (int i = 0; i < _uiNombreButton; i++) {
			// Créer les 2 surfaces en 1.
			SDL_BlitSurface(pSDLSurfaceTmp, NULL, _pSDLSurface, &SDLRectText);
			SDLRectText.x += m_pSDLRectSource.w;
		}

		

		// Delete surface temporaire du texte.
		SDL_FreeSurface(pSDLSurfaceTmp);

		// La texture du boutton avec le texte.
		m_pSDLTextureButton = SDL_CreateTextureFromSurface(_pSDLRenderer, _pSDLSurface);

		// L'image a laquel on est rendu dans le sprite ("L'état).
		m_uiButtonState = _uiNombreButton;

	}
	//
	// Constructeur pour un boutton qui n'a pas de texte.
	// Paramètres : _pSDLTexture : Texture du bouton, _SDLRectDestination : La destination de la texture, _uiNombreButton : Le nombre de button dans le sprite, _uiState : L'état (quel bouton).
	CButton(SDL_Texture* _pSDLTexture, SDL_Rect _SDLRectDestination, unsigned int _uiNombreButton, unsigned int _uiState) {
		// La texture du button.
		m_pSDLTextureButton = _pSDLTexture;

		//
		m_uiButtonState = _uiState;
		m_uiNombreButton = _uiNombreButton;


		// Position du rect.
		m_pSDLRectPositionButton = _SDLRectDestination;
		m_pSDLRectSource.w = (m_pSDLRectSource.w / _uiNombreButton); // La largeur d'une image.
		m_pSDLRectSource.x = m_pSDLRectSource.w * _uiState; // Pour afficher le bon état.
		m_pSDLRectSource.y = 0;
		m_pSDLRectPositionButton.w = m_pSDLRectPositionButton.w / _uiNombreButton; // La largeur de la destination.


	}
	
	// Procédure qui affiche le boutton.
	// Paramètre : _pSDLRenderer : Le renderer de la fdestination du controle.
	void ShowControl(SDL_Renderer* _pSDLRenderer) {
		// Met la texture dans le renderer.
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureButton, &m_pSDLRectSource, &m_pSDLRectPositionButton);
	}
	//
	// Procédure qui permet de changer l'état du boutton.
	// Paramètre : 
	void ModifyButtonState(unsigned int _uiState) {
		m_pSDLRectSource.x = m_pSDLRectSource.w * _uiState; // Aussi simple que ca ?
		
	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	// Sortie: Si le control à réagit
	bool ReactToEvent(SDL_Event* _pSDLEvent) {
		
	}
};