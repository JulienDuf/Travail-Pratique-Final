//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant un menu.
// Cr��e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CMenu {
private:

	CListeDC<CControl*>* m_pListeControl; // Liste des contr�les.
	bool m_boShow; // Bool�en qui dicte si le menu s'affiche ou non.

public:

	// Constructeur de Menu...
	// En entr�e:
	// Param1: bool�en qui dicte si le menu doit s'afficher ou non.
	// Param2; Nombre de Contr�les � ajouter.
	// Param3...; Contr�les � ajouter.
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

	// Proc�dure permettant d'ajouter des contr�les au menu...
	// En entr�e:
	// Param1; Nombre de Contr�les � ajouter.
	// Param2...; Contr�les � ajouter.
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

	// Proc�dure permettant de retirer des contr�les au menu...
	// En entr�e:
	// Param1; Nombre de Contr�les � retirer.
	// Param2...; Contr�les � retirer.
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

	// Porc�dure permettant d'afficher les menus...
	// En entr�e:
	// Param1: Le renderer de destination du menu.
	void ShowMenu(SDL_Renderer* _pSDLRenderer) {
		if (m_boShow) {
			//m_pListeControl-> � faire avec l'arbre???
		}
	}
};