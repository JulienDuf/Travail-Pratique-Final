//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant une fen�tre.
// Cr�� le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//
class CWindow {
private:
	SDL_Window* m_pSDLWindow; // La fenetre.
	SDL_Renderer* m_pSDLRenderer; // Renderer.
	unsigned int m_uiWindowID; // Le window ID.
public:
	// Constructeur ...
	// Parametre : _uiLargeur : Largeur de la fen�tre.
	//			 : _uiHauteur : Hauteur de la fen�tre.
	CWindow(unsigned int _uiLargeur, unsigned int _uiHauteur) {
		SDL_CreateWindowAndRenderer(_uiLargeur, _uiHauteur, NULL, &m_pSDLWindow, &m_pSDLRenderer); // Cr�er une fen�tre avec largeur, hauteur et le renderer.
		m_uiWindowID = SDL_GetWindowID(m_pSDLWindow); // Le ID de la window.


	}
	//
	// Proc�dure qui rendre l'image final.
	//Param�tre : _Renderer : Le renderer.
	void Rafraichir(SDL_Renderer* _Renderer) {
		SDL_RenderPresent(_Renderer); // Renderer.
	}
	// Le titre de la fen�tre.
	// Param�tre : _Titre : le titre de l'image.
	//
	void DefinirTitre(char* _Titre) {
		SDL_SetWindowTitle(m_pSDLWindow, _Titre);
	}
	//
	// Fonction qui sert � obtenir le ID de la windon.
	// Aucun Param�tre.
	// Retour : unsigned int
	unsigned int ObtenirWindowID(void) {
		return m_uiWindowID;
	}
	// Fonction qui retourne le Renderer de la window.
	// Retour : SDL_Renderer
	// Aucun param�tre.
	SDL_Renderer* ObtenirRenderer(void){
		return m_pSDLRenderer;
	}
	// Destructeur ...
	~CWindow(void) {
		SDL_DestroyRenderer(m_pSDLRenderer);
		SDL_DestroyWindow(m_pSDLWindow);
	}

};