// Classe représentant des labels,
// créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 30 septembre 2014.

class CTemporaryLabel {
private:

	SDL_Texture* m_pTextureLabel;
	SDL_Rect m_RectLabel;
	SDL_Rect m_RectLabelDebut;
	SDL_Rect m_RectLabelFin;
	CTimer* m_pTimer;
	CTimer* m_pTimerFinDeVie;
	int m_iNombrePixelParDeplacementX;
	int m_iNombrePixelParDeplacementY;

public:

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

		m_iNombrePixelParDeplacementX = (m_RectLabelFin.x - m_RectLabelDebut.x) / _iVitesseDeplacement;
		m_iNombrePixelParDeplacementY = (m_RectLabelFin.y - m_RectLabelDebut.y) / _iVitesseDeplacement;
	}

	void ShowControl(SDL_Renderer* _pRenderer) {
			
		if (m_RectLabel.x != m_RectLabelFin.x || m_RectLabel.y != m_RectLabelFin.y) {

			if (m_pTimer->IsDone()) {
				m_RectLabel.x += m_iNombrePixelParDeplacementX;
				m_RectLabel.y += m_iNombrePixelParDeplacementY;

				m_pTimer->Start();
			}
		}
		SDL_RenderCopy(_pRenderer, m_pTextureLabel, NULL, &m_RectLabel);
	}

	bool IsDead(void) {

		return m_pTimerFinDeVie->IsDone();
	}
};