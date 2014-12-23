/*
Classe qui pointe sur la racine d'un arbre binaire de type noeud.
Créée par Gabriel Beaudry (gabriel.bdry@gmail.com) le 22 avril 2014.
*/
#include "CNoeudArbreAVL.h"
#include <algorithm>

using namespace std;

template <class T>
class CArbreAVL {
private:

	// Données membres privées...
	CNoeudArbreAVL<T>* m_pRacine; // Pointeur sur la racine de l'arbre.
	unsigned int m_uiCompte; // Compte du nombre de Noeud dans l'arbre.

	// Méthodes privées...

	// Parcours [:

	// Procédure qui appel tous les ShowControl de mon arbre...
	void ParcoursMenu(CNoeudArbreAVL<T>* _pNoeud, SDL_Renderer* _pSDLRendererParcours) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursMenu(_pNoeud->ObtenirEG(), _pSDLRendererParcours);
		}
		_pNoeud->ObtenirElement()->ShowMenu(_pSDLRendererParcours);
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursMenu(_pNoeud->ObtenirED(), _pSDLRendererParcours);
		}
	}

	// Procédure qui appel tous les ShowControl de mon arbre...
	void ParcoursControl(CNoeudArbreAVL<T>* _pNoeud, SDL_Renderer* _pSDLRendererParcours) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursControl(_pNoeud->ObtenirEG(), _pSDLRendererParcours);
		}
		_pNoeud->ObtenirElement()->ShowControl(_pSDLRendererParcours);
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursControl(_pNoeud->ObtenirED(), _pSDLRendererParcours);
		}
	}

	// Procédure qui appel tous les ReactToEvent de mon arbre...
	void ParcoursReaction(CNoeudArbreAVL<T>* _pNoeud, SDL_Event* _pSDLEvent) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursReaction(_pNoeud->ObtenirEG(), _pSDLEvent);
		}
		_pNoeud->ObtenirElement()->ReactToEvent(_pSDLEvent);
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursReaction(_pNoeud->ObtenirED(), _pSDLEvent);
		}

	}
	
	void ParcoursDestructeur(CNoeudArbreAVL<T>* _pNoeud) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursDestructeur(_pNoeud->ObtenirEG());
		}
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursDestructeur(_pNoeud->ObtenirED());
		}
		delete _pNoeud;
	}

	// Procédure détruisant des textures...
	void ParcoursDestructionTexture(CNoeudArbreAVL<T>* _pNoeud) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursDestructionTexture(_pNoeud->ObtenirEG());
		}
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursDestructionTexture(_pNoeud->ObtenirED());
		}
		SDL_DestroyTexture(_pNoeud->ObtenirElement());
	}

	// Procédure détruisant des surfaces...
	void ParcoursDestructionSurface(CNoeudArbreAVL<T>* _pNoeud) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursDestructionSurface(_pNoeud->ObtenirEG());
		}
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursDestructionSurface(_pNoeud->ObtenirED());
		}
		SDL_FreeSurface(_pNoeud->ObtenirElement());
	}

	// Procédure détruisant des fonts...
	void ParcoursDestructionFont(CNoeudArbreAVL<T>* _pNoeud) {
		// S'il a un enfant gauche...
		if (_pNoeud->ObtenirEG() != nullptr) {
			ParcoursDestructionFont(_pNoeud->ObtenirEG());
		}
		// S'il a un enfant droit
		if (_pNoeud->ObtenirED() != nullptr) {
			ParcoursDestructionFont(_pNoeud->ObtenirED());
		}
		TTF_CloseFont(_pNoeud->ObtenirElement());
	}

	// :]

	// Rotations [:

	/*
	Applique une rotation vers la gauche à partir d'un noeud de l'arbre pour l'équilibrer.
	Prend un pointeur sur le noeud de rotation.
	*/
	void RotationGauche(CNoeudArbreAVL<T>* _NoeudPivot, bool _boRetirerEnCours) {
		// Si boRetirer n'est pas en cours ou que boRetirer est en cours et que l'enfant droit du pivot n'est pas a nullptr.
		if ((!_boRetirerEnCours) ^ ((_boRetirerEnCours) && (_NoeudPivot->ObtenirED() != nullptr))) {
			CNoeudArbreAVL<T>* ptmp = _NoeudPivot->ObtenirED(); // Pointeur temporaire sur l'enfant droit du noeud pivot.
			// Si le noeud pivot est à la racine...
			if (_NoeudPivot == m_pRacine) {
				m_pRacine = ptmp;
				ptmp->DefinirParent(nullptr);
			}
			else
			{

				if (_NoeudPivot->ObtenirID() == "") {
					// Si l'élément dans le noeud est plus petit que l'élément au parent ...
					if (_NoeudPivot->ObtenirElement() < _NoeudPivot->ObtenirParent()->ObtenirElement()) {
						_NoeudPivot->ObtenirParent()->DefinirEG(ptmp);
						ptmp->DefinirParent(_NoeudPivot->ObtenirParent());
					}
					else
					{
						_NoeudPivot->ObtenirParent()->DefinirED(ptmp);
						ptmp->DefinirParent(_NoeudPivot->ObtenirParent());
					}
				}

				else {

					// Si l'élément dans le noeud est plus petit que l'élément au parent ...
					if (_NoeudPivot->ObtenirID() < _NoeudPivot->ObtenirParent()->ObtenirID()) {
						_NoeudPivot->ObtenirParent()->DefinirEG(ptmp);
						ptmp->DefinirParent(_NoeudPivot->ObtenirParent());
					}
					else
					{
						_NoeudPivot->ObtenirParent()->DefinirED(ptmp);
						ptmp->DefinirParent(_NoeudPivot->ObtenirParent());
					}
				}
			}
			// Si l'enfant gauche de l'enfant droit du noeud pivot n'est pas null...
			if (ptmp->ObtenirEG() != nullptr) {
				_NoeudPivot->DefinirED(ptmp->ObtenirEG());
				ptmp->ObtenirEG()->DefinirParent(_NoeudPivot);
			}
			else
			{
				_NoeudPivot->DefinirED(nullptr);
			}
			ptmp->DefinirEG(_NoeudPivot);
			_NoeudPivot->DefinirParent(ptmp);
			
			// Modification des indices d'équilibre...
			_NoeudPivot->DefinirIndiceEquilibre(_NoeudPivot->ObtenirIE() - 1 - max(0, _NoeudPivot->ObtenirParent()->ObtenirIE()));
			_NoeudPivot->ObtenirParent()->DefinirIndiceEquilibre(_NoeudPivot->ObtenirParent()->ObtenirIE() - 1 + min(_NoeudPivot->ObtenirIE(), 0));
		}
	}
	

	/*
	Applique une rotation vers la droite à partir d'un noeud de l'arbre pour l'équilibrer.
	Prend un pointeur sur le noeud de rotation.
	*/
	void RotationDroite(CNoeudArbreAVL<T>* _NoeudPivot,bool _boRetirerEnCours) {
		// Si boRetirer n'est pas en cours ou que boRetirer est en cours et que l'enfant gauche du pivot n'est pas a nullptr.
		if ((!_boRetirerEnCours) ^ ((_boRetirerEnCours) && (_NoeudPivot->ObtenirEG() != nullptr))) {
			CNoeudArbreAVL<T>* ptmp = _NoeudPivot->ObtenirEG(); // Pointeur temporaire sur l'enfant gauche du noeud pivot.
			// Si le noeud pivot est à la racine...
			if (_NoeudPivot == m_pRacine) {
				m_pRacine = ptmp;
				ptmp->DefinirParent(nullptr);
			}
			else
			{
				if (_NoeudPivot->ObtenirID() == "") {
					// Si l'élément dans le noeud est plus grand que l'élément au parent ...
					if (_NoeudPivot->ObtenirElement() > _NoeudPivot->ObtenirParent()->ObtenirElement()) {
						_NoeudPivot->ObtenirParent()->DefinirED(ptmp);
					}
					else
					{
						_NoeudPivot->ObtenirParent()->DefinirEG(ptmp);
					}
					ptmp->DefinirParent(_NoeudPivot->ObtenirParent());
				}

				else {
					// Si l'élément dans le noeud est plus grand que l'élément au parent ...
					if (_NoeudPivot->ObtenirID() > _NoeudPivot->ObtenirParent()->ObtenirID()) {
						_NoeudPivot->ObtenirParent()->DefinirED(ptmp);
					}
					else
					{
						_NoeudPivot->ObtenirParent()->DefinirEG(ptmp);
					}
					ptmp->DefinirParent(_NoeudPivot->ObtenirParent());

				}
			}
			// Si l'enfant droit de l'enfant gauche du noeud pivot n'est pas null...
			if (ptmp->ObtenirED() != nullptr) {
				_NoeudPivot->DefinirEG(ptmp->ObtenirED());
				ptmp->ObtenirED()->DefinirParent(_NoeudPivot);
			}
			else
			{
				_NoeudPivot->DefinirEG(nullptr);
			}
			ptmp->DefinirED(_NoeudPivot);
			_NoeudPivot->DefinirParent(ptmp);


			// Modification des indices d'équilibre...
			_NoeudPivot->DefinirIndiceEquilibre(_NoeudPivot->ObtenirIE() + 1 - min(_NoeudPivot->ObtenirParent()->ObtenirIE(), 0));
			_NoeudPivot->ObtenirParent()->DefinirIndiceEquilibre(_NoeudPivot->ObtenirParent()->ObtenirIE() + 1 + max(_NoeudPivot->ObtenirIE(), 0));
		}
	}
	
	// :]

	// Autres [:

	/*
	Fonction permettant de rechercher un élément dans l'arbre.
	Prend un élément à rechercher et un pointeur de type Noeud pour la récursivité et retourne vrai si l'élément à été trouvé sinon elle retourne faux
	*/
	CNoeudArbreAVL<T>* Rechercher(T _ElementRechercher, CNoeudArbreAVL<T>* _pNoeud) {
		// Si l'élément dans le Noeud sur lequel pointe _pNoeud est égal à l'élément recherché...
		if (_pNoeud->ObtenirElement() == _ElementRechercher) {
			return _pNoeud;
		}
		else
		{
			// Si on doit passer par la gauche pour trouver l'élément...
			if (_ElementRechercher < _pNoeud->ObtenirElement() && _pNoeud->ObtenirEG() != nullptr) {
				Rechercher(_ElementRechercher, _pNoeud->ObtenirEG());
			}
			else
			// Si on doit passer par la droite pour trouver l'élément...
			if (_ElementRechercher > _pNoeud->ObtenirElement() && _pNoeud->ObtenirED() != nullptr) {
				Rechercher(_ElementRechercher, _pNoeud->ObtenirED());
			}
			else
				return nullptr;
		}
	}

	/*
	Fonction permettant de rechercher un élément dans l'arbre.
	Prend un élément à rechercher et un pointeur de type Noeud pour la récursivité et retourne vrai si l'élément à été trouvé sinon elle retourne faux
	*/
	CNoeudArbreAVL<T>* ObtenirElement(string _strID, CNoeudArbreAVL<T>* _pNoeud) {
		// Si l'élément dans le Noeud sur lequel pointe _pNoeud est égal à l'élément recherché...
		if (_pNoeud->ObtenirID() == _strID) {
			return _pNoeud;
		}
		else
		{
			// Si on doit passer par la gauche pour trouver l'élément...
			if (_strID < _pNoeud->ObtenirID() && _pNoeud->ObtenirEG() != nullptr) {
				return ObtenirElement(_strID, _pNoeud->ObtenirEG());
			}
			else
				// Si on doit passer par la droite pour trouver l'élément...
				if (_strID > _pNoeud->ObtenirID() && _pNoeud->ObtenirED() != nullptr) {
					return ObtenirElement(_strID, _pNoeud->ObtenirED());
				}
				else
					return NULL;
		}
	}

	/*
	Procédure permettant d'ajouter un Noeud dans l'arbre.
	Prend un élément de n'importe quel type comparable à ajouter dans le nouveau noeud et un pointeur pour la récursivité.
	*/
	void Ajouter(T _ElementAjouter, string _strID, CNoeudArbreAVL<T>* _pNoeud) {
		
		if (_strID == "") {

			// Si on doit passer par la gauche pour ajouter l'élément...
			if (_ElementAjouter < _pNoeud->ObtenirElement()) {
				// S'il n'y a pas de noeud à la gauche...
				if (_pNoeud->ObtenirEG() != nullptr)
					Ajouter(_ElementAjouter, _strID, _pNoeud->ObtenirEG());
				else
				{
					_pNoeud->DefinirEG(new CNoeudArbreAVL<T>(_ElementAjouter, _strID, _pNoeud, 0)); // Création d'un objet en mémoire de tye Noeud et retour de son adresse mémoire dans un nouvel enfant gauche.
					ModificationIndiceEquilibreGauche(_pNoeud, false);
				}
			}
			else {
				// Si on doit passer par la gauche pour ajouter l'élément...
				if (_ElementAjouter > _pNoeud->ObtenirElement()) {
					// S'il n'y a pas de noeud à la gauche...
					if (_pNoeud->ObtenirED() != nullptr)
						Ajouter(_ElementAjouter, _strID, _pNoeud->ObtenirED());
					else {
						_pNoeud->DefinirED(new CNoeudArbreAVL<T>(_ElementAjouter, _strID, _pNoeud, 0)); // Création d'un objet en mémoire de type Noeud et retour de son adresse mémoire dans un nouvel enfant droit.
						ModificationIndiceEquilibreDroit(_pNoeud, false);
					}
				}
			}
		}

		else {

			// Si on doit passer par la gauche pour ajouter l'élément...
			if (_strID < _pNoeud->ObtenirID()) {
				// S'il n'y a pas de noeud à la gauche...
				if (_pNoeud->ObtenirEG() != nullptr)
					Ajouter(_ElementAjouter, _strID, _pNoeud->ObtenirEG());
				else
				{
					_pNoeud->DefinirEG(new CNoeudArbreAVL<T>(_ElementAjouter, _strID, _pNoeud, 0)); // Création d'un objet en mémoire de tye Noeud et retour de son adresse mémoire dans un nouvel enfant gauche.
					ModificationIndiceEquilibreGauche(_pNoeud, false);
				}
			}
			else 
				// Si on doit passer par la gauche pour ajouter l'élément...
				if (_strID > _pNoeud->ObtenirID()) {
					// S'il n'y a pas de noeud à la gauche...
					if (_pNoeud->ObtenirED() != nullptr)
						Ajouter(_ElementAjouter, _strID, _pNoeud->ObtenirED());
					else {
						_pNoeud->DefinirED(new CNoeudArbreAVL<T>(_ElementAjouter, _strID, _pNoeud, 0)); // Création d'un objet en mémoire de type Noeud et retour de son adresse mémoire dans un nouvel enfant droit.
						ModificationIndiceEquilibreDroit(_pNoeud, false);
					}
				}
			

		}
	}

	void ModificationIndiceEquilibreGauche(CNoeudArbreAVL<T>* _pNoeud, bool _boRetirerEnCours) {
		// Si le noeud et le parent sont différent de nullptr...
		if (_pNoeud != nullptr) {
			// Si le retrait est en cours...
			if (!_boRetirerEnCours) {
				_pNoeud->DefinirIndiceEquilibre(_pNoeud->ObtenirIE() - 1); // Modification de l'indice d'équilibre.
				// Si l'indice d'équilibre est de deux, une rotation est à faire...
				if (_pNoeud->ObtenirIE() == -2) {
					// Si l'enfant droite est différent de nullptr...
					if (_pNoeud->ObtenirEG() != nullptr) {
						// Validation de la double rotation...
						if (_pNoeud->ObtenirEG()->ObtenirIE() == 1) {
							// Double rotation.
							RotationGauche(_pNoeud->ObtenirEG(), _boRetirerEnCours);
							RotationDroite(_pNoeud, _boRetirerEnCours);
						}
						else
							RotationDroite(_pNoeud, _boRetirerEnCours);
					}
				}
				else
				{
					// Voir si l'indice d'équilibre s'est rééquilibré...
					if (_pNoeud->ObtenirIE() != 0 && _pNoeud->ObtenirParent() != nullptr) {
						if (_pNoeud->ObtenirID() == "") {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirElement() < _pNoeud->ObtenirParent()->ObtenirElement())
								ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirElement() > _pNoeud->ObtenirParent()->ObtenirElement())
									ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
						}

						else {

							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirID() < _pNoeud->ObtenirParent()->ObtenirID())
								ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirID() > _pNoeud->ObtenirParent()->ObtenirID())
									ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
						}
					}
				}
			}
			else
			{
				_pNoeud->DefinirIndiceEquilibre(_pNoeud->ObtenirIE() + 1); // Modification de l'indice d'équilibre.
				// Validation, rotation à faire
				if (_pNoeud->ObtenirIE() == 2) {
					// Validation enfant gauche existant...
					if (_pNoeud->ObtenirED() != nullptr) {
						// Validation double rotation...
						if (_pNoeud->ObtenirED()->ObtenirIE() == -1) {
							// Double rotation
							RotationDroite(_pNoeud->ObtenirED(), _boRetirerEnCours);
							RotationGauche(_pNoeud, _boRetirerEnCours);
						}
						else
							//Simple Rotation
							RotationGauche(_pNoeud, _boRetirerEnCours);
					}
				}
				else
				{
					// Validation jusqu'à un débalencement...
					if (_pNoeud->ObtenirIE() == 0 && _pNoeud != m_pRacine) {

						if (_pNoeud->ObtenirID() == "") {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirElement() < _pNoeud->ObtenirParent()->ObtenirElement())
								ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirElement() > _pNoeud->ObtenirParent()->ObtenirElement())
									ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
						}

						else {

							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirID() < _pNoeud->ObtenirParent()->ObtenirID())
								ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirID() > _pNoeud->ObtenirParent()->ObtenirID())
									ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
						}
					}
				}
			}
		}
		
	}

	void ModificationIndiceEquilibreDroit(CNoeudArbreAVL<T>* _pNoeud, bool _boRetirerEnCours) {
		// Si le noeud et le parent sont différent de nullptr...
		if (_pNoeud != nullptr) {
			// Si le retrait est en cours...
			if (!_boRetirerEnCours) {
				_pNoeud->DefinirIndiceEquilibre(_pNoeud->ObtenirIE() + 1); // Modification de l'indice d'équilibre.
				// Si l'indice d'équilibre est de deux, une rotation est à faire...
				if (_pNoeud->ObtenirIE() == 2) {
					// Si l'enfant droite est différent de nullptr...
					if (_pNoeud->ObtenirED() != nullptr) {
						// Validation de la double rotation...
						if (_pNoeud->ObtenirED()->ObtenirIE() == -1) {
							// Double rotation.
							RotationDroite(_pNoeud->ObtenirED(), _boRetirerEnCours);
							RotationGauche(_pNoeud, _boRetirerEnCours);
						}
						else
							RotationGauche(_pNoeud, _boRetirerEnCours);
					}
				}
				else
				{
					// Voir si l'indice d'équilibre s'est rééquilibré...
					if (_pNoeud->ObtenirIE() != 0 && _pNoeud->ObtenirParent() != nullptr) {

						if (_pNoeud->ObtenirID() == "") {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirElement() > _pNoeud->ObtenirParent()->ObtenirElement())
								ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirElement() < _pNoeud->ObtenirParent()->ObtenirElement())
									ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);

						}

						else {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirID() > _pNoeud->ObtenirParent()->ObtenirID())
								ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirID() < _pNoeud->ObtenirParent()->ObtenirID())
									ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);

						}
					}
				}
			}
			else
			{
				_pNoeud->DefinirIndiceEquilibre(_pNoeud->ObtenirIE() - 1); // Modification de l'indice d'équilibre.
				// Validation, rotation à faire
				if (_pNoeud->ObtenirIE() == -2) {
					// Validation enfant gauche existant...
					if (_pNoeud->ObtenirEG() != nullptr) {
						// Validation double rotation...
						if (_pNoeud->ObtenirEG()->ObtenirIE() == 1) {
							// Double rotation.
							RotationGauche(_pNoeud->ObtenirEG(), _boRetirerEnCours);
							RotationDroite(_pNoeud, _boRetirerEnCours);
						}
						else
							RotationDroite(_pNoeud, _boRetirerEnCours);
					}
				}
				else
				{
					// Validation jusqu'à un débalencement...
					if (_pNoeud->ObtenirIE() == 0 && _pNoeud != m_pRacine) {
						if (_pNoeud->ObtenirID() == "") {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirElement() > _pNoeud->ObtenirParent()->ObtenirElement())
								ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirElement() < _pNoeud->ObtenirParent()->ObtenirElement())
									ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);
						}

						else {
							// Si l'élément est plus grand que son parent...
							if (_pNoeud->ObtenirID() > _pNoeud->ObtenirParent()->ObtenirID())
								ModificationIndiceEquilibreDroit(_pNoeud->ObtenirParent(), _boRetirerEnCours);
							else
								if (_pNoeud->ObtenirID() < _pNoeud->ObtenirParent()->ObtenirID())
									ModificationIndiceEquilibreGauche(_pNoeud->ObtenirParent(), _boRetirerEnCours);

						}
					}
				}
			}
		}
	}

	/*
	Fonction qui retourne le plus grand élément de l'enfant gauche de la racine d'un sous-arbre.
	Prend une pointeur noeud qui sera la racine.
	*/
	T ObtenirMax(CNoeudArbreAVL<T>* _pRacineSousArbre) {
		_pRacineSousArbre = _pRacineSousArbre->ObtenirEG();
		// Tant qu'on peut aller à l'enfant droit, qui est plus grand...
		while (_pRacineSousArbre->ObtenirED() != nullptr)
			_pRacineSousArbre = _pRacineSousArbre->ObtenirED();
		T Retour = _pRacineSousArbre->ObtenirElement();
		Retirer(Retour); // On retirer l'élément avec lequel on remplace...
		return Retour;
	}

