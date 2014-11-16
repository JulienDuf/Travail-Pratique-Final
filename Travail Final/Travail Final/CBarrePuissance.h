// 420-202-RE : Travail final
// Classe qui représente une barre pouvant dertéminer une puissance de lancer.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CBarrePuissance {

private:

	int m_iAngle; // L'angle de la barre.
	int m_iForce; // La force affiché par la barre.

	SDL_Texture* m_pSDLTextureBarrre; // La texture de la barre.
	SDL_Rect m_RectDestinationBarre; // La destination de la barre dans la fenêtre.

public:

	// Constructeur de la barre de puissance...
	// En entrée:
	// Param1: L'emplacement des fichiers.
	// Param2: Le renderer de la fenêtre.
	CBarrePuissance(string _strEmplacement, SDL_Renderer* _pRenderer) {

		_strEmplacement.append("BarrePuissance.png");
		m_pSDLTextureBarrre = IMG_LoadTexture(_pRenderer, _strEmplacement.c_str());

		m_RectDestinationBarre = { 0, 0, 0, 0 };
		SDL_QueryTexture(m_pSDLTextureBarrre, NULL, NULL, &m_RectDestinationBarre.w, &m_RectDestinationBarre.h);
	}

	// Procédure affichant la barre de puissance...
	// En entrée:
	// Param1: Le renderer de la fenêtre.
	void AfficherBarre(SDL_Renderer* _pRenderer) {

		SDL_Rect RectSource = { m_iForce * m_RectDestinationBarre.w, 0, m_RectDestinationBarre.w, m_RectDestinationBarre.h };

		SDL_RenderCopy(_pRenderer, m_pSDLTextureBarrre, &RectSource, &m_RectDestinationBarre);
	}

	void ModifierForceBarre(int _iForce) {

		m_iForce = _iForce;
	}

	

};