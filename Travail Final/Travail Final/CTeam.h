// 420-202-RE : Travail final
// Classe qui représente une équipe.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//


class CTeam {

private:

	CListeDC<CPlayer*>* m_pPlayerList; // Liste de joueur.

public:

	CTeam(string _strEmplacementFichier, int _iNombreJoueur, void _ProcedureCollision(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int _uiXMap, unsigned int _uiYMap, unsigned int _uiXRectCollision, unsigned int _uiYRectCollision), void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Renderer* _pRenderer) {

		m_pPlayerList = new CListeDC<CPlayer*>();

		int iX;

		for (int i = _iNombreJoueur; i > 0; i--) {

			iX = rand() % 1350;

			m_pPlayerList->AjouterFin(new CPlayer(_strEmplacementFichier, {iX, 5, 0, 0}, _ProcedureCollision, _MapDestruction, _pRenderer));

		}

	}

	CListeDC<CPlayer*>* obtenirListeTeam(void) {

		return m_pPlayerList;

	}

	CPlayer* ObtenirPlayerActif(void) {

		return m_pPlayerList->ObtenirElementCurseur();

	}

};