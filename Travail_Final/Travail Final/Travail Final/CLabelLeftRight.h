/*
Classe Gérant les Labels "Left Right"
Créée le 30 octobre par Samuel Rambaud (Sam.Rambaud@outlook.com)
*/

class CLabelLeftRight {
private:

	CListeDC<SDL_Texture*>* m_pListeTexture;
	SDL_Rect m_SDLRectLabel;
	CButton* m_pButtonLeft; // Bouton de droite du controle.
	CButton* m_pButtonRight; // Bouton de gauche du controle.

public:

	/*
	Contructeur de Label left right pour du texte...
	parametres:
	_pListeObjet : Donner des string à faire afficher dans un labelleftright (à détruire si inutille après).
	_pTTF_FontText : Font du text.
	_SDLColorText : Couleur du texte.
	_SDLRectLabel : Position du label.
	_pSLDRenderer : Renderer
	_pButtonRight : Bouton de droite.
	_pButtonLeft : Bouton de gauche.
	*/
	CLabelLeftRight(CListeDC<char*>* _pListechar, TTF_Font* _pTTF_FontText, SDL_Color _SDLColorText, SDL_Rect _SDLRectLabel, SDL_Renderer* _pSDLRenderer, CButton* _pButtonLeft, CButton* _pButtonRight) {
		_pListechar->AllerDebut();
		SDL_Surface* pSDLSurfacetmp;
		m_pListeTexture = new CListeDC<SDL_Texture*>();
		m_SDLRectLabel = _SDLRectLabel;
		for (int i = 0; i < _pListechar->ObtenirCompte(); i++) {
			pSDLSurfacetmp = TTF_RenderText_Blended(_pTTF_FontText, _pListechar->ObtenirElementCurseur(), _SDLColorText);
			m_pListeTexture->AjouterFin(SDL_CreateTextureFromSurface(_pSDLRenderer, pSDLSurfacetmp));
			SDL_FreeSurface(pSDLSurfacetmp);
			_pListechar->AllerSuivantCurseur();
		}
		m_pButtonLeft = _pButtonLeft;
		m_pButtonRight = _pButtonRight;
	}

	/*
	Construteur de Label left right pour des textures...
	En entrée:
		Param1: Liste des textures à faire afficher(à ne pas détruire).
		Param2; Position du label.
		Param3; Bouton de droite.
		Param4; Bouton de Gauche.
	*/
	CLabelLeftRight(CListeDC<SDL_Texture*>* _pListeTexture, SDL_Rect _SDLRectLabel, CButton* _pButtonLeft, CButton* _pButtonRight) {
		m_pListeTexture = _pListeTexture;
		m_pListeTexture->AllerDebut();
		m_SDLRectLabel = _SDLRectLabel;
		m_pButtonLeft = _pButtonLeft;
		m_pButtonRight = _pButtonRight;
	}
	/*
	Affiche les controles visuels
	parametres:
	_Renderer: Renderer ou s'affichera le controle
	*/
	void ShowControl(SDL_Renderer* _pSDLRenderer) {
		m_pButtonLeft->ShowControl(_pSDLRenderer);
		m_pButtonRight->ShowControl(_pSDLRenderer);
		// Met la texture dans le renderer.
		SDL_RenderCopy(_pSDLRenderer, m_pListeTexture->ObtenirElementCurseur(), NULL, &m_SDLRectLabel);
	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	CControl* ReactToEvent(SDL_Event* _pSDLEvent) {

	}

	//Accesseurs

	CButton* GetButtonLeft(void) {
		return m_pButtonLeft;
	}

	CButton* GetButtonRight(void) {
		return m_pButtonRight;
	}
};