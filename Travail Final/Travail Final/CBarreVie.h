// 420-202-RE : Travail final
// Classe qui représente une barre de vie.
// 25 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
// 29 novembre 2014 par Gabriel Beaudry (gabriel.bdry@gmail.com)
// Modification de la classe pour que les barres de vie représentent l'équipe du joueur.

class CBarreVie {
private:

	float m_fPourcentageVie; // Pourcentage de la barre de vie.
	SDL_Texture* m_pTextureContourBarre; // Texture du contour de la barre.
	SDL_Texture* m_pTextureInterieurBarre; // Texture de l'intérieur de la barre.
	SDL_Rect m_RectDestinationBarre; // Rect de destination de la barre(intérieur du contour).
	SDL_Rect m_RectDestinationContour; // Rect de destination du contour de la barre.
	SDL_Rect m_RectSourceBarre; // Rect source de la barre(Pour les couleurs des équipes).

public:

	// Constructeur de CBarreVie...
	// En entrée:
	// Param1: Numéro de la team.
	CBarreVie(unsigned int _uiIDTeam) {

		m_pTextureInterieurBarre = pGestionnaireTexture->ObtenirDonnee("pTextureBarreVie");

		m_pTextureContourBarre = pGestionnaireTexture->ObtenirDonnee("pTextureContourBarreVie");

		SDL_QueryTexture(m_pTextureContourBarre, NULL, NULL, &m_RectDestinationContour.w, &m_RectDestinationContour.h);

		m_fPourcentageVie = 1;
		m_RectDestinationBarre.w = m_RectDestinationContour.w - 4;
		m_RectDestinationBarre.h = m_RectDestinationContour.h - 4;
		m_RectSourceBarre = { _uiIDTeam * m_RectDestinationBarre.w, 0, m_RectDestinationBarre.w, m_RectDestinationBarre.h };
	}

	// Procédure permettant d'afficher la barre à une positon...
	// En entrée:
	// Param1: Render d'affichage de la game.
	// Param2: Destination de la barre.
	void ShowBarre(SDL_Renderer* _pRenderer, SDL_Point _PointDestination) {

		// Modification des Rects...
		m_RectDestinationContour.x = _PointDestination.x;
		m_RectDestinationContour.y = _PointDestination.y;
		m_RectDestinationBarre.x = _PointDestination.x + 2;
		m_RectDestinationBarre.y = _PointDestination.y + 2;
		// Affichage des textures...
		SDL_RenderCopy(_pRenderer, m_pTextureContourBarre, NULL, &m_RectDestinationContour);
		SDL_RenderCopy(_pRenderer, m_pTextureInterieurBarre, &m_RectSourceBarre, &m_RectDestinationBarre);
	}

	// Procédure permettant de modifier le pourcentage de vie de la barre...
	// En entrée:
	// Param1: Nouveau pourcentage de vie.
	void ModifierPourcentageVie(float _fNouveauPourcentage) {

		m_fPourcentageVie = _fNouveauPourcentage;

		m_RectDestinationBarre.w = m_fPourcentageVie * 36;
		m_RectSourceBarre.w = m_fPourcentageVie * 36;
	}

	// Accesseurs...

	float ObtenirVie(void) {
		return m_fPourcentageVie;
	}

	SDL_Rect ObtenirRectDestination() {
		return m_RectDestinationContour;
	}

};