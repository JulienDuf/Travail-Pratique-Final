// 29 novembre 2014 par Gabriel Beaudry(gabriel.bdry@gmail.com)
// Ajout de la proc�dure changertour().

class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'�quipe.
	CListeDC<CTemporaryLabel*>* m_pListeDommage; // Listes des domages.
	CToolBar* m_pToolBar; // La toolbar o� le joueur choisi son outils pour son tour.
	CTimer* m_pTimerPhysique; // Le timer pour la physique.

	bool m_boDebutPartie; // Si le jeu est en d�but de partie.
	bool m_boFinPartie;
	bool m_boFinTour; // Si le jeu est en fin de tour.

public:

	// Constructeur de CGame...
	// En entr�e:
	// Param1: Chemin de l'emplacement des maps.
	// Param2: Nombre d'�quipe.
	// Param3: Nombre de joueur.
	// Param4: Vent de la map.
	// Param5: Pointeur de fonction sur la destruction de la map.
	// Param6: Pointeur de fonction sur la rotation.
	// Param7: Renderer de la fenetre d'affichage de la game.
	CGame(string _strEmplacementMap, unsigned int _iNombre�quipe, unsigned int _iNombreJoueur, CVent* _pVent, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle), SDL_Renderer* _pRenderer) {


		m_boDebutPartie = true;
		m_boFinTour = false;

		m_pTeamList = new CListeDC<CTeam*>();
		m_pListeDommage = new CListeDC<CTemporaryLabel*>();

		// Initialisation de la map...
		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction);

		m_pTimerPhysique = new CTimer(20);

		// Ajout des �quipes...
		for (int i = _iNombre�quipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementMap, _pRenderer, i, _iNombreJoueur, _MapDestruction, _Rotation));
		}
		m_pTeamList->AllerDebut();

		// Initialisation de la toolbar...
		m_pToolBar = new CToolBar({ 0, 600, 1366, 168 }, { 0, 0, 0 }, _pRenderer, 265, 120, 60, 4, pGestionnaireTexture->ObtenirDonnee("BazookaTool"), pGestionnaireTexture->ObtenirDonnee("GrenadaTool"), pGestionnaireTexture->ObtenirDonnee("SwordTool"), pGestionnaireTexture->ObtenirDonnee("JetPackTool"));
		// D�but du Chrono de la physique
		m_pTimerPhysique->Start();
	}

	// Destructeur de CGame...
	~CGame() {

		delete m_pGameMap; //La map.
		delete m_pTeamList; // Liste d'�quipe.
		delete m_pListeDommage; // Listes des domages.
		delete m_pToolBar; // La toolbar o� le joueur choisi son outils pour son tour.
		delete m_pTimerPhysique; // Le timer pour la physique.
	}

	// Proc�dure permettant de changer de tour...
	// En entr�e:
	// Param1: Renderer d'affichage pour le nouveau vent.
	void ChangerTour(SDL_Renderer* _pRenderer) {
		m_pTeamList->AllerSuivantCurseur();
		m_pTeamList->ObtenirElementCurseur()->ChangerPlayerActif();
		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReinitialisationTool();
		m_pGameMap->NouveauVent(pGestionnaireFont->ObtenirDonnee("pFontBouton"), _pRenderer);
		m_pToolBar->NouveauTour();
	}

	// Proc�dure changeant l'�quipe acitive.
	void ChangerTeamActive() {
		m_pTeamList->AllerSuivantCurseur();
	}

	// Proc�dure permettant d'afficher la game...
	// En entr�e:
	// Param1: Renderer d'affichage.
	// Param2: Si la game est en pause.
	void AfficherGame(SDL_Renderer* _pRenderer, bool _boPause) {

		// Si c'est le d�but de la partie ou qu'un tour vient de se terminer...
		if (m_boDebutPartie && m_boFinTour) {
			m_boDebutPartie = !IsAllTeamStable();
			// Si tout le monde est stable, un nouveau tour sera fait...
			if (!m_boDebutPartie) {
				ChangerTour(_pRenderer);
				m_boFinTour = false;
			}
		}

		else if (m_boDebutPartie)
			m_boDebutPartie = !IsAllTeamStable();

		// Affichage de la map...
		m_pGameMap->ShowMap(_pRenderer);

		// Si le jeu n'est pas en pause, on fait la physique...
		if (!_boPause) {
			PhysiquePlayer(_pRenderer);
			PhysiquePack(_pRenderer);
		}

		// Affichage des players de toutes les teams...
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {
			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

		// Affichage d'un cercle vert pour reconna�tre le player actif...
		if (!m_boDebutPartie)
			m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ShowEtoile(_pRenderer);

		// Si le jeu n'est pas en pause, affichage de la toolbar et des descriptions...
		if (!_boPause) {
			m_pToolBar->ShowToolBar(_pRenderer);
			m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ShowDescription(_pRenderer);
		}

		// Affichage des dommages inflig�s aux joueurs...
		m_pListeDommage->AllerDebut();
		for (int i = 0; i < m_pListeDommage->ObtenirCompte(); i++) {

			m_pListeDommage->ObtenirElementCurseur()->ShowControl(_pRenderer);
			if (m_pListeDommage->ObtenirElementCurseur()->IsDead())
				m_pListeDommage->Retirer(true);
			else
				m_pListeDommage->AllerSuivantCurseur();

		}
	}

	// Proc�dure r�agissant au �v�nement du programme.
	// En entr�e:
	// Param1: Le gestionnaire d'�v�nement...
	void ReactToEvent(SDL_Event* _pEvent) {
		
		// �v�nement par rapport � la game...
		switch (_pEvent->type) {

		// Si c'est une touche d'enfonc�e...
		case SDL_KEYDOWN:

			// Si c'Est la touche "T"
			if (_pEvent->key.keysym.scancode == SDL_SCANCODE_T && !m_boDebutPartie  && !m_boFinTour && m_pToolBar->ObtenirPositionObjetDoubleClick() > 3)
				m_pToolBar->ReverseboShow();

			break;
		}
		
		// ReactToEvent de la tolbar...
		m_pToolBar->ReactToEvent(_pEvent, m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif());

		// Initialisation d'un entier correspondant � l'arme doubleclick�e...
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

		// Si il y encore des munitions, fait le reacToEvent du joueur...
		if (uiMunition > 0)
			m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReactToEvent(_pEvent, m_pToolBar->ObtenirPositionObjetDoubleClick());

		// Update de la description des outils...
		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->UpdateDescription(m_pToolBar->ObtenirPositionObjetSuvol(), m_pToolBar->ObtenirRectPositionSouris());
	}

	// Proc�dure qui g�re la physique du joueur actif...
	// Param�tre : _pRenderer, Rendeur de la fen�tre de jeu.
	// Ne retourne rien.
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

				// Si le player utilise un tool...
				if (pPlayerActif->IsUsingTool())
					PhysiqueTool(_pRenderer);
				// Si le player n'est pas stable...
				else if (!pPlayerActif->IsStable()) {

					// D�tection de collision avec pack...
					DetectionCollisionPack(pPlayerActif, &boExplosion, &PointExplosion, _pRenderer);		// On v�rifie si il y a une collision entre le joueur et un pack.

					// Explosion de mine en collision...
					if (boExplosion) {									// Si il y a eu une explosion...
						m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->Retirer(true);
						m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->AllerPrecedentCurseur();
						m_pGameMap->CreateHealthPack();
						DommageExplosion(PointExplosion, m_pGameMap->ObtenirPackList()->ObtenirElementCurseur()->GetRayon(), _pRenderer);
					}

					else if (pPlayerActif->IsFreeFalling()) {		// Si le personnage est en chute libre...

						if (VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {			// Si il y a une collision entre le sol et le joueur...

							pPlayerActif->ModifierChuteLibreJoueur(false);				// Le joueur n'est plus en chute libre.

							pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY());	
							if (DommageChuteLibre(pPlayerActif, _pRenderer)) {
								m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->Retirer(true);							// On ajoute du dommage au joueur si il va assez vite.
								m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->AllerPrecedentCurseur();				//
								m_pGameMap->CreateHealthPack();
							}
							pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);


						}

						else {				// Si il n'y a pas de collision...

							// Le joueur descend � une plus grande vitesse.
							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

							dComposanteY += pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() / 35;
							pPlayerActif->DefinirPositionY(dComposanteY);

						}

					}

					

					else if (pPlayerActif->ObtenirSpriteCourse()->IsActif() || pPlayerActif->ObtenirSpriteSaut()->IsActif() || pPlayerActif->ObtenirSpriteRepos()->IsActif()) {		// Si le personnage est en course, en saut ou en repos...

						// On le fait bouger selon son vecteur de d�placement vecteur vitesse...
						dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35.0;
						dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35.0;

						if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {			// Si il n'y a pas de collision...

							// On fait bouger le joueur selon sa vitesse et la gravit�
							pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

							dComposanteY += pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() / 35;

							pPlayerActif->DefinirPositionX(dComposanteX);
							pPlayerActif->DefinirPositionY(dComposanteY);


							// On v�rifie si la distance est assez grande entre le joueur et le sol pour qu'il tombe en chute libre.
							int iH = 0;

							if (pPlayerActif->ObtenirSpriteCourse()->IsActif()) {
								if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0) {
									if (dComposanteX >= 0 && dComposanteX < 1366 && dComposanteY + RectTmp.h + iH >= 0 && dComposanteY + iH < 768)
										while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(int)dComposanteX + ((int)dComposanteY + RectTmp.h + iH) * m_pGameMap->ObtenirSurfaceMap()->w] == 0) && iH < 50)
											iH++;
								}
								else {
									if (dComposanteX + RectTmp.w >= 0 && dComposanteX + RectTmp.w < 1366 && dComposanteY + RectTmp.h + iH >= 0 && dComposanteY + iH < 768)
										while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(int)dComposanteX + RectTmp.w + ((int)dComposanteY + RectTmp.h + iH) * m_pGameMap->ObtenirSurfaceMap()->w] == 0) && iH < 50)
											iH++;
								}
							}

							else if (pPlayerActif->ObtenirSpriteRepos()->IsActif()) {
								if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0) {
									if (dComposanteX + 10 >= 0 && dComposanteX + 10 < 1366 && dComposanteY + RectTmp.h + iH >= 0 && dComposanteY + iH < 768)
										while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(int)dComposanteX + 10 + ((int)dComposanteY + RectTmp.h + iH) * m_pGameMap->ObtenirSurfaceMap()->w] == 0) && iH < 50)
											iH++;
								}
								else {
									if (dComposanteX + 30 >= 0 && dComposanteX + 30 < 1366 && dComposanteY + RectTmp.h + iH >= 0 && dComposanteY + iH < 768)
										while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(int)dComposanteX + 30 + ((int)dComposanteY + RectTmp.h + iH) * m_pGameMap->ObtenirSurfaceMap()->w] == 0) && iH < 50)
											iH++;
								}
							}

							// Si la distance entre le joueur et le sol est assez grande on le fait tomber en chute libre.
							if (iH == 50) {
								pPlayerActif->ModifierChuteLibreJoueur(true);
								pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								pPlayerActif->ObtenirSpriteCourse()->DefinirActif(false);
							}


						}

						else {			// Sinon, si il y a une collision...

							if (pPlayerActif->ObtenirSpriteRepos()->IsActif())
								pPlayerActif->ModifierStabiliteJoueur(true);		// Si le joueur est au repos et il touche la map, il devient stable.

							if (pPlayerActif->ObtenirSpriteSaut()->IsActif()) {
								pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantX(0);		// Si le personnage est en saut, il ne bouge plus en x mais il peut bouger en y.
								//pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY());
								//if (DommageChuteLibre(pPlayerActif, _pRenderer)) {
									//m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->Retirer(true);
									//m_pTeamList->ObtenirElementCurseur()->ObtenirListePlayer()->AllerPrecedentCurseur();
									//m_pGameMap->CreateHealthPack();
								//}
								//pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);
							}


							if (boCorps) {			// Si il y a une collision avec le corps...

								if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0)	// On rajuste sa position selon la collision.
									dComposanteX -= (RectTmp.w - _uiXCorps);					//
								else
									dComposanteX += _uiXCorps;

								if (!boPied) {			// Si il n'y a pas de collision avec le pieds...


									// On continue � faire descendre le joueur selon la gravit�.
									pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() + m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

									dComposanteY += pPlayerActif->ObtenirVecteurPoids()->ObtenirComposanteY() / 35;

									pPlayerActif->DefinirPositionY(dComposanteY);



								}
							}

							if (boPied) {		// Si il y a une collision avec le pieds...

								pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(0);		// Le joueur ne vas plus descendre.

								if (pPlayerActif->ObtenirSpriteSaut()->IsActif()) {			// Si le sprite saut est actif...
									pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);		// On va le remettre au sprite repos et annuler sa vitesse.
									pPlayerActif->ObtenirSpriteSaut()->DefinirActif(false);
									pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								}

								if (!boCorps) {			// Si il n'y a pas de collision avec le corps...


									// On v�rifie si la pente est trop grande pour monter.
									unsigned int uiH = 0;

									while ((((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[RectTmp.x + 1 + _uiXPieds + (RectTmp.y + _uiYPieds - uiH) *m_pGameMap->ObtenirSurfaceMap()->w] != 0) && (uiH <= 2))
										uiH++;

									if (uiH <= 2) {			// Si elle est assez petite...
										dComposanteY -= uiH;
										pPlayerActif->DefinirPositionX(dComposanteX);		// On fait monter le joueur.
										pPlayerActif->DefinirPositionY(dComposanteY);
									}

									else {		// Si la pente est trop grande...

										if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 0 && (_uiXPieds > RectTmp.w / 2))	// On rajuste selon la collision pour qu'il ne monte pas.
											dComposanteX -= (RectTmp.w - _uiXPieds);													//

										if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1 && (_uiXPieds < RectTmp.w / 2))
											dComposanteX += _uiXPieds;

										pPlayerActif->DefinirPositionX(dComposanteX);
										pPlayerActif->DefinirPositionY(dComposanteY);

									}

								}

							}
							/*
							if (!pPlayerActif->IsSliding() && !pPlayerActif->ObtenirSpriteJetPack()->IsActif()) {
								if (abs(Regression({ _uiXPieds + RectTmp.x, RectTmp.y }, RectTmp.w / 2, 2 * RectTmp.h, true)) >= 60) {
									pPlayerActif->ModifierGlissadeJoueur(true);
									pPlayerActif->ModifierChuteLibreJoueur(false);
									pPlayerActif->ObtenirSpriteCourse()->DefinirActif(false);
									pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								}
							}
							*/
						}

					}

					// Les glissades n'ont pas �t� finies � temps pour la remise, malheureusement... :(  Ummm... C'est pas moi! Umm.... C'est Xavier! :|    -Nico
					else if (pPlayerActif->IsSliding() && !pPlayerActif->ObtenirSpriteJetPack()->IsActif()) {		// Si le joueur glisse...
						pPlayerActif->ModifierChuteLibreJoueur(false);
						CVecteur2D* VecteurFrottement = new CVecteur2D(0.0f, 0.0f);
						VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps);

						//double doAngle = Regression({ RectTmp.x + RectTmp.w / 2, RectTmp.y }, RectTmp.w / 2, 2 * RectTmp.h, true); // Variable qui sert a calcul juste 1 fois l'angle.


						if (pPlayerActif->ObtenirSpriteCourse()->ObtenirEtage() == 1) { // Si le joueur se d�place vers la droite.
							double doAngle = Regression({ _uiXPieds + RectTmp.x, RectTmp.y }, RectTmp.w / 2, 2 * RectTmp.h, true); // Variable qui sert a calcul juste 1 fois l'angle.
							VecteurFrottement->ModifierVecteur(1, doAngle + 180);
							if (doAngle > 0) { // Si le joueur se d�place vers la gauche et que la pente est vers le haut � gauche.

								pPlayerActif->ObtenirVecteurVitesse()->ModifierOrientation(doAngle);
								//pPlayerActif->ObtenirVecteurPoids()->ModifierOrientation(doAngle + 180);
								pPlayerActif->ObtenirVecteurPoids()->ModifierVecteur(m_pGameMap->ObtenirGravite()->ObtenirNorme(), doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
								//VecteurFrottement->ModifierOrientation(doAngle + 180);
								*pPlayerActif->ObtenirVecteurVitesse() += *VecteurFrottement;
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() + (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35));
								pPlayerActif->DefinirPositionX(pPlayerActif->ObtenirPositionX() + (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35));

							}

							if (doAngle < 0) { // Si le joueur se d�place vers la le bas a gauche.
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
							double doAngle = Regression({ _uiXPieds + RectTmp.x, RectTmp.y }, RectTmp.w / 2, 2 * RectTmp.h, true); // Variable qui sert a calcul juste 1 fois l'angle.
							VecteurFrottement->ModifierVecteur(1, doAngle);
							if (doAngle > 0) { // Si le joueur se d�place vers la droite et que la pente est vers le bas � droite.
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

						if (pPlayerActif->ObtenirVecteurVitesse() == 0 || RectTmp.x > 1366 || RectTmp.y > 768)  { // Si la glissade est fini. IL RESTE A TUER LE JOUEUR ...... aussi il faut optimiser la glissade ...................
							pPlayerActif->ModifierGlissadeJoueur(false);
							pPlayerActif->ModifierStabiliteJoueur(true);
						}

						if (!VerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiXPieds, &_uiYPieds, &_uiXCorps, &_uiYCorps)) {
							pPlayerActif->ModifierGlissadeJoueur(false);
							pPlayerActif->ModifierStabiliteJoueur(true);
						}

					}
					
					
					else if (pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->IsActif()) {		// Si le joueur est en JetPack...

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
								pPlayerActif->ModifierTypeMouvement(1);
								pPlayerActif->ObtenirSpriteRepos()->DefinirActif(true);
								pPlayerActif->ModifierStabiliteJoueur(false);
								m_boFinTour = true;
								m_boDebutPartie = true;
								m_pToolBar->NouveauTour();
								pPlayerActif->DefinirPositionY(pPlayerActif->ObtenirPositionY() - 5);
							}

							if (pPlayerActif->ObtenirJetPack()->ObtenirMunition() <= 0) {
								pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->DefinirActif(false);
								pPlayerActif->ObtenirSpriteRepos()->DefinirEtage(pPlayerActif->ObtenirJetPack()->ObtenirSprite("")->ObtenirEtage());
								pPlayerActif->ModifierTypeMouvement(1);
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

						pPlayer = pPlayerListTmp->ObtenirElementTrieur();
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
							pPlayer->ObtenirVecteurVitesse()->ModifierComposantY(0);

							DetectionCollisionPack(pPlayer, &boExplosion, &Pointexplosion, _pRenderer);

							if (boExplosion) {
								pPlayerListTmp->RetirerTrieur(true);
								DommageExplosion(Pointexplosion, 45, _pRenderer);
								m_pGameMap->CreateHealthPack();
								m_boFinTour = true;
								m_boDebutPartie = true;
							}

							if (boExplosion)
								DommageExplosion(Pointexplosion, m_pGameMap->ObtenirPackList()->ObtenirElementCurseur()->GetRayon(), _pRenderer);



							else if (DommageChuteLibre(pPlayer, _pRenderer)) {
								pPlayerListTmp->RetirerTrieur(true);
								m_pGameMap->CreateHealthPack();
							}

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
	

	// Proc�dure permettant d'appliquer la physique sur les packs...
	// En entr�e:
	// Param1: Le renderer de la fenetre.
	void PhysiquePack(SDL_Renderer* _pRenderer) {

		CListeDC<CPack*>* pPackListTmp = m_pGameMap->ObtenirPackList(); // Obtention de la liste de packs.
		SDL_Rect RectTmp; // Posiition du pack.
		int iX, iY; // Position d'une collision.
		
		// Boucle sur tous les packs...
		for (int i = 0; i < pPackListTmp->ObtenirCompte(); i++) {

			// Si le pack n'est pas stable...
			if (!pPackListTmp->ObtenirElementCurseur()->IsStable()) {

				// Obtention de la position du pack...
				RectTmp = pPackListTmp->ObtenirElementCurseur()->GetRectDestination();

				// Si le pack est en collision dans la map
				if (CollisionObjetMapChuteLibre(pPackListTmp->ObtenirElementCurseur()->GetSurface(), RectTmp, &iX, &iY)) {

					// Repositionnement du pack selon le Y de la CollisionObjetMapChuteLibre...
					RectTmp.y = iY;
					
					// Obtention de l'angle de la pente...

					double dAngle = Regression({ iX, RectTmp.y }, RectTmp.w / 2, 2*RectTmp.h, true);
					
					pPackListTmp->ObtenirElementCurseur()->ModifierAnlge(dAngle);
					pPackListTmp->ObtenirElementCurseur()->ModifierStabilePack(true);
					

					// Modification de sa position...
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
				}
				else {
					// Ajout de la gravit� si pas de collision...
					RectTmp.y += m_pGameMap->ObtenirGravite()->ObtenirComposanteY();
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
				}
			}

			pPackListTmp->AllerSuivantCurseur(); // Changement de packc.
		}
	}

	// Proc�dure appliquant la physique sur les projectiles...
	void PhysiqueTool(SDL_Renderer* _pRenderer) {

		// 0:Missile 1:Grenade 2:Melee
		if (m_pToolBar->ObtenirPositionObjetDoubleClick() <= 2) {

			// Pointeur sur le projectile...
			CProjectile* pProjectileTmp = m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirProjectile(m_pToolBar->ObtenirPositionObjetDoubleClick());

			// Initialisation de la position et du vecteur pour ne pas aller les chercher � r�p�tition...
			SDL_Rect* RectTmp = pProjectileTmp->ObtenirRectDestination();
			CVecteur2D* pVecteurVitesse = pProjectileTmp->ObtenirVecteurVitesse();
			int iX, iY; // Position d'une collision.
			float fangle; // Un angle...

			// Si le tour peut se finir...
			if (!pProjectileTmp->ObtenirSprite("")->IsActif() && pProjectileTmp->ExplosionEnCours()) {
				
				// On enl�ve les joueurs qui sont mort durant le processus...
				m_pTeamList->AllerATrieur(0);
				for (int i = m_pTeamList->ObtenirCompte(); i > 0; i--) {

					m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->AllerATrieur(0);
					for (int j = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirCompte(); j > 0; j--) {

						if (m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirElementTrieur()->GetHealth() <= 0)
							m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->RetirerTrieur(true);
						else
							m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->AllerSuivantTrieur();
					}

					if (m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer()->ObtenirCompte() == 0)
						m_pTeamList->RetirerTrieur(true);
					else
						m_pTeamList->AllerSuivantTrieur();
				}
				
				// Remet le joeur normal, ainsi que le projectile...
				m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
				pProjectileTmp->DefinirExplosion(false);
				pProjectileTmp->ObtenirSprite("")->DefinirActif(false);
				m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteRepos()->DefinirActif(true);
				
				// Tous les joueurs sont affect� par la physique.
				m_boDebutPartie = true;
				m_boFinTour = true;
				m_pToolBar->NouveauTour();
			}
			else {

				// Si un missile est lanc�...
				if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 0 && pProjectileTmp->EstLancer())  {

					// s'il y a une collision avec le missile...
					if (CollisionMissile(pProjectileTmp->ObtenirSurface(), *pProjectileTmp->ObtenirRectDestination(), &iX, &iY, _pRenderer)) {

						// R�action � la collision et dommage...
						pProjectileTmp->ReactionExplosion(pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY);
						DommageExplosion({ pProjectileTmp->ObtenirRectDestination()->x + iX, pProjectileTmp->ObtenirRectDestination()->y + iY }, 50, _pRenderer);
					}

					// Si le missile sort de l'�cran, sauf vers le haut...
					else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768) {
						pProjectileTmp->ReactionExplosion(-100, -100);
					}

					// Changement de la position du missile...
					RectTmp->x += pVecteurVitesse->ObtenirComposanteX() / 35;
					RectTmp->y += pVecteurVitesse->ObtenirComposanteY() / 35;

					// Addition de vecteur...
					*pVecteurVitesse += *m_pGameMap->ObtenirGravite();
					*pVecteurVitesse += *m_pGameMap->ObtenirVecteurVent();

					// Changement de l'angle du missile
					if (pVecteurVitesse->ObtenirComposanteY() < 0 && pVecteurVitesse->ObtenirComposanteX() >= 0)
						pProjectileTmp->DefinirAngle((180 / M_PI) * atanf(((-(float)pVecteurVitesse->ObtenirComposanteY()) / ((float)pVecteurVitesse->ObtenirComposanteX()))));

					if (pVecteurVitesse->ObtenirComposanteY() >= 0 && pVecteurVitesse->ObtenirComposanteX() < 0)
						pProjectileTmp->DefinirAngle(180 + (180 / M_PI) * atanf((((float)pVecteurVitesse->ObtenirComposanteY()) / (-(float)pVecteurVitesse->ObtenirComposanteX()))));

					if (pVecteurVitesse->ObtenirComposanteY() < 0 && pVecteurVitesse->ObtenirComposanteX() < 0)
						pProjectileTmp->DefinirAngle(180 - (180 / M_PI) * atanf(((-(float)pVecteurVitesse->ObtenirComposanteY()) / (-(float)pVecteurVitesse->ObtenirComposanteX()))));

					if (pVecteurVitesse->ObtenirComposanteY() >= 0 && pVecteurVitesse->ObtenirComposanteX() >= 0)
						pProjectileTmp->DefinirAngle(360 - (180 / M_PI) * atanf((((float)pVecteurVitesse->ObtenirComposanteY()) / ((float)pVecteurVitesse->ObtenirComposanteX()))));

				}

				// Si une grenade est lanc�e...
				else if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 1 && pProjectileTmp->EstLancer()) {

					bool boSensRotation;

					// AutoDesctruction de la grenade...
					if (pProjectileTmp->ReactionExplosion(0, 0)) {
						RectTmp = pProjectileTmp->ObtenirRectDestination();
						DommageExplosion({ RectTmp->x + pProjectileTmp->ObtenirRayonDommage(), RectTmp->y + pProjectileTmp->ObtenirRayonDommage() }, pProjectileTmp->ObtenirRayonDommage(), _pRenderer);

					}
					// Grenade en dehors de la map...
					else if (RectTmp->x >= 1366 || RectTmp->x + RectTmp->w <= 0 || RectTmp->y >= 768) {
						pProjectileTmp->ReinitialisationProjectile();
						m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->DefinirToolActif(false);
						m_boDebutPartie = true;
						m_boFinTour = true;
						m_pToolBar->NouveauTour();
					}
					else
					{
						// Ajout de l'influence de la gravit� au vecteur...
						*pVecteurVitesse += *m_pGameMap->ObtenirGravite();

						// Sauvergarde de l'ancienne position de la grenade...
						int x = RectTmp->x;
						int y = RectTmp->y;

						// Modification de la position de la grenade...
						RectTmp->x += pVecteurVitesse->ObtenirComposanteX() / 35;
						RectTmp->y += pVecteurVitesse->ObtenirComposanteY() / 35;

						// R�ajustament pour arrondissement n�gatif fait pas l'ordinateur...
						if (pVecteurVitesse->ObtenirComposanteX() < 0)
							RectTmp->x++;
						if (pVecteurVitesse->ObtenirComposanteY() < 0)
							RectTmp->y++;

						// V�rification de la collision avec la map...
						if (CollisionObjetMap(pProjectileTmp->ObtenirSurface(), *RectTmp, nullptr, nullptr)) {
							// Retour � la position d'avant...
							RectTmp->x = x;
							RectTmp->y = y;
							float fangle;
							// Repositionnement de la grenade, pixel par pixel...
							RepositionnementObjetMap(pProjectileTmp->ObtenirSurface(), pVecteurVitesse, RectTmp, &iX, &iY);
							// Variable X qui donne n'importe quoi...
							if (abs(iX) >= RectTmp->w)
								iX = RectTmp->w / 2;

							// �valuation de la pente(dessus ou dessous)...
							if (pVecteurVitesse->ObtenirComposanteY() >= 0 || iY >= RectTmp->h) {
								fangle = Regression({ RectTmp->x + iX, RectTmp->y }, RectTmp->w / 2, 2 * RectTmp->h, true);
							}
							else
							{
								fangle = Regression({ RectTmp->x + iX, RectTmp->y + RectTmp->h }, RectTmp->w / 2, -2 * RectTmp->h, false);
							}

							// Modification du vecteur par rapport au rebond si il est assez puissant pour rebondir soit jusqu'� 35...
							if (pVecteurVitesse->ObtenirNorme() >= 35) {
								CVecteur2D VecteurNormal;
								// Rebond par rapport au dessus ou au dessous...
								if (pVecteurVitesse->ObtenirComposanteY() >= 0 || iY >= RectTmp->h)
									VecteurNormal = CVecteur2D(1, fangle + 90);
								else
									VecteurNormal = CVecteur2D(1, fangle - 90);
								double dScalaire = 2 * pVecteurVitesse->Scalaire(VecteurNormal.ObtenirComposanteX(), VecteurNormal.ObtenirComposanteY());
								*pVecteurVitesse -= VecteurNormal * dScalaire;
								*pVecteurVitesse = *pVecteurVitesse * 0.5;
							}
							else
							{
								// Si l'angle de la pente est plus grand que 5 degr�s, il y aura roulade...
								if (abs(fangle) >= 5) {
									pVecteurVitesse->ModifierVecteur(pVecteurVitesse->ObtenirNorme(), fangle + 180);
								}
								// Angle inf�rieur � 5, le grenade arr�te
								else {
									pVecteurVitesse->ModifierVecteur(0, fangle);
								}
							}

							// Ajustemet du sens de rotation de la grenade...
							if (pVecteurVitesse->ObtenirComposanteX() >= 0) {
								boSensRotation = true; // Horraire...
							}
							else
							{
								boSensRotation = false; // Anti-Horraire...
							}
							if (pVecteurVitesse->ObtenirComposanteY() <= 0) {
								(boSensRotation) = !(boSensRotation);
							}

							// Rotation horraire, vitesse angulaire �gal � la vitesse de d�placement de la grenade...
							if (boSensRotation) {
								pProjectileTmp->DefinirRotation(pVecteurVitesse->ObtenirNorme());
							}
							else
								pProjectileTmp->DefinirRotation(-pVecteurVitesse->ObtenirNorme());
						}
					}
				}

				// Si c'est l'arme de m�l�e
				else if (m_pToolBar->ObtenirPositionObjetDoubleClick() == 2 && pProjectileTmp->EstLancer()) {

					// R�action lors de l'utilisation et dommage...
					pProjectileTmp->ReactionExplosion(0, 0);
					DommageMelee(m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif(), _pRenderer);

				}
			}
		}
	}

	// Regression bas
	// En entr�e:
	// Param1: Position de d�part de l'�valutation de la pente.
	// Pr�f�rences: Partir le x au point de collision et le y en haut du point, habituellement le y du rect.
	// Param2: Limite de l'�valutation de la pente (Mettre la moiti� de la largeur du rect).
	// Param3: Limite de l'�valuation de la pente en y. (Mettre 2 * le hauteur du Rect).
	// Param4: Direction de l'objet(True = vers le bas(chute libre habituellement) et False = vers le haut).
	// Retour: l'angle de la pent(Une pent montante donne un angle n�gatif).
	double Regression(SDL_Point _StartPoint, unsigned int _uiLimitX, unsigned int _uiLimiteY, bool boDirection) {
		_uiLimiteY += _StartPoint.y - 1; // Limite en y...
		unsigned short int usiGauche = 1; // D�placement vers la gauche...
		unsigned short int usiDroit = 1; // D�placement vers la droite...
		bool boPremiere = true; // Premiere fois...
		bool boPremierePenteDiffZero = false; // Premi�re fois pente diff�rente de z�ro
		double dPente = 0; // Valeur de la pente...
		double dNouvellePente = 0; // Valeur de la nouvelle pente calcul�e
		SDL_Point PointGauche; // Point de rep�re gauche pour calculer la pente
		SDL_Point PointDroit; // Point de rep�re droit pour calculer la pente
		SDL_Surface* pSurfaceMap = m_pGameMap->ObtenirSurfaceMap(); // Surface de la map

		// Position de la map par rapport � l'objet.
		if (boDirection) {

			// Tant que la diff�rence entre la nouvelle pente et l'ancienne <= tol et que j'e n'ai pas atteint la limite...
			while ((abs(dPente - dNouvellePente) <= abs(dPente) / 2 || !boPremierePenteDiffZero) && usiGauche <= _uiLimitX) {

				// Pas la premi�re fois...
				if (!boPremiere) {
					dPente = (dPente + dNouvellePente) / 2;
				}
				// Initialisation des points
				PointGauche = { _StartPoint.x - usiGauche, _StartPoint.y };
				PointDroit = { _StartPoint.x + usiDroit, _StartPoint.y };

				// Descente pointgauche...
				while ((((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == TRANSPARENCE32BIT) && PointGauche.y <= _uiLimiteY) {
					PointGauche.y++;
				}

				// Descente pointdroit...
				while ((((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == TRANSPARENCE32BIT) && PointDroit.y <= _uiLimiteY) {
					PointDroit.y++;
				}
				// Calcul de la nouvelle pente
				dNouvellePente = ((double)PointDroit.y - (double)PointGauche.y) / ((double)PointDroit.x - (double)PointGauche.x);
				if (dNouvellePente != 0)
					boPremierePenteDiffZero = true;
				usiGauche++;
				usiDroit++;
				// Si c'est la premi�re fois avec une pente diff�rente de 0...
				if (boPremiere && boPremierePenteDiffZero) {
					dPente = dNouvellePente;
					boPremiere = false;
				}
			}
		}
		else
		{
			// Tant que la diff�rence entre la nouvelle pente et l'ancienne <= tol et que j'e n'ai pas atteint la limite...
			while ((abs(dPente - dNouvellePente) <= abs(dPente) / 2 || !boPremierePenteDiffZero) && usiGauche <= _uiLimitX) {

				// Pas la premi�re fois...
				if (!boPremiere) {
					dPente = (dPente + dNouvellePente) / 2;
				}
				// Initialisation des points
				PointGauche = { _StartPoint.x - usiGauche, _StartPoint.y };
				PointDroit = { _StartPoint.x + usiDroit, _StartPoint.y };

				// Mont�e pointgauche...
				while ((((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointGauche.y * pSurfaceMap->w + PointGauche.x] == TRANSPARENCE32BIT) && PointGauche.y >= _uiLimiteY) {
					PointGauche.y--;
				}

				// Mont�e pointdroit...
				while ((((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == 0 || ((unsigned int*)pSurfaceMap->pixels)[PointDroit.y * pSurfaceMap->w + PointDroit.x] == TRANSPARENCE32BIT) && PointDroit.y >= _uiLimiteY) {
					PointDroit.y--;
				}
				// Calcul de la nouvelle pente
				dNouvellePente = ((double)PointDroit.y - (double)PointGauche.y) / ((double)PointDroit.x - (double)PointGauche.x);
				if (dNouvellePente != 0)
					boPremierePenteDiffZero = true;
				usiGauche++;
				usiDroit++;
				// Si c'est la premi�re fois avec une pente diff�rente de 0...
				if (boPremiere && boPremierePenteDiffZero) {
					dPente = dNouvellePente;
					boPremiere = false;
				}
			}
		}

		return (180 / M_PI) * atanf(dPente);
	}
	
	// Proc�dure d�terminant la position d'une collision entre un objet et la map, si il y en a une, � partir du point au milieu, bas de l'objet.
	// En entr�e:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X  de la collision qui sera retourn�.
	// Param4: La position en Y de la collision qui sera retourn�.
	// Retour True s'il y a eu une collision...
	bool CollisionObjetMapChuteLibre(SDL_Surface* _pSurfaceObjet, SDL_Rect _RectDestinationObjet, int* _iX, int* _iY) {

		// Objet dans la map...
		if (_RectDestinationObjet.x < 1366 && _RectDestinationObjet.x + _RectDestinationObjet.w >= 0 && _RectDestinationObjet.y < 768 && _RectDestinationObjet.y + _RectDestinationObjet.h >= 0) {
			// Positionnement au milieu, bas...
			SDL_Point PointCollisionPixelObjet = { _RectDestinationObjet.w / 2, _RectDestinationObjet.h - 1 };

			// Boucle jusqu'� ce qu'on ait atteint un pixel plein dans la surface...
			while ((((unsigned int*)_pSurfaceObjet->pixels)[PointCollisionPixelObjet.y * _pSurfaceObjet->w + PointCollisionPixelObjet.x] == 0 || ((unsigned int*)_pSurfaceObjet->pixels)[PointCollisionPixelObjet.y * _pSurfaceObjet->w + PointCollisionPixelObjet.x] == TRANSPARENCE32BIT) && PointCollisionPixelObjet.y >= 1) {
				PointCollisionPixelObjet.y--;
			}

			bool boCollision = false;
			// Initialisation de la position de la pixel dans la map...
			SDL_Point PointCollisionPixelMap = { _RectDestinationObjet.x + PointCollisionPixelObjet.x, _RectDestinationObjet.y + PointCollisionPixelObjet.y };
			// Tant que je suis dans les pixels non transparents de la map jusqu'� ...
			while (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(PointCollisionPixelMap.y)* m_pGameMap->ObtenirSurfaceMap()->w + PointCollisionPixelMap.x] != 0 && ((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(PointCollisionPixelMap.y)* m_pGameMap->ObtenirSurfaceMap()->w + PointCollisionPixelMap.x] != TRANSPARENCE32BIT) {
				// Il y a une collision et je remonte l'objet
				boCollision = true;
				PointCollisionPixelMap.y--;
			}
			// �tablissement du premier point de collision...
			*_iX = PointCollisionPixelMap.x;
			*_iY = PointCollisionPixelMap.y - PointCollisionPixelObjet.y;
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

		if (_iX != nullptr)
			*_iX = 0;
		if (_iY != nullptr)
			*_iY = 0;

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

						// V�rifier Transparence
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
	
	// Fontion d�terminant s'il y a une collison avac le missile.
	// En entr�e:
	// Param1: La surface du missile.
	// Param2: La destination du missile.
	// Param3: La position en X qui sera retourn�.
	// Param4: La position en Y qui sera retourn�.
	// Param5: Le renderer de la fen�tre pour afficher le d�gats.
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

		// Pour toutes les �quipes...
		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			// Obtient la liste des joueurs des �quipes.
			pPlayerListTmp = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();

			// Pour tous les joeurs...
			pPlayerListTmp->AllerATrieur(0);
			for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

				// On obtient les informations du joueur (Destination, source, et sa surface).
				RectPlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirRectDestination();
				RectSourcePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirRectSource();
				pSurfacePlayer = pPlayerListTmp->ObtenirElementTrieur()->ObtenirSpriteRepos()->ObtenirSurface();

				// Si le missile est � gauche du joueur, partiellement dans le joueur en x et au moins dans le joueur en y.
				if (_RectDestination.x + _RectDestination.w >= RectPlayer.x && _RectDestination.x < RectPlayer.x && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = 0;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = RectPlayer.x - _RectDestination.x;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}

				// Si le missile est compl�tement dans le joueur...
				else if (_RectDestination.x >= RectPlayer.x && _RectDestination.x + _RectDestination.w <= RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du missile.
					xPlayer = _RectDestination.x - RectPlayer.x;
					yPlayer = _RectDestination.y - RectPlayer.y;
					xMissile = 0;
					yMissile = 0;
					FinX = _RectDestination.w;
					FinY = _RectDestination.h;
					boCollsionJoeur = true;
				}
				
				// Si le missile est � droite du joueur, partiellement dans le joueur en x et au moins dans le joueur en y. 
				else if (_RectDestination.x <= RectPlayer.x + RectPlayer.w && _RectDestination.x + _RectDestination.w > RectPlayer.x + RectPlayer.w && _RectDestination.y >= RectPlayer.y && _RectDestination.y + _RectDestination.h <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du missile.
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

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du missile.
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

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du missile.
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
					
					// Pour toute la hauteur du missile qui est � v�rifier...
					for (; yMissile < FinY; yPlayer++, yMissile++) {
						
						// Pour toute la largeur du missile qu'il faut regarder...
						for (XPlayer = xPlayer, XMissile = xMissile; XMissile < FinX; XPlayer++, XMissile++) {

							// Si la surface du joueur n'est pas � transparente au point yPlayer et XPlayer.
							if (((unsigned int*)pSurfacePlayer->pixels)[(yPlayer + RectSourcePlayer.y) * pSurfacePlayer->w + (XPlayer + RectSourcePlayer.x)] != 0)

								// Si la surface du missile n'est pas � transparente au point yMissile et XMissile.
								if (((unsigned int*)_pSurfaceMissile->pixels)[(yMissile)* _pSurfaceMissile->w + (XMissile)] != 0) {

									*_iX = xPlayer; // Initialise la valeur de sorie en x.
									*_iY = yPlayer; // Initialise la valeur de sorie en y.
									
									// Cr�e un label avec le dommage inflig� au joueur.
									strDommage.append("-");
									strDommage.append(SDL_itoa(pPlayerListTmp->ObtenirElementTrieur()->GetHealth() * 100, chrTmp, 10));
									m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { RectPlayer.x, RectPlayer.y - 20, 0, 0 }, { RectPlayer.x, RectPlayer.y - 80, 0, 0 }, 30, 2000, _pRenderer));
									pPlayerListTmp->ObtenirElementTrieur()->SetHealth(0);

									return true;

								}

						}
					}
				}

			pPlayerListTmp->AllerSuivantTrieur(); // Prochain joueur...
			}

			m_pTeamList->AllerSuivantTrieur(); // Prochaine �quipe.
		}


		// Regarde s'il y a une collision avec la map...
		return CollisionObjetMap(_pSurfaceMissile, _RectDestination, _iX, _iY);
	}

	// Proc�dure permettant de positionner un objet dans la map, en avan�ant, pixel par pixel, jusqu'� ce qu'il y ait une collision o� que le vecteur ait �t� appliqu�...
	// En entr�e:
	// Param1: Surface de l'objet.
	// Param2: Vecteur de l'objet.
	// Param3: Position de l'objet qui sera modifi�e.
	// Param4: Application du sens de rotation de l'objet selon le vecteur.
	void RepositionnementObjetMap(SDL_Surface* _pSurfaceGrenade, CVecteur2D* _pVecteurGrenade, SDL_Rect* _RectDestinationGrenade, int* _iX, int* _iY) {
		
		// Initialisation des limites d'incr�mentation de la position de l'objet...
		int iIncrementationX = 0;
		int iIncrementationY = 0;
		int iVecteurX = _pVecteurGrenade->ObtenirComposanteX()/35;
		int iVecteurY = _pVecteurGrenade->ObtenirComposanteY()/35;

		// R�ajustament pour arrondissement n�gatif fait pas l'ordinateur...
		if (_pVecteurGrenade->ObtenirComposanteX() < 0)
			iVecteurX++;
		if (_pVecteurGrenade->ObtenirComposanteY() < 0)
			iVecteurY++;

		// Boucle jusqu'� ce que les vecteurs ont �t� compl�tement incr�ment�s ou qu'il y a eu une collision...
		for (int i = 0; i < abs(iVecteurX) || i < abs(iVecteurY); i++) {
			// S'il n'y a pas de collision(initialisation des variable de collision X et Y)...
			if (!CollisionObjetMap(_pSurfaceGrenade, *_RectDestinationGrenade, _iX, _iY)) {
				// Incr�mentation en X jusqu'� ce que le vecteur soit atteint...
				if (iIncrementationX < abs(iVecteurX) && iVecteurX != 0) {
					_RectDestinationGrenade->x += iVecteurX / abs(iVecteurX);
					iIncrementationX++;
				}
				// Incr�mentation en Y jusqu'� ce que le vecteur soit atteint...
				if (iIncrementationY < abs(iVecteurY) && iVecteurY != 0) {
					_RectDestinationGrenade->y += iVecteurY / abs(iVecteurY);
					iIncrementationY++;
				}
			}
			// S'il y une collision, sortie de la boucle...
			else
				i = INT_MAX - 1;
		}

		
		
	}

	// Fonction qui retourne la position d'une collision.
	// Param�tre: _pPlayer, joueur pour lequel on v�rifie les collisions avec la carte de jeu
	// Param�tre: _RectPlayer, rectangle qui encadre l'endroit ou l'on veut v�rifier les collisions sur la map. (ex: Prochain rectangle destination d'un player.)
	// Param�tre: _pboCollisionCorps, pointeur de bool�enne qui indique si il y a eu une collision au corps.
	// Param�tre: _pboCollisionPieds, pointeur de bool�enne qui indique si il y a eu une collision au pieds.
	// Param�tre: _puiXPieds, pointeur d'entier non-sign� qui indique la position en x de la collision dans le rectangle du joueur entre les pieds et la carte de jeu s'il y a lieu.
	// Param�tre: _puiYPieds, pointeur d'entier non-sign� qui indique la position en y de la collision dans le rectangle du joueur entre les pieds et la carte de jeu s'il y a lieu.
	// Param�tre: _puiXCorps, pointeur d'entier non-sign� qui indique la position en x de la collision dans le rectangle du joueur entre le corps et la carte de jeu s'il y a lieu.
	// Param�tre: _puiYCorps, pointeur d'entier non-sign� qui indique la position en y de la collision dans le rectangle du joueur entre le corps et la carte de jeu s'il y a lieu.
	// Retour: Un bool�en qui indique si il y a eu une collision.
	bool VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _pboCollisionCorps, bool* _pboCollisionPieds, unsigned int* _puiXPieds, unsigned int* _puiYPieds, unsigned int* _puiXCorps, unsigned int* _puiYCorps) {

		SDL_Surface* pTmpSDLSurfaceMap = m_pGameMap->ObtenirSurfaceMap();	// pointeur temporaire de surface qui pointe sur la surface de la carte de jeu.
		SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropri�e du joueur.

		SDL_Rect TmpSDLRectPlayerDestination = _RectPlayer;						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affich� sur la map.
		SDL_Rect TmpSDLRectPlayerSource = { 0, 0, 1, 1 };										// Structure SDL Rect qui contiendra le rectangle source de la surface appropri�e du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds = { 0, 0, 1, 1 };									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du corps.

		unsigned int uiTransparenceJoueur;
		unsigned int uiTransparenceMap;

		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de course.

			if (_pPlayer->ObtenirSpriteCourse()->ObtenirEtage() == 0)					// Si le jueur est orient� vers la droite...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
			else																		// Sinon...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	

		}

		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de saut.

			if (_pPlayer->ObtenirSpriteSaut()->ObtenirEtage() == 0)					// Si le jueur est orient� vers la droite...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
			else																		// Sinon...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	

		}

		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds appropri� pour le sprite de parachute.

			if (_pPlayer->ObtenirSpriteParachute()->ObtenirEtage() == 0)					// Si le jueur est orient� vers la droite...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
			else																		// Sinon...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	

		}

		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds appropri� pour le sprite de repos.

			if (_pPlayer->ObtenirSpriteRepos()->ObtenirEtage() == 0)					// Si le jueur est orient� vers la droite...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsDroite();		// On se sert du rectangle pour le corps de droite. 
			else																		// Sinon...
				TmpSDLRectPlayerHitboxCorps = _pPlayer->ObtenirHitboxCorpsGauche();		// On se sert du rectangle pour le corps de gauche.	

		}
		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {						// Si le sprite JetPack est actif...

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
																					
			TmpSDLRectPlayerHitboxCorps = { 0, 0, 40, 50 };

		}

		// Initialisation des variables...
		*_pboCollisionCorps = false;
		*_pboCollisionPieds = false;

		*_puiXPieds = 0;
		*_puiYPieds = 0;
		*_puiXCorps = 0;
		*_puiYCorps = 0;

		uiTransparenceJoueur = ((unsigned int*)pTmpSDLSurfacePlayer->pixels)[0];
		uiTransparenceMap = ((unsigned int *)pTmpSDLSurfaceMap->pixels)[0];

		// V�rification de la collision des pieds du joueur avec la carte de jeu...

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxPieds.h && !*_pboCollisionPieds; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxPieds.w && !*_pboCollisionPieds; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

				if (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x >= 0 && TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x < 1366 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y >= 0 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y < 768) {

					if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfaceMap->w] != uiTransparenceMap) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfacePlayer->w] != uiTransparenceJoueur)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents des pieds du joueur...

						*_puiXPieds = TmpSDLRectPlayerHitboxPieds.x + x;		// Dans le rectangle destination, on prend la position en x de la collision.
						*_puiYPieds = TmpSDLRectPlayerHitboxPieds.y + y;		// Dans le rectangle destination, on prend la position en y de la collision.

						*_pboCollisionPieds = true;				// On confirme la collision aux pieds.


					}
				}
			}

		}

		// V�rification de la collision du corps du joueur avec la carte de jeu.

		for (unsigned int y = 0; y < TmpSDLRectPlayerHitboxCorps.h && !*_pboCollisionCorps; y++) {				// On parcours les pixels dans le hitbox de haut en bas.

			for (unsigned int x = 0; x < TmpSDLRectPlayerHitboxCorps.w && !*_pboCollisionCorps; x++) {			// On parcours les pixels dans le hitbox de gauche � droite.

				if (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x >= 0 && TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x < 1366 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y >= 0 && TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y < 768) {

					if ((((unsigned int*)pTmpSDLSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfaceMap->w] != uiTransparenceMap) && (((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxCorps.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxCorps.y + y) * pTmpSDLSurfacePlayer->w] != uiTransparenceJoueur)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du corps du joueur...

						*_puiXCorps = TmpSDLRectPlayerHitboxCorps.x + x;		// Dans le rectangle destination, on prend la position en x de la collision.
						*_puiYCorps = TmpSDLRectPlayerHitboxCorps.y + y;		// Dans le rectangle destination, on prend la position en y de la collision.

						*_pboCollisionCorps = true;

					}
				}
			}

		}

		return (*_pboCollisionPieds || *_pboCollisionCorps);			// On retourne vrai si il y a eu une collision.


	}


	// Fonction v�rifiant s'il y a une collision entre un joueur et un pack.
	// En entr�e:
	// Param1: Le joeur...
	bool DetectionCollisionPack(CPlayer* _pPlayer, bool* _boExplosion, SDL_Point* _RectExplosion, SDL_Renderer* _pRenderer) {

		// Variables temporaire...
		CPack* pPackTmp;
		SDL_Surface* pSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropri�e du joueur.
		SDL_Rect RectPack;
		SDL_Rect RectPlayer = _pPlayer->ObtenirRectDestination();						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affich� sur la map.
		SDL_Rect RectSourcePlayer;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropri�e du joueur pour la situation.
		SDL_Rect HitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du pieds.
		SDL_Rect HitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra v�rifier les collisions du corps.
		bool boTerminerBoucle = false;
		int xPlayer, xPack, yPlayer, yPack, FinY, FinX;
		int XPlayer, XPack;
		bool boCollsionPack;
		string strDommage;
		char chrTmp[8];
		SDL_Color CouleurDommage;

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

			if (!pPackTmp->IsUse()) {

				// Si le pack est � gauche du joueur, partiellement dans le joueur en x et au moins dans le joueur en y.
				if (RectPack.x + RectPack.w >= RectPlayer.x && RectPack.x < RectPlayer.x && RectPack.y + RectPack.h >= RectPlayer.y && RectPack.y <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du pack.
					xPlayer = 0;
					yPlayer = RectPack.y - RectPlayer.y;
					xPack = RectPlayer.x - RectPack.x;
					yPack = 0;
					FinX = RectPack.w;
					FinY = RectPack.h;
					boCollsionPack = true;
				}

				// Si le pack est compl�tement dans le joueur...
				else if (RectPack.x >= RectPlayer.x && RectPack.x + RectPack.w <= RectPlayer.x + RectPlayer.w && RectPack.y >= RectPlayer.y && RectPack.y + RectPack.h <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du pack.
					xPlayer = RectPack.x - RectPlayer.x;
					yPlayer = RectPack.y - RectPlayer.y;
					xPack = 0;
					yPack = 0;
					FinX = RectPack.w;
					FinY = RectPack.h;
					boCollsionPack = true;
				}
				// Si le pack est � droite du joueur, partiellement dans le joueur en x et au moins dans le joueur en y. 
				else if (RectPack.x <= RectPlayer.x + RectPlayer.w && RectPack.x + RectPack.w > RectPlayer.x + RectPlayer.w && RectPack.y + RectPack.h >= RectPlayer.y && RectPack.y <= (RectPlayer.y + RectPlayer.h)) {

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du pack.
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
					// On d�finie o���� regarder dans le surface du joueur et dans la surface du pack.
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

					// On d�finie o���� regarder dans le surface du joueur et dans la surface du pack.
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

					// Pour toute la hauteur du pack qui est � v�rifier...
					for (; yPack < FinY; yPlayer++, yPack++) {

						// Pour toute la largeur du pack qui est � v�rifier...
						for (XPlayer = xPlayer, XPack = xPack; XPack < FinX; XPlayer++, XPack++) {

							// Si la surface du joueur n'est pas � transparente au point yPlayer et XPlayer.
							if (((unsigned int*)pSurfacePlayer->pixels)[(yPlayer + RectSourcePlayer.y) * pSurfacePlayer->w + (XPlayer + RectSourcePlayer.x)] != 0)

								// Si la surface du pack n'est pas � transparente au point yPack et XPack.
								if (((unsigned int*)pPackTmp->GetSurface()->pixels)[(yPack)* pPackTmp->GetSurface()->w + (XPack)] != 0) {

									// Si c'est un healthpack...
									if (pPackTmp->GetSpriteExplosion() == nullptr) {
										strDommage.append("+");
										strDommage.append(SDL_itoa((_pPlayer->GetHealth() * 100) * 0.25, chrTmp, 10));
										CouleurDommage = { 0, 200, 0, 255 };
									}

									// Si c'est une mine...
									else {
										strDommage.append("-");
										strDommage.append(SDL_itoa(_pPlayer->GetHealth() * 100, chrTmp, 10));
										CouleurDommage = { 200, 0, 0, 255 };
									}


									// Cr�� un label indiquant un dommage (ou un gain de sant�).
									m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, CouleurDommage, { RectPlayer.x, RectPlayer.y - 20, 0, 0 }, { RectPlayer.x, RectPlayer.y - 80, 0, 0 }, 30, 2000, _pRenderer));

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
			}

			m_pGameMap->ObtenirPackList()->AllerSuivantCurseur(); // Prochain pack.
		}

	}

	void DommageExplosion(SDL_Point _ExplosionPoint, unsigned int _uiRayon, SDL_Renderer* _pRenderer) {

		// Variable temporaires...
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

		// Pour tous les �quipes...
		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte() && m_pTeamList->ObtenirCompte() > 0; i++) {

			pPlayerList = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();

			// Pour tous les joueurs des �quipes...
			pPlayerList->AllerATrieur(0);
			for (int j = 0; j < pPlayerList->ObtenirCompte() && pPlayerList->ObtenirCompte() > 0; j++) {

				// Obtention du joueur...
				pPlayerTmp = pPlayerList->ObtenirElementTrieur();
				RectDestinationPlayer = pPlayerTmp->ObtenirRectDestination();

				// S'il a encore de la vie...
				if (pPlayerTmp->GetHealth() > 0) {

					// Si le joueur est � gauche de l'explosion, partiellement dans l'explosion en x et au moins dans l'explosion en y.
					if ((RectDestinationPlayer.x < RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w >= RectExplosion.x) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y && RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h)) {

						iDistanceRayon = (RectExplosion.x + _uiRayon) - (RectDestinationPlayer.x + RectDestinationPlayer.w);
						fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);

						iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

						strDomage.append("-");
						strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
						m_pListeDommage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

						pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
						pPlayerTmp->ModifierStabiliteJoueur(false);
					}

					// Si le joueur est � droite de l'explosion, partiellement dans l'explosion en x et au moins dans l'explosion en y. 
					else if ((RectDestinationPlayer.x <= RectExplosion.x + RectExplosion.w && RectDestinationPlayer.x + RectDestinationPlayer.w > RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y && RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h)) {

						iDistanceRayon = RectDestinationPlayer.x - (RectExplosion.x + _uiRayon);
						fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);

						iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

						strDomage.append("-");
						strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
						m_pListeDommage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

						pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
						pPlayerTmp->ModifierStabiliteJoueur(false);
					}

					// Si le joueur est compl�tement dans l'explosion...
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
						m_pListeDommage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

						pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
						pPlayerTmp->ModifierStabiliteJoueur(false);

					}

					// Si le joueur est en haut de l'explosion, partiellement dans l'explosion en y et au moins dans l'explosion en x.
					else if ((RectDestinationPlayer.x >= RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y < RectExplosion.y && RectDestinationPlayer.y + RectDestinationPlayer.h >= RectExplosion.y)) {

						iDistanceRayon = (RectExplosion.y + _uiRayon) - (RectDestinationPlayer.y + RectDestinationPlayer.h);
						fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);

						iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

						strDomage.append("-");
						strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
						m_pListeDommage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

						pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
						pPlayerTmp->ModifierStabiliteJoueur(false);
					}

					// Si le joueur est en bas de l'explosion, partiellement dans l'explosion en y et au moins dans l'explosion en x.
					else if ((RectDestinationPlayer.x >= RectExplosion.x && RectDestinationPlayer.x + RectDestinationPlayer.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPlayer.y <= RectExplosion.y + RectExplosion.h && RectDestinationPlayer.y + RectDestinationPlayer.h > RectExplosion.y + RectExplosion.h)) {

						iDistanceRayon = RectDestinationPlayer.y - (RectExplosion.y + _uiRayon);
						fPourcentage = ((float)iDistanceRayon / (float)_uiRayon);

						iDomage = (float)((1 - fPourcentage) * pPlayerTmp->GetHealth()) * 100;

						strDomage.append("-");
						strDomage.append(SDL_itoa(iDomage, chrTmp, 10));
						m_pListeDommage->AjouterFin(new CTemporaryLabel(strDomage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));

						pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * fPourcentage);
						pPlayerTmp->ModifierStabiliteJoueur(false);
					}
				}
				pPlayerList->AllerSuivantTrieur();
			}
			m_pTeamList->AllerSuivantTrieur();
		}

		// On obtient la liste des packs...
		pPackList = m_pGameMap->ObtenirPackList();

		// Pour tous les packs...
		pPackList->AllerDebut();
		for (int i = 0; i < pPackList->ObtenirCompte(); i++) {

			// Obtenrion des packs...
			pPackTmp = pPackList->ObtenirElementCurseur();
			RectDestinationPack = pPackTmp->GetRectDestination();

			// Si le pack n'est pas utilis�...
			if (!pPackTmp->IsUse()) {

				// Si le pack est � gauche de l'explosion, partiellement dans l'explosion en x et au moins dans l'explosion en y.
				if ((RectDestinationPack.x < RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w >= RectExplosion.x) && (RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y && RectDestinationPack.y <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, pPackTmp->GetRayon(), _pRenderer);
				}

				// Si le pack est � droite de l'explosion, partiellement dans l'explosion en x et au moins dans l'explosion en y. 
				else if ((RectDestinationPack.x <= RectExplosion.x + RectExplosion.w && RectDestinationPack.x + RectDestinationPack.w > RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y && RectDestinationPack.y <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, pPackTmp->GetRayon(), _pRenderer);
				}

				// Si le pack est compl�tement dans l'explosion...
				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y >= RectExplosion.y && RectDestinationPack.y + RectDestinationPack.h <= RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, pPackTmp->GetRayon(), _pRenderer);
				}

				// Si le pack est en haut de l'explosion, partiellement dans l'explosion en y et au moins dans l'explosion en x.
				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y < RectExplosion.y && RectDestinationPack.y + RectDestinationPack.h >= RectExplosion.y)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, pPackTmp->GetRayon(), _pRenderer);
				}

				// Si le pack est en bas de l'explosion, partiellement dans l'explosion en y et au moins dans l'explosion en x.
				else if ((RectDestinationPack.x >= RectExplosion.x && RectDestinationPack.x + RectDestinationPack.w <= RectExplosion.x + RectExplosion.w) && (RectDestinationPack.y <= RectExplosion.y + RectExplosion.h && RectDestinationPack.y + RectDestinationPack.h > RectExplosion.y + RectExplosion.h)) {

					pPackTmp->Use(nullptr);
					SDL_Point PointTmp = { RectDestinationPack.x + RectDestinationPack.w / 2, RectDestinationPack.y + RectDestinationPack.h / 2 };
					DommageExplosion(PointTmp, pPackTmp->GetRayon(), _pRenderer);
				}
			}
			pPackList->AllerSuivantCurseur(); // Prochain pack
		}
		
	}

	// Fonction infligeant un dommage � un joeur qui �tai en chute libre...
	// En entr�e:
	// Param1: Le joueur...
	// Param2: Le renderer de la fen�tre.
	// En sortie: Si le joueur est mort.
	bool DommageChuteLibre(CPlayer* _pPlayer, SDL_Renderer* _pRenderer) {
		
		// Nombre de pixel par d�placememnt...
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
				m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 20, 0, 0 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
			}

			// Si le dommage tu le joueur...
			else {
				strDommage.append(SDL_itoa(_pPlayer->GetHealth() * 100, chrTmp, 10));
				_pPlayer->SetHealth(0);
				m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 20, 0, 0 }, { _pPlayer->ObtenirPositionX(), _pPlayer->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
				return true;
			}
		}

		return false;
	}

	CMap* ObtenirMap(void) {
		return m_pGameMap;
	}

	CTeam* ObtenirTeamActive() {
		return m_pTeamList->ObtenirElementCurseur();
	}

	CListeDC<CTeam*>* ObtenirListeTeam(void) {
		return m_pTeamList;
	}

	// Inversement de l'affichage de la toolbar...
	void ReverseShowToolBar() {
		m_pToolBar->ReverseboShow();
	}

	bool IsAllTeamStable(void) {

		// Boucle sur les teams...
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			// Si tous les players ne sont pas stables
			if (!m_pTeamList->ObtenirElementCurseur()->IsAllPlayerStable())
				return false;

			m_pTeamList->AllerSuivantCurseur();
		}

		return true;
	}

	// Proc�dure infligeant un dommage avec l'arme de m�l�e...
	bool DommageMelee(CPlayer* _pPlayerActif, SDL_Renderer* _pRenderer) {

		// Variable temporaire...
		CListeDC<CPlayer*>* pPlayerListTmp;
		CPlayer* pPlayerTmp;
		string strDommage;
		char chrTmp[8];

		strDommage.append("-");

		// Pour toutes les �quipes...
		m_pTeamList->AllerATrieur(0);
		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			pPlayerListTmp = m_pTeamList->ObtenirElementTrieur()->ObtenirListePlayer();
			pPlayerListTmp->AllerATrieur(0);

			// Pour tous les joueurs...
			for (int j = 0; j < pPlayerListTmp->ObtenirCompte(); j++) {

				// Obtentions du joueur...
				pPlayerTmp = pPlayerListTmp->ObtenirElementTrieur();

				// Si ce n'est pas le joueur actif...
				if (_pPlayerActif != pPlayerTmp) {

					// S'il est vers la droite...
					if (_pPlayerActif->ObtenirSpriteRepos()->ObtenirEtage() == 0) {

						// S'il y a une collision avec le joueur actif, il a du dommage
						if (_pPlayerActif->ObtenirRectDestination().x + _pPlayerActif->ObtenirRectDestination().w >= pPlayerTmp->ObtenirRectDestination().x && _pPlayerActif->ObtenirRectDestination().x + _pPlayerActif->ObtenirRectDestination().w < pPlayerTmp->ObtenirRectDestination().x + pPlayerTmp->ObtenirRectDestination().w) {

							if (_pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->y + _pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->h / 2 >= pPlayerTmp->ObtenirRectDestination().y &&_pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->y + _pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->h / 2 <= pPlayerTmp->ObtenirRectDestination().y + pPlayerTmp->ObtenirRectDestination().h) {
								strDommage.append(SDL_itoa(_pPlayerActif->ObtenirProjectile(2)->ObtenirDommage() * 100, chrTmp, 10));
								m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
								pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() - _pPlayerActif->ObtenirProjectile(2)->ObtenirDommage());
								if (pPlayerTmp->GetHealth() <= 0)
									pPlayerListTmp->RetirerTrieur(true);

								return true;
							}

						}
					}

					// S'il est vers la gauche...
					else {

						// S'il y a une collision avec le joueur actif, il a du dommage
						if (_pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->x <= pPlayerTmp->ObtenirRectDestination().x + pPlayerTmp->ObtenirRectDestination().w && _pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->x > pPlayerTmp->ObtenirRectDestination().x) {

							if (_pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->y + _pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->h / 2 >= pPlayerTmp->ObtenirRectDestination().y &&_pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->y + _pPlayerActif->ObtenirProjectile(2)->ObtenirRectDestination()->h / 2 <= pPlayerTmp->ObtenirRectDestination().y + pPlayerTmp->ObtenirRectDestination().h) {
								strDommage.append(SDL_itoa(_pPlayerActif->ObtenirProjectile(2)->ObtenirDommage() * 100, chrTmp, 10));
								m_pListeDommage->AjouterFin(new CTemporaryLabel(strDommage, { 200, 0, 0, 255 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 20, 0, 0 }, { pPlayerTmp->ObtenirPositionX(), pPlayerTmp->ObtenirPositionY() - 80, 0, 0 }, 30, 2000, _pRenderer));
								pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() - _pPlayerActif->ObtenirProjectile(2)->ObtenirDommage());
								if (pPlayerTmp->GetHealth() <= 0)
									pPlayerListTmp->RetirerTrieur(true);

								return true;
							}

						}
					}

				}
				pPlayerListTmp->AllerSuivantTrieur();
			}
			m_pTeamList->AllerSuivantTrieur();
		}
	}
	
	bool IsDebut() {
		return m_boDebutPartie;
	}
	                                                                              
};
