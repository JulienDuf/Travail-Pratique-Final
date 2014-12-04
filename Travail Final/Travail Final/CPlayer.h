
// 420-202-RE : Travail final
// Classe qui représente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
// Modifier le 25 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Finalisation des mouvements.
// 02 décembre 2014, gabriel beaudry (gabriel.bdry@gmail.com)
// -Modification du reacttoevent pour la gestion des objets de la toolbar, début du jetpack, ajout de la classe jetpack
//Fin des modifications.
class CPlayer {

private:

	bool m_BoDeplacement;				// Si le joueur se déplace.

	CSprite* m_pSpriteCourse;			// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de course.
	CSprite* m_pSpriteSaut;				// pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de saut.
	CSprite* m_pSpriteParachute;		// Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est en état de chute.
	CSprite* m_pSpriteRepos;		    // Pointeur de sprite qui pointe sur le sprite qui représente le joueur qui est au repos.

	CJetPack* m_pJetPack;

	CBarreVie* m_pBarreVie;				// La barre de vie du personnage.

	SDL_Rect m_RectPlayerDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectParachuteDestination;   // La destination du joueur dans le fenêtre.
	SDL_Rect m_RectSource;				// Affiche si le joueur est ves le gauche ou vers le droite.
	SDL_Rect m_RectHitboxCorpsGauche;
	SDL_Rect m_RectHitboxCorpsDroite;
	SDL_Rect m_RectHitboxPieds;
	SDL_Rect m_RectHitboxPiedsParachute;

	string m_strName;					// Chaine de caractères qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.

	bool m_boStable;					// Si le joueur est stable

