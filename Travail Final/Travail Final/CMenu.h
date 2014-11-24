//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un menu.
// Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CMenu {
private:

	CArbreAVL<CControl*>* m_pArbreControl; // Liste des contrôles.
	bool m_boShow; // Booléen qui dicte si le menu s'affiche ou non.
	SDL_Texture* m_pTextureBackGroundMenu; // Le background du menu.
	SDL_Rect m_RectDestinationBackGround; // La position du background.

public:

	// Constructeur de Menu...
	// En entrée:
	// Param1: booléen qui dicte si le menu doit s'afficher ou non.
	// Param2; Nombre de Contrôles à ajouter.
	// Param3...; Contrôles à ajouter.
	CMenu(bool _boShow, SDL_Rect _RectDestination, SDL_Renderer* _pRenderer,unsigned int argc, ...) {
		m_boShow = _boShow;
		m_pArbreControl = new CArbreAVL<CControl*>();

		SDL_Surface* pSDLSurface = SDL_CreateRGBSurface(0, 470, 470, 32, 0, 0, 0, 0);
		SDL_FillRect(pSDLSurface, NULL, 0xFFFFFF);
		m_pTextureBackGroundMenu = SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface);
		SDL_FreeSurface(pSDLSurface);

		m_RectDestinationBackGround = _RectDestination;

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

			SDL_RenderCopy(_pSDLRenderer, m_pTextureBackGroundMenu, NULL, &m_RectDestinationBackGround);
			m_pArbreControl->ParcoursControl(_pSDLRenderer);
		}
	}

	void DefinirboShow(bool _boShow) {
		m_boShow = _boShow;
	}


};