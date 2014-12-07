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

	bool m_boStable;					// Si le joueur est stable
<<<<<<< HEAD
	bool m_boChuteLibre;				// Si le joueur est en chute libre. 
=======
	bool m_boToolActif;
>>>>>>> origin/Branche-jetpack

	CListeDC<CTool*>* m_pListeTools;

	CListeDC<CDeplacement*>* m_pListeDeplacement;

	CBarreVie* m_pBarreVie;				// La barre de vie du personnage.

	CSprite* m_pSpriteParachute;

	SDL_Rect m_RectPlayerDestination,   // La destination du joueur dans le fenêtre.
		m_RectParachuteDestination,   // La destination du joueur dans le fenêtre.
		m_RectHitboxCorpsGauche,
		m_RectHitboxCorpsDroite,
		m_RectHitboxPieds,
		m_RectHitboxPiedsParachute;

<<<<<<< HEAD
	CVecteur2D* VecteurVitesse;			// Vitesse.
	CVecteur2D* VecteurPoids;

	double m_dX;
	double m_dY;
=======
	CVecteur2D* m_pVecteurVitesse;			// Vitesse.
>>>>>>> origin/Branche-jetpack

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
<<<<<<< HEAD
	CPlayer(string _strEmplacement, SDL_Renderer* _pRenderer, CGestionaire<TTF_Font*>* _pGestionnaireFont, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, unsigned int _uiIDTeam, SDL_Rect _RectDestination, double _dGravite, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile)) {
=======
	CPlayer(string _strEmplacement, SDL_Renderer* _pRenderer, CGestionaire<TTF_Font*>* _pGestionnaireFont, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, unsigned int _uiIDTeam, SDL_Rect _RectDestination, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {
		
		m_pListeTools = new CListeDC<CTool*>();
		m_pListeDeplacement = new CListeDC<CDeplacement*>();
>>>>>>> origin/Branche-jetpack

		m_boStable = false;
		m_boToolActif = false;

		m_pListeDeplacement->AjouterFin(new CJetPack(_strEmplacement, _pGestionnaireFont, new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceJetPack"), _pGestionnaireTexture->ObtenirDonnee("pTextureJetPack"), _RectDestination, 6, 80, true, false, 2), new CBarreVie(_pGestionnaireTexture, { _RectDestination.x, _RectDestination.y + _RectDestination.h - 2, 0, 0 }, 6), _pRenderer));
		
		m_pListeDeplacement->AjouterFin(new CMouvement(_pGestionnaireSurface, _pGestionnaireTexture, _RectDestination));

		m_pListeDeplacement->AllerACurseur(1);
		m_pListeDeplacement->AllerATrieur(0);

		m_pListeTools->AjouterFin(new CMissile(_strEmplacement, _pGestionnaireFont, _pRenderer, _pGestionnaireSurface, _pGestionnaireTexture, _MapDestruction, _CollisionObjetMap, _Physique, _Rotation));

		m_pListeTools->AjouterFin(new CGrenade(_strEmplacement, _pRenderer));

		m_pListeTools->AjouterFin(new CMelee(_strEmplacement, _pRenderer));

		m_pListeTools->AllerACurseur(0);
		m_pListeTools->AllerATrieur(0);

		m_pSpriteParachute = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceParachute"), _pGestionnaireTexture->ObtenirDonnee("pTextureParachute"), _RectDestination, 24, 20, true, true, 1);

		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = ObtenirSpriteCourse()->ObtenirRectSource().w;
		m_RectPlayerDestination.h = ObtenirSpriteCourse()->ObtenirRectSource().h;
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
		m_RectHitboxPieds.y = 50;
		m_RectHitboxPieds.w = 40;
		m_RectHitboxPieds.h = 5;

		m_RectHitboxPiedsParachute.x = 0;
		m_RectHitboxPiedsParachute.y = 92;
		m_RectHitboxPiedsParachute.w = 58;
		m_RectHitboxPiedsParachute.h = 19;

<<<<<<< HEAD
		m_pToolList = new CListeDC<CTools*>();

		m_pToolList->AjouterFin(new CMissile(_strEmplacement, _pGestionnaireSurface, _pGestionnaireTexture, _MapDestruction, _CollisionObjetMap, _Physique, Rotation));
		m_pToolList->AllerDebut();

		m_BoDeplacement = false;
		m_boChuteLibre = false;

		m_pBarreVie = new CBarreVie(_pGestionnaireTexture, { _RectDestination.x, _RectDestination.y - 2, 0, 0 }, _uiIDTeam);

		VecteurVitesse = new CVecteur2D(0, 0.0f);
		VecteurPoids = new CVecteur2D(_dGravite, 90.0f);
=======
		m_pBarreVie = new CBarreVie(_pGestionnaireTexture, { _RectDestination.x, _RectDestination.y - 2, 0, 0 }, _uiIDTeam);

		m_pVecteurVitesse = new CVecteur2D(0, 0);

>>>>>>> origin/Branche-jetpack
	}

	// Procédure qui affiche le joueur.
	// Paramètre: _pSDLRenderer, Rendeur de la fenêtre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ReactToEvent(SDL_Event* _pSDLEvent, unsigned int _uiObjetSelectionner) {
<<<<<<< HEAD

		if (!m_pSpriteParachute->IsActif() && _uiObjetSelectionner > 3) {

			switch (_pSDLEvent->type) {
			case SDL_KEYDOWN:
				switch (_pSDLEvent->key.keysym.scancode) {
				case SDL_SCANCODE_RIGHT: // Flèche de droite appuyée...

					if (!m_pSpriteCourse->IsActif()) { // S'il était au repos
						m_pSpriteRepos->DefinirEtage(0);  // Il n'est plus au repos.
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(0);
						m_pSpriteCourse->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(35);
						m_boStable = false;
						m_BoDeplacement = true;
					}

					break;

				case SDL_SCANCODE_LEFT:

					if (!m_pSpriteCourse->IsActif()) { // S'il était au repos
						m_pSpriteRepos->DefinirEtage(1);  // Il n'est plus au repos.
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirEtage(1);
						m_pSpriteCourse->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(-35);
						m_boStable = false;
						m_BoDeplacement = true;
					}

					break;

				case SDL_SCANCODE_SPACE:        // Saut = Espace

					if (!m_pSpriteSaut->IsActif()) {
						m_pSpriteRepos->DefinirActif(false);
						m_pSpriteCourse->DefinirActif(false);
						m_pSpriteSaut->DefinirEtage(m_pSpriteCourse->ObtenirAnimation()); // Pour que le saut sois du même bord que la course.
						m_pSpriteSaut->DefinirActif(true);
						VecteurVitesse->ModifierComposantX(0);
						VecteurVitesse->ModifierComposantY(-35);
						m_boStable = false;
					}

					break;
				}

				break;

			case SDL_KEYUP:
				if (!m_boChuteLibre)
					VecteurVitesse->ModifierComposantX(0);
				m_pSpriteCourse->DefinirActif(false);     // Le sprite ne court plus.
				m_pSpriteRepos->DefinirActif(true);
				m_BoDeplacement = false;
				m_boStable = false;

				break;

			}
		}
		else {

			m_pToolList->AllerACurseur(_uiObjetSelectionner);
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
=======
		if (!m_pSpriteParachute->IsActif()) {
			if (_uiObjetSelectionner <= 2) {
				m_boToolActif = true;
				m_pListeTools->AllerACurseur(_uiObjetSelectionner);
				m_pListeTools->ObtenirElementCurseur()->ReactToEvent(_pSDLEvent);
			}
			else
			{
				if (_uiObjetSelectionner == 3)
					m_pListeDeplacement->AllerACurseur(0);
				else
					m_pListeDeplacement->AllerACurseur(1);
				m_pListeDeplacement->ObtenirElementCurseur()->ReactToEvent(_pSDLEvent, m_pVecteurVitesse, &m_boStable);
>>>>>>> origin/Branche-jetpack
			}
		}
	}

	// Procédure qui sert à afficher le joueur.
	//Paramètre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {
<<<<<<< HEAD
		m_RectPlayerDestination.x = m_dX;
		m_RectPlayerDestination.y = m_dY;
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

		m_pToolList->ObtenirElementCurseur()->ShowTools(_pRenderer);
=======
		if (m_pSpriteParachute->IsActif()) {
			m_pSpriteParachute->ModifierAnnimation();
			m_pSpriteParachute->Render(_pRenderer, m_RectParachuteDestination);
		}
		else
		{
			m_pListeDeplacement->ObtenirElementCurseur()->ShowPlayer(_pRenderer, m_RectPlayerDestination);

			m_pBarreVie->ShowBarre(_pRenderer, { m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2, 40, 6 });

			if (m_boToolActif)
				m_pListeTools->ObtenirElementCurseur()->ShowTool(_pRenderer, m_RectPlayerDestination);
		}
>>>>>>> origin/Branche-jetpack
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		m_pListeDeplacement->AllerATrieur(0);
		m_pListeDeplacement->ObtenirElementTrieur()->ShowDescription(_pRenderer);
		for (int i = 0; i < 3; i++) {
			m_pListeTools->AllerATrieur(i);
			m_pListeTools->ObtenirElementTrieur()->ShowDescription(_pRenderer);
		}
	}

	// Accesseur ... 

	void SetHealth(float _fHealth) {


		m_pBarreVie->ModifierPourcentageVie(_fHealth);
	}

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

<<<<<<< HEAD
	void ModifierChuteLibreJoueur(bool _boChuteLibre) {

		m_boChuteLibre = _boChuteLibre;
	}

	CSprite* ObtenirSpriteCourse(void) {

		return m_pSpriteCourse;

=======
	CSprite* ObtenirSpriteCourse() {
		m_pListeDeplacement->AllerATrieur(1);
		return m_pListeDeplacement->ObtenirElementTrieur()->ObtenirSprite("Course");
>>>>>>> origin/Branche-jetpack
	}

	CSprite* ObtenirSpriteRepos() {
		m_pListeDeplacement->AllerATrieur(1);
		return m_pListeDeplacement->ObtenirElementTrieur()->ObtenirSprite("Repos");
	}

	CSprite* ObtenirSpriteSaut() {
		m_pListeDeplacement->AllerATrieur(1);
		return m_pListeDeplacement->ObtenirElementTrieur()->ObtenirSprite("Saut");
	}

	CSprite* ObtenirSpriteJetPack() {
		m_pListeDeplacement->AllerATrieur(0);
		return m_pListeDeplacement->ObtenirElementTrieur()->ObtenirSprite("");
	}

	CSprite* ObtenirSpriteParachute(void) {

		return m_pSpriteParachute;
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

<<<<<<< HEAD
	bool IsMoving(void) {

		return m_BoDeplacement;
	}

	bool IsFreeFalling(void) {

		return m_boChuteLibre;
	}

=======
>>>>>>> origin/Branche-jetpack
	CVecteur2D* ObtenirVecteurVitesse(void) {
		return m_pVecteurVitesse;
	}

	CVecteur2D* ObtenirVecteurPoids(void)  {

		return VecteurPoids;
	}

	float GetHealth(void) {

		return m_pBarreVie->ObtenirVie();
	}

<<<<<<< HEAD
	void DefinirPositionX(double _dX) {
		m_dX = _dX;
	}

	void DefinirPositionY(double _dY) {
		m_dY = _dY;
	}

	double ObtenirPositionX(void) {
		return m_dX;
	}

	double ObtenirPositionY(void) {
		return m_dY;
=======
	void UpdateDescription(unsigned int _uiPosition, SDL_Rect _RectPositionSouris) {
		if (_uiPosition <= 2) {
			m_pListeTools->AllerATrieur(_uiPosition);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(true, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
		else if (_uiPosition == 3) {
			m_pListeDeplacement->AllerATrieur(0);
			m_pListeDeplacement->ObtenirElementTrieur()->UpdateDescription(true, _RectPositionSouris);
			m_pListeTools->AllerATrieur(_uiPosition);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
		else
		{
			m_pListeDeplacement->AllerATrieur(0);
			m_pListeDeplacement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerATrieur(_uiPosition);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
>>>>>>> origin/Branche-jetpack
	}
};