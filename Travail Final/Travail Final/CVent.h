// Classe représentant une direction et force d'un vent
// Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 09 novembre 2014
// Modifié par Julien Dufresne (dufresne_julien@hotmail.ca) le 13 décembre 2014
//   - Ajout du vecteur vitesse du vent.
class CVent {
private:

	SDL_Texture* m_pDirectionVent;
	SDL_Texture* m_pForceVent;
	SDL_Point* m_RotPoint;
	SDL_Rect m_RectDestinationFleche;
	SDL_Rect m_RectDestinationText;
	int m_iAngle;
	CVecteur2D* m_pVecteurVitesseVent; // Le vecteur de la force du vent.

public:

	// 
	CVent(TTF_Font* _pFont, char* _chrForceVent, SDL_Color _ColorText, SDL_Texture* _pTextureDirectionVent, SDL_Rect _RectDestinationText, int _uiAngle, SDL_Renderer* _pRenderer) {
		
		m_iAngle = _uiAngle;

		m_RotPoint = new SDL_Point();

		// Création de la texture du texte
		SDL_Surface* pSurfaceText = TTF_RenderText_Blended(_pFont, _chrForceVent, _ColorText);
		m_pForceVent = SDL_CreateTextureFromSurface(_pRenderer, pSurfaceText);

		//Création de la texture de la direction
		m_pDirectionVent = _pTextureDirectionVent;

		// initialisation du rect de destination
		// Texte
		m_RectDestinationText.x = _RectDestinationText.x;
		m_RectDestinationText.y = _RectDestinationText.y;
		m_RectDestinationText.w = pSurfaceText->w;
		m_RectDestinationText.h = pSurfaceText->h;
		
		//Fleche
		int w, h;
		SDL_QueryTexture(m_pDirectionVent, nullptr, nullptr, &w, &h);
		m_RectDestinationFleche.x = _RectDestinationText.x - (w / 2 - pSurfaceText->w / 2);
		m_RectDestinationFleche.y = _RectDestinationText.y + pSurfaceText->h + 20;
		m_RectDestinationFleche.w = w;
		m_RectDestinationFleche.h = h;

		m_RotPoint->x = m_RectDestinationFleche.w/2;
		m_RotPoint->y = m_RectDestinationFleche.h/2;

		SDL_FreeSurface(pSurfaceText);

		m_pVecteurVitesseVent = new CVecteur2D(0, 0.0f);
	}

	~CVent() {
		SDL_DestroyTexture(m_pForceVent);
	}

	
	void ModifierForce(TTF_Font* _pFont, const char* _chrForceVent, SDL_Color _ColorText, SDL_Renderer* _pRenderer) {

   		SDL_DestroyTexture(m_pForceVent);

		// Création de la texture du texte
		SDL_Surface* pSurfaceText = TTF_RenderText_Blended(_pFont, _chrForceVent, _ColorText);
		m_pForceVent = SDL_CreateTextureFromSurface(_pRenderer, pSurfaceText);
		SDL_FreeSurface(pSurfaceText);

		// initialisation du rect de destination
		// Texte
		m_RectDestinationText.w = pSurfaceText->w;
		m_RectDestinationText.h = pSurfaceText->h;
		//Fleche
		int w, h;
		SDL_QueryTexture(m_pDirectionVent, nullptr, nullptr, &w, &h);
		m_RectDestinationFleche.x = m_RectDestinationText.x - (w / 2 - pSurfaceText->w / 2);
		m_RectDestinationFleche.y = m_RectDestinationText.y + pSurfaceText->h + 15 + h / 2;

	}

	void ModifierAngle(unsigned int _iAngle) {
		m_iAngle = _iAngle;
	}

	void ModifierVecteurVent(double _dforceVent, float _fAngle) {

		m_pVecteurVitesseVent->ModifierVecteur(_dforceVent, _fAngle);
	}

	void ShowVent(SDL_Renderer* _pSDLRenderer) {
		SDL_RenderCopyEx(_pSDLRenderer, m_pDirectionVent, NULL, &m_RectDestinationFleche, m_iAngle, m_RotPoint, SDL_FLIP_NONE);
		SDL_RenderCopy(_pSDLRenderer, m_pForceVent, NULL, &m_RectDestinationText);
	}

	CVecteur2D* ObtenirVecteurVent(void) {

		return m_pVecteurVitesseVent;
	}
};