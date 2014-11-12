class CGame {

private:

	CMap* m_pGameMap; //La map.
	CListeDC<CTeam*>* m_pTeamList; // Liste d'Èquipe.

public:

	CGame(string _strEmplacementMap, int _iNombre…quipe, int _iNombreJoueur, SDL_Surface* _pSurfaceGabarie, SDL_Renderer* _pRenderer) {

		m_pTeamList = new CListeDC<CTeam*>();

		m_pGameMap = new CMap(_strEmplacementMap, { 0, 0, 0, 0 }, _pSurfaceGabarie, _pRenderer);

	}

	void AfficherGame(SDL_Renderer* _pRenderer) {


	}

};