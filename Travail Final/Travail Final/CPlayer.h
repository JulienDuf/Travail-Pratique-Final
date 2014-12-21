//// 420-202-RE : Travail final
// Classe qui représente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
// Modifier le 25 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Finalisation des mouvements.
// 02 décembre 2014, gabriel beaudry (gabriel.bdry@gmail.com)
// -Modification du reacttoevent pour la gestion des objets de la toolbar, début du jetpack, ajout de la classe jetpack
// Fin des modifications.


class CPlayer {

private:


	bool m_boStable;					// Si le joueur est stable.
	bool m_boToolActif;					// Si le joueur est en utilisation d'un outil.
	bool m_boChuteLibre;				// Si le joueur est en chute libre.
	bool m_boGlissade;					// Si le joueur est en train de glisser :)
	

	double m_dPositionX;
	double m_dPositionY;


	CListeDC<CProjectile*>* m_pListeTools;

	CListeDC<CMouvement*>* m_pListeMouvement;

	CBarreVie* m_pBarreVie;				// La barre de vie du personnage.

	CSprite* m_pSpriteParachute;

	SDL_Rect m_RectPlayerDestination,   // La destination du joueur dans le fenêtre.
		m_RectParachuteDestination,   // La destination du joueur dans le fenêtre.
		m_RectHitboxCorpsGauche,
		m_RectHitboxCorpsDroite,
		m_RectHitboxPieds,
		m_RectHitboxPiedsParachute;

	CVecteur2D* m_pVecteurVitesse;			// Vitesse.
	CVecteur2D* m_pVecteurPoids; 

public:

	// Constructeur...
	// Paramètre: _pSpriteCourse, pointe sur le sprite qui représente le joueur qui est en état de course.
	// Paramètre: _pSpriteSaut, pointe sur le sprite qui représente le joueur qui est en état de saut.
	// Paramètre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Paramètre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).

