// 420-202-RE : Travail final
// Classe qui repr�sente une �quipe.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//


class CTeam {

private:

	CListeDC<CPlayer*>* m_pPlayerList; // Liste de joueur.
	bool (*m_pVerifierCollisionJoueurMap)(CPlayer* _pPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap);

public:

	CTeam(string _strEmplacementFichier, int _iNombreJoueur, bool _VerifierCollisionJoueurMap(CPlayer* _pPlayer, bool* _boCollisionCorps, bool* _boCollisionPieds, unsigned int* _uiXMap, unsigned int* _uiYMap), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_pPlayerList = new CListeDC<CPlayer*>();

		m_pVerifierCollisionJoueurMap = _VerifierCollisionJoueurMap;

		int iX;

		for (int i = _iNombreJoueur; i > 0; i--) {

			iX = rand() % 1350;

			m_pPlayerList->AjouterFin(new CPlayer(_strEmplacementFichier, {iX, 5, 0, 0}, _MapDestruction, _CollisionObjetMap, _Physique, _pRenderer));

		}
		m_pPlayerList->AllerDebut();
	}

	CListeDC<CPlayer*>* obtenirListeTeam(void) {

		return m_pPlayerList;

	}

	void ShowTeam(SDL_Renderer* _pRenderer) {
		bool _boCorps;
		bool _boPieds;

		unsigned int _uiXMap;
		unsigned int _uiYMap;

		CPlayer* pPlayer;
		SDL_Rect RectPlayer;

		for (int i = 0; i < m_pPlayerList->ObtenirCompte(); i++) {

			pPlayer = m_pPlayerList->ObtenirElementCurseur();
			RectPlayer = pPlayer->ObtenirRectDestination();

			if (pPlayer->ObtenirSpriteParachute()->IsActif()) {
				RectPlayer.y += 1;
				pPlayer->ModifierRectDestination(RectPlayer);
			}
			if (m_pVerifierCollisionJoueurMap(pPlayer, &_boCorps, &_boPieds, &_uiXMap, &_uiYMap)) {
				RectPlayer.y += -1;
				pPlayer->ModifierRectDestination(RectPlayer);

			}
			m_pPlayerList->ObtenirElementCurseur()->ShowPlayer(_pRenderer);
			m_pPlayerList->AllerSuivantCurseur();
		}
	}

	CPlayer* ObtenirPlayerActif(void) {

		return m_pPlayerList->ObtenirElementCurseur();

	}

};