/*
Classe Gérant les Labels "Left Right"
Créée le 30 octobre par Samuel Rambaud (Sam.Rambaud@outlook.com)
*/

class CLabelLeftRight : public CControl{
private:

	CListeDC<SDL_Texture*>* m_pListeTexture;
	SDL_Rect m_SDLRectLabel;
	CButton* m_pButtonLeft; // Bouton de droite du controle.
	CButton* m_pButtonRight; // Bouton de gauche du controle.

	int m_iPositionLabel; // La position dans la liste.

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
	CLabelLeftRight(TTF_Font* _pTTF_FontText, SDL_Color _SDLColorText, SDL_Rect _SDLRectLabel, SDL_Renderer* _pSDLRenderer, CButton* _pButtonLeft, CButton* _pButtonRight, int argc, ...) {
		SDL_Surface* pSDLSurfacetmp;
		m_pListeTexture = new CListeDC<SDL_Texture*>();
		m_SDLRectLabel = _SDLRectLabel;
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				pSDLSurfacetmp = TTF_RenderText_Blended(_pTTF_FontText, va_arg(parametres, char*), _SDLColorText);
				m_pListeTexture->AjouterFin(SDL_CreateTextureFromSurface(_pSDLRenderer, pSDLSurfacetmp));
				SDL_FreeSurface(pSDLSurfacetmp);
			}
			va_end(parametres);
		}

		m_pListeTexture->AllerDebut();

		m_pButtonLeft = _pButtonLeft;
		m_pButtonRight = _pButtonRight;

		m_iPositionLabel = 0;
	}

	/*
	Construteur de Label left right pour des textures...
	En entrée:
		Param1: Liste des textures à faire afficher(à ne pas détruire).
		Param2; Position du label.
		Param3; Bouton de droite.
		Param4; Bouton de Gauche.
	*/
	CLabelLeftRight(SDL_Rect _SDLRectLabel, CButton* _pButtonLeft, CButton* _pButtonRight, int argc, ...) {
		//m_pListeTexture = _pListeTexture;
		//m_pListeTexture->AllerDebut();
		m_pListeTexture = new CListeDC<SDL_Texture*>();

		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pListeTexture->AjouterFin(va_arg(parametres, SDL_Texture*));
			}
			va_end(parametres);
		}

		m_pListeTexture->AllerDebut();

		m_SDLRectLabel = _SDLRectLabel;
		m_pButtonLeft = _pButtonLeft;
		m_pButtonRight = _pButtonRight;

		m_iPositionLabel = 0;
	}
	
	~CLabelLeftRight() {

		delete m_pButtonLeft;
		delete m_pButtonRight;

		m_pListeTexture->DetruireListeTexture();
		delete m_pListeTexture;
	}
	
	// Procédure permettant d'ajouter des textures à la liste.
	// En entrée:
	// Param1; Nombre de textures à ajouter.
	// Param2...; Texture à ajouter.
	void AjouterTexture(int argc, ...) {
		
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pListeTexture->AjouterFin(va_arg(parametres, SDL_Texture*));
			}
			va_end(parametres);

			m_pListeTexture->AllerDebut();
		}
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
	// Sortie: Si le control à réagit
	void ReactToEvent(SDL_Event* _pSDLEvent) {
		
		m_pButtonLeft->ReactToEvent(_pSDLEvent);
		m_pButtonRight->ReactToEvent(_pSDLEvent);

		if (m_pButtonLeft->ObtenirButtonState() == 2) {
			m_pListeTexture->AllerPrecedentCurseur();
			m_iPositionLabel--;
			if (m_iPositionLabel < 0)
				m_iPositionLabel = m_pListeTexture->ObtenirCompte() - 1;
		}

		else if (m_pButtonRight->ObtenirButtonState() == 2) {
			m_pListeTexture->AllerSuivantCurseur();
			m_iPositionLabel = (m_iPositionLabel + 1) % (m_pListeTexture->ObtenirCompte());
		}
	}

	void ChangeTexture(bool _boDroite) {


	}

	int ObtenirElement(string _Element) {

		if (_Element == "PositionLabel")
			return m_iPositionLabel;
	}

	int ObtenirPositionLabel(void) {

		return m_iPositionLabel;
	}

	void SetRectDestinationX(unsigned int _uiX) {
		m_SDLRectLabel.x = _uiX;
	}

	void SetRectDestinationY(unsigned int _uiY) {
		m_SDLRectLabel.y = _uiY;
	}
};