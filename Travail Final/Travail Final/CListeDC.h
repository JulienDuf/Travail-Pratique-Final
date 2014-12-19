/*
Liste doublement chaînée possédant un pointeur sur la dernière cellule de la liste et un autre sur une cellule pouvant être manipulé par l'utilisateur.
Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 15 avril 2014.
*/
#include "CCelluleListeDC.h"
template <class T>
class CListeDC {
private:

	// Données membres...

	unsigned int m_uiCompte; // Entier non signé représentant le compte du nombre de cellule.
	CCelluleListeDC<T>* m_pDerniere; // Pointeur de type CCelluleListeDC sur la dernière cellule de la liste.
	CCelluleListeDC<T>* m_pTrieur; // Pointeur de type CCelluleListeDC pour effectuer le trie.
	CCelluleListeDC<T>* m_pCurseur; // Pointeur de type CCelluleListeDC sur une cellule pouvant être manipulé par l'utilisateur.

public:

	// Constructeur...

	CListeDC() {
		m_uiCompte = 0;
		m_pDerniere = nullptr;
		m_pCurseur = nullptr;
		m_pTrieur = nullptr;
	}

	// Destructeur...

	~CListeDC() {
		while (m_uiCompte != 0) {
			Retirer(true);
		}
	}
	
	// Méthodes...
	
