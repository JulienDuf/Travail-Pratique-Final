// 420-202-RE : Travail final
// Classe permettant de g�rer l'affichage d'une section d'un objet qui peut varier.
// Cr��e par Gabriel Beaudry le 19 novembre 2014 (gabriel.bdry@gmail.com)

class CScrollBar {
private:

	SDL_Texture *m_pTrackTexture, // Texture de la track.
		*m_pBarTexture; // Texture de la barre.

	SDL_Rect m_RectDestinationTrack, // Rect de la track.
		m_RectTrueTrack, // Rect de la vraie track dans laquelle glisse la barre.
		m_RectBar; // Rect de la barre.

	bool m_boClick; // Bool�en pour savoir si la souris est click�e.

public:

	// Constructeur de la scrollbar...
	// En entr�e:
	// Param1: Rect de destination de la scrollbar.
	// Param2: La longueur du scroll.
	// Param3: Renderer d'affichage
	CScrollBar(SDL_Rect _RectScrollBarDestination, unsigned int _uiScrollingSize, SDL_Renderer* _pRenderer) {

		m_boClick = false;

		// Initialisation des rects...
		m_RectDestinationTrack = _RectScrollBarDestination;

		m_RectTrueTrack.x = m_RectDestinationTrack.x + 4;
		m_RectTrueTrack.y = m_RectDestinationTrack.y + 3;
		m_RectTrueTrack.w = m_RectDestinationTrack.w - 8;
		m_RectTrueTrack.h = m_RectDestinationTrack.h - 6;
		
		m_RectBar.x = m_RectTrueTrack.x;
		m_RectBar.y = m_RectTrueTrack.y;
		m_RectBar.w = ((m_RectTrueTrack.w * m_RectTrueTrack.w) / _uiScrollingSize);
		m_RectBar.h = m_RectTrueTrack.h;

		// Fill rect Track Texture...
		SDL_Surface* pSurfacetmp = SDL_CreateRGBSurface(NULL, m_RectDestinationTrack.w, m_RectDestinationTrack.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pSurfacetmp, NULL, SDL_MapRGB(pSurfacetmp->format, 255, 255, 255));
		m_pTrackTexture = SDL_CreateTextureFromSurface(_pRenderer, pSurfacetmp);
		SDL_FreeSurface(pSurfacetmp);

		// Fill rect Bar Texture...
		pSurfacetmp = SDL_CreateRGBSurface(NULL, m_RectDestinationTrack.w, m_RectDestinationTrack.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pSurfacetmp, NULL, SDL_MapRGB(pSurfacetmp->format, 160, 160, 160));
		m_pBarTexture = SDL_CreateTextureFromSurface(_pRenderer, pSurfacetmp);
		SDL_FreeSurface(pSurfacetmp);

	}

	~CScrollBar() {

		SDL_DestroyTexture(m_pBarTexture);
		SDL_DestroyTexture(m_pTrackTexture);
	}

	// Proc�dure permettant d'afficher la scrollbar...
	// En entr�e:
	// Param1: Renderer d'affichage.
	void ShowScrollBar(SDL_Renderer *_pSDLRenderer) {

		// Show Bar...
		SDL_RenderCopy(_pSDLRenderer, m_pTrackTexture, nullptr, &m_RectDestinationTrack);
		SDL_RenderCopy(_pSDLRenderer, m_pBarTexture, nullptr, &m_RectBar);

	}
	
	// Fonction permettant de g�rer les events associ�s � la scrollbar (MouseWheel, MouseMotion, Click, Unclick)
	// En entr�e:
	// Param1: SDL_Event
	// En sortie:
	// La position du event click selon la vrai longueur de l'objet � laquelle la scrollbar est rattach�e � partir du 0 de l'objet en question.
	unsigned int ReactToEvent(SDL_Event *_pEvent) {

		// Switch pour g�rer les �venements de la scrollbar...
		switch (_pEvent->type) {
		case SDL_MOUSEWHEEL: // Mouvement de la roue de la souris...
			// Si la barre peut continuer dans le sens de la roue(amplifi�e) sans d�passer la limite de la truetrack...
			if (((m_RectBar.x + _pEvent->wheel.y * 8) >= m_RectTrueTrack.x) && ((m_RectBar.x + _pEvent->wheel.y * 8 + m_RectBar.w) <= (m_RectTrueTrack.x + m_RectTrueTrack.w))) {
				m_RectBar.x += _pEvent->wheel.y * 8; // Amplification du mouvement.
			}
			else
			{
				// Si la barre peut continuer dans le sens de la roue sans d�passer la limite de la truetrack...
				if (((m_RectBar.x + _pEvent->wheel.y) >= m_RectTrueTrack.x) && ((m_RectBar.x + _pEvent->wheel.y + m_RectBar.w) <= (m_RectTrueTrack.x + m_RectTrueTrack.w)))
					m_RectBar.x += _pEvent->wheel.y;
			}
			break;
		case SDL_MOUSEMOTION: // Mouvement de la souris
			// Si la souris est click�e...
			if (m_boClick) {
				// Limites de la barre...
				if (((m_RectBar.x + _pEvent->motion.xrel) >= m_RectTrueTrack.x) && ((m_RectBar.x + _pEvent->motion.xrel + m_RectBar.w) <= (m_RectTrueTrack.x + m_RectTrueTrack.w)))
					m_RectBar.x += _pEvent->motion.xrel;
			}
			break;
		case SDL_MOUSEBUTTONDOWN: // Click de la souris
			// V�rification si la position de la souris lors du click �tait sur la barre...
			if ((_pEvent->motion.x >= m_RectBar.x) && (_pEvent->motion.y >= m_RectBar.y) && (_pEvent->motion.x <= (m_RectBar.x + m_RectBar.w)) && (_pEvent->motion.y <= (m_RectBar.y + m_RectBar.h))) {
				m_boClick = true;
			}

			break;
		case SDL_MOUSEBUTTONUP: // Relachement de la souris
			m_boClick = false;
			break;
		}

		return (float)(m_RectBar.x - m_RectTrueTrack.x) * ((float)m_RectTrueTrack.w / (float)m_RectBar.w); // Retour de la position.
	}
};