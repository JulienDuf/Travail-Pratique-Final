// 420-202-RE : Travail final
// Classe qui représente une barre de vie.
// 25 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CBarreVie {

private:

	float m_fPourcentageVie;
	SDL_Texture* m_pTextureContourBarre;
	SDL_Texture* m_pTextureInterieurBarre;
	SDL_Rect m_RectDestinationBarre;
	SDL_Rect m_RectDestinationContour;

public:

	CBarreVie(string _strEmplacement, SDL_Rect _RectDestination, SDL_Renderer* _pRenderer) {

		string strEmplacement = _strEmplacement;
		strEmplacement.append("Personnage\\Barre.png");
		m_pTextureInterieurBarre = IMG_LoadTexture(_pRenderer, strEmplacement.c_str());

		strEmplacement = _strEmplacement;
		strEmplacement.append("Personnage\\ContourBarre.png");
		m_pTextureContourBarre = IMG_LoadTexture(_pRenderer, strEmplacement.c_str());

		m_RectDestinationContour = _RectDestination;
		SDL_QueryTexture(m_pTextureContourBarre, NULL, NULL, &m_RectDestinationContour.w, &m_RectDestinationContour.h);

		m_RectDestinationBarre = _RectDestination;
		m_RectDestinationBarre.x += 2;
		m_RectDestinationBarre.y += 2;
		SDL_QueryTexture(m_pTextureInterieurBarre, NULL, NULL, &m_RectDestinationBarre.w, &m_RectDestinationBarre.h);

		m_fPourcentageVie = 1;
		m_RectDestinationBarre.w = m_fPourcentageVie * 36;
	}

	void ShowBarre(SDL_Renderer* _pRenderer) {

		SDL_Rect RectSource;

		if (m_fPourcentageVie >= .70)
			RectSource = { 0, 0, 36, 6 };

		else if (m_fPourcentageVie >= .40 && m_fPourcentageVie < .70)
			RectSource = { 36, 0, 36, 6 };

		else
			RectSource = { 72, 0, 36, 6 };

		SDL_RenderCopy(_pRenderer, m_pTextureContourBarre, NULL, &m_RectDestinationContour);
		SDL_RenderCopy(_pRenderer, m_pTextureInterieurBarre, &RectSource, &m_RectDestinationBarre);
	}

	void DiminuerPourcentageVie(float _Diminution) {

		m_fPourcentageVie -= _Diminution;

		m_RectDestinationBarre.w = m_fPourcentageVie * 36;
	}

	void ModifierPositionBarre(int _iX, int _iY) {

		m_RectDestinationContour.x = _iX;
		m_RectDestinationContour.y = _iY;

		m_RectDestinationBarre.x = m_RectDestinationContour.x + 2;
		m_RectDestinationBarre.y = m_RectDestinationContour.y + 2;
	}

};