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

	CGame(string _strEmplacementMap, CGestionaire<TTF_Font*>* _pGestionnaireFont, CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, int _iNombreÉquipe, int _iNombreJoueur, CVent* _pVent, bool _VerifierCollisionJoueurMap(CPlayer* _pPlayer, SDL_Rect _RectPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle), SDL_Renderer* _pRenderer) {

		m_boDebutPartie = true;

		m_pVerifierCollisionJoueurMap = _VerifierCollisionJoueurMap;

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, _pGestionnaireSurface, { 0, 0, 0, 0 }, _pVent, _pRenderer, _MapDestruction, _CollisionObjetMap);

		m_pTimerPhysique = new CTimer(20);

		for (int i = _iNombreÉquipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementMap, _pRenderer, _pGestionnaireFont, _pGestionnaireSurface, _pGestionnaireTexture, i, _iNombreJoueur, _MapDestruction, _CollisionObjetMap, _Physique, _Rotation));
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
		PhysiquePack();

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {
			m_pTeamList->ObtenirElementCurseur()->ShowTeam(_pRenderer);
			m_pTeamList->AllerSuivantCurseur();
		}

		m_pToolBar->ShowToolBar(_pRenderer);

		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ShowDescription(_pRenderer);
	}

	void ReactToEvent(SDL_Event* _pEvent) {

		m_pToolBar->ReactToEvent(_pEvent, m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif());

		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ReactToEvent(_pEvent, m_pToolBar->ObtenirPositionObjetDoubleClick());
		
		m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->UpdateDescription(m_pToolBar->ObtenirPositionObjetSuvol(), m_pToolBar->ObtenirRectPositionSouris());
	}

	void PhysiquePlayer(void) {

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
				unsigned int _uiX;
				unsigned int _uiY;
				SDL_Rect RectExplosion;
				pPlayerActif->ObtenirVecteurPoids()->ModifierComposantY(m_pGameMap->ObtenirGravite()->ObtenirComposanteY());

				if (pPlayerActif->IsFreeFalling()) {
					*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
					dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
					dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

					if (pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() == 0)
						pPlayerActif->ModifierChuteLibreJoueur(false);
				}

				if (pPlayerActif->IsMoving()) {

					*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
					dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
					dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

				}

				*pPlayerActif->ObtenirVecteurVitesse() += *pPlayerActif->ObtenirVecteurPoids();
				dComposanteX += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteX() / 35;
				dComposanteY += pPlayerActif->ObtenirVecteurVitesse()->ObtenirComposanteY() / 35;

				//	double angle = RegressionLineaire(pPlayerActif->ObtenirHitboxPieds(), pPlayerActif->ObtenirRectDestination());

				RectTmp.x = dComposanteX;
				RectTmp.y = dComposanteY;
				DetectionCollisionPack(pPlayerActif, &boExplosion, &RectExplosion);
				if (boExplosion) {

					DomageExplosion(RectExplosion, 45);
				}


				else if (!m_pVerifierCollisionJoueurMap(pPlayerActif, RectTmp, &boCorps, &boPied, &_uiX, &_uiY)) {
					pPlayerActif->DefinirPositionX(dComposanteX);
					pPlayerActif->DefinirPositionY(dComposanteY);
				}

				else {

					if (boCorps && pPlayerActif->ObtenirSpriteRepos()->ObtenirEtage() == 0)
						dComposanteX -= (RectTmp.w - _uiX);

					if (boCorps && pPlayerActif->ObtenirSpriteRepos()->ObtenirEtage() == 1)
						dComposanteX += (pPlayerActif->ObtenirHitboxCorpsGauche().w - _uiX);

					if (boPied && !boCorps)
						dComposanteY -= (RectTmp.h - _uiY);

					pPlayerActif->DefinirPositionX(dComposanteX);
					pPlayerActif->DefinirPositionY(dComposanteY);
					pPlayerActif->ObtenirVecteurVitesse()->ModifierComposantY(0);
					pPlayerActif->ModifierStabiliteJoueur(true);
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
								pPlayer->DefinirPositionY(RectPlayer.y);

							else {

								bool boExplosion;
								SDL_Rect RectTmp;

								RectPlayer.y -= (RectPlayer.h - _uiYMap);
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
							if (!m_pVerifierCollisionJoueurMap(pPlayer, RectPlayer, &_boCorps, &_boPieds, &_uiXMap, &_uiYMap)) {
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
				RectTmp.y += m_pGameMap->ObtenirGravite()->ObtenirComposanteY();

				if (CollisionObjetMap(pPackListTmp->ObtenirElementCurseur()->GetSurface(), RectTmp, &iX, &iY)) {

					RectTmp.y -= RectTmp.h - iY;
					pPackListTmp->ObtenirElementCurseur()->ModifierStabilePack(true);

					pPackListTmp->ObtenirElementCurseur()->ModifierAnlge(RegressionLineaire({0,0,RectTmp.w,RectTmp.h}, RectTmp));
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
				}

				else
					pPackListTmp->ObtenirElementCurseur()->ModifierPosition(RectTmp);
			}

			pPackListTmp->AllerSuivantCurseur();
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
	float RegressionLineaire(SDL_Rect _RectPiedJoueur, SDL_Rect _RectJoueur) {

		float fPente = 0;
		float iCov = 0; // Variable en y moyenne.
		float iVar = 0; // Variable en x moyen.
		float fX = 0; // Valeur en x pour la régression.
		float fY = 0; // Valeur en y pour la régression.
		int iN = 0; // Le nombre de fois qu'il y a des "différent de transparent" Sert a savoir le milieu de la régressuion
		int* iTableau = new int[_RectPiedJoueur.w, _RectPiedJoueur.h]; // Tableau.
		for (int j = 0; j < _RectPiedJoueur.h; j++) { // Boucler sur toute le rect du pied dans la position de la map.

			float fPente = 0;
			float iCov = 0; // Variable en y moyenne.
			float iVar = 0; // Variable en x moyen.
			float fX = 0; // Valeur en x pour la régression.
			float fY = 0; // Valeur en y pour la régression.
			int iN = 0; // Le nombre de fois qu'il y a des "différent de transparent" Sert a savoir le milieu de la régressuion
			int* iTableau = new int[_RectPiedJoueur.w, _RectPiedJoueur.h]; // Tableau.
			for (int j = 0; j < _RectPiedJoueur.h; j++) { // Boucler sur toute le rect du pied dans la position de la map.

				for (int i = 0; i < _RectPiedJoueur.w; i++) {
					if (((unsigned int*)m_pGameMap->ObtenirSurfaceMap()->pixels)[(i + _RectPiedJoueur.x + _RectJoueur.x) + ((j + _RectPiedJoueur.y + _RectJoueur.y) * m_pGameMap->ObtenirSurfaceMap()->w)] != 0) { // Si le pixel est différent de transparent.
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
						iVar += pow((i - fX), 2);	   // Calcul pour X moyens avec le X moyens.
					}
				}
			}

			if (iCov != 0 && iVar != 0) {
				iCov = (iCov / iN); //moyenne
				iCov -= 2 * iCov;
				iVar = (iVar / iN); //moyenne
			}

			delete[] iTableau;


			fPente = iCov / iVar;

			if (iCov != 0 && iVar != 0) {

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente > 0) // Le joueur se déplace vers la droite et la pente est positive.
					return -(180 / M_PI) * atanf(fPente);

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente < 0) // Le joueur se déplace vers la droite et la pente est négative.
					return -(180 / M_PI) * atanf(fPente);

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente > 0) // Le joueur se déplace vers la gauche et la pente est positive.
					return 180 - (180 / M_PI) * atanf(fPente);

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente < 0) // Le joueur se déplace vers la gauche et la pente est négative.
					return 180 + (180 / M_PI) * atanf(fPente);
			}

			return 362;




			fPente = iCov / iVar; // Donne la pente. iCov = y , iVar = x.

			if (iCov != 0 && iVar != 0) {
				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente > 0) // Le joueur se déplace vers la droite et la pente est positive.
					return (180 / M_PI) * atanf(fPente);

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 0 && fPente < 0) // Le joueur se déplace vers la droite et la pente est négative.
					return 360 - ((180 / M_PI) * atanf(-fPente));

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente > 0) // Le joueur se déplace vers la gauche et la pente est positive.
					return 180 + (180 / M_PI) * atanf(fPente);

				if (m_pTeamList->ObtenirElementCurseur()->ObtenirPlayerActif()->ObtenirSpriteCourse()->ObtenirEtage() == 1 && fPente < 0) // Le joueur se déplace vers la gauche et la pente est négative.
					return 90 + (180 / M_PI) * atanf(-fPente);

			}

		}
	}
	// Procédure déterminant la position d'une collision entre un objet et la map, si il y en a une.
	// En entrée:
	// Param1: La surface de l'objet.
	// Param2: La destination de l'objet.
	// Param3: La position en X qui sera retourné.
	// Param4: La position en Y qui sera retourné.
	bool CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY) {

		*_iX = 0;
		*_iY = 0;

		unsigned int ix, iy;

		SDL_Surface* pSDLSurfaceMap = m_pGameMap->ObtenirSurfaceMap();

		for (int y = 0; y < _RectDestination.h; y++) {
			for (int x = 0; x <_RectDestination.w; x++) {

				if (x >= 0 && x <= 1366 && y >= 0 && y <= 768) {

					if ((((unsigned int*)pSDLSurfaceMap->pixels)[(y + _RectDestination.y) * pSDLSurfaceMap->w + (x + _RectDestination.x)] != 0) && (((unsigned int*)_pSDLSurface->pixels)[(y) * _pSDLSurface->w + (x)] != 0)) {
						
						*_iX = x;
						*_iY = y;
						
						return true;
					}
				}
			}
		}
		return false;
	}

	/*
	Fonction qui detecte si un personnage marche sur une mine ou un pack
	parametres:
	_pTeamList = liste d'equipes pour obtenir les joueurs
	_Pack = Pack sur lequel la collision sera verifiée
	¸retour: true = une collision a lieu
	*/
	bool DetectionCollisionPack(CPlayer* _pPlayer, bool* _boExplosion, SDL_Rect* _RectExplosion) {

		CPack* pPackTmp;

		SDL_Surface* pTmpSDLSurfacePlayer = nullptr;			// pointeur temporaire de surface qui pointera sur la surface appropriée du joueur.
		SDL_Rect RectPack;

		SDL_Rect TmpSDLRectPlayerDestination = _pPlayer->ObtenirRectDestination();						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affiché sur la map.
		SDL_Rect TmpSDLRectPlayerSource;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropriée du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vérifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vérifier les collisions du corps.

		bool boTerminerBoucle = false;

		*_boExplosion = false;

		if (_pPlayer->ObtenirSpriteCourse()->IsActif()) {						// Si le joueur est en train de courir...
			
			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteCourse()->ObtenirSurface();			// On se sert de la surface du sprite de course.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteCourse()->ObtenirRectSource();		// On se sert du Rect source du sprite de course.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds approprié pour le sprite de course.
		}

		else if (_pPlayer->ObtenirSpriteSaut()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteSaut()->ObtenirSurface();			// On se sert de la surface du sprite de saut.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds approprié pour le sprite de saut.
		}

		else if (_pPlayer->ObtenirSpriteParachute()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteParachute()->ObtenirSurface();			// On se sert de la surface du sprite de parachute.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteParachute()->ObtenirRectSource();		// On se sert du Rect source du sprite de parachute.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPiedsParachute();						// On se sert du hitbox des pieds approprié pour le sprite de parachute.
		}

		else if (_pPlayer->ObtenirSpriteRepos()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteRepos()->ObtenirSurface();			// On se sert de la surface du sprite de repos.

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteRepos()->ObtenirRectSource();		// On se sert du Rect source du sprite de repos.

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds approprié pour le sprite de repos.
		}

		else if (_pPlayer->ObtenirSpriteJetPack()->IsActif()) {

			pTmpSDLSurfacePlayer = _pPlayer->ObtenirSpriteJetPack()->ObtenirSurface();

			TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteJetPack()->ObtenirRectSource();

			TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();
		}


		for (int i = 0; i < m_pGameMap->ObtenirPackList()->ObtenirCompte(); i++) {

			pPackTmp = m_pGameMap->ObtenirPackList()->ObtenirElementCurseur();
			RectPack = pPackTmp->GetRectDestination();

			if ((RectPack.x >= TmpSDLRectPlayerDestination.x && RectPack.x <= (TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w)) && (RectPack.y >= TmpSDLRectPlayerDestination.y && RectPack.y <= (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h))){

				for (int x = TmpSDLRectPlayerDestination.w; x > 0; x--) {

					for (int y = TmpSDLRectPlayerDestination.h; y > 0; y--) {

						if ((((unsigned int*)pTmpSDLSurfacePlayer->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + x) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + y) * pTmpSDLSurfacePlayer->w] != 0)) {

							*_boExplosion = pPackTmp->Use(_pPlayer);
							*_RectExplosion = pPackTmp->GetRectDestination();
							m_pGameMap->ObtenirPackList()->Retirer(true);
							return true;

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

		int iDistanceRayon;
		float fPourcentage;

		for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++) {

			pPlayerList = m_pTeamList->ObtenirElementCurseur()->obtenirListeTeam();

			for (int j = 0; j < pPlayerList->ObtenirCompte(); j++) {

				pPlayerTmp = pPlayerList->ObtenirElementCurseur();
				RectDestinationPlayer = pPlayerTmp->ObtenirRectDestination();

				if (RectDestinationPlayer.x + RectDestinationPlayer.w >= _RectPositionExplosion.x - _iRayon && _RectPositionExplosion.x > RectDestinationPlayer.x && (_RectPositionExplosion.y - (RectDestinationPlayer.y + RectDestinationPlayer.h)) < 5 && (_RectPositionExplosion.y - (RectDestinationPlayer.y + RectDestinationPlayer.h)) > -5) {

					iDistanceRayon = _RectPositionExplosion.x - (RectDestinationPlayer.x + RectDestinationPlayer.w);
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * (1 - fPourcentage));
				}

				else if (RectDestinationPlayer.x <= _RectPositionExplosion.x + _iRayon && _RectPositionExplosion.x < RectDestinationPlayer.x && (_RectPositionExplosion.y - (RectDestinationPlayer.y + RectDestinationPlayer.h)) < 5 && (_RectPositionExplosion.y - (RectDestinationPlayer.y + RectDestinationPlayer.h)) > -5) {

					iDistanceRayon = RectDestinationPlayer.x - _RectPositionExplosion.x;
					fPourcentage = ((float)iDistanceRayon / (float)_iRayon);
					pPlayerTmp->SetHealth(pPlayerTmp->GetHealth() * (1 - fPourcentage));
				}

				if (pPlayerTmp->GetHealth() <= 0)
					pPlayerList->Retirer(true);

				else
					pPlayerList->AllerSuivantCurseur();

			}

			pPackList = m_pGameMap->ObtenirPackList();

			pPackList->AllerDebut();

			for (int i = 0; i < pPackList->ObtenirCompte(); i++) {

				pPackTmp = pPackList->ObtenirElementCurseur();
				RectDestinationPack = pPackTmp->GetRectDestination();

				if (RectDestinationPack.x + RectDestinationPack.w >= (_RectPositionExplosion.x - _iRayon) && _RectPositionExplosion.x >= RectDestinationPack.x) {

					pPackTmp->Use(nullptr);
					m_pGameMap->ObtenirPackList()->Retirer(true);
				}

				else if ((RectDestinationPack.x) <= (_RectPositionExplosion.x + _iRayon) && _RectPositionExplosion.x <= RectDestinationPack.x){

					pPackTmp->Use(nullptr);
					m_pGameMap->ObtenirPackList()->Retirer(true);
				}

				else
					pPackList->AllerSuivantCurseur();
			}
		}
	}

	double CalculerPente(CPlayer* _pPlayer) {

		SDL_Surface* pSurfaceMap = m_pGameMap->ObtenirSurfaceMap();
		double dPente;
		int iNombrePixel;

		SDL_Rect TmpSDLRectPlayerDestination = _pPlayer->ObtenirRectDestination();						// Structure SDL Rect qui contient le rectangle dans lequel le joueur est affiché sur la map.
		SDL_Rect TmpSDLRectPlayerSource;										// Structure SDL Rect qui contiendra le rectangle source de la surface appropriée du joueur pour la situation.

		SDL_Rect TmpSDLRectPlayerHitboxCorps;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vérifier les collisions du pieds.
		SDL_Rect TmpSDLRectPlayerHitboxPieds;									// Structure SDL Rect qui contiendra le rectangle dans le rectangle source dans lequel il faudra vérifier les collisions du corps.

		

		TmpSDLRectPlayerSource = _pPlayer->ObtenirSpriteSaut()->ObtenirRectSource();		// On se sert du Rect source du sprite de saut.
		TmpSDLRectPlayerHitboxPieds = _pPlayer->ObtenirHitboxPieds();						// On se sert du hitbox des pieds approprié pour le sprite de saut.
		

		if (_pPlayer->ObtenirSpriteRepos()->ObtenirEtage() == 0) {

			if ((((unsigned int*)pSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w + 5) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h) * pSurfaceMap->w] == 0)) {

				bool boNonTransparence = false;
				iNombrePixel = 0;

				while (!boNonTransparence) {

					iNombrePixel++;

					if (iNombrePixel > 100) {
						boNonTransparence = true;
						return 361;
					}

					if ((((unsigned int*)pSurfaceMap->pixels)[(TmpSDLRectPlayerDestination.x + TmpSDLRectPlayerDestination.w + 5) + (TmpSDLRectPlayerDestination.y + TmpSDLRectPlayerDestination.h + iNombrePixel) * pSurfaceMap->w] != 0)) {
						boNonTransparence = true;
						dPente = ((double)iNombrePixel / 5);
						return (180 / M_PI) * atanf(dPente);
					}
				}
			}

			else {

				bool boTransparence = false;
				iNombrePixel = 0;

				while (!boTransparence) {

					iNombrePixel--;

					if ((((unsigned int*)pSurfaceMap->pixels)[(TmpSDLRectPlayerSource.x + TmpSDLRectPlayerHitboxPieds.x + 5) + (TmpSDLRectPlayerSource.y + TmpSDLRectPlayerHitboxPieds.y + iNombrePixel) * pSurfaceMap->w] == 0))
						boTransparence = true;
				}

				dPente = ((double)iNombrePixel / 5);
				return -(180 / M_PI) * atanf(dPente);
			}
		}
	}

	bool IsDebut() {
		return m_boDebutPartie;
	}

};