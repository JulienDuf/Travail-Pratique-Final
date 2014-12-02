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

	/*
	Fonction qui detecte si un personnage marche sur une mine ou un pack
	parametres:
	_pTeamList = liste d'equipes pour obtenir les joueurs
	_Pack = Pack sur lequel la collision sera verifiée
	¸retour: true = une collision a lieu
	*/
	void DetectionCollisionPack(){
		SDL_Surface* pTempSurfacePlayer;
		SDL_Surface* pTempSurfacePack;
		SDL_Rect TempRectDestinationPack;

		for (int p = 0; p < m_pGameMap->ObtenirPackList()->ObtenirCompte(); p++){ //Parcours des packs

			CPack* tempPack = m_pGameMap->ObtenirPackList()->ObtenirElementCurseur();
			pTempSurfacePack = tempPack->GetSurface();
			TempRectDestinationPack = tempPack->GetRectDestination();

			for (int i = 0; i < m_pTeamList->ObtenirCompte(); i++){ //Parcours des equipes
				CTeam* tempTeam = m_pTeamList->ObtenirElementCurseur();
				CListeDC<CPlayer*>* tempPlayerList = tempTeam->obtenirListeTeam();

				for (int j = 0; j < tempPlayerList->ObtenirCompte(); j++){ //Parcours des personnages dans l'equipe
					CPlayer* tempPlayer = tempPlayerList->ObtenirElementCurseur();
					SDL_Rect tempPositionPlayer = tempPlayer->ObtenirRectDestination();

					//Obtention de la bonne surface du joueur
					if (tempPlayer->ObtenirSpriteCourse()->IsActif()){
						pTempSurfacePlayer = tempPlayer->ObtenirSpriteCourse()->ObtenirSurface();
					}
					else if (tempPlayer->ObtenirSpriteSaut()->IsActif()){
						pTempSurfacePlayer = tempPlayer->ObtenirSpriteSaut()->ObtenirSurface();
					}
					else if (tempPlayer->ObtenirSpriteParachute()->IsActif()){
						pTempSurfacePlayer = tempPlayer->ObtenirSpriteParachute()->ObtenirSurface();
					}
					else if (tempPlayer->ObtenirSpriteRepos()->IsActif()){
						pTempSurfacePlayer = tempPlayer->ObtenirSpriteRepos()->ObtenirSurface();
					}

					for (int y = tempPositionPlayer.y; y < tempPositionPlayer.y + tempPositionPlayer.h; y++) { //Verification des collisions
						for (int x = tempPositionPlayer.x; x < tempPositionPlayer.x + tempPositionPlayer.w; x++) {
							for (int y2 = TempRectDestinationPack.y; y2 < TempRectDestinationPack.y + TempRectDestinationPack.h; y2++) {
								for (int x2 = TempRectDestinationPack.x; x2 < TempRectDestinationPack.x + TempRectDestinationPack.w; x2++) {
									if ((((unsigned int*)pTempSurfacePlayer->pixels)[(tempPositionPlayer.x + x) + (tempPositionPlayer.y + y) * pTempSurfacePlayer->w] != 0) && (((unsigned int*)pTempSurfacePack->pixels)[(TempRectDestinationPack.x + x) + (TempRectDestinationPack.y + y) * pTempSurfacePack->w] != 0)){ //Verification des pixels transparents
										if ((x2 == x) && (y2 == y)){
											tempPack->Use();
										}
									}
								}
							}
						}
					}
					tempPlayerList->AllerSuivantCurseur();
				}
				m_pTeamList->AllerSuivantCurseur();
			}
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