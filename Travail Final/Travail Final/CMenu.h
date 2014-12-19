//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un menu.
// Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CMenu {
private:

	CArbreAVL<CControl*>* m_pArbreControl; // Liste des contrôles.
	bool m_boShow; // Booléen qui dicte si le menu s'affiche ou non.
	SDL_Rect m_RectDestinationBackGround; // La position du background.
	SDL_Color m_CouleurBackground; // La couleur du background.
	SDL_Texture* m_pTextureBackground; // La texture du background.

public:

	// Constructeur de Menu...
	// En entrée:
	// Param1: booléen qui dicte si le menu doit s'afficher ou non.
	// Param2: La destination du menu.
	// Param3: La couleur du background.
	// Param4: Nombre de Contrôles à ajouter.
	// Param5...: Contrôles à ajouter.
	CMenu(bool _boShow, SDL_Rect _RectDestination, SDL_Color _CouleurBackGround, unsigned int argc, ...) {

		m_boShow = _boShow;
		m_pArbreControl = new CArbreAVL<CControl*>();

		m_RectDestinationBackGround = _RectDestination;

		m_CouleurBackground = _CouleurBackGround;
		m_pTextureBackground = nullptr;

		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControl->Ajouter(va_arg(parametres, CControl*), "");
			}
			va_end(parametres);
		}
	}

	// Constructeur de Menu...
	// En entrée:
	// Param1: booléen qui dicte si le menu doit s'afficher ou non.
	// Param2: La destination du menu.
	// Param3: La texture du background.
	// Param4: Nombre de Contrôles à ajouter.
	// Param5...: Contrôles à ajouter.
	CMenu(bool _boShow, SDL_Rect _RectDestination, SDL_Texture* _pTextureBackground, unsigned int argc, ...) {

		m_boShow = _boShow;
		m_pArbreControl = new CArbreAVL<CControl*>();

		m_RectDestinationBackGround = _RectDestination;

		m_pTextureBackground = _pTextureBackground;

		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControl->Ajouter(va_arg(parametres, CControl*), "");
			}
			va_end(parametres);
		}
	}

	~CMenu() {

		delete m_pArbreControl;
		if (m_pTextureBackground != nullptr)
			SDL_DestroyTexture(m_pTextureBackground);
	}

	// Procédure permettant d'ajouter des contrôles au menu...
	// En entrée:
	// Param1; Nombre de Contrôles à ajouter.
	// Param2...; Contrôles à ajouter.
	void AjouterControl(int argc, ...) {
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControl->Ajouter(va_arg(parametres, CControl*), "");
			}
			va_end(parametres);
		}
	}

	// Procédure permettant de retirer des contrôles au menu...
	// En entrée:
	// Param1; Nombre de Contrôles à retirer.
	// Param2...; Contrôles à retirer.
	void RetirerControl(int argc, ...) {
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControl->Retirer(va_arg(parametres, CControl*));
			}
			va_end(parametres);
		}
	}

	void ReactToEvent(SDL_Event* _pSDLEvent) {
		if (m_boShow)
			m_pArbreControl->ParcoursReaction(_pSDLEvent);
	}

	// Porcédure permettant d'afficher le menu...
	// En entrée:
	// Param1: Le renderer de destination du menu.
	void ShowMenu(SDL_Renderer* _pSDLRenderer) {
		if (m_boShow) {

			if (m_pTextureBackground == nullptr) {
				SDL_SetRenderDrawBlendMode(_pSDLRenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(_pSDLRenderer, m_CouleurBackground.r, m_CouleurBackground.b, m_CouleurBackground.b, m_CouleurBackground.a);

				SDL_RenderFillRect(_pSDLRenderer, &m_RectDestinationBackGround);
			}

			else {

				SDL_RenderCopy(_pSDLRenderer, m_pTextureBackground, NULL, NULL);
			}
			m_pArbreControl->ParcoursControl(_pSDLRenderer);
		}
	}

	void DefinirboShow(bool _boShow) {
		m_boShow = _boShow;
	}


};