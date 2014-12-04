// Classe représentant la gestion d'un jetpack...
// Créée ppar Gabriel Beaudry le 02 décembre 2014 (gabriel.bdry@gmail.com)

class CJetPack {
private:

	CSprite* m_pSpriteJetPack;
	CBarreVie* m_pBarreDeCarburant;
	bool m_boSpace;

	unsigned int m_uiDepart;

public:

	CJetPack(CSprite* _pSpriteJetPack, CBarreVie* _pBarreDeCarburant) {
		m_pSpriteJetPack = _pSpriteJetPack;
		m_pBarreDeCarburant = _pBarreDeCarburant;
		m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
		m_uiDepart = 0;
	}

	bool ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse) {

		if (m_pBarreDeCarburant->ObtenirPourcentage() <= 0) {
			m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
			return false;
		}
		
		switch (_pEvent->key.keysym.scancode) {
		case SDL_SCANCODE_RIGHT:
			if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(0);
					_pVecteurVitesse->ModifierComposantX(10);
					return false;
			}
				break;
		case SDL_SCANCODE_LEFT:
			if (_pEvent->key.type == SDL_KEYDOWN && m_boSpace) {
					m_pSpriteJetPack->DefinirEtage(1);
					_pVecteurVitesse->ModifierComposantX(-10);
					return false;
			}
				break;
		case SDL_SCANCODE_SPACE:
			if (_pEvent->key.type == SDL_KEYDOWN) {
				m_pBarreDeCarburant->DiminuerPourcentageVie(0.005);
				_pVecteurVitesse->ModifierComposantY(-50);
				m_uiDepart++;
				if (m_uiDepart >= 3) {
					m_pSpriteJetPack->DefinirPositionDeBouclage(4, 6);
				}
				else
				{
					m_pSpriteJetPack->DefinirPositionDeBouclage(0, 6);
				}	
				m_boSpace = true;
				return false;
			}
			else
			{
				m_pSpriteJetPack->DefinirPositionDeBouclage(0, 1);
				m_boSpace = false;
				m_uiDepart = 0;
				return false;
			}
			break;
		}
		return false;
	}

	void ShowJetPack(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSpriteJetPack->ModifierAnnimation();
		m_pSpriteJetPack->Render(_pRenderer, _RectPlayerDestination);
		if (m_pSpriteJetPack->IsActif())
			m_pBarreDeCarburant->ShowBarre(_pRenderer, { _RectPlayerDestination.x, _RectPlayerDestination.y + _RectPlayerDestination.h + 2, 40, 6 });
	}

	void DefinirActif(bool _boActif) {
		m_pSpriteJetPack->DefinirActif(_boActif);
	}

	CSprite* ObtenirSprite() {
		return m_pSpriteJetPack;
	}
};