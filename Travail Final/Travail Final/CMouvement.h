// Classe qui gère les mouvement du joueur actif
// Créée par Gabriel Beaudry le 06 décembre 2014 (gabriel.bdry@gmail.com)

class CMouvement : public CDeplacement {
private:

	CSprite *m_pSpriteCourse,			// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de course.
		*m_pSpriteSaut,				// pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de saut.
		*m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est au repos.

	unsigned int m_uiCompte;

	/*
	m_uiCompte++;
	if (m_uiCompte == 100) {
		char chr = 'lol';
	}
	*/
public:

	CMouvement(CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, SDL_Rect _RectDestination) {

		m_uiCompte = 0;

		m_pSpriteCourse = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceCourse"), _pGestionnaireTexture->ObtenirDonnee("pTextureCourse"), _RectDestination, 9, 50, true, false, 2);

		m_pSpriteSaut = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceSaut"), _pGestionnaireTexture->ObtenirDonnee("pTextureSaut"), _RectDestination, 9, 100, false, false, 2);

		m_pSpriteRepos = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceRepos"), _pGestionnaireTexture->ObtenirDonnee("pTextureRepos"), _RectDestination, 1, 50, true, false, 2);
	}

	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) {

			// Event scancode...
			switch (_pEvent->key.keysym.scancode) {
			case SDL_SCANCODE_RIGHT: // Flèche de droite appuyée...

				if (_pEvent->type == SDL_KEYDOWN) {
					
					// Sprite de course déjà actif ou non...
					switch (m_pSpriteCourse->IsActif()) {
					case true:
						m_pSpriteCourse->DefinirEtage(0);
						_pVecteurVitesse->ModifierComposantX(35);
						break;
					case false:
						m_pSpriteRepos->DefinirEtage(0);
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(0);
						m_pSpriteCourse->DefinirActif(true);
						_pVecteurVitesse->ModifierComposantX(35);
						break;
					}
					*_boStable = false;
				}
				else
				{
					m_pSpriteCourse->DefinirActif(false);
					_pVecteurVitesse->ModifierComposantX(0);
					if (!m_pSpriteSaut->IsActif()) {
						m_pSpriteRepos->DefinirActif(true);
						*_boStable = true;
					}
				}
				break;

			case SDL_SCANCODE_LEFT: // Flèche de gauche appuyée...

				if (_pEvent->type == SDL_KEYDOWN) {
					switch (m_pSpriteCourse->IsActif()) {
					case true:
						m_pSpriteCourse->DefinirEtage(1);
						_pVecteurVitesse->ModifierComposantX(-35);
						break;
					case false:
						m_pSpriteRepos->DefinirEtage(1);
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(1);
						m_pSpriteCourse->DefinirActif(true);
						_pVecteurVitesse->ModifierComposantX(-35);
						break;
					}
					*_boStable = false;
				}
				else
				{
					m_pSpriteCourse->DefinirActif(false);
					_pVecteurVitesse->ModifierComposantX(0);
					if (!m_pSpriteSaut->IsActif()) {
						m_pSpriteRepos->DefinirActif(true);
						*_boStable = true;
					}
				}
				break;

			case SDL_SCANCODE_SPACE: // Saut

				if (_pEvent->type == SDL_KEYDOWN) {
					// S'il n'était pas déjà actif...
					if (!m_pSpriteSaut->IsActif()) {
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirActif(false);
						m_pSpriteSaut->DefinirEtage(m_pSpriteCourse->ObtenirEtage()); // Pour que le saut sois du même bord que la course.
						m_pSpriteSaut->DefinirActif(true);
						_pVecteurVitesse->ModifierComposantY(-100);
						*_boStable = false;
					}
				}
				else
				{

					m_pSpriteSaut->DefinirActif(true);
					*_boStable = false;
				}
				break;
			}
	}

	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		
		if (m_pSpriteRepos->IsActif()) {
			m_pSpriteRepos->ModifierAnnimation();
			m_pSpriteRepos->Render(_pRenderer, _RectPlayerDestination);
		}
		else if (m_pSpriteCourse->IsActif()) {
			m_pSpriteCourse->ModifierAnnimation();
			m_pSpriteCourse->Render(_pRenderer, _RectPlayerDestination);
		}
		else if (m_pSpriteSaut->IsActif()) {
			m_pSpriteSaut->ModifierAnnimation();
			m_pSpriteSaut->Render(_pRenderer, _RectPlayerDestination);
		}
	}

	void DefinirActif(bool _boActif) {}

	CSprite* ObtenirSprite(string _strNom) {
		if (_strNom == "Course") {
			return m_pSpriteCourse;
		}
		else if (_strNom == "Saut") {
			return m_pSpriteSaut;
		}
		else if (_strNom == "Repos") {
			return m_pSpriteRepos;
		}
	}

	unsigned int ObtenirMunition() { return 0; }

	void DefinirboShowDescription(bool _boShow) {}

	// Accesseurs...
	CSprite* ObtenirSpriteActif() {
		if (m_pSpriteCourse->IsActif()) {
			return m_pSpriteCourse;
		}
		else if (m_pSpriteSaut->IsActif()) {
			return m_pSpriteSaut;
		}
		else if (m_pSpriteRepos->IsActif()) {
			return m_pSpriteRepos;
		}
	}
};