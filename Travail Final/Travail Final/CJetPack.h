// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack {
private:

	CSprite* m_pSpriteJetPack;
	bool m_boSpace;

public:

	CJetPack(CSprite* _pSpriteJetPack) {
		m_pSpriteJetPack = _pSpriteJetPack;
	}

	bool ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse) {

		switch (_pEvent->type) {
		case SDL_KEYDOWN:
			switch (_pEvent->key.keysym.scancode) {
			case SDL_SCANCODE_RIGHT:

				if (m_boSpace) {
					m_pSpriteJetPack->DefinirAnimation(0);
					_pVecteurVitesse->ModifierVecteur(35, 315);
					return false;
				}

				break;
			case SDL_SCANCODE_LEFT:

				if (m_boSpace) {
					m_pSpriteJetPack->DefinirAnimation(0);
					_pVecteurVitesse->ModifierVecteur(35, 225);
					return false;
				}

				break;
			case SDL_SCANCODE_SPACE:

				m_boSpace = true;

				break;
			}

			break;
		case SDL_KEYUP:

			m_boSpace = false;

			break;
		}
	}

	void ShowJetPack(SDL_Renderer* _pRenderer, SDL_Rect m_RectPlayerDestination) {
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, m_RectPlayerDestination);
	}

	void DefinirActif(bool _boActif) {
		m_pSpriteJetPack->DefinirActif(_boActif);
	}

	CSprite* ObtenirSprite() {
		return m_pSpriteJetPack;
	}
};