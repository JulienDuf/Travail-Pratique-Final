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
		m_uiNombreButton = _uiNombreButton;
		// Rend le text en surface.
		SDL_Surface* pSDLSurfaceTmp; // Surface temporaire.

		pSDLSurfaceTmp = TTF_RenderText_Blended(_pSDLFont, _chrTexte, _SDLColor); // Avoir la surface du texte.

		// Créer les 2 surfaces en 1.
		SDL_BlitSurface(pSDLSurfaceTmp, NULL, _pSDLSurface, &m_pSDLRectSource); 

		// Delete surface temporaire du texte.
		SDL_FreeSurface(pSDLSurfaceTmp);

		// La texture du boutton avec le texte.
		m_pSDLTextureButton = SDL_CreateTextureFromSurface(_pSDLRenderer, _pSDLSurface);

		// Avoir les attribu des Rects.
		SDL_QueryTexture(m_pSDLTextureButton, NULL, NULL, &m_pSDLRectSource.w, &m_pSDLRectSource.h); // avoir la largueur et hauteur de la texture.
		// Largeur d'une image.
		m_pSDLRectSource.w = m_pSDLRectSource.w / _uiNombreButton;

		// La destination est le meme que la source.
		m_pSDLRectPositionButton.h = m_pSDLRectSource.h; // La hauteur.
		m_pSDLRectPositionButton.w = m_pSDLRectSource.w; // La destination est la même que la source.

		// L'image a laquel on est rendu dans le sprite ("L'état).
		m_uiButtonState = _uiNombreButton;

	}
};