// 420-202-RE : Travail final
// Classe qui représente une équipe.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//


class CTeam {

private:

	CListeDC<CPlayer*>* m_pPlayerList; // Liste de joueur.

public:

	CTeam(string _strEmplacementFichier, int _iNombreJoueur, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile), SDL_Renderer* _pRenderer) {

		m_pPlayerList = new CListeDC<CPlayer*>();

		int iX;

		for (int i = _iNombreJoueur; i > 0; i--) {

			iX = rand() % 1350;

			m_pPlayerList->AjouterFin(new CPlayer(_strEmplacementFichier, {iX, 5, 0, 0}, _ProcedureCollision, _MapDestruction, _CollisionObjetMap, _Physique, _pRenderer));

		}
		m_pPlayerList->AllerDebut();
	}

	CListeDC<CPlayer*>* obtenirListeTeam(void) {

		return m_pPlayerList;

	}

	void ShowTeam(SDL_Renderer* _pRenderer) {

		for (int i = 0; i < m_pPlayerList->ObtenirCompte(); i++) {

			m_pPlayerList->ObtenirElementCurseur()->ShowPlayer(_pRenderer);
			m_pPlayerList->AllerSuivantCurseur();
		}
	}

	CPlayer* ObtenirPlayerActif(void) {

		return m_pPlayerList->ObtenirElementCurseur();

	}

};