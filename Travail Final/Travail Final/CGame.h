class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'Èquipe.

public:

	CGame(string _strEmplacementMap, string _strEmplacementFichier, int _iNombre…quipe, int _iNombreJoueur, SDL_Surface* _pSurfaceGabarie, CVent* _pVent, SDL_Renderer* _pRenderer) {

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pSurfaceGabarie, _pVent, _pRenderer);

		for (int i = _iNombre…quipe; i > 0; i--) {

			m_pTeamList->AjouterFin(new CTeam(_strEmplacementFichier, _iNombreJoueur, m_pGameMap->VerifierCollisionMap, m_pGameMap->MapDestruction, _pRenderer));
		}

	}

	void AfficherGame(SDL_Renderer* _pRenderer) {

		m_pGameMap->ShowMap(_pRenderer);

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