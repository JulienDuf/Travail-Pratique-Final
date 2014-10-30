//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant un boutton.
// Cr�� le 29 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//
class CButton {
private:

	SDL_Texture* m_pSDLTextureButton; // La texture du button.
	SDL_Rect m_pSDLRectPositionButton; // Position qu'on veut l'afficher.
	unsigned int m_uiNombreButton; // Le nombre de button dans le sprite.
	SDL_Rect m_pSDLRectSource; // La source du sprite.
	unsigned int m_uiButtonState; // � quel sprite on est rendu.
public:

	CButton(char* _chrTexte, TTF_Font* _pSDLFont, SDL_Color _SDLColor, SDL_Surface* _pSDLSurface, SDL_Rect _SDLRectPosition, unsigned int _uiNombreButton, unsigned int _uiState, SDL_Renderer* _pSDLRenderer) {
		
		/*
		C'est tr�s bon, mais il faut faire un for avec le SDL_BlitSurface, car en ce moment le texte se met seulement sur un �tat de 
		bouton.
		Attention !! : Tu utilise le m_pSDLRectSource sans l'avoir initialis� !!
		Note : Dans le for, il va falloir que modifie le x du rect source pour que le texte ne soit pas toujours mit � la m�me place.
		*/
		
		/*
		Autre chose : Un certain Gabriel pr�f�re que nos w et nos h soit d�fini dans le main, donc les grandeurs ne respecteront pas 
		n�cessairement les grandeurs r�elles de la texture, donc pas de SDL_QueryTexture. Il faut plut�t utiliser le _SDLRectPosition.
		Derni�re chose, tu n'initialise pas les x et les y des rect.
		*/
		
		/*
		Je sais qu je t'ai �crit beaucoup de chose, mais ta logique pour le constructeur est tr�s bonne, il y a juste quelque que manquement.
		*/
		m_uiNombreButton = _uiNombreButton;
		// Rend le text en surface.
		SDL_Surface* pSDLSurfaceTmp; // Surface temporaire.

		pSDLSurfaceTmp = TTF_RenderText_Blended(_pSDLFont, _chrTexte, _SDLColor); // Avoir la surface du texte.

		// Faire un for...

		// Cr�er les 2 surfaces en 1.
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
		m_pSDLRectPositionButton.w = m_pSDLRectSource.w; // La destination est la m�me que la source.

		// L'image a laquel on est rendu dans le sprite ("L'�tat).
		m_uiButtonState = _uiNombreButton;

	}
};