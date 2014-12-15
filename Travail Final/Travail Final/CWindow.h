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
	CGame* m_pGame; // La partie qui sera dans la fen�tre.
	int m_iFrame; // Frame.
	CTimer* pTimerFrame;

public:

	// Constructeur de CWindow...
	// Parametre : _uiLargeur : Largeur de la fen�tre.
	//			 : _uiHauteur : Hauteur de la fen�tre.
	CWindow(unsigned int _uiLargeur, unsigned int _uiHauteur) {
		SDL_CreateWindowAndRenderer(_uiLargeur, _uiHauteur, NULL, &m_pSDLWindow, &m_pSDLRenderer); // Cr�er une fen�tre avec largeur, hauteur et le renderer.
		SDL_SetWindowPosition(m_pSDLWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		m_pArbreMenu = new CArbreAVL<CMenu*>();

		SDL_SetRenderDrawColor(m_pSDLRenderer, 255,255,255,255);

		pTimerFrame = new CTimer(1000);
		m_iFrame = 0;
		pTimerFrame->Start();
	}

	// Destructeur ...
	~CWindow(void) {
		SDL_DestroyRenderer(m_pSDLRenderer);
		SDL_DestroyWindow(m_pSDLWindow);
		delete m_pArbreMenu;
	}

	// Proc�dure permettant d'ajouter des menus...
	// en entr�e:
	// Param1: -Nombre de menus � ajouter.
	// Param2; Les menus � ajouter.
	void AjouterMenu(unsigned int argc, ...) {
		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreMenu->Ajouter(va_arg(parametres, CMenu*), "");
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
	
	void CreateGame(string _strEmplacementMap, int _iNombre�quipe, int _iNombreJoueur, CVent* _pVent, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {

		m_pGame = new CGame(_strEmplacementMap, _iNombre�quipe, _iNombreJoueur, _pVent, _MapDestruction, _Rotation, m_pSDLRenderer);
	}

	// Proc�dure qui rendre l'image final.
	// Param�tre : _Renderer : Le renderer.
	void Rafraichir(void) {
		SDL_RenderClear(m_pSDLRenderer);

		if (pTimerFrame->IsDone()) {

			char chr[50];
			SDL_SetWindowTitle(m_pSDLWindow, SDL_itoa(m_iFrame, chr, 10));
			m_iFrame = 0;
			pTimerFrame->Start();
		}

		if (m_pGame != nullptr)
			m_pGame->AfficherGame(m_pSDLRenderer);

		m_pArbreMenu->ParcoursMenu(m_pSDLRenderer);

		SDL_RenderPresent(m_pSDLRenderer); // Renderer.
		m_iFrame++;
	}

	void FinDePartie(void) {
		delete m_pGame;
		m_pGame = nullptr;
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

	CGame* ObtenirGame(void) {

		return m_pGame;
	}

	void GetSize(int* _iX, int* _iY) {

		SDL_GetWindowSize(m_pSDLWindow, _iX, _iY);
	}
};