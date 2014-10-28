//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un menu.
// Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CMenu {
private:

	CListeDC<CControl*>* m_pListeControl; // Liste des contrôles.
	bool m_boShow; // Booléen qui dicte si le menu s'affiche ou non.

public:

	// Constructeur de Menu...
	// En entrée:
	// Param1: booléen qui dicte si le menu doit s'afficher ou non.
	// Param2; Nombre de Contrôles à ajouter.
	// Param3...; Contrôles à ajouter.
	CMenu(bool _boShow, unsigned int argc, ...) {
		m_boShow = _boShow;
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pListeControl->AjouterDebut(va_arg(parametres, CControl*));
			}
			va_end(parametres);
		}
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
				m_pListeControl->AjouterDebut(va_arg(parametres, CControl*));
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
				m_pListeControl->AllerAObjetCurseur(va_arg(parametres, CControl*), true);
			}
			va_end(parametres);
		}
	}

	// Porcédure permettant d'afficher les menus...
	// En entrée:
	// Param1: Le renderer de destination du menu.
	void ShowMenu(SDL_Renderer* _pSDLRenderer) {
		if (m_boShow) {
			//m_pListeControl-> à faire avec l'arbre???
		}
	}
};