/*
Classe G�rant les Labels "Left Right"
Cr��e le 30 octobre par Samuel Rambaud (Sam.Rambaud@outlook.com)
*/

class CLabelLeftRight : public CControl{
private:

	CListeDC<SDL_Texture*>* m_pListeTexture;
	SDL_Rect m_SDLRectLabel;
	CButton* m_pButtonLeft; // Bouton de droite du controle.
	CButton* m_pButtonRight; // Bouton de gauche du controle.

public:

	/*
	Contructeur de Label left right pour du texte...
	parametres:
	_pListeObjet : Donner des string � faire afficher dans un labelleftright (� d�truire si inutille apr�s).
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
	}

	/*
	Construteur de Label left right pour des textures...
	En entr�e:
		Param1: Liste des textures � faire afficher(� ne pas d�truire).
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
	}
	
	~CLabelLeftRight() {

		delete m_pButtonLeft;
		delete m_pButtonRight;
	}
	
	// Proc�dure permettant d'ajouter des textures � la liste.
	// En entr�e:
	// Param1; Nombre de textures � ajouter.
	// Param2...; Texture � ajouter.
	void AjouterControl(int argc, ...) {
		
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

	// Proc�dure r�agissant � un �v�nement...
	// Entr�e:
	// Sortie: Si le control � r�agit
	void ReactToEvent(SDL_Event* _pSDLEvent) {
		
		m_pButtonLeft->ReactToEvent(_pSDLEvent);
		m_pButtonRight->ReactToEvent(_pSDLEvent);

		if (m_pButtonLeft->ObtenirButtonState() == 2)
			m_pListeTexture->AllerPrecedentCurseur();

		else if (m_pButtonRight->ObtenirButtonState() == 2)
			m_pListeTexture->AllerSuivantCurseur();
	}

};