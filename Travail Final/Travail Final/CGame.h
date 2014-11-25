class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'Èquipe.

public:

	CGame(string _strEmplacementMap, string _strEmplacementFichier, int _iNombre…quipe, int _iNombreJoueur, CVent* _pVent, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pVent, _pRenderer);

		for (int i = _iNombre…quipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementFichier, _iNombreJoueur, _ProcedureCollision, _MapDestruction, _CollisionObjetMap, _Physique, _pRenderer));
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