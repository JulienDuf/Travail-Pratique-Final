﻿//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un boutton.
// Créé le 29 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//
class CButton : public CControl{
private:

	SDL_Texture* m_pSDLTextureButton; // La texture du button.
	SDL_Rect m_SDLRectPositionButton; // Position qu'on veut l'afficher.
	unsigned int m_uiNombreButton; // Le nombre de button dans le sprite.
	SDL_Rect m_SDLRectSource; // La source du sprite.
	unsigned int m_uiButtonState; // À quel sprite on est rendu ( =0 quand il est accessible, =1 quand la souris est au dessus, =2 quand il est clické ).
	void(*m_pProcedureClic)(void); // La procédure qui détermine quoi faire lors d'un clic.

public:

	CButton(char* _chrTexte, TTF_Font* _pSDLFont, SDL_Color _SDLColor, string _strEmplacement, SDL_Rect _SDLRectPosition, unsigned int _uiNombreButton, unsigned int _uiState, SDL_Renderer* _pSDLRenderer, void _ProcedureClic(void)) {
		
		// Nombre de boutton dans le sprite.
		m_uiNombreButton = _uiNombreButton;

		// Rend le text en surface.
		SDL_Surface* pSDLSurfaceTmp; // Surface temporaire.
		SDL_Surface* pSDLSurfaceTexte; // Surface du texte.

		pSDLSurfaceTexte = TTF_RenderText_Blended(_pSDLFont, _chrTexte, _SDLColor); // Avoir la surface du texte.
		pSDLSurfaceTmp = IMG_Load(_strEmplacement.c_str());

		SDL_Rect SDLRectText = {0,0,0,0}; // Rect du texte.
		
		// La destination est le meme que la source.
		m_SDLRectPositionButton = _SDLRectPosition;
		m_SDLRectSource = m_SDLRectPositionButton;

		// Largeur d'une image.
		m_SDLRectSource.w = pSDLSurfaceTmp->w;
		m_SDLRectSource.h = pSDLSurfaceTmp->h;
		m_SDLRectSource.w = m_SDLRectSource.w / m_uiNombreButton;
		m_SDLRectSource.y = 0;

		// Pour centrer le texte sur les bouttons.

		SDLRectText.x = (m_SDLRectSource.w - pSDLSurfaceTexte->w) / 2;
		SDLRectText.y = (m_SDLRectSource.h - pSDLSurfaceTexte->h) / 2;

		// Faire un for...
		for (int i = 0; i < _uiNombreButton; i++) {
			// Créer les 2 surfaces en 1.
			SDL_BlitSurface(pSDLSurfaceTexte, NULL, pSDLSurfaceTmp, &SDLRectText);
			SDLRectText.x += m_SDLRectSource.w;
		}

		// La texture du boutton avec le texte.
		m_pSDLTextureButton = SDL_CreateTextureFromSurface(_pSDLRenderer, pSDLSurfaceTmp);

		// Delete surface temporaire du texte.
		SDL_FreeSurface(pSDLSurfaceTmp);
		SDL_FreeSurface(pSDLSurfaceTexte);

		// L'image a laquel on est rendu dans le sprite ("L'état).
		m_uiButtonState = _uiState;

		ModifyButtonState(_uiState);

		m_pProcedureClic = _ProcedureClic;

	}
	//
	// Constructeur pour un boutton qui n'a pas de texte.
	// Paramètres : _pSDLTexture : Texture du bouton, _SDLRectDestination : La destination de la texture, _uiNombreButton : Le nombre de button dans le sprite, _uiState : L'état (quel bouton).
	CButton(SDL_Texture* _pSDLTexture, SDL_Rect _SDLRectDestination, unsigned int _uiNombreButton, unsigned int _uiState, void _ProcedureClic(void)) {
		// La texture du button.
		m_pSDLTextureButton = _pSDLTexture;

		//
		m_uiButtonState = _uiState;
		m_uiNombreButton = _uiNombreButton;

		m_SDLRectSource = _SDLRectDestination;
		m_SDLRectSource.y = 0;
		SDL_QueryTexture(m_pSDLTextureButton, NULL, NULL, &m_SDLRectSource.w, &m_SDLRectSource.h);
		m_SDLRectSource.w = m_SDLRectSource.w / m_uiNombreButton;
		ModifyButtonState(m_uiButtonState);

		// Position du rect.
		m_SDLRectPositionButton = _SDLRectDestination;

		m_pProcedureClic = _ProcedureClic;

	}
	

	~CButton() {

		SDL_DestroyTexture(m_pSDLTextureButton);
	}
	// Procédure qui affiche le boutton.
	// Paramètre : _pSDLRenderer : Le renderer de la fdestination du controle.
	void ShowControl(SDL_Renderer* _pSDLRenderer) {
		// Met la texture dans le renderer.
		SDL_RenderCopy(_pSDLRenderer, m_pSDLTextureButton, &m_SDLRectSource, &m_SDLRectPositionButton);
	}
	//
	// Procédure qui permet de changer l'état du boutton.
	// Paramètre : _uiState : Variable qui indique l'état auquel on veut modifier le bouton.
	void ModifyButtonState(unsigned int _uiState) {
		m_SDLRectSource.x = m_SDLRectSource.w * _uiState; // Aussi simple que ca ?
		m_uiButtonState = _uiState;
		
	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	// Sortie: Si le control à réagit
	void ReactToEvent(SDL_Event* _pSDLEvent) {

		// Si le curseur est sur le bouton...
		switch (_pSDLEvent->type) {

		case SDL_MOUSEMOTION:	// Hover
			if ((m_SDLRectPositionButton.x <= _pSDLEvent->motion.x && m_SDLRectPositionButton.x + m_SDLRectPositionButton.w >= _pSDLEvent->motion.x) && (m_SDLRectPositionButton.y <= _pSDLEvent->motion.y && m_SDLRectPositionButton.y + m_SDLRectPositionButton.h >= _pSDLEvent->motion.y))
				ModifyButtonState(1);
			else
				ModifyButtonState(0);
			break;

		case SDL_MOUSEBUTTONDOWN:	// Click
			if ((m_SDLRectPositionButton.x <= _pSDLEvent->motion.x && m_SDLRectPositionButton.x + m_SDLRectPositionButton.w >= _pSDLEvent->motion.x) && (m_SDLRectPositionButton.y <= _pSDLEvent->motion.y && m_SDLRectPositionButton.y + m_SDLRectPositionButton.h >= _pSDLEvent->motion.y)) {
				ModifyButtonState(2);
			}
			break;

		case SDL_MOUSEBUTTONUP:		// Unclick
			if ((m_SDLRectPositionButton.x <= _pSDLEvent->motion.x && m_SDLRectPositionButton.x + m_SDLRectPositionButton.w >= _pSDLEvent->motion.x) && (m_SDLRectPositionButton.y <= _pSDLEvent->motion.y && m_SDLRectPositionButton.y + m_SDLRectPositionButton.h >= _pSDLEvent->motion.y)) {
				ModifyButtonState(1);
				if (m_pProcedureClic != nullptr)
					m_pProcedureClic();
			}
			break;

		}
	}

	void AjouterTexture(int argc, ...) {}

	void ChangeTexture(bool _boDroite) {}

	void SetRectDestinationX(unsigned int _uiX) {
		m_SDLRectPositionButton.x = _uiX;
	}

	void SetRectDestinationY(unsigned int _uiY) {
		m_SDLRectPositionButton.y = _uiY;
	}

	// Accesseurs...

	int ObtenirElement(string _Element) { return 0; }

	unsigned int ObtenirButtonState(void) {

		return m_uiButtonState;
	}

};