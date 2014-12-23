// Classe qui gère les mouvement du joueur actif
// Créée par Gabriel Beaudry le 06 décembre 2014 (gabriel.bdry@gmail.com)

class CDeplacement : public CMouvement {
private:

	CSprite *m_pSpriteCourse,			// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de course.
		*m_pSpriteSaut,				// pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de saut.
		*m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est au repos.

	bool m_boSpace; // Si la barre d'espace est enfoncé...

public:

	// Contructeur...
	CDeplacement(SDL_Rect _RectDestination) {

		m_boSpace = false;

		m_pSpriteCourse = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceCourse"), pGestionnaireTexture->ObtenirDonnee("pTextureCourse"), _RectDestination, 9, 50, true, false, 2);

		m_pSpriteSaut = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceSaut"), pGestionnaireTexture->ObtenirDonnee("pTextureSaut"), _RectDestination, 5, 70, false, false, 2);

		m_pSpriteRepos = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceRepos"), pGestionnaireTexture->ObtenirDonnee("pTextureRepos"), _RectDestination, 1, 50, true, false, 2);
	}

	// Classe réagissant au évémement du programme...
	// En entrée: 
	// Param1: Le gestionnaire d'événement
	// Param2: Le vecteur vitesse du joueur.
	// Param3: Le boléan de stabilité du joueur.
	void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) {

		// Event scancode...
		switch (_pEvent->key.keysym.scancode) {
		case SDL_SCANCODE_RIGHT: // Flèche de droite appuyée...

			// Si le joeur ne saute pas...
			if (!m_pSpriteSaut->IsActif()) {
				
				// Si c'est la touche qui est enfoncé
				if (_pEvent->type == SDL_KEYDOWN) {
					
					// Sprite de course déjà actif ou non...
					if (!m_pSpriteCourse->IsActif()) {
						_pVecteurVitesse->ModifierComposantX(35);
						m_pSpriteCourse->DefinirEtage(0);
						m_pSpriteRepos->DefinirEtage(0);
						m_pSpriteCourse->DefinirActif(true);
						m_pSpriteRepos->DefinirActif(false);
						*_boStable = false;
					}


				}

				// Si la touche n'est plus enfoncé
				else if (m_pSpriteCourse->IsActif()) {

					m_pSpriteCourse->DefinirActif(false);
					_pVecteurVitesse->ModifierComposantX(0);
					m_pSpriteRepos->DefinirActif(true);

				}

			}
			break;

		case SDL_SCANCODE_LEFT: // Flèche de gauche appuyée...

			// Si le joeur ne saute pas...
			if (!m_pSpriteSaut->IsActif()) {

				// Si c'est la touche qui est enfoncé
				if (_pEvent->type == SDL_KEYDOWN) {

					// Sprite de course déjà actif ou non...
					if (!m_pSpriteCourse->IsActif()) {

						_pVecteurVitesse->ModifierComposantX(-35);
						m_pSpriteCourse->DefinirEtage(1);
						m_pSpriteRepos->DefinirEtage(1);
						m_pSpriteCourse->DefinirActif(true);
						m_pSpriteRepos->DefinirActif(false);
						*_boStable = false;

					}

				}

				// Si la touche n'est plus enfoncé
				else if (m_pSpriteCourse->IsActif()) {

					m_pSpriteCourse->DefinirActif(false);
					_pVecteurVitesse->ModifierComposantX(0);
					m_pSpriteRepos->DefinirActif(true);

				}

			}
			break;

		case SDL_SCANCODE_SPACE: // Saut

			// Si c'est la touche qui est enfoncé
			if (_pEvent->type == SDL_KEYDOWN) {

				// Si le joeur ne saute pas...
				if (!m_pSpriteSaut->IsActif()) {

					// Si le joueur bouge...
					if (m_pSpriteRepos->IsActif()) {
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteSaut->DefinirEtage(m_pSpriteRepos->ObtenirEtage()); // Pour que le saut sois du même bord que le repos.
						_pVecteurVitesse->ModifierComposantX(50 + m_pSpriteRepos->ObtenirEtage() * -100 );
					}

					// Si le joueur cours...
					else if (m_pSpriteCourse->IsActif()) {
						m_pSpriteCourse->DefinirActif(false);
						m_pSpriteSaut->DefinirEtage(m_pSpriteCourse->ObtenirEtage()); // Pour que le saut sois du même bord que le repos.
						_pVecteurVitesse->ModifierComposantX(50 + m_pSpriteCourse->ObtenirEtage() * -100);
					}

					// Dédéfinition des variables...
					m_pSpriteSaut->DefinirPositionDeBouclage(0, 5);
					m_pSpriteSaut->DefinirboBoucle(false);
					m_pSpriteSaut->DefinirActif(true);
					_pVecteurVitesse->ModifierComposantY(-125);
					*_boStable = false;
				}

			}

			break;

		}
	}

	// Procédure affichant le déplacement...
	// En entrée: 
	// Param1: Le renderer de la fenêtre.
	// Param2: Le rect destination du joueur.
	void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		
		// Affiches le sprite course...
		m_pSpriteCourse->ModifierAnnimation();
		m_pSpriteCourse->Render(_pRenderer, _RectPlayerDestination);
		
		// Si le joueur saute...
		if (m_pSpriteSaut->IsActif()) {

			// Affiche le sprite...
			m_pSpriteSaut->ModifierAnnimation();
			m_pSpriteSaut->Render(_pRenderer, _RectPlayerDestination);

			// Si le sprite est fini...
			if (!m_pSpriteSaut->IsActif()) {
				m_pSpriteSaut->DefinirActif(true);
				m_pSpriteSaut->DefinirboBoucle(true);
				m_pSpriteSaut->DefinirPositionDeBouclage(4, 5);
				
			}
		}
		
		// Affiche le sprite repos...
		m_pSpriteRepos->Render(_pRenderer, _RectPlayerDestination);
		
	}


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

	bool IsActive(void) {

		return true;
	}


	// Métodes inutiles pour cette classe...

	void ShowDescription(SDL_Renderer* _pRenderer) {}

	void DefinirActif(bool _boActif) {}

	unsigned int ObtenirMunition() {
		return NULL;
	}

	CVecteur2D* ObtenirVecteur(void) {

		return nullptr;
	}

	void UpdateDescription(bool _boShow, SDL_Point _PositionDescription) {}

};