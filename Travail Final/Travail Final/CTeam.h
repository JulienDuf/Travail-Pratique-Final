// 420-202-RE : Travail final
// Classe qui représente une équipe.
// 4 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//


class CTeam {
private:

	CListeDC<CPlayer*>* m_pPlayerList; // Liste de joueur.

public:

	// Constructeur de CTeam...
	// Param1: Emplacement des donnés relatives à aux team.
	// Param2: Render d'affichage des teams.
	// Param3: Numéro de la team.
	// Param4: Nombre de joueur dans la team.
	// Param5: Pointeur de fonction sur la procédure MapDestruction.
	// Param6: Pointeur de fonction sur la fonction Rotation.
	CTeam(string _strEmplacement, SDL_Renderer* _pRenderer, unsigned int _uiIDTeam, int _iNombreJoueur, void _MapDestruction(int _iRayon, int _iX, int _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)) {

		m_pPlayerList = new CListeDC<CPlayer*>();

		int iX;

		// Boucle qui ajoute les players à la team...
		for (int i = _iNombreJoueur; i > 0; i--) {

			iX = rand() % (1366 - pGestionnaireSurface->ObtenirDonnee("pSurfaceParachute")->w/24);
			m_pPlayerList->AjouterFin(new CPlayer( _strEmplacement, _pRenderer, _uiIDTeam, {iX, 5, 0, 0}, _MapDestruction, _Rotation));
		}
		m_pPlayerList->AllerDebut();
	}

	// Procédure qui affichage la team...
	// En entrée:
	// Param1: Render d'affichage.
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

	CListeDC<CPlayer*>* ObtenirListePlayer(void) {
		return m_pPlayerList;
	}

	// Retourne true si toute la team est stable...
	bool IsAllPlayerStable(void) {

		for (int i = 0; i < m_pPlayerList->ObtenirCompte(); i++) {

			if (!m_pPlayerList->ObtenirElementCurseur()->IsStable())
				return false;

			m_pPlayerList->AllerSuivantCurseur();
		}

		return true;
	}
};