public:

	// Constructeur...

	CArbreAVL() {
		m_pRacine = nullptr;
		m_uiCompte = 0;
	}

	// Destructeur...

	~CArbreAVL() {
		ParcoursDestructeur();
	}

	// Méthodes public ...
	
	// Procédure qui parcours les contrôles pour ShowControl...
	void ParcoursMenu(SDL_Renderer* _pSDLRendererParcours) {
		if (m_pRacine != nullptr) {
			ParcoursMenu(m_pRacine, _pSDLRendererParcours); // Parcour des contrôles.
		}
	}

	// Procédure qui parcours les contrôles pour ShowControl...
	void ParcoursControl(SDL_Renderer* _pSDLRendererParcours) {
		if (m_pRacine != nullptr) {
			ParcoursControl(m_pRacine, _pSDLRendererParcours); // Parcour des contrôles.
		}
	}
	
	// Procédure qui parcours les contrôles pour ReactToEvent...
	void ParcoursReaction(SDL_Event* _pSDLEvent) {
		if (m_pRacine != nullptr) {
			ParcoursReaction(m_pRacine, _pSDLEvent); // Parcour des contrôles.
		}
	}

	void ParcoursDestructeur() {
		if (m_pRacine != nullptr) {
			ParcoursDestructeur(m_pRacine);
		}
	}

	// Procédure détruisants toutes les textures...
	void ParcoursDestructionTexture() {
		if (m_pRacine != nullptr) {
			ParcoursDestructionTexture(m_pRacine);
		}
	}

	// Procédure détruisants toutes les surfaces...
	void ParcoursDestructionSurface() {
		if (m_pRacine != nullptr) {
			ParcoursDestructionSurface(m_pRacine);
		}
	}

	// Procédure détruisants toutes les fonts...
	void ParcoursDestructionFont() {
		if (m_pRacine != nullptr) {
			ParcoursDestructionFont(m_pRacine);
		}
	}

	/*
	Procédure permettant d'ajouter un Noeud dans l'arbre.
	Prend un élément de n'importe quel type comparable à ajouter dans le nouveau noeud.
	*/
	void Ajouter(T _ElementAjouter, string _strID) {
		// Vérifie si l'élément n'est pas déjà dans l'arbre...
		if (!Rechercher(_ElementAjouter)) {
			// Si le racine est nullptr...
			if (m_pRacine == nullptr) {
				m_pRacine = new CNoeudArbreAVL<T>(_ElementAjouter, _strID, nullptr, 0); // Création d'un objet en mémoire de tye Noeud et retour de son adresse mémoire dans m_pRacine.
			}
			else
				Ajouter(_ElementAjouter, _strID, m_pRacine);
			m_uiCompte++; // Incrémentation du compte
		}
	}

	/*
	Fonction permettant de rechercher un élément dans l'arbre.
	Prend un élément à rechercher et retourne vrai si l'élément à été trouvé sinon elle retourne faux.
	*/
	bool Rechercher(T _ElementRechercher) {
		// Si la racine n'est pas égale à nullptr...
		if (m_pRacine != nullptr) {
			// Si l'élément recherché est dans l'arbre...
			if (Rechercher(_ElementRechercher, m_pRacine) != nullptr)
				return true;
			else
				return false;
		}
		else
			return false;

	}

	/*
	Fonction permettant de rechercher un élément dans l'arbre.
	Prend un élément à rechercher et retourne vrai si l'élément à été trouvé sinon elle retourne faux.
	*/
	T ObtenirElement(string _strID) {
		// Si la racine n'est pas égale à nullptr...
		if (m_pRacine != nullptr) {

			return ObtenirElement(_strID, m_pRacine)->ObtenirElement();
		}
		else
			return NULL;

	}

	/*
	Procédure qui retirer un élément de l'arbre.
	Prend un élément à retirer.
	*/
	void Retirer(T _ElementRetirer) {
			// Voir si l'élément est dans l'arbre...
			if (Rechercher(_ElementRetirer)) {
				CNoeudArbreAVL<T>* pRunner = Rechercher(_ElementRetirer, m_pRacine); // Pointeur de type Noeud sur l'élément à retirer.
				// Si on retirer le dernier élément.
				if (m_uiCompte == 1) {
					m_pRacine = nullptr;
					delete pRunner->ObtenirElement();
					m_uiCompte--; // Décrémentation du compte...
				}
				else
				// Si le noeud n'a aucun enfant...
				if (pRunner->ObtenirED() == nullptr && pRunner->ObtenirEG() == nullptr) {
					// Si l'élément du noeud est plus grand que celui de son parent...
					if (pRunner->ObtenirElement() > pRunner->ObtenirParent()->ObtenirElement()) {
						ModificationIndiceEquilibreDroit(pRunner->ObtenirParent(), true);
						pRunner->ObtenirParent()->DefinirED(nullptr);
					}
					else
					{
						ModificationIndiceEquilibreGauche(pRunner->ObtenirParent(), true);
						pRunner->ObtenirParent()->DefinirEG(nullptr);
					}
					delete pRunner;
					m_uiCompte--;
				}
				else
				// S'il a un des deux enfants...
				if ((pRunner->ObtenirED() != nullptr) ^ (pRunner->ObtenirEG() != nullptr)) {
					// Si le retrait est à la racine...
					if (pRunner == m_pRacine) {
						// Si c'est une enfant droit...
						if (pRunner->ObtenirED() != nullptr) {
							m_pRacine = pRunner->ObtenirED();
							pRunner->ObtenirED()->DefinirParent(nullptr);
						}
						else
						{
							m_pRacine = pRunner->ObtenirEG();
							pRunner->ObtenirEG()->DefinirParent(nullptr);
						}
					}
					else
					// Si l'élément est plus grand que celui de son parent...
					if (pRunner->ObtenirElement() > pRunner->ObtenirParent()->ObtenirElement()) {
						// Si l'enfant droit n'est pas null...
						if (pRunner->ObtenirED() != nullptr) {
								pRunner->ObtenirParent()->DefinirED(pRunner->ObtenirED());
								pRunner->ObtenirED()->DefinirParent(pRunner->ObtenirParent());
						}
						else
						// Enfant gauche pas null...
						{
								pRunner->ObtenirParent()->DefinirED(pRunner->ObtenirEG());
								pRunner->ObtenirEG()->DefinirParent(pRunner->ObtenirParent());
						}
						ModificationIndiceEquilibreDroit(pRunner->ObtenirParent(),true);
					}
					else
					// Si l'élément est plus petit que celui de son parent...
					if (pRunner->ObtenirElement() < pRunner->ObtenirParent()->ObtenirElement()) {
						// Si l'enfant droit n'est pas null...
						if (pRunner->ObtenirED() != nullptr) {
								pRunner->ObtenirParent()->DefinirEG(pRunner->ObtenirED());
								pRunner->ObtenirED()->DefinirParent(pRunner->ObtenirParent());
						}
						else
						// Enfant gauche pas null...
						{
							pRunner->ObtenirParent()->DefinirEG(pRunner->ObtenirEG());
							pRunner->ObtenirEG()->DefinirParent(pRunner->ObtenirParent());
						}
						ModificationIndiceEquilibreGauche(pRunner->ObtenirParent(),true);
					}
					delete pRunner;
					m_uiCompte--;
				}
				else
				// S'il a deux enfant, on modifie son élément pour le plus grand de son arbre gauche...
				if (pRunner->ObtenirED() != nullptr && pRunner->ObtenirEG() != nullptr) {
					pRunner->DefinirElement(ObtenirMax(pRunner));
				}
			}
	}

	// Accesseur...

	unsigned int ObtenirCompte() {
		return m_uiCompte;
	}
};