	CPlayer(string _strEmplacement, SDL_Renderer* _pRenderer, unsigned int _uiIDTeam, SDL_Rect _RectDestination, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {
		
		m_pListeTools = new CListeDC<CProjectile*>();
		m_pListeMouvement = new CListeDC<CMouvement*>();

		m_boStable = false;
		m_boToolActif = false;
		m_boGlissade = false;
		m_boChuteLibre = false;

		
		m_pListeMouvement->AjouterFin(new CMelee(_strEmplacement, new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceMelee"), pGestionnaireTexture->ObtenirDonnee("pTextureMelee"), _RectDestination, 30, 30, true, true, 2), _pRenderer));

		m_pListeMouvement->AjouterFin(new CJetPack(_strEmplacement, new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceJetPack"), pGestionnaireTexture->ObtenirDonnee("pTextureJetPack"), _RectDestination, 6, 80, true, false, 2), new CBarreVie({ _RectDestination.x, _RectDestination.y + _RectDestination.h - 2, 0, 0 }, 5), _pRenderer));
		
		m_pListeMouvement->AjouterFin(new CDeplacement(_RectDestination));

		m_pListeMouvement->AllerACurseur(2);
		m_pListeMouvement->AllerATrieur(0);

		m_pListeTools->AjouterFin(new CMissile(_strEmplacement, _pRenderer, _MapDestruction, _Rotation));

		m_pListeTools->AjouterFin(new CGrenade(_strEmplacement, _pRenderer, _MapDestruction, _Rotation));

		m_pListeTools->AllerACurseur(0);
		m_pListeTools->AllerATrieur(0);

		m_pSpriteParachute = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceParachute"), pGestionnaireTexture->ObtenirDonnee("pTextureParachute"), _RectDestination, 24, 20, true, true, 1);

		m_RectPlayerDestination = _RectDestination;
		m_RectPlayerDestination.w = ObtenirSpriteCourse()->ObtenirRectSource().w;
		m_RectPlayerDestination.h = ObtenirSpriteCourse()->ObtenirRectSource().h;
		m_RectParachuteDestination = _RectDestination;
		m_RectParachuteDestination.w = m_pSpriteParachute->ObtenirRectSource().w;
		m_RectParachuteDestination.h = m_pSpriteParachute->ObtenirRectSource().h;


		m_RectHitboxCorpsGauche.x = 0;
		m_RectHitboxCorpsGauche.y = 0;
		m_RectHitboxCorpsGauche.w = 10;
		m_RectHitboxCorpsGauche.h = 40;

		m_RectHitboxCorpsDroite.x = 30;
		m_RectHitboxCorpsDroite.y = 0;
		m_RectHitboxCorpsDroite.w = 10;
		m_RectHitboxCorpsDroite.h = 40;

		m_RectHitboxPieds.x = 0;
		m_RectHitboxPieds.y = 45;
		m_RectHitboxPieds.w = 40;
		m_RectHitboxPieds.h = 10;

		m_RectHitboxPiedsParachute.x = 0;
		m_RectHitboxPiedsParachute.y = 92;
		m_RectHitboxPiedsParachute.w = 58;
		m_RectHitboxPiedsParachute.h = 19;

		m_pBarreVie = new CBarreVie({ _RectDestination.x, _RectDestination.y - 2, 0, 0 }, _uiIDTeam);

		m_pVecteurVitesse = new CVecteur2D(0, 0.0f);
		m_pVecteurPoids = new CVecteur2D(0, 0.0f);

	}

	// Procédure qui affiche le joueur.
	// Paramètre: _pSDLRenderer, Rendeur de la fenêtre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ReactToEvent(SDL_Event* _pSDLEvent, unsigned int _uiObjetSelectionner) {
		if (!m_pSpriteParachute->IsActif()) {
			if (_uiObjetSelectionner <= 1) {
				m_boToolActif = true;
				m_pListeTools->AllerACurseur(_uiObjetSelectionner);
				m_pListeTools->ObtenirElementCurseur()->ReactToEvent(_pSDLEvent);
				
			}
			else
			{
				if (_uiObjetSelectionner <= 3) {
					m_pListeMouvement->AllerACurseur(_uiObjetSelectionner - 2);
					if (!m_pListeMouvement->ObtenirElementCurseur()->ObtenirSprite("")->IsActif()) 
						m_pListeMouvement->ObtenirElementCurseur()->ObtenirSprite("")->DefinirEtage(ObtenirSpriteRepos()->ObtenirEtage());
						
					
				}
				else
					m_pListeMouvement->AllerACurseur(2);
				m_pListeMouvement->ObtenirElementCurseur()->ReactToEvent(_pSDLEvent, m_pVecteurVitesse, &m_boStable);
			}
		}
	}

	// Procédure qui sert à afficher le joueur.
	//Paramètre : _pRenderer : Le render de la fenetre.
	//Retour : rien.
	void ShowPlayer(SDL_Renderer* _pRenderer) {
		
		m_RectPlayerDestination.x = m_dPositionX;
		m_RectPlayerDestination.y = m_dPositionY;
		
		m_pBarreVie->ModifierPositionBarre(m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2);

		if (m_pSpriteParachute->IsActif()) {
			m_pSpriteParachute->ModifierAnnimation();
			m_pSpriteParachute->Render(_pRenderer, m_RectParachuteDestination);
		}
		else
		{
			m_pListeMouvement->ObtenirElementCurseur()->ShowPlayer(_pRenderer, m_RectPlayerDestination);

			m_pBarreVie->ShowBarre(_pRenderer, { m_RectPlayerDestination.x, m_RectPlayerDestination.y - 2, 40, 6 });

			if (m_boToolActif)
				m_pListeTools->ObtenirElementCurseur()->ShowTool(_pRenderer, m_RectPlayerDestination);
		}
		SDL_SetRenderDrawColor(_pRenderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(_pRenderer, &m_RectPlayerDestination);
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		m_pListeMouvement->AllerATrieur(0);
		m_pListeTools->AllerATrieur(0);
		for (int i = 0; i < 2; i++) {
			m_pListeMouvement->ObtenirElementTrieur()->ShowDescription(_pRenderer);
			m_pListeTools->ObtenirElementTrieur()->ShowDescription(_pRenderer);
			m_pListeMouvement->AllerSuivantTrieur();
			m_pListeTools->AllerSuivantTrieur();
		}
	}

	// Accesseur ... 

	void ReinitialisationTool(void) {

		m_pListeTools->AllerDebut();
		for (int i = 0; i < m_pListeTools->ObtenirCompte(); i++) {

			m_pListeTools->ObtenirElementCurseur()->ReinitialisationProjectile();
			m_pListeTools->AllerSuivantCurseur();
		}
	}

	void SetHealth(float _fHealth) {


		m_pBarreVie->ModifierPourcentageVie(_fHealth);
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

	void ModifierChuteLibreJoueur(bool _boChuteLibre) {

		m_boChuteLibre = _boChuteLibre;
	}

	void ModifierGlissadeJoueur(bool _boGlissade) {
		m_boGlissade = _boGlissade;
	}

	void ModifierTypeMouvement(int _iPositionCurseur) {

		m_pListeMouvement->AllerACurseur(_iPositionCurseur);
	}

	void DefinirPositionX(double _dX) {

		m_dPositionX = _dX;
	}

	void DefinirPositionY(double _dY) {

		m_dPositionY = _dY;
	}

	void DefinirToolActif(bool _boActif){

		m_boToolActif = _boActif;
	}

	CSprite* ObtenirSpriteCourse() {
		m_pListeMouvement->AllerATrieur(2);
		return m_pListeMouvement->ObtenirElementTrieur()->ObtenirSprite("Course");
	}

	CSprite* ObtenirSpriteRepos() {
		m_pListeMouvement->AllerATrieur(2);
		return m_pListeMouvement->ObtenirElementTrieur()->ObtenirSprite("Repos");
	}

	CSprite* ObtenirSpriteSaut() {
		m_pListeMouvement->AllerATrieur(2);
		return m_pListeMouvement->ObtenirElementTrieur()->ObtenirSprite("Saut");
	}

	CSprite* ObtenirSpriteJetPack() {
		m_pListeMouvement->AllerATrieur(1);
		return m_pListeMouvement->ObtenirElementTrieur()->ObtenirSprite("");
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

	bool IsFreeFalling(void) {

		return m_boChuteLibre;
	}

	bool IsUsingTool(void) {

		return m_boToolActif;
	}

	bool IsSliding(void) {
		return m_boGlissade;
	}

	double ObtenirPositionX(void) {

		return m_dPositionX;
	}

	double ObtenirPositionY(void) {

		return m_dPositionY;
	}

	CVecteur2D* ObtenirVecteurVitesse(void) {
		return m_pVecteurVitesse;
	}

	CVecteur2D* ObtenirVecteurTool() {

		return m_pListeTools->ObtenirElementCurseur()->ObtenirVecteurVitesse();
	}

	SDL_Rect* ObtenirRectTool() {

		return m_pListeTools->ObtenirElementCurseur()->ObtenirRectDestination();
	}

	CVecteur2D* ObtenirVecteurPoids(void) {

		return m_pVecteurPoids;

	}

	CProjectile* ObtenirProjectile(unsigned int _uiPosition) {

		m_pListeTools->AllerACurseur(_uiPosition);
		return m_pListeTools->ObtenirElementCurseur();
	}

	CMouvement* ObtenirJetPack(void) {

		m_pListeMouvement->AllerACurseur(1);
		return m_pListeMouvement->ObtenirElementCurseur();
	}

	float GetHealth(void) {

		return m_pBarreVie->ObtenirVie();
	}

	void UpdateDescription(unsigned int _uiPosition, SDL_Rect _RectPositionSouris) {
		if (_uiPosition <= 1) {
			m_pListeTools->AllerATrieur(_uiPosition);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(true, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeMouvement->AllerATrieur(0);
			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeMouvement->AllerSuivantTrieur();
			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
		else if (_uiPosition <= 3) {
			int i = _uiPosition - 2;
			m_pListeMouvement->AllerATrieur(i);
			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(true, _RectPositionSouris);

			if (i == 0)
				m_pListeMouvement->AllerSuivantTrieur();
			else
				m_pListeMouvement->AllerPrecedentTrieur();

			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerATrieur(0);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
		else
		{
			m_pListeMouvement->AllerATrieur(0);
			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeMouvement->AllerSuivantTrieur();
			m_pListeMouvement->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerATrieur(0);
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
			m_pListeTools->AllerSuivantTrieur();
			m_pListeTools->ObtenirElementTrieur()->UpdateDescription(false, _RectPositionSouris);
		}
	}

	unsigned int ObtenirMunition(unsigned int _uiPosition) {
		if (_uiPosition <= 1) {
			m_pListeTools->AllerATrieur(_uiPosition);
			return m_pListeTools->ObtenirElementTrieur()->ObtenirMunition();
		}
		else if (_uiPosition == 3) {
			m_pListeMouvement->AllerATrieur(_uiPosition - 2);
			return m_pListeMouvement->ObtenirElementTrieur()->ObtenirMunition();
		}
	}

	void ParcoursChangementTour() {

	}

	
};