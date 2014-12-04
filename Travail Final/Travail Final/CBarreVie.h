// 420-202-RE : Travail final
// Classe qui représente une barre de vie.
// 25 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
// 29 novembre 2014 par Gabriel Beaudry (gabriel.bdry@gmail.com)
// Modification de la classe pour que les barres de vie représentent l'équipe du joueur.

class CBarreVie {

private:

	float m_fPourcentageVie;
	SDL_Texture* m_pTextureContourBarre;
	SDL_Texture* m_pTextureInterieurBarre;
	SDL_Rect m_RectDestinationBarre;
	SDL_Rect m_RectDestinationContour;
	SDL_Rect m_RectSourceBarre;

public:

	CBarreVie(CGestionaire<SDL_Texture*>* _pGestionnaireTexture, SDL_Rect _RectDestination, unsigned int _uiIDTeam) {

		m_pTextureInterieurBarre = _pGestionnaireTexture->ObtenirDonnee("pTextureBarreVie");

		m_pTextureContourBarre = _pGestionnaireTexture->ObtenirDonnee("pTextureContourBarreVie");

		m_RectDestinationContour = _RectDestination;
		SDL_QueryTexture(m_pTextureContourBarre, NULL, NULL, &m_RectDestinationContour.w, &m_RectDestinationContour.h);

		m_fPourcentageVie = 1;

		m_RectDestinationBarre = _RectDestination;
		m_RectDestinationBarre.x += 2;
		m_RectDestinationBarre.y += 2;
		m_RectDestinationBarre.w = m_RectDestinationContour.w - 4;
		m_RectDestinationBarre.h = m_RectDestinationContour.h - 4;

		m_RectSourceBarre = { _uiIDTeam * m_RectDestinationBarre.w, 0, m_RectDestinationBarre.w, m_RectDestinationBarre.h };
	}

	void ShowBarre(SDL_Renderer* _pRenderer, SDL_Rect _RectDestination) {
		m_RectDestinationBarre = { _RectDestination.x + 2, _RectDestination.y + 2, m_RectDestinationBarre.w, m_RectDestinationBarre.h};
		SDL_RenderCopy(_pRenderer, m_pTextureContourBarre, NULL, &_RectDestination);
		SDL_RenderCopy(_pRenderer, m_pTextureInterieurBarre, &m_RectSourceBarre, &m_RectDestinationBarre);
	}

	void ModifierPourcentageVie(float _Diminution) {

		m_fPourcentageVie = _Diminution;

		m_RectDestinationBarre.w = m_fPourcentageVie * 36;
		m_RectSourceBarre.w = m_fPourcentageVie * 36;
	}

	void ModifierPositionBarre(int _iX, int _iY) {

		m_RectDestinationContour.x = _iX;
		m_RectDestinationContour.y = _iY;

		m_RectDestinationBarre.x = m_RectDestinationContour.x + 2;
		m_RectDestinationBarre.y = m_RectDestinationContour.y + 2;
	}

	float ObtenirPourcentage() {
		return m_fPourcentageVie;
	}
	float ObtenirVie(void) {

		return m_fPourcentageVie;
	}

};