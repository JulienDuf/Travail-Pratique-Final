//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un label.
// Créé le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification fait par Julien Dufresne (dufresne_julien@hotmai.ca) le 28 octobre 2014
// Modifiée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
// Fin du codage de la classe.
//

class CLabel : public CControl {
private:

	CListeDC<SDL_Texture*>* m_pListeTextureLabel; // La texture d'un label.
	SDL_Rect m_RectPosition; // La postion/destination qu'on veut l'afficher.

public:

	// Constructeurs de CLabel de text...
	// En entrée:
	// Param1: Le renderer de destination du Control.
	// Param2: Le texte qui sera présent dans le label.
	// Param3: La font du texte qui sera présent dans le label.
	// Param4: La couleur du texte qui sera présent dans le label.
	// Param5: La position du label dans la fenêtre.
	CLabel(SDL_Renderer* _pRenderer, string _strTexte, TTF_Font* _Font, SDL_Color _Couleur, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		SDL_Surface* pSDLSurface; // Variable temporaire...

		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte.c_str(), _Couleur); // Créé la surface contenant le texte.

		// Met la surface en texture.
		m_pListeTextureLabel->AjouterFin(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface));
		m_pListeTextureLabel->AllerDebut();
		SDL_FreeSurface(pSDLSurface); // Détruit la surface.

		// Définie le rect de position...
		m_RectPosition = _RectPosition;
	}

	// Constructeur de CLabel de texture...
	// En entrée:
	// Param1: La texture qui sera présente dans le label.
	// Param2: La position du label dans la fenêtre.
	CLabel(SDL_Texture* _pTextureLabel, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		// Défini la texture...
		m_pListeTextureLabel->AjouterFin(_pTextureLabel);
		m_pListeTextureLabel->AllerDebut();

		// Défini le rect de position...
		m_RectPosition.x = _RectPosition.x;
		m_RectPosition.y = _RectPosition.y;

		if (_RectPosition.w == 0 || _RectPosition.h == 0) {
			int iW, iH;
			SDL_QueryTexture(_pTextureLabel, nullptr, nullptr, &iW, &iH);

			if (_RectPosition.w == 0)
				m_RectPosition.w = iW;
			else
				m_RectPosition.w = _RectPosition.w;

			if (_RectPosition.h == 0)
				m_RectPosition.h = iH;
			else
				m_RectPosition.h = iH;
		}
		else
		{
			m_RectPosition.w = _RectPosition.w;
			m_RectPosition.h = _RectPosition.h;
		}
	}

	// Constructeur de CLabel...
	// En entrée:
	// Param1: Le renderer de destination du Control. sinon ça plente........
	// Param2: La position du label dans la fenêtre.
	CLabel(SDL_Renderer* _pRenderer, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		// Définie le rect de position...
		m_RectPosition = _RectPosition;

	}

	~CLabel() {

		
	}

	// Procédure permettant d'afficher le label...
	// En entrée:
	// Param1: Le renderer de destination du Control.
	void ShowControl(SDL_Renderer* _pRenderer) {

		// Met la texture dans le renderer.
		if (m_pListeTextureLabel->ObtenirCompte() > 0)
			SDL_RenderCopy(_pRenderer, m_pListeTextureLabel->ObtenirElementCurseur(), NULL, &m_RectPosition);

	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	// Sortie: Si le control à réagit
	void ReactToEvent(SDL_Event* _pSDLEvent) {
	

	}

	// Procédure ajoutant une texture à la liste.
	// En entré:
	// Param1: Le renderer de destination du Control.
	// Param2: Le tableau contenant le texte à mettre ensemble.
	// Param3: Le nombre d'élément du tableau.
	// Param4: La font du texte.
	// Param5: La couleur du texte.

	void AjouterTexture(int argc, ...) {

		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pListeTextureLabel->AjouterFin(va_arg(parametres, SDL_Texture*));
			}
			va_end(parametres);

			m_pListeTextureLabel->AllerDebut();
		}
	}

	// Méthodes...

	// Procédure changeant le position du curseur.
	// En entrée:
	// Param1: True(Aller Droite) False(Aller Gauche)
	void ChangeTexture(bool _boDroite) {

		if (_boDroite)
			m_pListeTextureLabel->AllerSuivantCurseur();

		else
			m_pListeTextureLabel->AllerPrecedentCurseur();
	}

	// En entrée:
	// Param1; Nouvelle texture.
	// Param2: Position de la texture à modifier.
	void RemplacerTexture(SDL_Texture* _pTexture, unsigned int _uiPosition) {
		m_pListeTextureLabel->AllerACurseur(_uiPosition);
		m_pListeTextureLabel->DefinirElementCurseur(_pTexture);
	}

	int ObtenirElement(string _Element) {

		return 0;
	}

	

	void ModifierTexture(SDL_Texture* _pTexture, unsigned int _uiPosition) {
		m_pListeTextureLabel->AllerACurseur(_uiPosition);
		m_pListeTextureLabel->DefinirElementCurseur(_pTexture);
	}

	// Procédure qui obtient la dimension d'une texture.
	// En entrée:
	// Param1: Position de la texture voulue(Ordre de création)
	// Param2: Une variable Width à affecter
	// Param3: Une variable Height à affecter
	void GetTextureDimension(unsigned int _uiPosition, int* _iW, int* _iH) {
		m_pListeTextureLabel->AllerACurseur(_uiPosition);
		SDL_QueryTexture(m_pListeTextureLabel->ObtenirElementCurseur(), nullptr, nullptr, _iW, _iH);
	}


	void SetRectDestinationX(unsigned int _uiX) {
		m_RectPosition.x = _uiX;
	}

	void SetRectDestinationY(unsigned int _uiY) {
		m_RectPosition.y = _uiY;
	}
};