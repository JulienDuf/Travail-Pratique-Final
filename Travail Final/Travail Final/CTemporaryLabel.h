// Classe représentant des labels,
// créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 30 septembre 2014.
// Modifier par Julien Dufresne (dufresne_julien@hotmail.ca) le 19 décembre 2014.
// La classe est maintenant optimisé...

class CTemporaryLabel {
private:

	SDL_Texture* m_pTextureLabel; // La texture du label.
	SDL_Rect m_RectLabel; // Le destination du label...
	SDL_Rect m_RectLabelDebut; // Le rect du début du label.
	SDL_Rect m_RectLabelFin; // Le rect de fin du label.
	CTimer* m_pTimer; // Le timer de déplacement du label.
	CTimer* m_pTimerFinDeVie; // Le timer de vie du label.
	double m_dNombrePixelParDeplacementX; // Nombre de Pixels Par Deplacement en X.
	double m_dNombrePixelParDeplacementY; // Nombre de Pixels Par Deplacement en Y.

public:

	// Constructeur...
	CTemporaryLabel(string _strTextLabel, SDL_Color _CouleurTexte, SDL_Rect _RectDepart, SDL_Rect _RectFin, int _iVitesseDeplacement, int _iDureVie, SDL_Renderer* _pRenderer) {
		
		SDL_Surface* pSurfaceTmp = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDomage"), _strTextLabel.c_str(), _CouleurTexte);
		m_pTextureLabel = SDL_CreateTextureFromSurface(_pRenderer, pSurfaceTmp);

		m_RectLabel = _RectDepart;
		m_RectLabelDebut = _RectDepart;
		m_RectLabelFin = _RectFin;

		m_RectLabel.w = m_RectLabelDebut.w = m_RectLabelFin.w = pSurfaceTmp->w;
		m_RectLabel.h = m_RectLabelDebut.h = m_RectLabelFin.h = pSurfaceTmp->h;

		SDL_FreeSurface(pSurfaceTmp);

		m_pTimer = new CTimer(_iVitesseDeplacement);
		m_pTimerFinDeVie = new CTimer(_iDureVie);
		m_pTimer->Start();
		m_pTimerFinDeVie->Start();

		m_dNombrePixelParDeplacementX = (m_RectLabelFin.x - m_RectLabelDebut.x) / _iVitesseDeplacement;
		m_dNombrePixelParDeplacementY = (m_RectLabelFin.y - m_RectLabelDebut.y) / _iVitesseDeplacement;
	}

	~CTemporaryLabel() {

		SDL_DestroyTexture(m_pTextureLabel);
		delete m_pTimer;
		delete m_pTimerFinDeVie;
	}

	// Procédure affichant le label...
	// En entrée:
	// Param1: Le renderer de la fenêtre...
	void ShowControl(SDL_Renderer* _pRenderer) {
			
		if (m_RectLabel.x != m_RectLabelFin.x || m_RectLabel.y != m_RectLabelFin.y) {

			if (m_pTimer->IsDone()) {
				m_RectLabel.x += m_dNombrePixelParDeplacementX;
				m_RectLabel.y += m_dNombrePixelParDeplacementY;

				m_pTimer->Start();
			}
		}
		SDL_RenderCopy(_pRenderer, m_pTextureLabel, NULL, &m_RectLabel);
	}

	bool IsDead(void) {

		return m_pTimerFinDeVie->IsDone();
	}
};