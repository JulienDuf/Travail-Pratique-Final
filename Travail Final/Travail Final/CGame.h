// 29 novembre 2014 par Gabriel Beaudry(gabriel.bdry@gmail.com)
// Ajout de la procédure changertour().

class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'équipe.
	CToolBar* m_pToolBar; // La toolbar où le joueur choisi son outils pour son tour.
	CTimer* m_pTimerPhysique; // Le timer pour la physique.

	bool m_boDebutPartie; // Si le jeu est en début de partie.
	bool(*m_pVerifierCollisionJoueurMap)(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap);

public:

	CGame(string _strEmplacementMap, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, int _iNombreÉquipe, int _iNombreJoueur, CVent* _pVent, bool _VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_boDebutPartie = true;

		m_pVerifierCollisionJoueurMap = _VerifierCollisionJoueurMap;

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, _pGestionnaireSurface, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction, _CollisionObjetMap);

		m_pTimerPhysique = new CTimer(20);

		for (int i = _iNombreÉquipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_pGestionnaireSurface, _pGestionnaireTexture, i, _iNombreJoueur, _MapDestruction, _CollisionObjetMap, _Physique));
		}
		m_pTeamList->AllerDebut();

		m_pToolBar = new CToolBar({ 0, 600, 1366, 168 }, { 0, 0, 0 }, _pRenderer, 265, 120, 60, 4, _pGestionnaireTexture->ObtenirDonnee("BazookaTool"), _pGestionnaireTexture->ObtenirDonnee("GrenadaTool"), _pGestionnaireTexture->ObtenirDonnee("SwordTool"), _pGestionnaireTexture->ObtenirDonnee("JetPackTool"));
		m_pTimerPhysique->Start();
	}

	void ChangerTour(TTF_Font* _pFont, SDL_Renderer* _pRenderer) {
		m_pTeamList->ObtenirElementCurseur()->ChangerPlayerActif();
		m_pTeamList->AllerSuivantCurseur();
		m_pGameMap->NouveauVent(_pFont, _pRenderer);
	}

	void ChangerTeamActive() {
		m_pTeamList->AllerSuivantCurseur();
	}

	CTeam* ObtenirTeamActive() {
		return m_pTeamList->ObtenirElementCurseur();
	}

	void AfficherGame(SDL_Renderer* _pRenderer) {

		if (m_boDebutPartie)
			m_boDebutPartie = !IsAllTeamStable();

		m_pGameMap->ShowMap(_pRenderer);

		PhysiquePlayer();

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

	void PhysiquePlayer(void) {

		if (!m_boDebutPartie) {

			if (m_pTimerPhysique->IsDone()) {

				CPlayer* pPlayerActif = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif();
				SDL_Rect Recttmp = pPlayerActif->ObtenirRectDestination();
				bool boCorps;
				bool boPied;
				unsigned int _uiX;
				unsigned int _uiY;
				
				if (!pPlayerActif->IsStable()) 
					*pPlayerActif->ObtenirVecteurVitesse() += *m_pGameMap->ObtenirGravite();
				Recttmp.x += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
				Recttmp.y += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;
				pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(RegressionLineaire(pPlayerActif->ObtenirHitboxPieds()));
					if (!m_pVerifierCollisionJoueurMap(pPlayerActif, Recttmp, &boCorps, &boPied, &_uiX, &_uiY)) {
						pPlayerActif->ModifierRectDestination(Recttmp);
					}
					else { 

						Recttmp.y -= (Recttmp.h - _uiY);
						pPlayerActif->ModifierRectDestination(Recttmp);
						pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);
					}
				

				m_pTimerPhysique->Start();
			}
		}

		else {

			bool _boCorps;
			bool _boPieds;

			unsigned int _uiXMap;
			unsigned int _uiYMap;

			CListeDC<CPlayer*>* pPlayerListTmp;
			CPlayer* pPlayer;
			SDL_Rect RectPlayer;

			if (m_pTimerPhysique->IsDone()) {

				for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

					pPlayerListTmp = m_pTeamList->ObtenirElementCurseur()->obtenirListeTeam();

					for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

						pPlayer = pPlayerListTmp->ObtenirElementCurseur();

						if (pPlayer->ObtenirSpriteRepos()->IsActif() && !pPlayer->IsStable()) {
							RectPlayer = pPlayer->ObtenirRectDestination();
							RectPlayer.y += 9.8;
							if (!m_pVerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXMap, &_uiYMap))
								pPlayer->ModifierRectDestination(RectPlayer);

							else {

								RectPlayer.y -= (RectPlayer.h - _uiYMap);
								pPlayer->ModifierRectDestination(RectPlayer);
								pPlayer->ModifierStabiliteJoueur(true);

							}
						}


						if (pPlayer->ObtenirSpriteParachute()->IsActif()) {
							RectPlayer = pPlayer->ObtenirRectDestinationParachute();
							RectPlayer.y += 11;
							if (!m_pVerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXMap, &_uiYMap))
								pPlayer->ModifierRectDestinationParachute(RectPlayer);

							else {


								pPlayer->ObtenirSpriteParachute()->DefinirActif(false);
								pPlayer->ObtenirSpriteRepos()->DefinirActif(true);
								RectPlayer.w = pPlayer->ObtenirRectDestination().w;
								RectPlayer.h = pPlayer->ObtenirRectDestination().h;
								RectPlayer.y += (pPlayer->ObtenirRectDestinationParachute().h - pPlayer->ObtenirRectDestination().h);
								pPlayer->ModifierRectDestination(RectPlayer);

							}


						}
						pPlayerListTmp->AllerSuivantCurseur();
					}
					m_pTeamList->AllerSuivantCurseur();
				}

				m_pTimerPhysique->Start();
			}
		}
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

	bool IsAllTeamStable(void) {

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			if (!m_pTeamList->ObtenirElementCurseur()->IsAllPlayerStable())
				return false;

			m_pTeamList->AllerSuivantCurseur();
		}

		return true;
	}
	// Procédure qui retourne la pente 
	// Paramètre : _RectPiedJoueur : Le rect pied du joueur acitf.
	// Retour : integer double qui représente l'angle de la pente.
	 double RegressionLineaire(SDL_Rect _RectPiedJoueur) {
		 int iAngle;
		 float iCov = 0; // Variable en y moyenne.
		 float iVar = 0; // Variable en x moyen.
		 float fX = 0; // Valeur en x pour la régression.
		 float fY = 0; // Valeur en y pour la régression.
		 int iN = 0; // Le nombre de fois qu'il y a des "différent de transparent" Sert a savoir le milieu de la régressuion
		 int* iTableau = new int[_RectPiedJoueur.w, _RectPiedJoueur.h]; // Tableau.
		for (int j = 0; j <  _RectPiedJoueur.w; j++) { // Boucler sur toute le rect du pied dans la position de la map.
			for (int i = 0; i < _RectPiedJoueur.h; i++) {
				if (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(i + _RectPiedJoueur.x) + ((j + _RectPiedJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] != 0) { // Si le pixel est différent de transparent.
					iTableau[i, j] = 1; // Mettre 1 dans mon tableau.
					fX += i; // fX va servir a faire la moyenne des X.
					fY += j; // fX va servir a faire la moyenne des Y.
					iN += 1; // Pour diviser le nombre d'éléments.
				}
			}
		}
		if (fX != 0 && fY != 0) {
			fX = fX / iN; // moyenne
			fY = fY / iN; // moyenne
		}
		for (int j = 0; j < _RectPiedJoueur.w; j++) {
			for (int i = 0; i < _RectPiedJoueur.h; i++) {
				if (iTableau[i, j] == 1) {
					iCov += ((i - fX) * (j - fY)); // Calcul pour Y moyens avec le Y moyens.
					iVar += pow ((i - fX), 2);	   // Calcul pour X moyens avec le X moyens.
				}
			}
		}
		
		if (iCov != 0 && iVar != 0) {
			iCov = (iCov / iN); //moyenne
			iVar = (iVar / iN); //moyenne
		}
		if (iCov != 0 && iVar != 0) {
			if (iCov < 0 && iVar >= 0)
				return (180 / M_PI) * atanf(((-(float)iCov) / ((float)iVar)));

			if (iCov >= 0 && iVar < 0)
				return 180 + (180 / M_PI) * atanf((((float)iCov) / (-(float)iVar)));

			if (iCov < 0 && iVar < 0)
				return 180 - (180 / M_PI) * atanf(((-(float)iCov) / (-(float)iVar)));

			if (iCov >= 0 && iVar >= 0)
				return 360 - (180 / M_PI) * atanf((((float)iCov) / ((float)iVar)));
		}
		
	}

};