class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'équipe.

	bool m_boDebutPartie; // Si le jeu est en début de partie.

public:

	CGame(string _strEmplacementMap, string _strEmplacementFichier, int _iNombreÉquipe, int _iNombreJoueur, CVent* _pVent, bool _VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, _strEmplacementFichier, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction, _CollisionObjetMap);

		for (int i = _iNombreÉquipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementFichier, _iNombreJoueur, _VerifierCollisionJoueurMap, _MapDestruction, _CollisionObjetMap, _Physique, _pRenderer));
		}
		m_pTeamList->AllerDebut();
	}

	void AfficherGame(SDL_Renderer* _pRenderer) {

		m_pGameMap->ShowMap(_pRenderer);

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

	}

	void PhysiquePlayer(void) {
		CPlayer* pPlayerActif = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif();
		SDL_Rect Recttmp = pPlayerActif->ObtenirRectDestination();
		bool boCorps;
		bool boPied;
		if (!pPlayerActif->IsStable()) {
			Recttmp.x += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX();
			Recttmp.y += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY();
			//m_pTeamList->ObtenirElementCurseur()->_VerifierCollisionJoueurMap(pPlayerActif,Recttmp,&boCorps,&boPied,)
			//pPlayerActif->ModifierRectDestination(
		}
	}
	CMap* ObtenirMap(void) {

		return m_pGameMap;

	}

	CListeDC<CTeam*>* ObtenirListeTeam(void) {

		return m_pTeamList;

	}

	CTeam* ObtenirTeamActive(void) {

		return m_pTeamList->ObtenirElementCurseur();

	}

};