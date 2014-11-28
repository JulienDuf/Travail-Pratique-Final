class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'équipe.
	CToolBar* m_pToolBar; // La toolbar où le joueur choisi son outils pour son tour.

	bool m_boDebutPartie; // Si le jeu est en début de partie.

public:

	CGame(string _strEmplacementMap, string _strEmplacementFichier, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, int _iNombreÉquipe, int _iNombreJoueur, CVent* _pVent, bool _VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, _strEmplacementFichier, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction, _CollisionObjetMap);

		for (int i = _iNombreÉquipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementFichier, _iNombreJoueur, _VerifierCollisionJoueurMap, _MapDestruction, _CollisionObjetMap, _Physique, _pRenderer));
		}
		m_pTeamList->AllerDebut();

		m_pToolBar = new CToolBar({ 0, 600, 1366, 168 }, { 0, 0, 0 }, _pRenderer, 265, 120, 60, 4, _pGestionnaireTexture->ObtenirDonnee("BazookaTool"), _pGestionnaireTexture->ObtenirDonnee("GrenadaTool"), _pGestionnaireTexture->ObtenirDonnee("SwordTool"), _pGestionnaireTexture->ObtenirDonnee("JetPackTool"));
	}

	void ChangerTeamActive() {
		m_pTeamList->AllerSuivantCurseur();
	}

	CTeam* ObtenirTeamActive() {
		return m_pTeamList->ObtenirElementCurseur();
	}

	void AfficherGame(SDL_Renderer* _pRenderer) {

		m_pGameMap->ShowMap(_pRenderer);

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

		m_pToolBar->ShowToolBar(_pRenderer);

	}

	void ReactToEvent(SDL_Event* _pEvent) {

		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReactToEvent(_pEvent);

		m_pToolBar->ReactToEvent(_pEvent);

	}

	CMap* ObtenirMap(void) {

		return m_pGameMap;

	}

	CListeDC<CTeam*>* ObtenirListeTeam(void) {

		return m_pTeamList;

	}

	void ReverseShowToolBar() {
		m_pToolBar->ReverseboShow();
	}

};