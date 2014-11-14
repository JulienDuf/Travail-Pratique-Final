//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant un label.
// Cr�� le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification fait par Julien Dufresne (dufresne_julien@hotmai.ca) le 28 octobre 2014
// Modifi�e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 28 octobre 2014.
// Fin du codage de la classe.
//

class CLabel : public CControl {
private:

	CListeDC<SDL_Texture*>* m_pListeTextureLabel; // La texture d'un label.
	SDL_Rect m_RectPosition; // La postion/destination qu'on veut l'afficher.

	// Fonction qui met le texte d'un tableau en une seule surface.
	// En entr�e: 
	// Param1: Le tableau contenant le texte � mettre ensemble.
	// Param2: Le nombre d'�l�ment du tableau.
	// Param3: La font du texte.
	// Param4: La couleur du texte.
	// En sortie:
	// La surface finale.
	SDL_Surface* BlitTexte(string _strTexte[], int _iNombreElementTableau, TTF_Font* _Font, SDL_Color _Couleur) {

		SDL_Surface* pSDLSurface; // Variable temporaire...
		SDL_Surface* pSDLSurfaceTmp;
		SDL_Rect RectTmp = { 0, 0, 0, 0 };

		if (_iNombreElementTableau > 0) {
			pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[0].c_str(), _Couleur); // Cr�� la surface contenant le texte.
			pSDLSurfaceTmp = SDL_CreateRGBSurface(pSDLSurface->flags, m_RectPosition.w, pSDLSurface->h * _iNombreElementTableau, pSDLSurface->format->BitsPerPixel, pSDLSurface->format->Rmask, pSDLSurface->format->Gmask, pSDLSurface->format->Bmask, pSDLSurface->format->Amask);

			SDL_BlitSurface(pSDLSurface, NULL, pSDLSurfaceTmp, &RectTmp);
			RectTmp.y = pSDLSurface->h;			
			SDL_FreeSurface(pSDLSurface);

			for (int i = 1; i < _iNombreElementTableau; i++) {

				pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte[i].c_str(), _Couleur); // Cr�� la surface contenant le texte.
				SDL_BlitSurface(pSDLSurface, NULL, pSDLSurfaceTmp, &RectTmp);
				RectTmp.y = pSDLSurface->h * (i + 1);
				SDL_FreeSurface(pSDLSurface);
			}

			return pSDLSurfaceTmp;
		}

		return nullptr;
	}

public:

	// Constructeurs de CLabel de text...
	// En entr�e:
	// Param1: Le renderer de destination du Control.
	// Param2: Le texte qui sera pr�sent dans le label.
	// Param3: La font du texte qui sera pr�sent dans le label.
	// Param4: La couleur du texte qui sera pr�sent dans le label.
	// Param5: La position du label dans la fen�tre.
	CLabel(SDL_Renderer* _pRenderer, string _strTexte, TTF_Font* _Font, SDL_Color _Couleur, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		SDL_Surface* pSDLSurface; // Variable temporaire...

		pSDLSurface = TTF_RenderText_Blended(_Font, _strTexte.c_str(), _Couleur); // Cr�� la surface contenant le texte.

		// Met la surface en texture.
		m_pListeTextureLabel->AjouterFin(SDL_CreateTextureFromSurface(_pRenderer, pSDLSurface));
		m_pListeTextureLabel->AllerDebut();
		SDL_FreeSurface(pSDLSurface); // D�truit la surface.

		// D�finie le rect de position...
		m_RectPosition = _RectPosition;
	}

	// Constructeur de CLabel de texture...
	// En entr�e:
	// Param1: La texture qui sera pr�sente dans le label.
	// Param2: La position du label dans la fen�tre.
	CLabel(SDL_Texture* _pTextureLabel, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		// D�fini la texture...
		m_pListeTextureLabel->AjouterFin(_pTextureLabel);
		m_pListeTextureLabel->AllerDebut();

		// D�fini le rect de position...
		m_RectPosition = _RectPosition;
	}

	// Constructeur de CLabel...
	// En entr�e:
	// Param1: Le renderer de destination du Control. sinon �a plente........
	// Param2: La position du label dans la fen�tre.
	CLabel(SDL_Renderer* _pRenderer, SDL_Rect _RectPosition) {

		m_pListeTextureLabel = new CListeDC<SDL_Texture*>();

		// D�finie le rect de position...
		m_RectPosition = _RectPosition;

	}

	~CLabel() {

		
	}

	// Proc�dure permettant d'afficher le label...
	// En entr�e:
	// Param1: Le renderer de destination du Control.
	void ShowControl(SDL_Renderer* _pRenderer) {

		// Met la texture dans le renderer.
		if (m_pListeTextureLabel->ObtenirCompte() > 0)
			SDL_RenderCopy(_pRenderer, m_pListeTextureLabel->ObtenirElementCurseur(), NULL, &m_RectPosition);

	}

	// Proc�dure r�agissant � un �v�nement...
	// Entr�e:
	// Param1: Le gestionaire d'�v�nement de SDL.
	// Sortie: Si le control � r�agit
	void ReactToEvent(SDL_Event* _pSDLEvent) {
	

	}

	// Proc�dure ajoutant une texture � la liste.
	// En entr�:
	// Param1: Le renderer de destination du Control.
	// Param2: Le tableau contenant le texte � mettre ensemble.
	// Param3: Le nombre d'�l�ment du tableau.
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

	int ObtenirElement(string _Element) {

		return 0;
	}

	// Proc�dure changeant le position du curseur.
	// En entr�e:
	// Param1: True(Aller Droite) False(Aller Gauche)
	void ChangeTexture(bool _boDroite) {

		if (_boDroite)
			m_pListeTextureLabel->AllerSuivantCurseur();

		else
			m_pListeTextureLabel->AllerPrecedentCurseur();
	}

	// Proc�dure qui obtient la dimension d'une texture.
	// En entr�e:
	// Param1: Position de la texture voulue(Ordre de cr�ation)
	// Param2: Une variable Width � affecter
	// Param3: Une variable Height � affecter
	void GetTextureDimension(unsigned int _uiPosition, int* _iW, int* _iH) {
		m_pListeTextureLabel->AllerACurseur(_uiPosition);
		SDL_QueryTexture(m_pListeTextureLabel->ObtenirElementCurseur(), nullptr, nullptr, _iW, _iH);
	}

};