	CVecteur2D* VecteurVitesse;			// Vitesse.

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, unsigned int _uiIDTeam, SDL_Rect _RectDestination, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile)) {

		m_boStable = false;

		m_pSpriteCourse = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceCourse"), _pGestionnaireTexture->ObtenirDonnee("pTextureCourse"), _RectDestination, 9, 50, true, false, 2);

		m_pSpriteSaut = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceSaut"), _pGestionnaireTexture->ObtenirDonnee("pTextureSaut"), _RectDestination, 9, 100, false, false, 2);

		m_pSpriteParachute = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceParachute"), _pGestionnaireTexture->ObtenirDonnee("pTextureParachute"), _RectDestination, 24, 20, true, true, 1);

		m_pSpriteRepos = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceRepos"), _pGestionnaireTexture->ObtenirDonnee("pTextureRepos"), _RectDestination, 1, 50, true, false, 2);

		m_pJetPack = new CJetPack(new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceJetPack"), _pGestionnaireTexture->ObtenirDonnee("pTextureJetPack"), _RectDestination, 6, 80, true, false, 2), new CBarreVie(_pGestionnaireTexture, { _RectDestination.x, _RectDestination.y + _RectDestination.h - 2, 0, 0 }, 6));

		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = m_pSpriteCourse->ObtenirRectSource().w;
		m_RectPlayerDestination.h = m_pSpriteCourse->ObtenirRectSource().h;
		m_RectParachuteDestination = _RectDestination;
		m_RectParachuteDestination.w = m_pSpriteParachute->ObtenirRectSource().w;
		m_RectParachuteDestination.h = m_pSpriteParachute->ObtenirRectSource().h;


		m_RectHitboxCorpsGauche.x = 0;
		m_RectHitboxCorpsGauche.y = 0;
		m_RectHitboxCorpsGauche.w = 20;
		m_RectHitboxCorpsGauche.h = 40;

		m_RectHitboxCorpsDroite.x = 20;
		m_RectHitboxCorpsDroite.y = 0;
		m_RectHitboxCorpsDroite.w = 20;
		m_RectHitboxCorpsDroite.h = 40;

		m_RectHitboxPieds.x = 0;
		m_RectHitboxPieds.y = 40;
		m_RectHitboxPieds.w = 40;
		m_RectHitboxPieds.h = 15;

		m_RectHitboxPiedsParachute.x = 0;
		m_RectHitboxPiedsParachute.y = 92;
		m_RectHitboxPiedsParachute.w = 58;
		m_RectHitboxPiedsParachute.h = 19;

		m_pToolList = new CListeDC<CTools*>();

		m_pToolList->AjouterFin(new CMissile(_pGestionnaireSurface, _pGestionnaireTexture, _MapDestruction, _CollisionObjetMap, _Physique, NULL));

		m_BoDeplacement = false;

		m_pBarreVie = new CBarreVie(_pGestionnaireTexture, { _RectDestination.x, _RectDestination.y - 2, 0, 0 }, _uiIDTeam);

		VecteurVitesse = new CVecteur2D(0, 0);

	}

	// Procédure qui affiche le joueur.
	// Paramètre: _pSDLRenderer, Rendeur de la fenêtre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ReactToEvent(SDL_Event* _pSDLEvent, unsigned int _uiObjetSelectionner) {
		
		if (!m_pSpriteParachute->IsActif() && _uiObjetSelectionner > 3) {

			switch (_pSDLEvent->type) {
			case SDL_KEYDOWN:
				switch (_pSDLEvent->key.keysym.scancode) {
				case SDL_SCANCODE_RIGHT:	// Flèche de droite appuyée...

					if (!m_pSpriteCourse->IsActif()) {	// S'il était au repos
						m_pSpriteRepos->DefinirEtage(0);  // Il n'est plus au repos.
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(0);
						m_pSpriteCourse->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(35);
						m_boStable = false;
					}

					break;

				case SDL_SCANCODE_LEFT:

					if (!m_pSpriteCourse->IsActif()) {	// S'il était au repos
						m_pSpriteRepos->DefinirEtage(1);  // Il n'est plus au repos.
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(1);
						m_pSpriteCourse->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(-35);
						m_boStable = false;
					}

					break;

				case SDL_SCANCODE_SPACE:								// Saut = Espace

					if (!m_pSpriteSaut->IsActif()) {
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirActif(false);
						m_pSpriteSaut->DefinirEtage(m_pSpriteCourse->ObtenirAnimation()); // Pour que le saut sois du même bord que la course.
						m_pSpriteSaut->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(0);
						VecteurVitesse->ModifierComposantY(-150);
						m_boStable = false;
					}

					break;
				}

				break;

			case SDL_KEYUP:

				VecteurVitesse->ModifierComposantX(0);
				m_pSpriteCourse->DefinirActif(false);					// Le sprite ne court plus.
				m_pSpriteRepos->DefinirActif(true);
				if (!m_pSpriteSaut->IsActif())
					m_boStable = true;

				break;

			}
		}
		else
		{
			switch (_uiObjetSelectionner) {
			case 0: // Bazooka

				break;
			case 1: // Grenada

				break;
			case 2: // Épée

				break;
			case 3: // JetPack

				m_pJetPack->DefinirActif(true);
				m_pSpriteCourse->DefinirActif(false);
				m_pSpriteParachute->DefinirActif(false);
				m_pSpriteRepos->DefinirActif(false);
				m_pSpriteSaut->DefinirActif(false);
   				m_boStable = m_pJetPack->ReactToEvent(_pSDLEvent, VecteurVitesse);

				break;
			}
		}
	}

	// Procédure qui sert à afficher le joueur.
	//Paramètre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {

		m_pSpriteCourse->ModifierAnnimation();
		m_pSpriteCourse->Render(_pRenderer, m_RectPlayerDestination);

		m_pSpriteParachute->ModifierAnnimation();
		m_pSpriteParachute->Render(_pRenderer, m_RectParachuteDestination);

		m_pSpriteSaut->ModifierAnnimation();
		m_pSpriteSaut->Render(_pRenderer, m_RectPlayerDestination);

		m_pJetPack->ShowJetPack(_pRenderer, m_RectPlayerDestination);
		
		m_pSpriteRepos->ModifierAnnimation();
		m_pSpriteRepos->Render(_pRenderer, m_RectPlayerDestination);

		if (!m_pSpriteParachute->IsActif())
			m_pBarreVie->ShowBarre(_pRenderer, { m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2, 40, 6 });
	}

	// Accesseur ... 



	void ModifierRectDestination(SDL_Rect _RectDestination) {

		m_RectPlayerDestination = _RectDestination;

		m_pBarreVie->ModifierPositionBarre(m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2);

	}

	void ModifierRectDestinationParachute(SDL_Rect _RectDestination) {

		m_RectParachuteDestination = _RectDestination;
	}
	
	void AjouterAPositionY(int _iY) {

		m_RectPlayerDestination.x += _iY;
	}

	void ModifierStabiliteJoueur(bool _boStable) {

		m_boStable = _boStable;
	}

	CSprite* ObtenirSpriteCourse(void) {

		return m_pSpriteCourse;

	}

	CSprite* ObtenirSpriteSaut(void) {

		return m_pSpriteSaut;

	}

	CSprite* ObtenirSpriteRepos(void) {

		return m_pSpriteRepos;

	}

	CSprite* ObtenirSpriteParachute(void) {

		return m_pSpriteParachute;

	}

	CSprite* ObtenirSpriteJetPack(void) {

		return m_pJetPack->ObtenirSprite();

	}

	SDL_Rect ObtenirHitboxCorpsDroite(void) {

		return m_RectHitboxCorpsDroite;

	}

	SDL_Rect ObtenirHitboxCorpsGauche(void) {

		return m_RectHitboxCorpsGauche;

	}

	SDL_Rect ObtenirHitboxPieds(void) {

		return m_RectHitboxPieds;

	}

	SDL_Rect ObtenirHitboxPiedsParachute(void) {

		return m_RectHitboxPiedsParachute;

	}

	SDL_Rect ObtenirRectDestination(void) {

		return m_RectPlayerDestination;
	}

	SDL_Rect ObtenirRectDestinationParachute(void) {

		return m_RectParachuteDestination;
	}

	bool IsStable(void) {

		return m_boStable;
	}

	CVecteur2D* ObtenirVecteurVitesse(void) {
		return VecteurVitesse;
	}
};