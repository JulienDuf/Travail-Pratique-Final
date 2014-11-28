// 420-202-RE : Travail final
// Classe qui représente une équipe.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//


class CTeam {

private:

	CListeDC<CPlayer*>* m_pPlayerList; // Liste de joueur.

public:

	CTeam(CGestionaire<SDL_Surface*>* _pGestionnaireSurface, CGestionaire<SDL_Texture*>* _pGestionnaireTexture, int _iNombreJoueur, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionObjetMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), double _Physique(CVecteur2D* _VitesseMissile, SDL_Rect* _DestinationMissile)) {

		m_pPlayerList = new CListeDC<CPlayer*>();

		int iX;

		for (int i = _iNombreJoueur; i > 0; i--) {

			iX = rand() % 1298;

			m_pPlayerList->AjouterFin(new CPlayer(_pGestionnaireSurface, _pGestionnaireTexture, {iX, 5, 0, 0}, _MapDestruction, _CollisionObjetMap, _Physique));

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

	// Procédure permettant de changer de joueur...
	void ChangerPlayerActif() {
		m_pPlayerList->AllerSuivantCurseur();
	}

	CPlayer* ObtenirPlayerActif(void) {

		return m_pPlayerList->ObtenirElementCurseur();

	}
};