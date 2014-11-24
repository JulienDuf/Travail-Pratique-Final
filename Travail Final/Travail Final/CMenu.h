//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant un menu.
// Cr��e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CMenu {
private:

	CArbreAVL<CControl*>* m_pArbreControl; // Liste des contr�les.
	bool m_boShow; // Bool�en qui dicte si le menu s'affiche ou non.
	SDL_Texture* m_pTextureBackGroundMenu; // Le background du menu.
	SDL_Rect m_RectDestinationBackGround; // La position du background.

public:

	// Constructeur de Menu...
	// En entr�e:
	// Param1: bool�en qui dicte si le menu doit s'afficher ou non.
	// Param2; Nombre de Contr�les � ajouter.
	// Param3...; Contr�les � ajouter.
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

	// Proc�dure permettant d'ajouter des contr�les au menu...
	// En entr�e:
	// Param1; Nombre de Contr�les � ajouter.
	// Param2...; Contr�les � ajouter.
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

	// Proc�dure permettant de retirer des contr�les au menu...
	// En entr�e:
	// Param1; Nombre de Contr�les � retirer.
	// Param2...; Contr�les � retirer.
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

	// Porc�dure permettant d'afficher le menu...
	// En entr�e:
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