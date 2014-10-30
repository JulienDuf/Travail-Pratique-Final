//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant une fen�tre.
// Cr�� le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modifi�e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
//

class CWindow {
private:

	CArbreAVL<CMenu*>* m_pArbreMenu; // Liste des menus.
	SDL_Window* m_pSDLWindow; // La fenetre.
	SDL_Renderer* m_pSDLRenderer; // Renderer.
public:

	// Constructeur ...
	// Parametre : _uiLargeur : Largeur de la fen�tre.
	//			 : _uiHauteur : Hauteur de la fen�tre.
	CWindow(unsigned int _uiLargeur, unsigned int _uiHauteur) {
		SDL_CreateWindowAndRenderer(_uiLargeur, _uiHauteur, NULL, &m_pSDLWindow, &m_pSDLRenderer); // Cr�er une fen�tre avec largeur, hauteur et le renderer.
		SDL_SetWindowPosition(m_pSDLWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		m_pArbreMenu = new CArbreAVL<CMenu*>();
	}

	// Destructeur ...
	~CWindow(void) {
		SDL_DestroyRenderer(m_pSDLRenderer);
		SDL_DestroyWindow(m_pSDLWindow);
	}

	// Proc�dure permettant d'ajouter des menus...
	// en entr�e:
	// Param1: -Nombre de menus � ajouter.
	// Param2; Les menus � ajouter.
	void AjouterMenu(unsigned int argc, ...) {
		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreMenu->Ajouter(va_arg(parametres, CMenu*));
		}
		va_end(parametres);
	}

	// Proc�dure permettant de retirer des menus...
	// en entr�e:
	// Param1: -Nombre de menus � retirer.
	// Param2; Les menus � retirer.
	void RetirerMenu(unsigned int argc, ...) {
		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreMenu->Retirer(va_arg(parametres, CMenu*));
		}
		va_end(parametres);
	}

	// Proc�dure qui rendre l'image final.
	// Param�tre : _Renderer : Le renderer.
	void Rafraichir(SDL_Renderer* _Renderer) {
		SDL_RenderPresent(_Renderer); // Renderer.
	}

	// Le titre de la fen�tre.
	// Param�tre : _Titre : le titre de l'image.
	//
	void DefinirTitre(char* _Titre) {
		SDL_SetWindowTitle(m_pSDLWindow, _Titre);
	}

	// Fonction qui retourne le Renderer de la window.
	// Retour : SDL_Renderer
	// Aucun param�tre.
	SDL_Renderer* ObtenirRenderer(void){
		return m_pSDLRenderer;
	}
};