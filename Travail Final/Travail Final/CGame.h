// 29 novembre 2014 par Gabriel Beaudry(gabriel.bdry@gmail.com)
// Ajout de la procÈdure changertour().

class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'Èquipe.
	CListeDC<CTemporaryLabel*>* m_pListeDomage; // Listes des domages.
	CToolBar* m_pToolBar; // La toolbar o˘ le joueur choisi son outils pour son tour.
	CTimer* m_pTimerPhysique; // Le timer pour la physique.

	bool m_boDebutPartie; // Si le jeu est en dÈbut de partie.
	bool m_boFinTour; // Si le jeu est en fin de tour.

public:

	CGame(string _strEmplacementMap, int _iNombre…quipe, int _iNombreJoueur, CVent* _pVent, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle), SDL_Renderer* _pRenderer) {


		m_boDebutPartie = true;
		m_boFinTour = false;

		m_pTeamList = new CListeDC<CTeam*>();
		m_pListeDomage = new CListeDC<CTemporaryLabel*>();

		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction);

		m_pTimerPhysique = new CTimer(20);

		for (int i = _iNombre…quipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementMap, _pRenderer, i, _iNombreJoueur, _MapDestruction, _Rotation));
		}
		m_pTeamList->AllerDebut();

		m_pToolBar = new CToolBar({ 0, 600, 1366, 168 }, { 0, 0, 0 }, _pRenderer, 265, 120, 60, 4, pGestionnaireTexture->ObtenirDonnee("BazookaTool"), pGestionnaireTexture->ObtenirDonnee("GrenadaTool"), pGestionnaireTexture->ObtenirDonnee("SwordTool"), pGestionnaireTexture->ObtenirDonnee("JetPackTool"));
		m_pTimerPhysique->Start();
	}

	void ChangerTour(SDL_Renderer* _pRenderer) {
		m_pTeamList->AllerSuivantCurseur();
		m_pTeamList->ObtenirElementCurseur()->ChangerPlayerActif();
		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReinitialisationTool();
		m_pGameMap->NouveauVent(pGestionnaireFont->ObtenirDonnee("pFontBouton"), _pRenderer);
		m_pToolBar->NouveauTour();
	}

	void ChangerTeamActive() {
		m_pTeamList->AllerSuivantCurseur();
	}

	CTeam* ObtenirTeamActive() {
		return m_pTeamList->ObtenirElementCurseur();
	}

	void AfficherGame(SDL_Renderer* _pRenderer, bool _boPause) {

		if (m_boDebutPartie && m_boFinTour) {
			m_boDebutPartie = !IsAllTeamStable();
			if (!m_boDebutPartie) {
				ChangerTour(_pRenderer);
				m_boFinTour = false;
			}
		}

		else if (m_boDebutPartie)
			m_boDebutPartie = !IsAllTeamStable();

		m_pGameMap->ShowMap(_pRenderer);

		if (!_boPause) {
			PhysiquePlayer(_pRenderer);
			PhysiquePack(_pRenderer);
		}

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {
			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

		if (!_boPause) {
			m_pToolBar->ShowToolBar(_pRenderer);
			m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ShowDescription(_pRenderer);
		}

		m_pListeDomage->AllerDebut();
		for (int i = 0; i < m_pListeDomage->ObtenirCompte(); i++) {

			m_pListeDomage->ObtenirElementCurseur()->ShowControl(_pRenderer);
			if (m_pListeDomage->ObtenirElementCurseur()->IsDead())
				m_pListeDomage->Retirer(true);
			else
				m_pListeDomage->AllerSuivantCurseur();

		}
	}

	void ReactToEvent(SDL_Event* _pEvent) {
		
		switch (_pEvent->type) {

		case SDL_KEYDOWN:

			if (_pEvent->key.keysym.scancode == SDL_SCANCODE_T && !m_boDebutPartie && m_pToolBar->ObtenirPositionObjetDoubleClick() > 3)
				m_pToolBar->ReverseboShow();

			break;
		}

		m_pToolBar->ReactToEvent(_pEvent, m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif());

		unsigned int uiMunition = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirMunition(m_pToolBar->ObtenirPositionObjetDoubleClick());
		switch (m_pToolBar->ObtenirPositionObjetDoubleClick()) {
		case 0:

			if (uiMunition <= 0) {
				m_pToolBar->ReverseboShow();
			}
			break;
		case 1:

			if (uiMunition <= 0) {
				m_pToolBar->ReverseboShow();
			}
			break;
		case 3:

			if (uiMunition <= 0) {
				m_pToolBar->ReverseboShow();
			}
			break;
		}

		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReactToEvent(_pEvent, m_pToolBar->ObtenirPositionObjetDoubleClick());

		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->UpdateDescription(m_pToolBar->ObtenirPositionObjetSuvol(), m_pToolBar->ObtenirRectPositionSouris());
	}

	void PhysiquePlayer(SDL_Renderer* _pRenderer) {

		if (!m_boDebutPartie) {

			if (m_pTimerPhysique->IsDone()) {
				CPlayer* pPlayerActif = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif();
				SDL_Rect RectTmp = pPlayerActif->ObtenirRectDestination();
				double dComposanteX = pPlayerActif->ObtenirPositionX();
				double dComposanteY = pPlayerActif->ObtenirPositionY();
				double dAngle;
				bool boCorps;
				bool boPied;
				bool boExplosion;
				unsigned int _uiXPieds;
				unsigned int _uiYPieds;
				unsigned int _uiXCorps;
				unsigned int _uiYCorps;
				SDL_Point PointExplosion;

				if (pPlayerActif->IsUsingTool())
					PhysiqueTool(_pRenderer);

				else if (!pPlayerActif->IsStable()) {


					DetectionCollisionPack(pPlayerActif, &boExplosion, &PointExplosion, _pRenderer);

					if (boExplosion)
						DommageExplosion(PointExplosion, 45, _pRenderer);


					else if (pPlayerActif->ObtenirSpriteCourse()->IsActif()) {

						dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35.0;

						if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {

							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

							dComposanteY += pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() / 35;

							pPlayerActif->DefinirPositionX(dComposanteX);
							pPlayerActif->DefinirPositionY(dComposanteY);

						}

						else {

							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(0);

							if (boPied && !boCorps) {


								if (boCorps) {

									if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0)
										dComposanteX -= (RectTmp.w - _uiXCorps);

									if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1)
										dComposanteX += _uiXCorps;

									pPlayerActif->DefinirPositionX(dComposanteX);
									pPlayerActif->DefinirPositionY(dComposanteY);

								}

								unsigned int uiH = 0;

								while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + 1 + _uiXPieds + (RectTmp.y + _uiYPieds - uiH) *m_pGameMap->ObtenirSurfaceMap()->w] != 0) && (uiH <= 2))
									uiH++;

								if (uiH <= 2) {

									dComposanteY -= uiH;


									pPlayerActif->DefinirPositionX(dComposanteX);
									pPlayerActif->DefinirPositionY(dComposanteY);

								}

								else {

									if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0 && (_uiXPieds > RectTmp.w / 2))
										dComposanteX -= (RectTmp.w - _uiXPieds);

									if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1 && (_uiXPieds < RectTmp.w / 2))
										dComposanteX += _uiXPieds;

									pPlayerActif->DefinirPositionX(dComposanteX);
									pPlayerActif->DefinirPositionY(dComposanteY);

								}

							}

						}

						if (abs(RegressionTest({ RectTmp.x + RectTmp.w / 2, RectTmp.y }, RectTmp.w / 2, 1)) >= 60) {
							//if ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(RectTmp.x + RectTmp.w) + ((RectTmp.y + RectTmp.h) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0) || (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(RectTmp.x) + ((RectTmp.y + RectTmp.h)* m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1)) {
								pPlayerActif->ModifierGlissadeJoueur(true);
								pPlayerActif->ObtenirSpriteCourse()->DefinirActif(false);
								pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								pPlayerActif->DefinirToolActif(false);
							//}

						}

					}


					else if (pPlayerActif->ObtenirSpriteSaut()->IsActif()) {



					}


					else if (pPlayerActif->IsSliding()) {
						CVecteur2D* VecteurFrottement = new CVecteur2D(0.0f, 0.0f);
						double doAngle = RegressionTest({ RectTmp.x + RectTmp.w / 2, RectTmp.y }, RectTmp.w / 2, 1); // Variable qui sert a calcul juste 1 fois l'angle.
						VecteurFrottement->ModifierVecteur(1, doAngle + 180);

						if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1) { // Si le joueur se dÈplace vers la droite.

							if (doAngle > 0) { // Si le joueur se dÈplace vers la gauche et que la pente est vers le haut ‡ gauche.

								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								//pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle + 180);
								pPlayerActif->ObtenirVecteurPoids()->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								//VecteurFrottement->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *VecteurFrottement;
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() + (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35));
								pPlayerActif->DefinirPositionX(pPlayerActif->ObtenirPositionX() + (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35));

							}

							if (doAngle < 0) { // Si le joueur se dÈplace vers la le bas a gauche.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								//pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), doAngle);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								//VecteurFrottement->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *VecteurFrottement;
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35));
								pPlayerActif->DefinirPositionX(pPlayerActif->ObtenirPositionX() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35));
							}





						}
						else { // Si le joueur va vers la droite

							if (doAngle > 0) { // Si le joueur se dÈplace vers la droite et que la pente est vers le bas ‡ droite.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle + 180);
								pPlayerActif->ObtenirVecteurPoids()->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(doAngle);
								*pPlayerActif->ObtenirVecteurVitesse() += *VecteurFrottement;
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35));
								pPlayerActif->DefinirPositionX(pPlayerActif->ObtenirPositionX() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35));
							}

							if (doAngle < 0) { // Si la pente est en haut a droite.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *VecteurFrottement;
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35));
								pPlayerActif->DefinirPositionX(pPlayerActif->ObtenirPositionX() - (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35));
							}
						}

						if (pPlayerActif->ObtenirVecteurVitesse() == 0)  { // Si la glissade est fini.
							pPlayerActif->ModifierGlissadeJoueur(false);
							pPlayerActif->ModifierStabiliteJoueur(true);
						}
						/*if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {
							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

							dComposanteY += pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() / 35;

							pPlayerActif->DefinirPositionX(dComposanteX);
							pPlayerActif->DefinirPositionY(dComposanteY);
						}*/
					}




					else if (pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->IsActif()) {

						if (pPlayerActif->ObtenirJetPack()->IsActive()) {
							if (pPlayerActif->ObtenirJetPack()->ObtenirVecteur()->ObtenirComposanteX() == 0)
								pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantX(0);
							*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirJetPack()->ObtenirVecteur();
							*pPlayerActif->ObtenirVecteurVitesse() += *m_pGameMap->ObtenirGravite();
							dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
							dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

							if (!VerifierCollisionJoueurMap(pPlayerActif, { dComposanteX, dComposanteY }, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYPieds)) {
								pPlayerActif->DefinirPositionX(dComposanteX);
								pPlayerActif->DefinirPositionY(dComposanteY);
							}

							else {

								pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->DefinirActif(false);
								pPlayerActif->ObtenirSpriteRepos()->DefinirEtage(pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->ObtenirEtage());
								pPlayerActif->ModifierTypeMouvement(2);
								pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								pPlayerActif->ModifierStabiliteJoueur(false);
								m_boFinTour = true;
								m_boDebutPartie = true;
								m_pToolBar->NouveauTour();
							}
						}
					}

				}

				m_pTimerPhysique->Start();

			}

		}

		else {

			bool _boCorps;
			bool _boPieds;

			unsigned int _uiXPieds;
			unsigned int _uiYPieds;
			unsigned int _uiXCorps;
			unsigned int _uiYCorps;

			double dPositionX;
			double dPositionY;

			CListeDC<CPlayer*>* pPlayerListTmp;
			CPlayer* pPlayer;
			SDL_Rect RectPlayer;

			if (m_pTimerPhysique->IsDone()) {

				m_pTeamList->AllerATrieur(0);
				for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

					pPlayerListTmp = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();

					pPlayerListTmp->AllerATrieur(0);
					for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

						pPlayer = pPlayerListTmp->ObtenirElementTrieur(); // Quand je dÈbogais, il y a eu une violation d'accËs l'objet au curseur Ètait ‡ ??????? (deleted)
						dPositionX = pPlayer->ObtenirPositionX();
						dPositionY = pPlayer->ObtenirPositionY();

						if (pPlayer->ObtenirSpriteRepos()->IsActif() && !pPlayer->IsStable()) {
							RectPlayer = pPlayer->ObtenirRectDestination();
							*pPlayer->ObtenirVecteurVitesse() += *m_pGameMap->ObtenirGravite();

							dPositionX += pPlayer->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
							dPositionY += pPlayer->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

							if (RectPlayer.y >= 1366) {
								pPlayerListTmp->RetirerTrieur(true);
								m_pGameMap->CreateHealthPack();
							}

							else if (!VerifierCollisionJoueurMap(pPlayer, { dPositionX, dPositionY }, &_boCorps, &_boPieds, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps))

								pPlayer->DefinirPositionY(dPositionY);

							else {

								bool boExplosion;
								SDL_Point Pointexplosion;

								dPositionY -= (RectPlayer.h - _uiYPieds);
								pPlayer->DefinirPositionY(dPositionY);
								pPlayer->ModifierStabiliteJoueur(true);

								DetectionCollisionPack(pPlayer, &boExplosion, &Pointexplosion, _pRenderer);

								if (boExplosion)
									DommageExplosion(Pointexplosion, 45, _pRenderer);



								else if (DommageChuteLibre(pPlayer, _pRenderer))
									pPlayerListTmp->RetirerTrieur(true);

							}
						}

						else if (pPlayer->ObtenirSpriteParachute()->IsActif()) {
							RectPlayer = pPlayer->ObtenirRectDestinationParachute();
							RectPlayer.y += 1;

							if (RectPlayer.y >= 1366) {
								pPlayerListTmp->RetirerTrieur(true);
								m_pGameMap->CreateHealthPack();
							}

							else if (!VerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {

								pPlayer->ModifierRectDestinationParachute(RectPlayer);

							}

							else {


								pPlayer->ObtenirSpriteParachute()->DefinirActif(false);
								pPlayer->ObtenirSpriteRepos()->DefinirActif(true);
								RectPlayer.w = pPlayer->ObtenirRectDestination().w;
								RectPlayer.h = pPlayer->ObtenirRectDestination().h;
								RectPlayer.y += (pPlayer->ObtenirRectDestinationParachute().h - pPlayer->ObtenirRectDestination().h);
								pPlayer->DefinirPositionX(RectPlayer.x);
								pPlayer->DefinirPositionY(RectPlayer.y);
								if (RectPlayer.x >= (1366 / 2))
									pPlayer->ObtenirSpriteRepos()->DefinirEtage(1);

							}


						}
						pPlayerListTmp->AllerSuivantTrieur();
					}
					m_pTeamList->AllerSuivantTrieur();
				}

				m_pTimerPhysique->Start();
			}
		}
	}
	

	void PhysiquePack(SDL_Renderer* _pRenderer) {

		CListeDC<CPack*>* pPackListTmp = m_pGameMap->ObtenirPackList();
		SDL_Rect RectTmp;
		int iX, iY;
		

		for (int i = 0; i < pPackListTmp->ObtenirCompte(); i++) {

			if (!pPackListTmp->ObtenirElementCurseur()->IsStable()) {

				RectTmp = pPackListTmp->ObtenirElementCurseur()->GetRectDestination();

				if (CollisionObjetMapChuteLibre(pPackListTmp->ObtenirElementCurseur()->GetSurface(), RectTmp, &iX, &iY)) {

					RectTmp.y = iY;
					double dAngle = RegressionTest({ iX, RectTmp.y }, RectTmp.w / 2, 1);
					double d = abs(dAngle);
					if (abs(dAngle) <= 3) {
						int i = 0;
						double dAngle = RegressionTest({ iX, RectTmp.y }, RectTmp.w / 2, 1);
					}
					if (abs(dAngle) >= 70) {
						// GLISSADEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE ‡ faire..........
						/*CVecteur2D* VecteurVitessePack = new CVecteur2D(0.0f,0.0f);
						CVecteur2D* VecteurFrottement = new CVecteur2D(0.0f,0.0f);*/
						if (dAngle < 0) {
							/*VecteurFrottement->ModifierVecteur(1, dAngle + 180);
							VecteurVitessePack->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), dAngle);
							pPackListTmp->ObtenirElementCurseur()->ModifierStabilePack(false);
							*VecteurVitessePack += *VecteurFrottement;

							RectTmp.x -= VecteurVitessePack->ObtenirComposanteX();
							RectTmp.y -= VecteurVitessePack->ObtenirComposanteY();*/
							RectTmp.x -= 50;
						}
						else
						{
							RectTmp.x += 50;
						}
						
					}
					else
					{
						pPackListTmp->ObtenirElementCurseur()->ModifierAnlge(dAngle);
						pPackListTmp->ObtenirElementCurseur()->ModifierStabilePack(true);
					}
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
				}
				else {
					RectTmp.y += m_pGameMap->ObtenirGravite()->ObtenirComposanteY();
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
				}
			}

			pPackListTmp->AllerSuivantCurseur();
		}
	}

	// ProcÈdure appliquant la physique sur les projectiles...
	void PhysiqueTool(SDL_Renderer* _pRenderer) {

		// 0:Missile 1:Grenade
		if (m_pToolBar->ObtenirPositionObjetDoubleClick() <= 1) {

			// Pointeur sur le projectile...
			CProjectile* pProjectileTmp = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirProjectile(m_pToolBar->ObtenirPositionObjetDoubleClick());

			// Initialisation de la position et du vecteur pour ne pas aller les chercher ‡ rÈpÈtition...
			SDL_Rect* RectTmp = pProjectileTmp->ObtenirRectDestination();
			CVecteur2D* pVecteurVitesse = pProjectileTmp->ObtenirVecteurVitesse();
			int iX, iY; // Position d'une collision.
			
			if (pProjectileTmp->ObtenirSprite("") != nullptr) {

				if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
					m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
					pProjectileTmp->DefinirExplosion(false);
					m_boDebutPartie = true;
					m_boFinTour = true;
					m_pToolBar->NouveauTour();
				}
			}

			// Si le sprite d'explosion est terminÈ et qu'une explosion Ètait en cours...
			if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
				m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
				pProjectileTmp->DefinirExplosion(false);
				m_boDebutPartie = true;
				m_boFinTour = true;

			}

			if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 0 && pProjectileTmp->EstLancer())  {

				if (CollisionMissile(pProjectileTmp->ObtenirSurface(), *pProjectileTmp->ObtenirRectDestination(), &iX, &iY, _pRenderer)) {

					pProjectileTmp->ReactionExplosion(pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY);
					DommageExplosion({ pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY}, 50, _pRenderer);
				}

				else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768) {
					pProjectileTmp->ReactionExplosion(-100, -100);
				}

				RectTmp->x += pVecteurVitesse->ObtenirComposanteX() / 35;
				RectTmp->y += pVecteurVitesse->ObtenirComposanteY() / 35;

				*pVecteurVitesse += *m_pGameMap->ObtenirGravite();
				*pVecteurVitesse += *m_pGameMap->ObtenirVecteurVent();

				if (pVecteurVitesse->ObtenirComposanteY() < 0 && pVecteurVitesse->ObtenirComposanteX() >= 0)
					pProjectileTmp->DefinirAngle((180 / M_PI) * atanf(((-(float)pVecteurVitesse->ObtenirComposanteY()) / ((float)pVecteurVitesse->ObtenirComposanteX()))));

				if (pVecteurVitesse->ObtenirComposanteY() >= 0 && pVecteurVitesse->ObtenirComposanteX() < 0)
					pProjectileTmp->DefinirAngle(180 + (180 / M_PI) * atanf((((float)pVecteurVitesse->ObtenirComposanteY()) / (-(float)pVecteurVitesse->ObtenirComposanteX()))));

				if (pVecteurVitesse->ObtenirComposanteY() < 0 && pVecteurVitesse->ObtenirComposanteX() < 0)
					pProjectileTmp->DefinirAngle(180 - (180 / M_PI) * atanf(((-(float)pVecteurVitesse->ObtenirComposanteY()) / (-(float)pVecteurVitesse->ObtenirComposanteX()))));

				if (pVecteurVitesse->ObtenirComposanteY() >= 0 && pVecteurVitesse->ObtenirComposanteX() >= 0)
					pProjectileTmp->DefinirAngle(360 - (180 / M_PI) * atanf((((float)pVecteurVitesse->ObtenirComposanteY()) / ((float)pVecteurVitesse->ObtenirComposanteX()))));

			}
			// Si une grenade est lancÈe
			else if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 1 && pProjectileTmp->EstLancer()) {

				if (pProjectileTmp->ObtenirSprite("") != nullptr) {

					if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
						m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
						pProjectileTmp->DefinirExplosion(false);
						m_boDebutPartie = true;
						m_boFinTour = true;
					}
				}

				bool boSensRotation = false;
				RectTmp = pProjectileTmp->ObtenirRectDestination();

				// AutoDesctruction de la grenade...
				if (pProjectileTmp->ReactionExplosion(0, 0)) {
					RectTmp = pProjectileTmp->ObtenirRectDestination();
					DommageExplosion({ RectTmp->x + pProjectileTmp->ObtenirRayonExplosion(), RectTmp->y + pProjectileTmp->ObtenirRayonExplosion() }, pProjectileTmp->ObtenirRayonExplosion(), _pRenderer);

				}
				else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768) {
					pProjectileTmp->ReinitialisationProjectile();
				}
				else
				{
					pVecteurVitesse = pProjectileTmp->ObtenirVecteurVitesse();
					int x = RectTmp->x;
					int y = RectTmp->y;
					RectTmp->x += pVecteurVitesse->ObtenirComposanteX() / 35;
					RectTmp->y += pVecteurVitesse->ObtenirComposanteY() / 35;

					if (CollisionGrenadeJoueur(pProjectileTmp->ObtenirSurface(), *pProjectileTmp->ObtenirRectDestination(), &iX, &iY)) {

						pVecteurVitesse->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirComposanteY(),pVecteurVitesse->ObtenirOrientation() + 180);
					}
					
					if (CollisionObjetMap(pProjectileTmp->ObtenirSurface(), *RectTmp, nullptr, nullptr)) {
						RectTmp->x = x;
						RectTmp->y = y;
						int iW;
						RepositionnementGrenadeMap(pProjectileTmp->ObtenirSurface(), pVecteurVitesse, RectTmp, &boSensRotation, &iW, nullptr);
						float angle = RegressionTest({ RectTmp->x + iW, RectTmp->y }, RectTmp->w / 2, 1);
						//float angle = (float)RegressionLineaire(*pProjectileTmp->ObtenirRectDestination(), *pProjectileTmp->ObtenirRectDestination(), true);
						if (pVecteurVitesse->ObtenirNorme() >= 35) {
							CVecteur2D VecteurNormal = CVecteur2D(1, angle + 90);
							double dScalaire = 2 * pVecteurVitesse->Scalaire(VecteurNormal.ObtenirComposanteX(), VecteurNormal.ObtenirComposanteY());
							*pVecteurVitesse -= VecteurNormal * dScalaire;
							*pVecteurVitesse = *pVecteurVitesse * 0.35;
						}
						else
						{
							pVecteurVitesse->ModifierVecteur(35, angle);
							pProjectileTmp->DefinirRotation(0);
						}
						if (boSensRotation)
							pProjectileTmp->DefinirRotation(pVecteurVitesse->ObtenirNorme());
						else
							pProjectileTmp->DefinirRotation(-pVecteurVitesse->ObtenirNorme());
					}
					
					*pVecteurVitesse += *m_pGameMap->ObtenirGravite();
				}
			}
		}
	}

	CMap* ObtenirMap(void) {

		return m_pGameMap;

	}

	CListeDC<CTeam*>* ObtenirListePlayer(void) {

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

	// RegressionTest bas
	double RegressionTest(SDL_Point _StartPoint, unsigned int _uiLimit, double _dTolerance) {
		double dTolerance = _dTolerance;
		double dDiminutionTolerance = 0.0000000000001;
		unsigned short int usiGauche = 1;
		unsigned short int usiDroit = 1;
		bool boPremiere = true;
		bool boDeuxieme = true;
		bool boPremierePenteDiffZero = false;
		double dPente = 0;
		double dNouvellePente = 0;
		SDL_Point PointGauche;
		SDL_Point PointDroit;
		SDL_Surface* pSurfaceMap = m_pGameMap->ObtenirSurfaceMap();

		// Tant que la diffÈrence entre la nouvelle pente et l'ancienne <= tol et que j'e n'ai pas atteint la limite...
		while ((abs(dPente - dNouvellePente) <= dTolerance / (dDiminutionTolerance) || !boPremierePenteDiffZero) && usiGauche <= _uiLimit) {
			
			// Pas la premiËre fois...
			if (!boPremiere) {
				dPente = (dPente + dNouvellePente) / 2;
			}
			// Initialisation des points
			PointGauche = { _StartPoint.x - usiGauche, _StartPoint.y };
			PointDroit = { _StartPoint.x + usiDroit, _StartPoint.y };

			// Descente pointgauche...
			while (((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == TRANSPARENCE32BIT) {
				PointGauche.y++;
			}

			// Descente pointdroit...
			while (((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == TRANSPARENCE32BIT) {
				PointDroit.y++;
			}
			// Calcul de la nouvelle pente
			dNouvellePente = ((double)PointDroit.y - (double)PointGauche.y) / ((double)PointDroit.x - (double)PointGauche.x);
			if (dNouvellePente != 0)
				boPremierePenteDiffZero = true;
			usiGauche++;
			usiDroit++;
			if (boPremierePenteDiffZero)
				dDiminutionTolerance++;
			// Si c'est la premiËre fois
			if (boPremiere && boPremierePenteDiffZero) {
				dPente = dNouvellePente;
				boPremiere = false;
			}
		}

		return (180 / M_PI) * atanf(dPente);
	}

	// ProcÈdure qui retourne la pente 
	// ParamËtre : _RectPiedJoueur : Le rect pied du joueur acitf. Si c'est des mines ou des objets le mettre ‡ 0.
	//			 : _RectJoueur -> L'emplacement de l'objet ou le joueur dans la maop.
	//			 : _boObjet -> Si c'est un objet ou non.
	// Retour : integer double qui reprÈsente l'angle de la pente.
	double RegressionLineaire(SDL_Rect _RectPiedJoueur, SDL_Rect _RectJoueur, bool _boObjet) {
		float fPente = 0;
		float iCov = 0; // Variable en y moyenne.
		float iVar = 0; // Variable en x moyen.
		float fX = 0; // Valeur en x pour la rÈgression.
		float fY = 0; // Valeur en y pour la rÈgression.
		int iN = 0; // Le nombre de fois qu'il y a des "diffÈrent de transparent" Sert a savoir le milieu de la rÈgressuion.
		SDL_Rect _RectRegression;
		//if (!_boObjet) {
			/*_RectRegression.x = _RectPiedJoueur.x + (_RectJoueur.w / 2); // Le rect commence au milieu du joueur.
			_RectRegression.y = _RectPiedJoueur.y + _RectPiedJoueur.h;
			_RectRegression.w = 15; // Largeur du Rect.
			int y = 0; // Utiliser pour ma boucle au lieu d'utiliser mon rect pour vÈrifier.
			if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1) {
				_RectRegression.x = _RectRegression.x - (_RectRegression.w / 2);
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.x + _RectJoueur.x) + ((y + _RectRegression.y + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0) {
					y++;
					_RectRegression.h = y;

				}
				y = 0; // Au cas qu'il est entrer dans la premiere boucle.
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.x + _RectJoueur.x) + ((y + _RectRegression.y + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] != 0) {
					y--;
					_RectRegression.h = abs(y);

				}
			}

			else if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0) {
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.w + _RectRegression.x + _RectJoueur.x) + ((y + _RectRegression.y + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0) {
					y++;
					_RectRegression.h = y;

				}
				y = 0; // Au cas qu'il est entrer dans la premiere boucle.
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.w + _RectRegression.x + _RectJoueur.x) + ((y + _RectRegression.y + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] != 0) {
					y--;
					_RectRegression.h = abs(y);

				}
			}*/
		//}

		

			_RectRegression = _RectPiedJoueur;
			
			int iMaxPente = 20;
			bool boEssai = false; // Ca verifie si y'a du vide a gauche. comme ca ne pas le faire a droite.
			int j = 0;
			float y1 = 0;
			float y2 = 0;
			float x1 = _RectRegression.x;
			float x2 = _RectRegression.x + _RectRegression.w;
			if (!_boObjet)
				iMaxPente = 75;
			while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.x + _RectJoueur.x) + ((j + _RectJoueur.h + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && j < iMaxPente) { // Coter gauche du rect.
				j++;
				y1 = j;
				boEssai = true;
			}
			if (j == iMaxPente) {
				j = 0;
				y1 = 0;
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(10 + _RectJoueur.h + _RectJoueur.x) + ((j + _RectRegression.h + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && j < iMaxPente - 15) { // Si le coter gauche na pas de trouver de boute de pente, je le pars a la moitier.
					j++;
					y1 = j;
				}
			}
			if (j == iMaxPente - 15) // Si y'a toujours rien trouver, ca veut dire que c'est a la verticale ou presque.
				return 0;
			j = 0;
			if (!boEssai) {
				while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(_RectRegression.w + _RectRegression.x + _RectJoueur.x) + ((j + _RectJoueur.h + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && j < iMaxPente) { // Coter droite du rect
					j++;
					y2 = j;
				}

				if (j == iMaxPente) { // Si y'a rien trouver le faire partir a la moitier.
					j = 0;
					y2 = 0;
					while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(-5 + _RectRegression.x + _RectJoueur.x) + ((j + _RectJoueur.h + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] == 0 && j < iMaxPente - 15) {
						j++;
						y2 = j;
					}
				}
				if (j == iMaxPente - 15) // Si y'a rien trouver encore ca veut dire que c'est ‡ la verticale.
					return 0;
			}

			if (!_boObjet && y1 < y2) // Si a droite c'est plus gros ca veut dire que la pente est positive. donc mettre le y1 ‡ 0.
				y1 = 0;

			fPente = ((y2 - y1) / (x2 - x1));

			return (180 / M_PI) * atanf((fPente));


		




		
		/*
		int iTableau[10000];
		for (int j = 0; j < _RectRegression.h; j++) { // Boucler sur toute le rect du pied dans la position de la map.
			for (int i = 0; i < _RectRegression.w; i++) {
				if (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(i + _RectRegression.x + _RectJoueur.x) + ((j + _RectRegression.y + _RectJoueur.y) * (m_pGameMap->ObtenirSurfaceMap()->w))] != 0)  { // Si le pixel est transparent.
					iTableau[i + j * _RectRegression.w] = 1; // Mettre 1 dans mon tableau.
					fX += i; // fX va servir a faire la moyenne des X.
					fY += j; // fY va servir a faire la moyenne des Y.
					iN += 1; // Pour diviser le nombre d'ÈlÈments.
				}
				else
					iTableau[i + j * _RectRegression.w] = 0;
			}
		}
		if (fX != 0 && fY != 0) {
			fX = fX / iN; // moyenne
			fY = fY / iN; // moyenne
		}

		for (int j = 0; j < _RectRegression.h; j++) {
			for (int i = 0; i < _RectRegression.w; i++) {
				if (iTableau[i + j * _RectRegression.w] == 1) {
					iCov += ((j - fY) * (i - fX)); // Calcul pour Y moyens avec le Y moyens.
					iVar += pow((i - fX), 2);    // Calcul pour X moyens avec le X moyens.
				}
			}
		}

		if (iCov != 0 && iVar != 0) {
			iCov = (iCov / iN); //moyenne
			iVar = (iVar / iN); //moyenne
			fPente = iCov / iVar; // Donne la pente. iCov = y , iVar = x.
		}




		if (!_boObjet) {

			return (180 / M_PI) * atanf(fPente);
			/*if (iCov != 0 && iVar != 0) {

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente > 0) { // Le joueur se dÈplace vers la droite et la pente est nÈgative.
					fPente = ((180 / M_PI) * atanf(fPente));
					return fPente;
				}

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente < 0) { // Le joueur se dÈplace vers la droite et la pente est nÈgative.
					fPente = 180 - ((180 / M_PI) * atanf(fPente));
					return fPente;
				}
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente > 0) { // Le joueur se dÈplace vers la gauche et la pente est positive.
					fPente = 180 + (180 / M_PI) * atanf(fPente);
					return fPente;
				}
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente < 0) { // Le joueur se dÈplace vers la gauche et la pente est nÈgative.
					fPente = 360 - (180 / M_PI) * atanf(fPente);
					return fPente;
				}

			}

		}
		
		else {
<<<<<<< HEAD
=======

>>>>>>> origin/Branche-Player
			if (fPente != 0)
				return (180 / M_PI) * atanf(fPente);
			else
				return 270;
<<<<<<< HEAD
=======

			if (iCov != 0 && iVar != 0) 
				return (180 / M_PI) * atanf(fPente);

>>>>>>> origin/Branche-Player
		}
		return 362;
		*/
	}


	// ProcÈdure dÈterminant la position d'une collision entre un objet et la map, si il y en a une.
	// En entrÈe:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X qui sera retournÈ.
	// Param4: La position en Y qui sera retournÈ.
	bool CollisionObjetMapChuteLibre(SDL_Surface* _pSurfaceObjet, SDL_Rect _RectDestinationObjet, int* _iX, int* _iY) {

		if (_RectDestinationObjet.x <= 1366 && _RectDestinationObjet.x + _RectDestinationObjet.w >= 0 && _RectDestinationObjet.y <= 768 && _RectDestinationObjet.y + _RectDestinationObjet.h > 0) {
			int iY = _RectDestinationObjet.h - 1;
			int iX = _RectDestinationObjet.w / 2;

			while (((unsigned int*)_pSurfaceObjet->pixels)[iY * _pSurfaceObjet->w + iX] == 0 || ((unsigned int*)_pSurfaceObjet->pixels)[iY * _pSurfaceObjet->w + iX] == TRANSPARENCE32BIT) {
				iY--;
			}

			bool boCollision = false;
			int iPixelObjetMapX = _RectDestinationObjet.x + iX;
			int iPixelObjetMapY = _RectDestinationObjet.y + iY;
			int iArret = 0;
			while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(iPixelObjetMapY)* m_pGameMap->ObtenirSurfaceMap()->w + iPixelObjetMapX] != 0 && ((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(iPixelObjetMapY)* m_pGameMap->ObtenirSurfaceMap()->w + iPixelObjetMapX] != TRANSPARENCE32BIT && iArret < 30) {
				boCollision = true;
				iPixelObjetMapY--;
				iArret++;
			}
			*_iX = iPixelObjetMapX;
			*_iY = iPixelObjetMapY - iY;
			return boCollision;
		}
		return false;
	}

	// ProcÈdure dÈterminant la position d'une collision entre un objet et la map, si il y en a une.
	// En entrÈe:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X qui sera retournÈ.
	// Param4: La position en Y qui sera retournÈ.
	bool CollisionObjetMap(SDL_Surface* _pSurfaceObjet, SDL_Rect _RectDestinationObjet, int* _iX, int* _iY) {

		// Objet dans la map...
		if (_RectDestinationObjet.x + _RectDestinationObjet.w < 1366 && _RectDestinationObjet.x >= 0 && _RectDestinationObjet.y + _RectDestinationObjet.h < 768 && _RectDestinationObjet.y >= 0) {

			SDL_Surface* pSurfaceMap = m_pGameMap->ObtenirSurfaceMap();

			// Tous les y
			for (int y = 0; y < _RectDestinationObjet.h; y++) {
				// Tous les x
				for (int x = 0; x < _RectDestinationObjet.w; x++) {
					// Si dans la surface de l'objet, c'est pas transparent...
					if (((unsigned int*)_pSurfaceObjet->pixels)[y * _pSurfaceObjet->w + x] != 0 && ((unsigned int*)_pSurfaceObjet->pixels)[y * _pSurfaceObjet->w + x] != TRANSPARENCE32BIT) {

						// Position de la pixel dans le rect du joueur
						int iPixelObjetMapX = _RectDestinationObjet.x + x;
						int iPixelObjetMapY = _RectDestinationObjet.y + y;

						// VÈrifier Transparence
						if (((unsigned int*)pSurfaceMap->pixels)[iPixelObjetMapY * pSurfaceMap->w + iPixelObjetMapX] != 0 && ((unsigned int*)pSurfaceMap->pixels)[iPixelObjetMapY* pSurfaceMap->w + iPixelObjetMapX] != TRANSPARENCE32BIT) {
							if (_iX != nullptr)
								*_iX = iPixelObjetMapX - _RectDestinationObjet.x; // Pixel en collision dans la surface de l'objet en X.
							if (_iY != nullptr)
								*_iY = iPixelObjetMapY - _RectDestinationObjet.y; // Pixel en collision dans la surface de l'objet en Y.
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	// Fontion dÈterminant s'il y a une collison avac le missile.
	// En entrÈe:
	// Param1: La surface du missile.
	// Param2: La destination du missile.
	// Param3: La position en X qui sera retournÈ.
	// Param4: La position en Y qui sera retournÈ.
	// Param5: Le renderer de la fenÍtre pour afficher le dÈgats.
	// Retour : S'il y a une collision.
	bool CollisionMissile(SDL_Surface* _pSurfaceMissile, SDL_Rect _RectDestination, int* _iX, int* _iY, SDL_Renderer* _pRenderer) {

		// Variables temporaires...
		CListeDC<CPlayer*>* pPlayerListTmp;
		SDL_Rect RectPlayer;
		SDL_Rect RectSourcePlayer;
		SDL_Surface* pSurfacePlayer;
		string strDommage;
		char chrTmp[8];
		int xPlayer, xMissile, yPlayer, yMissile, FinY, FinX;
		int XPlayer, XMissile, YPlayer, YMissile;
		bool boCollsionJoeur;

		// Pour toutes les Èquipes...
		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			// Obtient la liste des joueurs des Èquipes.
			pPlayerListTmp = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();

			// Pour tous les joeurs...
			pPlayerListTmp->AllerATrieur(0);
			for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

				// On obtient les informations du joueur (Destination, source, et sa surface).
				RectPlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirRectDestination();
				RectSourcePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirRectSource();
				pSurfacePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirSurface();

				// Si le missile est ‡ gauche du joueur, partiellement dans le joueur en x et au moins dans le joueur en y.
				if (_RectDestination.x + _RectDestination.w >= RectPlayer.x && _RectDestination.x < RectPlayer.x && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = 0;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = RectPlayer.x - _RectDestination.x;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}

				// Si le missile est complËtement dans le joueur...
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = 0;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}
				
				// Si le missile est ‡ droite du joueur, partiellement dans le joueur en x et au moins dans le joueur en y. 
				else if (_RectDestination.x <= RectPlayer.x + RectPlayer.w && _RectDestination.x + _RectDestination.w > RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = RectPlayer.w - ((RectPlayer.x + RectPlayer.w) - _RectDestination.x);
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = 0;
					yMissile = 0;
					FinX = (RectPlayer.x + RectPlayer.w) - _RectDestination.x;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}

				// Si le missile est en haut du joueur, partiellement dans le joueur en y et au moins dans le joueur en x.
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y < RectPlayer.y && _RectDestination.y + _RectDestination.h >= RectPlayer.y) {

					// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = 0;
					xMissile = 0;
					yMissile = RectPlayer.y - _RectDestination.y;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}

				// Si le missile est en bas du joueur, partiellement dans le joueur en y et au moins dans le joueur en x.
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y < RectPlayer.y + RectPlayer.w && _RectDestination.y + _RectDestination.h > RectPlayer.y + RectPlayer.h) {

					// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = RectPlayer.h - ((RectPlayer.y + RectPlayer.h) - _RectDestination.y);
					xMissile = 0;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = (RectPlayer.y + RectPlayer.h) - _RectDestination.y;
					boCollsionJoeur = true;
				}

				else {

					// Il n'y a pas de collision avec le joueur.
					boCollsionJoeur = false;
				}

				// S'il y a une collision avec le joueur...
				if (boCollsionJoeur) {
					
					// Pour toute la hauteur du missile qui est ‡ vÈrifier...
					for (; yMissile < FinY; yPlayer++, yMissile++) {
						
						// Pour toute la largeur du missile qu'il faut regarder...
						for (XPlayer = xPlayer, XMissile = xMissile; XMissile < FinX; XPlayer++, XMissile++) {

							// Si la surface du joueur n'est pas ‡ transparente au point yPlayer et XPlayer.
							if (((unsigned int*)pSurfacePlayer->pixels)[(yPlayer + RectSourcePlayer.y) * pSurfacePlayer->w + (XPlayer + RectSourcePlayer.x)] != 0)

								// Si la surface du missile n'est pas ‡ transparente au point yMissile et XMissile.
								if (((unsigned int*)_pSurfaceMissile->pixels)[(yMissile)* _pSurfaceMissile->w + (XMissile)] != 0) {

									*_iX = xPlayer; // Initialise la valeur de sorie en x.
									*_iY = yPlayer; // Initialise la valeur de sorie en y.
									
									// CrÈe un label avec le dommage infligÈ au joueur.
									strDommage.append("-");
									strDommage.append(SDL_itoa(pPlayerListTmp->ObtenirElementTrieur()->GetHealth() * 100, chrTmp, 10));
									m_pListeDomage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { RectPlayer.x, RectPlayer.y - 20, 0, 0 }, { RectPlayer.x, RectPlayer.y - 80, 0, 0 }, 30, 2000, _pRenderer));
									
									// Retire le joueur de la partie.
									pPlayerListTmp->RetirerTrieur(true);
									m_pGameMap->CreateHealthPack();
									return true;

								}

						}
					}
				}

			pPlayerListTmp->AllerSuivantTrieur(); // Prochain joueur...
			}

			m_pTeamList->AllerSuivantTrieur(); // Prochaine Èquipe.
		}


		// Regarde s'il y a une collision avec la map...
		return CollisionObjetMap(_pSurfaceMissile, _RectDestination, _iX, _iY);
	}

	void RepositionnementGrenadeMap(SDL_Surface* _pSurfaceGrenade, CVecteur2D* _pVecteurGrenade, SDL_Rect* _RectDestinationGrenade, bool* _boSensRotation, int* _iX, int* _iY) {
		
		int iIncrementationX = 0;
		int iIncrementationY = 0;
		int iVecteurX = _pVecteurGrenade->ObtenirComposanteX()/35;
		int iVecteurY = _pVecteurGrenade->ObtenirComposanteY()/35;


		for (int i = 0; i <= abs(iVecteurX) || i <= abs(iVecteurY); i++) {
			if (!CollisionObjetMap(_pSurfaceGrenade, *_RectDestinationGrenade, _iX, _iY)) {
				if (iIncrementationX <= abs(iVecteurX) && iVecteurX != 0) {
					_RectDestinationGrenade->x += iVecteurX / abs(iVecteurX);
					iIncrementationX++;
				}
				if (iIncrementationY <= abs(iVecteurY) && iVecteurY != 0) {
					_RectDestinationGrenade->y += iVecteurY / abs(iVecteurY);
					iIncrementationY++;
				}
			}
			else
				i = 600;
		}

		
		
	}

	bool CollisionGrenadeJoueur(SDL_Surface* _pSurfaceGrenade, SDL_Rect _RectDestinationGrenade, int* iX, int* iY) {

		SDL_Rect RectPlayer;
		SDL_Rect RectSourcePlayer;
		SDL_Surface* pSurfacePlayer;

		// Boucle des teams
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			m_pTeamList->AllerATrieur(i);

			// Boucle des players
			for (int j = 0; j < m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirCompte(); j++) {

				m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->AllerATrieur(j);

				RectPlayer = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirElementTrieur()->ObtenirRectDestination();
				RectSourcePlayer = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirRectSource();
				pSurfacePlayer = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirSurface();

				// Voir Si les deux rects sont en contact...
				if ((_RectDestinationGrenade.x >= RectPlayer.x && _RectDestinationGrenade.x <= RectPlayer.x + RectPlayer.w) && (_RectDestinationGrenade.y >= RectPlayer.y && _RectDestinationGrenade.y <= RectPlayer.y + RectPlayer.h) || (RectPlayer.x >= _RectDestinationGrenade.x && RectPlayer.x <= _RectDestinationGrenade.x + _RectDestinationGrenade.w) && (RectPlayer.y >= _RectDestinationGrenade.y && RectPlayer.y <= _RectDestinationGrenade.y + _RectDestinationGrenade.h)) {
					
					// Tous les y
					for (int y = 0; y < _RectDestinationGrenade.h; y++) {
						// Tous les x
						for (int x = 0; x < _RectDestinationGrenade.w; x++) {
							if (((unsigned int*)_pSurfaceGrenade->pixels)[y * _pSurfaceGrenade->w + x] != 0 && ((unsigned int*)_pSurfaceGrenade->pixels)[y * _pSurfaceGrenade->w + x] != TRANSPARENCE32BIT) {
								// Position de la pixel dans la map
								int iPixelGrenadeX = _RectDestinationGrenade.x + x;
								int iPixelGrenadeY = _RectDestinationGrenade.y + y;
								// Si la pixel est dans le rect du joueur...
								if ((iPixelGrenadeX >= RectPlayer.x && iPixelGrenadeX <= RectPlayer.x + RectPlayer.w) && (iPixelGrenadeY >= RectPlayer.y && iPixelGrenadeY <= RectPlayer.y + RectPlayer.h)) {
									// Position de la pixel dans le rect du joueur
									int iPixelJoueurX = iPixelGrenadeX - RectPlayer.x;
									int iPixelJoueurY = iPixelGrenadeY - RectPlayer.y;
									// VÈrifier Transparence
									if (((unsigned int*)pSurfacePlayer->pixels)[(iPixelJoueurY)* pSurfacePlayer->w + iPixelJoueurX] != 0 && ((unsigned int*)pSurfacePlayer->pixels)[(iPixelJoueurY)* pSurfacePlayer->w + iPixelJoueurX] != TRANSPARENCE32BIT) {
										*iX = iPixelGrenadeX;
										*iY = iPixelGrenadeY;
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	// Fonction qui retourne la position d'une collision.
	// ParamËtre: _pPlayer, joueur pour lequel on vÈrifie les collisions avec la carte de jeu
	// ParamËtre: _RectPlayer, rectangle qui encadre l'endroit ou l'on veut vÈrifier les collisions sur la map. (ex: Prochain rectangle destination d'un player.)
	// ParamËtre: _pboCollisionCorps, pointeur de boolÈenne qui indique si il y a eu une collision au corps.
	// ParamËtre: _pboCollisionPieds, pointeur de boolÈenne qui indique si il y a eu une collision au pieds.
	// ParamËtre: _puiXCorps, pointeur d'entier non-signÈ qui indique la position en x de la collision dans le rectangle du joueur entre le corps et la carte de jeu s'il y a lieu.
	// ParamËtre: _puiYPieds, pointeur d'entier non-signÈ qui indique la position en y de la collision dans le rectangle du joueur entre les pieds et la carte de jeu s'il y a lieu.
	// Retour: Un boolÈen qui indique si il y a eu une collision.
	bool VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _pboCollisionCorps, bool* _pboCollisionPieds, unsigned int* _puiXPieds, unsigned int* _puiYPieds, unsigned int* _puiXCorps, unsigned int* _puiYCorps) {

		SDL_Surface* pTmpSDLSurfaceMap = m_pGameMap->ObtenirSurfaceMap();	// pointeur temporaire de surface qui pointe sur la surface de la carte de jeu.
		SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropriÈe du joueur.

		SDL_Rect TmpSDLRectPlayerDestination = _RectPlayer;						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affichÈ sur la map.
		SDL_Rect TmpSDLRectPlayerSource = { 0, 0, 1, 1 };										// Structure SDL Rect qui contiendra le rectangle source de la surface appropriÈe du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vÈrifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vÈrifier les collisions du corps.

		unsigned int uiTransparenceJoueur;
		unsigned int uiTransparenceMap;

		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropriÈ pour le sprite de course.
		}

		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropriÈ pour le sprite de saut.
		}

		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds appropriÈ pour le sprite de parachute.
		}

		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropriÈ pour le sprite de repos.
		}
		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}

		if (_pPlayer->ObtenirSpriteCourse()->ObtenirEtage() == 0)					// Si le jueur est orientÈ vers la droite...
			TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
		else																		// Sinon...
			TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	


		*_pboCollisionCorps = false;
		*_pboCollisionPieds = false;

		*_puiXPieds = 0;
		*_puiYPieds = 0;
		*_puiXCorps = 0;
		*_puiYCorps = 0;

		uiTransparenceJoueur = ((unsigned int*)pTmpSDLSurfacePlayer->pixels)[0];
		uiTransparenceMap = ((unsigned int *)pTmpSDLSurfaceMap->pixels)[0];

		// VÈrification de la collision des pieds du joueur avec la carte de jeu...

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxPieds.h && !*_pboCollisionPieds; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxPieds.w && !*_pboCollisionPieds; x++) {			// On parcours les pixels dans le hitbox de gauche ‡ droite.

				if (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x >= 0 && TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x < 1366 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y >= 0 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y < 768) {

					if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfaceMap->w] != uiTransparenceMap) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfacePlayer->w] != uiTransparenceJoueur)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents des pieds du joueur...

						*_puiXPieds = TmpSDLRectPlayerHitboxPieds.x + x;
						*_puiYPieds = TmpSDLRectPlayerHitboxPieds.y + y;		// Dans le rectangle destination, on prend la position en y de la collision pour la stocker.

						*_pboCollisionPieds = true;				// On confirme la collision aux pieds.


					}
				}
			}

		}

		// VÈrification de la collision du corps du joueur avec la carte de jeu.

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxCorps.h && !*_pboCollisionCorps; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxCorps.w && !*_pboCollisionCorps; x++) {			// On parcours les pixels dans le hitbox de gauche ‡ droite.

				if (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x >= 0 && TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x < 1366 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y >= 0 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y < 768) {

					if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfaceMap->w] != uiTransparenceMap) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfacePlayer->w] != uiTransparenceJoueur)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du corps du joueur...

						*_puiXCorps = TmpSDLRectPlayerHitboxCorps.x + x;
						*_puiYCorps = TmpSDLRectPlayerHitboxCorps.y + y;		// Dans le rectangle destination, on prend la position en x de la collision pour la stocker.

						*_pboCollisionCorps = true;

					}
				}
			}

		}

		return (*_pboCollisionPieds || *_pboCollisionCorps);			// On retourne vrai dËs qu'il y a eu une collision.


	}


	// Fonction vÈrifiant s'il y a une collision entre un joueur et un pack.
	// En entrÈe:
	// Param1: Le joeur...
	bool DetectionCollisionPack(CPlayer* _pPlayer, bool* _boExplosion, SDL_Point* _RectExplosion, SDL_Renderer* _pRenderer) {

		// Variables temporaire...
		CPack* pPackTmp;
		SDL_Surface* pSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropriÈe du joueur.
		SDL_Rect RectPack;
		SDL_Rect RectPlayer = _pPlayer->ObtenirRectDestination();						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affichÈ sur la map.
		SDL_Rect RectSourcePlayer;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropriÈe du joueur pour la situation.
		SDL_Rect HitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vÈrifier les collisions du pieds.
		SDL_Rect HitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vÈrifier les collisions du corps.
		bool boTerminerBoucle = false;
		int xPlayer, xPack, yPlayer, yPack, FinY, FinX;
		int XPlayer, XPack;
		bool boCollsionPack;
		string strDommage;
		char chrTmp[8];

		*_boExplosion = false;

		// Si le joueur est en train de courir...
		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {
			
			// On utilise les informations du sprite course...
			pSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();
			RectSourcePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();
			HitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}

		// Si le joueur est en train de sauter...
		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			// On utilise les informations du sprite saut...
			pSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();
			RectSourcePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();
			HitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}

		// Si le joueur est en parachute...
		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			// On utilise les informations du sprite parachute...
			pSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();
			RectSourcePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();
			HitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();
		}

		// Si le joueur est au repos...
		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			// On utilise les informations du sprite repos...
			pSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();
			RectSourcePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();
			HitboxPieds = _pPlayer->ObtenirHitboxPieds();						
		}

		// Si le joueur est en jetpack...
		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

			// On utilise les informations du sprite jetpack...
			pSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();
			RectSourcePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();
			HitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}

		// Pour tous les packs...
		for (int j = 0; j < m_pGameMap->ObtenirPackList()->ObtenirCompte(); j++) {

			// On met le pack dans une variable temporaire...
			pPackTmp = m_pGameMap->ObtenirPackList()->ObtenirElementCurseur();
			RectPack = pPackTmp->GetRectDestination();

			// Si le pack est ‡ gauche du joueur, partiellement dans le joueur en x et au moins dans le joueur en y.
			if (RectPack.x + RectPack.w >= RectPlayer.x && RectPack.x < RectPlayer.x && RectPack.y + RectPack.h >= RectPlayer.y && RectPack.y <= (RectPlayer.y + RectPlayer.h)) {

				// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du pack.
				xPlayer = 0;
				yPlayer = RectPack.y - RectPlayer.y;
				xPack = RectPlayer.x - RectPack.x;
				yPack = 0;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionPack = true;
			}

			// Si le pack est complËtement dans le joueur...
			else if (RectPack.x >= RectPlayer.x && RectPack.x + RectPack.w <= RectPlayer.x + RectPlayer.w && RectPack.y >= RectPlayer.y && RectPack.y + RectPack.h <= (RectPlayer.y + RectPlayer.h)) {

				// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du pack.
				xPlayer = RectPack.x - RectPlayer.x;
				yPlayer = RectPack.y - RectPlayer.y;
				xPack = 0;
				yPack = 0;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionPack = true;
			}
			// Si le pack est ‡ droite du joueur, partiellement dans le joueur en x et au moins dans le joueur en y. 
			else if (RectPack.x <= RectPlayer.x + RectPlayer.w && RectPack.x + RectPack.w > RectPlayer.x + RectPlayer.w && RectPack.y + RectPack.h >= RectPlayer.y && RectPack.y <= (RectPlayer.y + RectPlayer.h)) {

				// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du pack.
				xPlayer = RectPlayer.w - ((RectPlayer.x + RectPlayer.w) - RectPack.x);
				yPlayer = RectPack.y - RectPlayer.y;
				xPack = 0;
				yPack = 0;
				FinX = (RectPlayer.x + RectPlayer.w) - RectPack.x;
				FinY = RectPack.h;
				boCollsionPack = true;
			}
			// Si le pack est en haut du joueur, partiellement dans le joueur en y et au moins dans le joueur en x.
			else if (RectPack.x >= RectPlayer.x && RectPack.x + RectPack.w <= RectPlayer.x + RectPlayer.w && RectPack.y < RectPlayer.y && RectPack.y + RectPack.h >= RectPlayer.y) {
				// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du pack.
				xPlayer = RectPack.x - RectPlayer.x;
				yPlayer = 0;
				xPack = 0;
				yPack = RectPlayer.y - RectPack.y;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionPack = true;
			}
			// Si le pack est en bas du joueur, partiellement dans le joueur en y et au moins dans le joueur en x.
			else if (RectPack.x + RectPack.w >= RectPlayer.x && RectPack.x <= RectPlayer.x + RectPlayer.w && RectPack.y <= RectPlayer.y + RectPlayer.h && RectPack.y + RectPack.h > RectPlayer.y + RectPlayer.h) {

				// On dÈfinie oÅÅÅ˘ regarder dans le surface du joueur et dans la surface du pack.
				xPlayer = RectPack.x - RectPlayer.x;
				yPlayer = RectPlayer.h - ((RectPlayer.y + RectPlayer.h) - RectPack.y);
				xPack = 0;
				yPack = 0;
				FinX = RectPack.w;
				FinY = (RectPlayer.y + RectPlayer.h) - RectPack.y;
				boCollsionPack = true;
			}

			else {

				// Il n'y a pas de collision...
				boCollsionPack = false;
			}

			// S'il y a une collison entre le pack et le joeur...
			if (boCollsionPack) {

				// Pour toute la hauteur du pack qui est ‡ vÈrifier...
				for (; yPack < FinY; yPlayer++, yPack++) {

					// Pour toute la largeur du pack qui est ‡ vÈrifier...
					for (XPlayer = xPlayer, XPack = xPack; XPack < FinX; XPlayer++, XPack++) {

						// Si la surface du joueur n'est pas ‡ transparente au point yPlayer et XPlayer.
						if (((unsigned int*)pSurfacePlayer->pixels)[(yPlayer + RectSourcePlayer.y) * pSurfacePlayer->w + (XPlayer + RectSourcePlayer.x)] != 0)

							// Si la surface du pack n'est pas ‡ transparente au point yPack et XPack.
							if (((unsigned int*)pPackTmp->GetSurface()->pixels)[(yPack)* pPackTmp->GetSurface()->w + (XPack)] != 0) {

								// Si c'est un healthpack...
								if (pPackTmp->GetSpriteExplosion() == nullptr) {
									strDommage.append("+");
									strDommage.append(SDL_itoa((_pPlayer->GetHealth() * 100) * 0.25, chrTmp, 10));
								}

								// Si c'est une mine...
								else {
									strDommage.append("-");
									strDommage.append(SDL_itoa(_pPlayer->GetHealth() * 100, chrTmp, 10));

								}

								// CrÈÈ un label indiquant un dommage (ou un gain de santÈ).
								m_pListeDomage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { RectPlayer.x, RectPlayer.y - 20, 0, 0 }, { RectPlayer.x, RectPlayer.y - 80, 0, 0 }, 30, 2000, _pRenderer));

								// Si c'est une mine...
								if (pPackTmp->Use(_pPlayer)) {
									*_boExplosion = true;
									*_RectExplosion = { pPackTmp->GetRectDestination().x + pPackTmp->GetRectDestination().w / 2, pPackTmp->GetRectDestination().y + pPackTmp->GetRectDestination().h / 2 };
								}

								return true;



							}
					}
				}
			}

			m_pGameMap->ObtenirPackList()->AllerSuivantCurseur(); // Prochain pack.
		}

	}

	void DommageExplosion(SDL_Point _ExplosionPoint, unsigned int _uiRayon, SDL_Renderer* _pRenderer) {

		CListeDC<CPlayer*>* pPlayerList;
		CListeDC<CPack*>* pPackList;
		CPlayer* pPlayerTmp;
		CPack* pPackTmp;
		SDL_Rect RectDestinationPlayer;
		SDL_Rect RectDestinationPack;
		SDL_Rect RectExplosion = { _ExplosionPoint.x - _uiRayon, _ExplosionPoint.y - _uiRayon, 2 * _uiRayon, 2 * _uiRayon };

		int iDistanceRayon, iDomage;
		float fPourcentage;
		char chrTmp[8];
		string strDomage;

		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte() && m_pTeamList->ObtenirCompte() > 0; i++) {

			pPlayerList = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();


			pPlayerList->AllerATrieur(0);
			for (int j = 0; j < pPlayerList->ObtenirCompte() && pPlayerList->ObtenirCompte() > 0; j++) {

				pPlayerTmp = pPlayerList->ObtenirElementTrieur();
				RectDestinationPlayer = pPlayerTmp->ObtenirRectDestination();

				if ((RectDestinationPlayer.x < RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w >= RectExplosion.x) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y && RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h)) {

					iDistanceRayon = (RectExplosion.x + _uiRayon) - (RectDestinationPlayer.x + RectDestinationPlayer.w);
					fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);

					iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

					strDomage.append("-");
					strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
					m_pListeDomage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
					pPlayerTmp->ModifierStabiliteJoueur(false);
				}

				else if ((RectDestinationPlayer.x <= RectExplosion.x + RectExplosion.w && RectDestinationPlayer.x + RectDestinationPlayer.w > RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y && RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h)) {

					iDistanceRayon = RectDestinationPlayer.x - (RectExplosion.x + _uiRayon);
					fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);
					
					iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

					strDomage.append("-");
					strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
					m_pListeDomage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
					
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
					pPlayerTmp->ModifierStabiliteJoueur(false);
				}

				else if ((RectDestinationPlayer.x >= RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y >= RectExplosion.y && RectDestinationPlayer.y + RectDestinationPlayer.h <= RectExplosion.y + RectExplosion.h)) {

					if (RectDestinationPlayer.x + RectDestinationPlayer.w >= RectExplosion.x + _uiRayon && RectDestinationPlayer.x <= RectExplosion.x + _uiRayon)
						iDistanceRayon = 0;

					else if (RectDestinationPlayer.x + RectDestinationPlayer.w < RectExplosion.x + _uiRayon && RectDestinationPlayer.x > RectExplosion.x)
						iDistanceRayon = (RectExplosion.x + _uiRayon) - (RectDestinationPlayer.x + RectDestinationPlayer.w);

					else
						iDistanceRayon = RectDestinationPlayer.x - (RectExplosion.x + _uiRayon);

					fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);
					
					iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

					strDomage.append("-");
					strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
					m_pListeDomage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
					
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
					pPlayerTmp->ModifierStabiliteJoueur(false);

				}

				else if ((RectDestinationPlayer.x >= RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y < RectExplosion.y && RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y)) {

					iDistanceRayon = (RectExplosion.y + _uiRayon) - (RectDestinationPlayer.y + RectDestinationPlayer.h);
					fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);
					
					iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

					strDomage.append("-");
					strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
					m_pListeDomage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
					
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
					pPlayerTmp->ModifierStabiliteJoueur(false);
				}

				else if ((RectDestinationPlayer.x >= RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h && RectDestinationPlayer.y + RectDestinationPlayer.h > RectExplosion.y + RectExplosion.h)) {

					iDistanceRayon = RectDestinationPlayer.y - (RectExplosion.y + _uiRayon);
					fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);
					
					iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

					strDomage.append("-");
					strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
					m_pListeDomage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
					
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
					pPlayerTmp->ModifierStabiliteJoueur(false);
				}
				if (pPlayerTmp->GetHealth() <= 0)  {
					pPlayerList->RetirerTrieur(true);
					m_pGameMap->CreateHealthPack();
				}
				

				else
					pPlayerList->AllerSuivantTrieur();

			}

			if (pPlayerList->ObtenirCompte() == 0)
				m_pTeamList->RetirerTrieur(true);
			
			else 
				m_pTeamList->AllerSuivantTrieur();
		}
		pPackList = m_pGameMap->ObtenirPackList();

		pPackList->AllerDebut();

		for (int i = 0; i < pPackList->ObtenirCompte(); i++) {

			pPackTmp = pPackList->ObtenirElementCurseur();
			RectDestinationPack = pPackTmp->GetRectDestination();

			if (!pPackTmp->IsUse()) {

				if ((RectDestinationPack.x < RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w >= RectExplosion.x) && (RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y && RectDestinationPack.y <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, 45, _pRenderer);
				}

				else if ((RectDestinationPack.x <= RectExplosion.x + RectExplosion.w && RectDestinationPack.x + RectDestinationPack.w > RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y && RectDestinationPack.y <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, 45, _pRenderer);
				}

				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y >= RectExplosion.y && RectDestinationPack.y + RectDestinationPack.h <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, 45, _pRenderer);
				}

				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y < RectExplosion.y && RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, 45, _pRenderer);
				}

				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y <= RectExplosion.y + RectExplosion.h && RectDestinationPack.y + RectDestinationPack.h > RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, 45, _pRenderer);
				}
			}
			pPackList->AllerSuivantCurseur();
		}
		
	}

	// Fonction infligeant un dommage ‡ un joeur qui Ètai en chute libre...
	// En entrÈe:
	// Param1: Le joueur...
	// Param2: Le renderer de la fenÍtre.
	// En sortie: Si le joueur est mort.
	bool DommageChuteLibre(CPlayer* _pPlayer, SDL_Renderer* _pRenderer) {
		
		// Nombre de pixel par dÈplacememnt...
		double dRapportVitesse = _pPlayer->ObtenirVecteurVitesse()->ObtenirNorme() / 35;
		
		// Autres variables temporaires...
		double dPerteVie;
		string strDommage;
		char chrTmp[8];

		// Si le ratio est acceptable...
		if (dRapportVitesse > 5) {

			// Calcul du dommage avec cette petite formule...
			dPerteVie = ((dRapportVitesse - 5) * 15) / 100;
			strDommage.append("-");
			
			// Si le dommage ne tue pas le joueur...
			if (_pPlayer->GetHealth() > dPerteVie) {
				strDommage.append(SDL_itoa(dPerteVie * 100, chrTmp, 10));
				_pPlayer->SetHealth(_pPlayer->GetHealth() - dPerteVie);
				m_pListeDomage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 20, 0, 0 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
			}

			// Si le dommage tu le joueur...
			else {
				strDommage.append(SDL_itoa(_pPlayer->GetHealth() * 100, chrTmp, 10));
				_pPlayer->SetHealth(0);
				m_pListeDomage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 20, 0, 0 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
				return true;
			}
		}

		return false;
	}

	bool IsDebut() {
		return m_boDebutPartie;
	}
	                                                                              
};
