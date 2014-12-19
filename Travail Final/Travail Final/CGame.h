// 29 novembre 2014 par Gabriel Beaudry(gabriel.bdry@gmail.com)
// Ajout de la proc�dure changertour().

#include <Windows.h>
class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'�quipe.
	CToolBar* m_pToolBar; // La toolbar o� le joueur choisi son outils pour son tour.
	CTimer* m_pTimerPhysique; // Le timer pour la physique.

	bool m_boDebutPartie; // Si le jeu est en d�but de partie.


public:

	CGame(string _strEmplacementMap, int _iNombre�quipe, int _iNombreJoueur, CVent* _pVent, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle), SDL_Renderer* _pRenderer) {


		m_boDebutPartie = true;

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction);

		m_pTimerPhysique = new CTimer(20);

		for (int i = _iNombre�quipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementMap, _pRenderer, i, _iNombreJoueur, _MapDestruction, _Rotation));
		}
		m_pTeamList->AllerDebut();

		m_pToolBar = new CToolBar({ 0, 600, 1366, 168 }, { 0, 0, 0 }, _pRenderer, 265, 120, 60, 4, pGestionnaireTexture->ObtenirDonnee("BazookaTool"), pGestionnaireTexture->ObtenirDonnee("GrenadaTool"), pGestionnaireTexture->ObtenirDonnee("SwordTool"), pGestionnaireTexture->ObtenirDonnee("JetPackTool"));
		m_pTimerPhysique->Start();
	}

	void ChangerTour(SDL_Renderer* _pRenderer) {
		m_pTeamList->AllerSuivantCurseur();
		m_pTeamList->ObtenirElementCurseur()->ChangerPlayerActif();
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

		if (m_boDebutPartie)
			m_boDebutPartie = !IsAllTeamStable();

		m_pGameMap->ShowMap(_pRenderer);

		if (!_boPause) {
			PhysiquePlayer(_pRenderer);
			PhysiquePack();
		}

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {
			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

		if (!_boPause) {
			m_pToolBar->ShowToolBar(_pRenderer);
			m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ShowDescription(_pRenderer);
		}
	}

	void ReactToEvent(SDL_Event* _pEvent) {
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
				SDL_Rect RectExplosion;

				if (pPlayerActif->IsUsingTool())
					PhysiqueTool(_pRenderer);

				else if (!pPlayerActif->IsStable()) {

					DetectionCollisionPack(pPlayerActif, &boExplosion, &RectExplosion);
					if (boExplosion) {

						DomageExplosion(RectExplosion, 45);
					}

					else if (pPlayerActif->ObtenirSpriteCourse()->IsActif()) {

						dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35.0;

						if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {

							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY( pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY() );

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

								while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + 1 + _uiXPieds + (RectTmp.y + _uiYPieds - uiH) *m_pGameMap->ObtenirSurfaceMap()->w] != 0) && ( uiH <=2 ))
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

					}

		
				/*
				else if (!pPlayerActif->IsStable()) {

					if (pPlayerActif->ObtenirSpriteCourse()->IsActif()) {

						DetectionCollisionPack(pPlayerActif, &boExplosion, &RectExplosion);
						if (boExplosion) {

							DomageExplosion(RectExplosion, 45);
						}

						else if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {

							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(m_pGameMap->ObtenirGravite()->ObtenirComposanteY()*3);

							*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
							dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
							dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

						}

						else {

							if (boPied) {

								int uim = 2;
								int uin = 0;

								while (((pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0) && (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + pPlayerActif->ObtenirHitboxPieds().x + pPlayerActif->ObtenirHitboxPieds().w + uim + (RectTmp.y + pPlayerActif->ObtenirHitboxPieds().y + pPlayerActif->ObtenirHitboxPieds().h - uin) * m_pGameMap->ObtenirSurfaceMap()->w] == 0)) || ((pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1) && (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + pPlayerActif->ObtenirHitboxPieds().x - uim + (RectTmp.y + pPlayerActif->ObtenirHitboxPieds().y + pPlayerActif->ObtenirHitboxPieds().h - uin) * m_pGameMap->ObtenirSurfaceMap()->w] == 0)))
									uin--;

								if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1)
									uim = -2;


								while ((uin >= -4) && ((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + _uiXPieds + uim + (RectTmp.y + _uiYPieds + uin) * m_pGameMap->ObtenirSurfaceMap()->w] != 0)
									uin--;

								if (uin >= -4) {

									dComposanteX += uim;
									dComposanteY += uin;

								}

							}

							if (boCorps) {

								if (boCorps && pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0)
									dComposanteX -= (pPlayerActif->ObtenirRectDestination().w - _uiXCorps);

								if (boCorps && pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1)
									dComposanteX += _uiXCorps;

							}


						}

						pPlayerActif->DefinirPositionX(dComposanteX);
						pPlayerActif->DefinirPositionY(dComposanteY);
						pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);


					}

					else if (pPlayerActif->ObtenirSpriteSaut()->IsActif()) {



					}
					*/
					/*
					else if (pPlayerActif->IsSliding()) {
						CVecteur2D* VecteurFrottement = new CVecteur2D(1, 0.0f);
						double doAngle = RegressionLineaire(pPlayerActif->ObtenirHitboxPieds(), pPlayerActif->ObtenirRectDestination(), false); // Variable qui sert a calcul juste 1 fois l'angle.
						if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1) { // Si le joueur se d�place vers la droite.

							if (doAngle > 90 && doAngle < 180) { // Si le joueur se d�place vers la gauche et que la pente est vers le haut � gauche.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() -= *VecteurFrottement;
							}

							if (doAngle > 180 && doAngle < 270) { // Si le joueur se d�place vers la le bas a gauche.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(doAngle - 180);
								*pPlayerActif->ObtenirVecteurVitesse() -= *VecteurFrottement;
							}





						}
						else { // Si le joueur va vers la droite

							if (doAngle > 270) { // Si le joueur se d�place vers la droite et que la pente est vers le bas � droite.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(doAngle - 180);
								*pPlayerActif->ObtenirVecteurVitesse() -= *VecteurFrottement;
							}

							if (doAngle < 90) { // Si la pente est en haut a droite.
								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								VecteurFrottement->ModifierOrientation(-doAngle);
								*pPlayerActif->ObtenirVecteurVitesse() -= *VecteurFrottement;
							}
						}

						if (pPlayerActif->ObtenirVecteurVitesse() == 0 && RegressionLineaire(pPlayerActif->ObtenirHitboxPieds(), pPlayerActif->ObtenirRectDestination(), false) < 60) // Si la glissade est fini.
							pPlayerActif->ModifierGlissadeJoueur(false);
					}
					*/
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

			CListeDC<CPlayer*>* pPlayerListTmp;
			CPlayer* pPlayer;
			SDL_Rect RectPlayer;

			if (m_pTimerPhysique->IsDone()) {

				for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

					pPlayerListTmp = m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer();

					for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

						pPlayer = pPlayerListTmp->ObtenirElementCurseur(); // Quand je d�bogais, il y a eu une violation d'acc�s l'objet au curseur �tait � ??????? (deleted)

						if (pPlayer->ObtenirSpriteRepos()->IsActif() && !pPlayer->IsStable()) {
							RectPlayer = pPlayer->ObtenirRectDestination();
							RectPlayer.y += 9.8;

							if (!VerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps))

								pPlayer->DefinirPositionY(RectPlayer.y);

							else {

								bool boExplosion;
								SDL_Rect RectTmp;

								RectPlayer.y -= (RectPlayer.h - _uiYPieds);
								pPlayer->DefinirPositionY(RectPlayer.y);
								pPlayer->ModifierStabiliteJoueur(true);

								DetectionCollisionPack(pPlayer, &boExplosion, &RectTmp);

								if (boExplosion) {
									pPlayerListTmp->Retirer(true);
									DomageExplosion(RectTmp, 45);
								}

							}
						}


						else if (pPlayer->ObtenirSpriteParachute()->IsActif()) {
							RectPlayer = pPlayer->ObtenirRectDestinationParachute();
							RectPlayer.y += 1;

							if (!VerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {

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
						pPlayerListTmp->AllerSuivantCurseur();
					}
					m_pTeamList->AllerSuivantCurseur();
				}

				m_pTimerPhysique->Start();
			}
		}
	}

	void PhysiquePack(void) {

		CListeDC<CPack*>* pPackListTmp = m_pGameMap->ObtenirPackList();
		SDL_Rect RectTmp;
		int iX, iY;

		for (int i = 0; i < pPackListTmp->ObtenirCompte(); i++) {

			if (!pPackListTmp->ObtenirElementCurseur()->IsStable()) {

				RectTmp = pPackListTmp->ObtenirElementCurseur()->GetRectDestination();

				if (CollisionObjetMapChuteLibre(pPackListTmp->ObtenirElementCurseur()->GetSurface(), RectTmp, &iX, &iY)) {

					RectTmp.y = iY;
					pPackListTmp->ObtenirElementCurseur()->ModifierAnlge(RegressionLineaire({ 0, 0, RectTmp.w, RectTmp.h }, RectTmp, true));
					pPackListTmp->ObtenirElementCurseur()->ModifierStabilePack(true);
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

	void PhysiqueTool(SDL_Renderer* _pRenderer) {

		if (m_pToolBar->ObtenirPositionObjetDoubleClick() <= 1) {

			CProjectile* pProjectileTmp = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirProjectile(m_pToolBar->ObtenirPositionObjetDoubleClick());
			SDL_Rect* RectTmp;
			CVecteur2D* pVecteurVitesse;
			int iX, iY;

			if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
				m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
				ChangerTour(_pRenderer);
				pProjectileTmp->DefinirExplosion(false);
			}
			
			if (pProjectileTmp->ObtenirSprite("") != nullptr) {

				if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
					m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
					ChangerTour(_pRenderer);
					pProjectileTmp->DefinirExplosion(false);
				}
			}
			if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 0 && pProjectileTmp->EstLancer())  {

				RectTmp = pProjectileTmp->ObtenirRectDestination();
				pVecteurVitesse = pProjectileTmp->ObtenirVecteurVitesse();

				if (CollisionMissile(pProjectileTmp->ObtenirSurface(), *pProjectileTmp->ObtenirRectDestination(), &iX, &iY)) {

					pProjectileTmp->ReactionExplosion(pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY);

					AfficherGame(_pRenderer, false);
					SDL_RenderPresent(_pRenderer);

					DomageExplosion({ pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY}, 50);
				}

				else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768)
					pProjectileTmp->ReactionExplosion(-100, -100);

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
			else if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 1 && pProjectileTmp->EstLancer()) {

				bool boSensRotation;
				RectTmp = pProjectileTmp->ObtenirRectDestination();

				// AutoDesctruction de la grenade...
				if (pProjectileTmp->ReactionExplosion(0, 0)) {
					RectTmp = pProjectileTmp->ObtenirRectDestination();
					DomageExplosion({ RectTmp->x, RectTmp->y }, RectTmp->w / 2);
				}
				else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768) {
					pProjectileTmp->DestructionProjectile();
				}
				else
				{
					pVecteurVitesse = pProjectileTmp->ObtenirVecteurVitesse();

					if (CollisionGrenadeJoueur(pProjectileTmp->ObtenirSurface(), *pProjectileTmp->ObtenirRectDestination(), &iX, &iY)) {

						pVecteurVitesse->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirComposanteY(),pVecteurVitesse->ObtenirOrientation() + 180);
						//pProjectileTmp->ReactionColision(pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY);
						//DomageExplosion({ pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y }, 40);
						//ChangerTour()
					}

					if (CollisionGrenadeMap(pProjectileTmp->ObtenirSurface(), pVecteurVitesse, RectTmp, &boSensRotation)) {
						float angle = (float)RegressionLineaire(*pProjectileTmp->ObtenirRectDestination(), *pProjectileTmp->ObtenirRectDestination(), true);
						CVecteur2D VecteurNormal = CVecteur2D(1, angle + 90);
						double dScalaire = 2 * pVecteurVitesse->Scalaire(VecteurNormal.ObtenirComposanteX(), VecteurNormal.ObtenirComposanteY());
						*pVecteurVitesse -= VecteurNormal * dScalaire;
						*pVecteurVitesse = *pVecteurVitesse * 0.35;

						if (boSensRotation)
							pProjectileTmp->DefinirRotation(-300);
						else
							pProjectileTmp->DefinirRotation(300);
					}

					RectTmp->x += pVecteurVitesse->ObtenirComposanteX() / 35;
					RectTmp->y += pVecteurVitesse->ObtenirComposanteY() / 35;
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

	// Proc�dure qui retourne la pente 
	// Param�tre : _RectPiedJoueur : Le rect pied du joueur acitf. Si c'est des mines ou des objets le mettre � 0.
	//			 : _RectJoueur -> L'emplacement de l'objet ou le joueur dans la maop.
	//			 : _boObjet -> Si c'est un objet ou non.
	// Retour : integer double qui repr�sente l'angle de la pente.
	double RegressionLineaire(SDL_Rect _RectPiedJoueur, SDL_Rect _RectJoueur, bool _boObjet) {
		float fPente = 0;
		float iCov = 0; // Variable en y moyenne.
		float iVar = 0; // Variable en x moyen.
		float fX = 0; // Valeur en x pour la r�gression.
		float fY = 0; // Valeur en y pour la r�gression.
		int iN = 0; // Le nombre de fois qu'il y a des "diff�rent de transparent" Sert a savoir le milieu de la r�gressuion.
		SDL_Rect _RectRegression;
		if (!_boObjet) {
			_RectRegression.x = _RectPiedJoueur.x + (_RectJoueur.w / 2); // Le rect commence au milieu du joueur.
			_RectRegression.y = _RectPiedJoueur.y + _RectPiedJoueur.h;
			_RectRegression.w = 15; // Largeur du Rect.
			int y = 0; // Utiliser pour ma boucle au lieu d'utiliser mon rect pour v�rifier.
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
			}
		}

		else {
			_RectRegression = _RectPiedJoueur;
			_RectRegression.y += _RectPiedJoueur.h / 2;
			_RectRegression.h += 10;
		}

		//int* iTableau = new int[_RectRegression.w,_RectRegression.h]; // Tableau.
		int iTableau[38][28];
		for (int j = 0; j < _RectRegression.h; j++) { // Boucler sur toute le rect du pied dans la position de la map.
			for (int i = 0; i < _RectRegression.w; i++) {
				if (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(i + _RectRegression.x + _RectJoueur.x) + ((j + _RectRegression.y + _RectJoueur.y) * (m_pGameMap->ObtenirSurfaceMap()->w))] != 0)  { // Si le pixel est transparent.
					iTableau[i][j] = 1; // Mettre 1 dans mon tableau.
					fX += i; // fX va servir a faire la moyenne des X.
					fY += j; // fY va servir a faire la moyenne des Y.
					iN += 1; // Pour diviser le nombre d'�l�ments.
				}
				else
					iTableau[i][j] = 0;
			}
		}
		if (fX != 0 && fY != 0) {
			fX = fX / iN; // moyenne
			fY = fY / iN; // moyenne
		}
		for (int j = 0; j < _RectRegression.h; j++) {
			for (int i = 0; i < _RectRegression.w; i++) {
				if (iTableau[i][j] == 1) {
					iCov += ((j - fY) * (i - fX)); // Calcul pour Y moyens avec le Y moyens.
					iVar += pow((j - fY), 2);    // Calcul pour X moyens avec le X moyens.
				}
			}
		}

		if (iCov != 0 && iVar != 0) {
			iCov = (iCov / iN); //moyenne
			iVar = (iVar / iN); //moyenne
		}



		fPente = iCov / iVar; // Donne la pente. iCov = y , iVar = x.
		if (!_boObjet) {
			if (iCov != 0 && iVar != 0) {
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente > 0) { // Le joueur se d�place vers la droite et la pente est positive.
					fPente = (180 / M_PI) * atanf(fPente);
					return fPente;
				}

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente < 0) { // Le joueur se d�place vers la droite et la pente est n�gative.
					fPente = 360 - ((180 / M_PI) * atanf(fPente));
					return fPente;
				}
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente > 0) { // Le joueur se d�place vers la gauche et la pente est positive.
					fPente = 180 + (180 / M_PI) * atanf(fPente);
					return fPente;
				}
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente < 0) { // Le joueur se d�place vers la gauche et la pente est n�gative.
					fPente = 180 + (180 / M_PI) * atanf(fPente);
					return fPente;
				}
			}
		}
		else {
			return (180 / M_PI) * atanf(fPente);
		}

		return 362;

	}

	// Proc�dure d�terminant la position d'une collision entre un objet et la map, si il y en a une.
	// En entr�e:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X qui sera retourn�.
	// Param4: La position en Y qui sera retourn�.
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
			*_iX = _RectDestinationObjet.x;
			*_iY = iPixelObjetMapY - iY;
			return boCollision;
		}
		return false;
	}

	// Proc�dure d�terminant la position d'une collision entre un objet et la map, si il y en a une.
	// En entr�e:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X qui sera retourn�.
	// Param4: La position en Y qui sera retourn�.
	bool CollisionObjetMap(SDL_Surface* _pSurfaceObjet, SDL_Rect _RectDestinationObjet, int* _iX, int* _iY) {



		if (_RectDestinationObjet.x + _RectDestinationObjet.w <= 1366 && _RectDestinationObjet.x >= 0 && _RectDestinationObjet.y + _RectDestinationObjet.h <= 768 && _RectDestinationObjet.y >= 0) {
			// Tous les y
			for (int y = 0; y < _RectDestinationObjet.h; y++) {
				// Tous les x
				for (int x = 0; x < _RectDestinationObjet.w; x++) {
					// Si dans la surface de l'objet, c'est pas transparent...
					if (((unsigned int*)_pSurfaceObjet->pixels)[y * _pSurfaceObjet->w + x] != 0 && ((unsigned int*)_pSurfaceObjet->pixels)[y * _pSurfaceObjet->w + x] != TRANSPARENCE32BIT) {

						// Position de la pixel dans le rect du joueur
						int iPixelObjetMapX = _RectDestinationObjet.x + x;
						int iPixelObjetMapY = _RectDestinationObjet.y + y;
						// V�rifier Transparence

						if (iPixelObjetMapX > 0 && iPixelObjetMapX < 1366 && iPixelObjetMapY > 0 && iPixelObjetMapY < 768) {

							if (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(iPixelObjetMapY - 1) * m_pGameMap->ObtenirSurfaceMap()->w + iPixelObjetMapX] != 0 && ((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(iPixelObjetMapY - 1)* m_pGameMap->ObtenirSurfaceMap()->w + iPixelObjetMapX] != TRANSPARENCE32BIT) {
								*_iX = iPixelObjetMapX - _RectDestinationObjet.x;
								*_iY = iPixelObjetMapY - _RectDestinationObjet.y;
								return true;
							}
						}
					}
				}
			}
		}

		return false;

		/* Ancienne Collision
		*_iX = 0;
		*_iY = 0;
	
		unsigned int ix, iy;
		
		SDL_Surface* pSDLSurfaceMap = pWindowJeu->ObtenirGame()->ObtenirMap()->ObtenirSurfaceMap();
	
		for (int y = _RectDestination.y; y < _RectDestination.y + _RectDestination.h; y++) {
			for (int x = _RectDestination.x; x < _RectDestination.x + _RectDestination.w; x++) {
	
				if (x >= 0 && x <= 1366 && y >= 0 && y <= 768) {
	
					ix = ((unsigned int*)pSDLSurfaceMap->pixels)[y * pSDLSurfaceMap->w + x];
	
					if ((((unsigned int*)pSDLSurfaceMap->pixels)[y * pSDLSurfaceMap->w + x] != 0) && (((unsigned int*)_pSDLSurface->pixels)[(y - _RectDestination.y) * _pSDLSurface->w + (x - _RectDestination.x)] != 0)) {
	
						if (*_iX == 0) {
							*_iX = x;
							*_iY = y;
						}
					}
				}
			}
		}
		*/
	}

	bool CollisionMissile(SDL_Surface* _pSurfaceMissile, SDL_Rect _RectDestination, int* _iX, int* _iY) {

		CListeDC<CPlayer*>* pPlayerListTmp;
		SDL_Rect RectPlayer;
		SDL_Rect RectSourcePlayer;
		SDL_Surface* pSurfacePlayer;

		int xPlayer, xMissile, yPlayer, yMissile, FinY, FinX;
		int XPlayer, XMissile, YPlayer, YMissile;
		bool boCollsionJoeur;

		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			pPlayerListTmp = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();

			pPlayerListTmp->AllerATrieur(0);
			for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

				RectPlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirRectDestination();
				RectSourcePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirRectSource();
				pSurfacePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirSurface();

				//1
				if (_RectDestination.x + _RectDestination.w >= RectPlayer.x && _RectDestination.x < RectPlayer.x && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					xPlayer = 0;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = RectPlayer.x - _RectDestination.x;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}

				//5
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = 0;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}
				//2
				else if (_RectDestination.x <= RectPlayer.x + RectPlayer.w && _RectDestination.x + _RectDestination.w > RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					xPlayer = RectPlayer.w - ((RectPlayer.x + RectPlayer.w) - _RectDestination.x);
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = 0;
					yMissile = 0;
					FinX = (RectPlayer.x + RectPlayer.w) - _RectDestination.x;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}
				//3
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y < RectPlayer.y && _RectDestination.y + _RectDestination.h >= RectPlayer.y) {

					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = 0;
					xMissile = 0;
					yMissile = RectPlayer.y - _RectDestination.y;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}
				//4
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y < RectPlayer.y + RectPlayer.w && _RectDestination.y + _RectDestination.h > RectPlayer.y + RectPlayer.h) {

					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = RectPlayer.h - ((RectPlayer.y + RectPlayer.h) - _RectDestination.y);
					xMissile = 0;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = (RectPlayer.y + RectPlayer.h) - _RectDestination.y;
					boCollsionJoeur = true;
				}

				else {

					boCollsionJoeur = false;
				}

				if (boCollsionJoeur) {
					for (; yMissile < FinY; yPlayer++, yMissile++) {
						for (XPlayer = xPlayer, XMissile = xMissile; XMissile < FinX; XPlayer++, XMissile++) {

							if (xPlayer >= 0 && xPlayer <= 1366 && yPlayer >= 0 && yPlayer <= 768) {

								if (((unsigned int*)pSurfacePlayer->pixels)[(yPlayer + RectSourcePlayer.y) * pSurfacePlayer->w + (XPlayer + RectSourcePlayer.x)] != 0) 
									
									if (((unsigned int*)_pSurfaceMissile->pixels)[(yMissile)* _pSurfaceMissile->w + (XMissile)] != 0) {

									*_iX = xPlayer;
									*_iY = yPlayer;
									pPlayerListTmp->ObtenirElementTrieur()->SetHealth(0);
									return true;

								}
							}
						}
					}
				}

			pPlayerListTmp->AllerSuivantTrieur();
			}

			m_pTeamList->AllerSuivantTrieur();
		}

		return CollisionObjetMap(_pSurfaceMissile, _RectDestination, _iX, _iY);
	}

	bool CollisionGrenadeMap(SDL_Surface* _pSurfaceGrenade, CVecteur2D* _pVecteurGrenade, SDL_Rect* _RectDestinationGrenade, bool* _boSensRotation) {
		
		// Grenade dans la map...
		if (_RectDestinationGrenade->x + _RectDestinationGrenade->w <= 1366 && _RectDestinationGrenade->x >= 0 && _RectDestinationGrenade->y + _RectDestinationGrenade->h <= 768 && _RectDestinationGrenade->y >= 0) {

			bool boArret;
			bool boCollision = false;
			int iX;
			int iY;
			SDL_Surface* pSurfaceMap = m_pGameMap->ObtenirSurfaceMap();

			// Vecteur X Positif...
			if (_pVecteurGrenade->ObtenirComposanteX() >= 0) {

				*_boSensRotation = true;

				// Boucle qui v�rifie les pixels du c�t� droit du rect...
				for (int j = 0; j < _pSurfaceGrenade->h; j++) {
					iX = _RectDestinationGrenade->w - 1;
					iY = j;
					boArret = false;
					for (int i = _RectDestinationGrenade->w - 1; i >= 0; i--) {
						if (((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == 0 || ((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == TRANSPARENCE32BIT) {
							iX--;
							if (i == 0)
								boArret = true;
						}
						else
							i = -1;
					}

					if (!boArret) {
						iX += _RectDestinationGrenade->x;
						iY += _RectDestinationGrenade->y;
						if (((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != 0 && ((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != TRANSPARENCE32BIT) {
							_RectDestinationGrenade->x--;
							boCollision = true;
						}
					}
				}
			}
			else
			{

				*_boSensRotation = false;

				// Boucle qui v�rifie les pixels du c�t� gauche du rect...
				for (int j = 0; j < _pSurfaceGrenade->h; j++) {
					iX = 0;
					iY = j;
					boArret = false;
					for (int i = _RectDestinationGrenade->w - 1; i <= _RectDestinationGrenade->w - 1; i++) {
						if (((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == 0 || ((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == TRANSPARENCE32BIT) {
							iX++;
							if (i == _RectDestinationGrenade->w - 1)
								boArret = true;
						}
						else
							i = _RectDestinationGrenade->w;
					}
					if (!boArret) {
						iX += _RectDestinationGrenade->x;
						iY += _RectDestinationGrenade->y;
						if (((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != 0 && ((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != TRANSPARENCE32BIT) {
							_RectDestinationGrenade->x--;
							boCollision = true;
						}
					}
				}
			}

			// Vecteur Y Positif...
			if (_pVecteurGrenade->ObtenirComposanteY() >= 0) {

				// Boucle qui v�rifie les pixels du c�t� bas du rect...
				for (int i = 0; i < _RectDestinationGrenade->w; i++) {
					iX = i;
					iY = _RectDestinationGrenade->h - 1;
					boArret = false;
					for (int j = _RectDestinationGrenade->h - 1; j >= 0; j--) {
						if (((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == 0 || ((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == TRANSPARENCE32BIT) {
							iY--;
							if (j == 0)
								boArret = true;
						}
						else
							j = -1;
					}
					if (!boArret) {
						iX += _RectDestinationGrenade->x;
						iY += _RectDestinationGrenade->y;
						if (((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != 0 && ((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != TRANSPARENCE32BIT) {
							_RectDestinationGrenade->y--;
							boCollision = true;
						}
					}
				}
			}
			// Vecteur Y N�gatif...
			else
			{

				(*_boSensRotation) = !(*_boSensRotation);

				// Boucle qui v�rifie les pixels du c�t� haut du rect...
				for (int i = 0; i < _RectDestinationGrenade->w; i++) {
					iX = i;
					iY = 0;
					boArret = false;
					for (int j = _RectDestinationGrenade->h - 1; j <= _RectDestinationGrenade->h - 1; j++) {
						if (((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == 0 || ((unsigned int*)_pSurfaceGrenade->pixels)[iY * _pSurfaceGrenade->w + iX] == TRANSPARENCE32BIT) {
							iY++;
							if (j == _RectDestinationGrenade->h - 1)
								boArret = true;
						}
						else
							j = _RectDestinationGrenade->h;
					}
					if (!boArret) {
						iX += _RectDestinationGrenade->x;
						iY += _RectDestinationGrenade->y;
						if (((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != 0 && ((unsigned int*)pSurfaceMap->pixels)[(iY)* pSurfaceMap->w + iX] != TRANSPARENCE32BIT) {
							_RectDestinationGrenade->y++;
							boCollision = true;
						}
					}
				}
			}
			if (boCollision)
				int i = 0;
			return boCollision;
		}
		return false;
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
									// V�rifier Transparence
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
	// Param�tre: _pPlayer, joueur pour lequel on v�rifie les collisions avec la carte de jeu
	// Param�tre: _RectPlayer, rectangle qui encadre l'endroit ou l'on veut v�rifier les collisions sur la map. (ex: Prochain rectangle destination d'un player.)
	// Param�tre: _pboCollisionCorps, pointeur de bool�enne qui indique si il y a eu une collision au corps.
	// Param�tre: _pboCollisionPieds, pointeur de bool�enne qui indique si il y a eu une collision au pieds.
	// Param�tre: _puiXCorps, pointeur d'entier non-sign� qui indique la position en x de la collision dans le rectangle du joueur entre le corps et la carte de jeu s'il y a lieu.
	// Param�tre: _puiYPieds, pointeur d'entier non-sign� qui indique la position en y de la collision dans le rectangle du joueur entre les pieds et la carte de jeu s'il y a lieu.
	// Retour: Un bool�en qui indique si il y a eu une collision.
	bool VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _pboCollisionCorps, bool* _pboCollisionPieds, unsigned int* _puiXPieds, unsigned int* _puiYPieds, unsigned int* _puiXCorps, unsigned int* _puiYCorps) {

		SDL_Surface* pTmpSDLSurfaceMap = m_pGameMap->ObtenirSurfaceMap();	// pointeur temporaire de surface qui pointe sur la surface de la carte de jeu.
		SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropri�e du joueur.

		SDL_Rect TmpSDLRectPlayerDestination = _RectPlayer;						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affich� sur la map.
		SDL_Rect TmpSDLRectPlayerSource = { 0, 0, 1, 1 };										// Structure SDL Rect qui contiendra le rectangle source de la surface appropri�e du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du corps.

		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de course.
		}

		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de saut.
		}

		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds appropri� pour le sprite de parachute.
		}

		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de repos.
		}
		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}

		if (_pPlayer->ObtenirSpriteCourse()->ObtenirEtage() == 0)					// Si le jueur est orient� vers la droite...
			TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
		else																		// Sinon...
			TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	


		*_pboCollisionCorps = false;
		*_pboCollisionPieds = false;

		*_puiXPieds = 0;
		*_puiYPieds = 0;
		*_puiXCorps = 0;
		*_puiYCorps = 0;

		// V�rification de la collision des pieds du joueur avec la carte de jeu...

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxPieds.h && !*_pboCollisionPieds; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxPieds.w && !*_pboCollisionPieds; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

				if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfaceMap->w] != 0) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfacePlayer->w] != 0)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents des pieds du joueur...

					*_puiXPieds = TmpSDLRectPlayerHitboxPieds.x + x;
					*_puiYPieds = TmpSDLRectPlayerHitboxPieds.y + y;		// Dans le rectangle destination, on prend la position en y de la collision pour la stocker.

					*_pboCollisionPieds = true;				// On confirme la collision aux pieds.


				}

			}

		}

		// V�rification de la collision du corps du joueur avec la carte de jeu.

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxCorps.h && !*_pboCollisionCorps; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxCorps.w && !*_pboCollisionCorps; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

				if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfaceMap->w] != 0) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfacePlayer->w] != 0)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du corps du joueur...

					*_puiXCorps = TmpSDLRectPlayerHitboxCorps.x + x;
					*_puiYCorps = TmpSDLRectPlayerHitboxCorps.y + y;		// Dans le rectangle destination, on prend la position en x de la collision pour la stocker.

					*_pboCollisionCorps = true;

				}

			}

		}

		return (*_pboCollisionPieds || *_pboCollisionCorps);			// On retourne vrai d�s qu'il y a eu une collision.


	}

	/*
	Fonction qui detecte si un personnage marche sur une mine ou un pack
	parametres:
	_pTeamList = liste d'equipes pour obtenir les joueurs
	_Pack = Pack sur lequel la collision sera verifi�e
	�retour: true = une collision a lieu
	*/
	bool DetectionCollisionPack(CPlayer* _pPlayer, bool* _boExplosion, SDL_Rect* _RectExplosion) {

		CPack* pPackTmp;

		SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropri�e du joueur.
		SDL_Rect RectPack;

		SDL_Rect TmpSDLRectPlayerDestination = _pPlayer->ObtenirRectDestination();						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affich� sur la map.
		SDL_Rect TmpSDLRectPlayerSource;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropri�e du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du corps.

		bool boTerminerBoucle = false;

		int xPlayer, xMissile, yPlayer, yMissile, FinY, FinX;
		int XPlayer, XMissile, YPlayer, YMissile;
		bool boCollsionJoeur;

		*_boExplosion = false;

		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...
			
			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de course.
		}

		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de saut.
		}

		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds appropri� pour le sprite de parachute.
		}

		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de repos.
		}

		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}


		for (int j = 0; j < m_pGameMap->ObtenirPackList()->ObtenirCompte(); j++) {

			pPackTmp = m_pGameMap->ObtenirPackList()->ObtenirElementCurseur();
			RectPack = pPackTmp->GetRectDestination();

			//1
			if (RectPack.x + RectPack.w >= TmpSDLRectPlayerDestination.x && RectPack.x < TmpSDLRectPlayerDestination.x && RectPack.y + RectPack.h >= TmpSDLRectPlayerDestination.y && RectPack.y <= (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h)) {

				xPlayer = 0;
				yPlayer = RectPack.y - TmpSDLRectPlayerDestination.y;
				xMissile = TmpSDLRectPlayerDestination.x - RectPack.x;
				yMissile = 0;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionJoeur = true;
			}

			//5
			else if (RectPack.x >= TmpSDLRectPlayerDestination.x && RectPack.x + RectPack.w <= TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w && RectPack.y >= TmpSDLRectPlayerDestination.y && RectPack.y + RectPack.h <= (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h)) {

				xPlayer = RectPack.x - TmpSDLRectPlayerDestination.x;
				yPlayer = RectPack.y - TmpSDLRectPlayerDestination.y;
				xMissile = 0;
				yMissile = 0;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionJoeur = true;
			}
			//2
			else if (RectPack.x <= TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w && RectPack.x + RectPack.w > TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w && RectPack.y + RectPack.h >= TmpSDLRectPlayerDestination.y && RectPack.y <= (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h)) {

				xPlayer = TmpSDLRectPlayerDestination.w - ((TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w) - RectPack.x);
				yPlayer = RectPack.y - TmpSDLRectPlayerDestination.y;
				xMissile = 0;
				yMissile = 0;
				FinX = (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w) - RectPack.x;
				FinY = RectPack.h;
				boCollsionJoeur = true;
			}
			//3
			else if (RectPack.x >= TmpSDLRectPlayerDestination.x && RectPack.x + RectPack.w <= TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w && RectPack.y < TmpSDLRectPlayerDestination.y && RectPack.y + RectPack.h >= TmpSDLRectPlayerDestination.y) {

				xPlayer = RectPack.x - TmpSDLRectPlayerDestination.x;
				yPlayer = 0;
				xMissile = 0;
				yMissile = TmpSDLRectPlayerDestination.y - RectPack.y;
				FinX = RectPack.w;
				FinY = RectPack.h;
				boCollsionJoeur = true;
			}
			//4
			else if (RectPack.x >= TmpSDLRectPlayerDestination.x && RectPack.x + RectPack.w <= TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w && RectPack.y < TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.w && RectPack.y + RectPack.h > TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h) {

				xPlayer = RectPack.x - TmpSDLRectPlayerDestination.x;
				yPlayer = TmpSDLRectPlayerDestination.h - ((TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h) - RectPack.y);
				xMissile = 0;
				yMissile = 0;
				FinX = RectPack.w;
				FinY = (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h) - RectPack.y;
				boCollsionJoeur = true;
			}

			else {

				boCollsionJoeur = false;
			}

			if (boCollsionJoeur) {
				for (; yMissile < FinY; yPlayer++, yMissile++) {
					for (XPlayer = xPlayer, XMissile = xMissile; XMissile < FinX; XPlayer++, XMissile++) {

						if (xPlayer >= 0 && xPlayer <= 1366 && yPlayer >= 0 && yPlayer <= 768) {

							if (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(yPlayer + TmpSDLRectPlayerSource.y) * pTmpSDLSurfacePlayer->w + (XPlayer + TmpSDLRectPlayerSource.x)] != 0)

								if (((unsigned int*)pPackTmp->GetSurface()->pixels)[(yMissile)* pPackTmp->GetSurface()->w + (XMissile)] != 0) {

									pPackTmp->Use(_pPlayer);
									*_boExplosion = true;
									*_RectExplosion = { pPackTmp->GetRectDestination().x + pPackTmp->GetRectDestination().w / 2, pPackTmp->GetRectDestination().y + pPackTmp->GetRectDestination().h / 2, pPackTmp->GetRectDestination().w, pPackTmp->GetRectDestination().h };
									return true;

								}
						
						}
					}
				}
			}

			m_pGameMap->ObtenirPackList()->AllerSuivantCurseur();
		}

	}

	void DomageExplosion(SDL_Rect _RectPositionExplosion, int _iRayon) {

		CListeDC<CPlayer*>* pPlayerList;
		CListeDC<CPack*>* pPackList;
		CPlayer* pPlayerTmp;
		CPack* pPackTmp;
		SDL_Rect RectDestinationPlayer;
		SDL_Rect RectDestinationPack;

		_RectPositionExplosion.w = 2 * _iRayon;
		_RectPositionExplosion.h = _RectPositionExplosion.w;
		_RectPositionExplosion.x -= _iRayon;
		_RectPositionExplosion.y -= _iRayon;

		int iDistanceRayon;
		float fPourcentage;

		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte() && m_pTeamList->ObtenirCompte() > 0; i++) {

			pPlayerList = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();


			pPlayerList->AllerATrieur(0);
			for (int j = 0; j < pPlayerList->ObtenirCompte() && pPlayerList->ObtenirCompte() > 0; j++) {

				pPlayerTmp = pPlayerList->ObtenirElementTrieur();
				RectDestinationPlayer = pPlayerTmp->ObtenirRectDestination();

				if ((RectDestinationPlayer.x < _RectPositionExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w >= _RectPositionExplosion.x) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= _RectPositionExplosion.y && RectDestinationPlayer.y <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					iDistanceRayon = (_RectPositionExplosion.x + _iRayon) - (RectDestinationPlayer.x + RectDestinationPlayer.w);
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
				}

				else if ((RectDestinationPlayer.x <= _RectPositionExplosion.x + _RectPositionExplosion.w && RectDestinationPlayer.x + RectDestinationPlayer.w > _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= _RectPositionExplosion.y && RectDestinationPlayer.y <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					iDistanceRayon = RectDestinationPlayer.x - (_RectPositionExplosion.x + _iRayon);
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
				}

				else if ((RectDestinationPlayer.x >= _RectPositionExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPlayer.y >= _RectPositionExplosion.y && RectDestinationPlayer.y + RectDestinationPlayer.h <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					if (RectDestinationPlayer.x + RectDestinationPlayer.w >= _RectPositionExplosion.x + _iRayon && RectDestinationPlayer.x <= _RectPositionExplosion.x + _iRayon)
						iDistanceRayon = 0;

					else if (RectDestinationPlayer.x + RectDestinationPlayer.w < _RectPositionExplosion.x + _iRayon && RectDestinationPlayer.x > _RectPositionExplosion.x)
						iDistanceRayon = (_RectPositionExplosion.x + _iRayon) - (RectDestinationPlayer.x + RectDestinationPlayer.w);

					else
						iDistanceRayon = RectDestinationPlayer.x - (_RectPositionExplosion.x + _iRayon);

					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);

				}

				else if ((RectDestinationPlayer.x >= _RectPositionExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPlayer.y < _RectPositionExplosion.y && RectDestinationPlayer.y + RectDestinationPlayer.h >= _RectPositionExplosion.y)) {

					iDistanceRayon = (_RectPositionExplosion.y + _iRayon) - (RectDestinationPlayer.y + RectDestinationPlayer.h);
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
				}

				else if ((RectDestinationPlayer.x >= _RectPositionExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPlayer.y <= _RectPositionExplosion.y + _RectPositionExplosion.h && RectDestinationPlayer.y + RectDestinationPlayer.h > _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					iDistanceRayon = RectDestinationPlayer.y - (_RectPositionExplosion.y + _iRayon);
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
				}
				if (pPlayerTmp->GetHealth() <= 0) 
					pPlayerList->RetirerTrieur(true);
				

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

				if ((RectDestinationPack.x < _RectPositionExplosion.x && RectDestinationPack.x + RectDestinationPack.w >= _RectPositionExplosion.x) && (RectDestinationPack.y + RectDestinationPack.h >= _RectPositionExplosion.y && RectDestinationPack.y <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					pPackTmp->Use(nullptr);
					DomageExplosion(pPackTmp->GetRectDestination(), 45);
				}

				else if ((RectDestinationPack.x <= _RectPositionExplosion.x + _RectPositionExplosion.w && RectDestinationPack.x + RectDestinationPack.w > _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPack.y + RectDestinationPack.h >= _RectPositionExplosion.y && RectDestinationPack.y <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					pPackTmp->Use(nullptr);
					DomageExplosion(pPackTmp->GetRectDestination(), 45);
				}

				else if ((RectDestinationPack.x >= _RectPositionExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPack.y >= _RectPositionExplosion.y && RectDestinationPack.y + RectDestinationPack.h <= _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					pPackTmp->Use(nullptr);
					DomageExplosion(pPackTmp->GetRectDestination(), 45);
				}

				else if ((RectDestinationPack.x >= _RectPositionExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPack.y < _RectPositionExplosion.y && RectDestinationPack.y + RectDestinationPack.h >= _RectPositionExplosion.y)) {

					pPackTmp->Use(nullptr);
					DomageExplosion(pPackTmp->GetRectDestination(), 45);
				}

				else if ((RectDestinationPack.x >= _RectPositionExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= _RectPositionExplosion.x + _RectPositionExplosion.w) && (RectDestinationPack.y <= _RectPositionExplosion.y + _RectPositionExplosion.h && RectDestinationPack.y + RectDestinationPack.h > _RectPositionExplosion.y + _RectPositionExplosion.h)) {

					pPackTmp->Use(nullptr);
					DomageExplosion(pPackTmp->GetRectDestination(), 45);
				}
			}
		}
		
	}

	bool IsDebut() {
		return m_boDebutPartie;
	}
	                                                                              
};