	/*
	Fonction permettant de modifier le m_pTrieur sur la cellule suivante de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerSuivantTrieur() {
		// Si m_pDernière et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pTrieur != nullptr) {
			m_pTrieur = m_pTrieur->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pTrieur sur la cellule précédente de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerPrecedentTrieur() {
		// Si m_pDernière et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pTrieur != nullptr) {
			m_pTrieur = m_pTrieur->ObtenirPrecedente();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur le début de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerDebut() {
		// Si m_pDernière n'est pas nulle...
		if (m_pDerniere != nullptr) {
			m_pCurseur = m_pDerniere->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la fin de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerFin() {
		// Si m_pDernière n'est pas nulle...
		if (m_pDerniere != nullptr) {
			m_pCurseur = m_pDerniere;
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la cellule suivante de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerSuivantCurseur() {
		// Si m_pDernière et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pCurseur != nullptr) {
			m_pCurseur = m_pCurseur->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la cellule précédente de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerPrecedentCurseur() {
		// Si m_pDernière et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pCurseur != nullptr) {
			m_pCurseur = m_pCurseur->ObtenirPrecedente();
			return true;
		}
		else
			return false;
	}

	// Procédure permettant de positionner le curseur sur un objet voulu et le retirer si voulu...
	// En entrée:
	// Param1: Objet voulu.
	// Param2; Booléen qui dicte si l'objet doit être retirer.
	void AllerAObjetCurseur(T _Element, bool _boRetirer) {
		if (m_pDerniere != nullptr) {
			int i = 0;
			while (_Element != m_pCurseur->ObtenirElement() && i < m_uiCompte) {
				if (m_pCurseur->ObtenirElement() = _Element) {
					i = m_uiCompte;
					if (_boRetirer) {
						Retirer(true);
					}
				}
				else
					i++;
			}
		}
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une position de cellule de la liste, en partant de 0.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un entier non signé de la position d'insertion.
	*/
	bool AllerATrieur(unsigned int _uiPosition) {
		// Si m_pDernière n'est pas nulle et que la position voulue est plus petite que le compte.
		if (m_pDerniere != nullptr && _uiPosition < m_uiCompte) {
			// Si le compte est pair, on incrémente la position momentannément pour faire le moins de déplacements possibles.
			if (m_uiCompte % 2 != 0) {
				_uiPosition + 1;
				// Si la position voulue est moins grande que le compte divisé par deux pour partir le plus près de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					_uiPosition - 1;
					m_pTrieur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pTrieur = m_pTrieur->ObtenirSuivante();
					}
				}
				else
				{
					_uiPosition - 1;
					m_pTrieur = m_pDerniere;
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pTrieur = m_pTrieur->ObtenirPrecedente();
					}
				}
			}
			else
			{
				// Si la position voulue est moins grande que le compte divisé par deux pour partir le plus près de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					m_pTrieur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pTrieur = m_pTrieur->ObtenirSuivante();
					}
				}
				else
				{
					m_pTrieur = m_pDerniere;
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pTrieur = m_pTrieur->ObtenirPrecedente();
					}
				}
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une position de cellule de la liste, en partant de 0.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un entier non signé de la position d'insertion.
	*/
	bool AllerACurseur(unsigned int _uiPosition) {
		// Si m_pDernière n'est pas nulle et que la position voulue est plus petite que le compte.
		if (m_pDerniere != nullptr && _uiPosition < m_uiCompte) {
			// Si le compte est pair, on incrémente la position momentannément pour faire le moins de déplacements possibles.
			if (m_uiCompte % 2 != 0) {
				_uiPosition + 1;
				// Si la position voulue est moins grande que le compte divisé par deux pour partir le plus près de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					_uiPosition - 1;
					m_pCurseur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pCurseur = m_pCurseur->ObtenirSuivante();
					}
				}
				else
				{
					_uiPosition - 1;
					m_pCurseur = m_pDerniere;
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pCurseur = m_pCurseur->ObtenirPrecedente();
					}
				}
			}
			else
			{
				// Si la position voulue est moins grande que le compte divisé par deux pour partir le plus près de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					m_pCurseur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pCurseur = m_pCurseur->ObtenirSuivante();
					}
				}
				else
				{
					m_pCurseur = m_pDerniere;
					// Boucle qui s'arrête quand le pointeur m_pCurseur est à la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pCurseur = m_pCurseur->ObtenirPrecedente();
					}
				}
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une nouvelle cellule créée en mémoire.
	Si la modification a peut avoir lieu, retourne vrai, sinon, retourne faux.
	Prend un élément à ajouter.
	*/
	bool Inserer(T _Element) {
		// Si le curseur n'est pas null...
		if (m_pCurseur != nullptr) {
			m_pCurseur->ObtenirPrecedente()->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pCurseur, m_pCurseur->ObtenirPrecedente())); // Création d'un obbjet en mémoire et retour de son adresse dans m_pSuivante.
			m_pCurseur->DefinirPrecedente(m_pCurseur->ObtenirPrecedente()->ObtenirSuivante());
			m_pCurseur = m_pCurseur->ObtenirPrecedente();
			m_uiCompte++; // Incrémentation du compte.
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de retirer le cellule à la position du curseur et de faire pointer celui-ci sur la cellule suivante, sauf s'il n'y en a pas.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un bool qui dit s'il faut supprimer l'élément dans la cellule.
	*/
	bool Retirer(bool _boSupprimer) {
		// Si le curseur n'est pas null...
		if (m_pCurseur != nullptr) {
			m_pCurseur->ObtenirPrecedente()->DefinirSuivante(m_pCurseur->ObtenirSuivante());
			m_pCurseur->ObtenirSuivante()->DefinirPrecedente(m_pCurseur->ObtenirPrecedente());
			CCelluleListeDC<T>* pCelltmpListeDC = m_pCurseur; // Pointeur de type CCelluleListeDC sur m_pCurseur.
			// Si le curseur n'est pas sur m_pDernière... il sera égal à sa suivante
			if (m_pCurseur != m_pDerniere)
				m_pCurseur = m_pCurseur->ObtenirSuivante();
			// Sinon il est égal à sa précédente pour ne pas revenir au début.
			else
			{
				m_pCurseur = m_pCurseur->ObtenirPrecedente();
				m_pDerniere = m_pCurseur;
			}
			// Si l'utilisateur veut effacer l'élément dans la cellule.
			if (_boSupprimer)
				delete pCelltmpListeDC->ObtenirElement();
			delete pCelltmpListeDC;
			m_uiCompte--; // Décrémentation du compte.
			// Si le compte est à zéro, tout mettre à nullptr.
			if (m_uiCompte == 0) {
				m_pCurseur = nullptr;
				m_pDerniere = nullptr;
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de retirer le cellule à la position du curseur et de faire pointer celui-ci sur la cellule suivante, sauf s'il n'y en a pas.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un bool qui dit s'il faut supprimer l'élément dans la cellule.
	*/
	bool RetirerTrieur(bool _boSupprimer) {
		// Si le curseur n'est pas null...
		if (m_pTrieur != nullptr) {
			m_pTrieur->ObtenirPrecedente()->DefinirSuivante(m_pTrieur->ObtenirSuivante());
			m_pTrieur->ObtenirSuivante()->DefinirPrecedente(m_pTrieur->ObtenirPrecedente());
			CCelluleListeDC<T>* pCelltmpListeDC = m_pTrieur; // Pointeur de type CCelluleListeDC sur m_pCurseur.
			
			if (m_pTrieur == m_pCurseur)
				m_pCurseur = m_pCurseur->ObtenirPrecedente();
			
			// Si le curseur n'est pas sur m_pDernière... il sera égal à sa suivante
			if (m_pTrieur != m_pDerniere)
				m_pTrieur = m_pTrieur->ObtenirSuivante();
			// Sinon il est égal à sa précédente pour ne pas revenir au début.
			else
			{
				m_pTrieur = m_pTrieur->ObtenirPrecedente();
				m_pDerniere = m_pTrieur;
			}
			// Si l'utilisateur veut effacer l'élément dans la cellule.
			if (_boSupprimer)
				delete pCelltmpListeDC->ObtenirElement();
			delete pCelltmpListeDC;
			m_uiCompte--; // Décrémentation du compte.
			// Si le compte est à zéro, tout mettre à nullptr.
			if (m_uiCompte == 0) {
				m_pTrieur = nullptr;
				m_pCurseur = nullptr;
				m_pDerniere = nullptr;
			}
			return true;
		}
		else
			return false;
	}

	/*
	Procédure permettant d'ajouter une cellule au début de la liste
	Prend un élément à ajouter.
	*/
	void AjouterDebut(T _Element) {
		// Si m_pDernière est null, on ajoute une première cellule.
		if (m_pDerniere == nullptr) {
			m_pDerniere = new CCelluleListeDC<T>(_Element, nullptr, nullptr); // Création d'un objet en mémoire et retour de l'adresse mémoire dans m_pDernière.
			m_pDerniere->DefinirSuivante(m_pDerniere);
			m_pDerniere->DefinirPrecedente(m_pDerniere);
		}
		else
		{
			m_pDerniere->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pDerniere->ObtenirSuivante(), m_pDerniere)); // Création d'un objet en mémoire et retour de l'addresse mémoire dans la suivante de la dernière.
			m_pDerniere->ObtenirSuivante()->ObtenirSuivante()->DefinirPrecedente(m_pDerniere->ObtenirSuivante());
		}
		m_uiCompte++; // Incrémentation du compte.
	}

	/*
	Procédure permettant d'ajouter une cellule à la fin de la liste
	Prend un élément à ajouter.
	*/
	void AjouterFin(T _Element) {
		// Si m_pDernière est null , on ajoute comme ajouter début.
		if (m_pDerniere == nullptr) {
			AjouterDebut(_Element);
		}
		else
		{
			m_pDerniere->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pDerniere->ObtenirSuivante(), m_pDerniere));
			m_pDerniere = m_pDerniere->ObtenirSuivante();
			m_pDerniere->ObtenirSuivante()->DefinirPrecedente(m_pDerniere);
			m_uiCompte++; // Incrémentation du compte.
		}
	}

	// Accesseurs...

	// Fonction qui retourne l'élément dans la cellule où pointe le curseur.

	T ObtenirElementTrieur() {
		return m_pTrieur->ObtenirElement();
	}

	T ObtenirElementCurseur() {
		return m_pCurseur->ObtenirElement();
	}

	void DefinirElementTrieur(T _Element) {
		m_pTrieur->DefinirElement(_Element);
	}

	void DefinirElementCurseur(T _Element) {
		m_pCurseur->DefinirElement(_Element);
	}

	// Fonction qui retourne le compte.
	unsigned int ObtenirCompte() {
		return m_uiCompte;
	}
};