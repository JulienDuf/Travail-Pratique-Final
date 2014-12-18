

// 420-202-RE : Travail final
// Classe qui représente un sprite.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Modifier le 18 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Ajout de la procédure ModifierAnnimation.
//				  -Ajout d'accesseur.
// Modifier le 02 décembre 2014 pas Gabriel Beaudry (gabriel.bdry@gmail.com)
// -Ajout d'une variable et d'une méthode pour gérer la position sur laquelle le sprite boucle.
// -Modification à la fonction IsActif().
// Modifier le 03 décembre 2014 pas Gabriel Beaudry (gabriel.bdry@gmail.com)
// -Le Frame ne s'associe plus à 10000.
// -Ajout de Méthodes
//Fin des modifications.
class CSprite {

private:

	SDL_Surface* m_pSurfaceSprite;		// pointeur de surface SDL qui pointe la surface qui contient l'image du sprite.

	SDL_Texture* m_pTextureSprite;		// pointeur de texture SDL qui pointe sur la texture qui contient l'image du sprite.

	SDL_Rect m_RectSource;				// Rectangle qui représente la position et les dimensions du rectangle qui contiendra l'image que l'on veut afficher

	CTimer* m_pTimerDelay;				// Pointeur de minuterie qui indique le temps de délai entre chaque cadre.

	unsigned int m_uiEtage,				// Quel annimation on veut afficher.
		m_uiPositionBouclageDebut,      // La position de bouclage départ du sprite.
		m_uiPositionBouclageFin;        // La position de bouclage fin du sprite.

	bool m_boBoucle,					// Variable booléenne qui indique si le sprite doit continuer à boucler ( Si oui = true ).
		m_boActif;						// Variable booléenne qui indique si le sprite est en animation ( si oui = true ).

public:

	// Constructeur...
	// Paramètre :  _SurfaceSprite : Surface du sprite.
	//				_Texture : Texture du sprite.
	//			    _RectDestination : Destination du sprite.
	//			    _uiNbrFrames : Le nombre de cadres qu'à le sprite.
	//			    _uiDelay : Le delai avant de changer de frames donc la vitesse de l'annimation.
	//			    _boBoucle : Si le sprite est en boucle donc qu'il ne recommence pas l'animation.
	//				_boActif : Si le sprite est actif à sa création.
	//				_uiNbrEtage : Le nombre d'étage que comporte le sprite.
	CSprite(SDL_Surface* _SurfaceSprite, SDL_Texture* _Texture, SDL_Rect _RectDestination, unsigned int _uiNbrCadres, unsigned int _uiDelay, bool _boBoucle, bool _boActif, unsigned int _uiNbrEtage) {

		m_pSurfaceSprite = _SurfaceSprite;		// La texture de notre sprite est entrée en paramètre.

		m_pTextureSprite = _Texture;

		m_pTimerDelay = new CTimer(_uiDelay);		// Le délai de notre minuterie est entré en paramètre.
		m_pTimerDelay->Start();						// On démarre la minuterie.

		m_uiEtage = 0;
		m_uiPositionBouclageDebut = 0;
		m_uiPositionBouclageFin = _uiNbrCadres;

		m_boBoucle = _boBoucle;						// Par défaut l'animation ne boucle pas. ( faux mais on ne sent sert pas dans l'atelier... )
		m_boActif = _boActif;						// Par défaut l'animation n'est pas active.  ( faux mais on ne s'en serts pas dans l'atelier... )

		m_RectSource = { 0, 0, m_pSurfaceSprite->w / _uiNbrCadres, m_pSurfaceSprite->h / _uiNbrEtage };

	}

	//Destructeur...
	~CSprite(void) {

		delete m_pTimerDelay;

	}

	// Procédure qui rends le sprite dans la fenêtre.
	// Paramètres:
	// Param1; Renderer de la fenetre.
	// Param2: Rect de destination du sprite.
	void Render(SDL_Renderer* _Renderer, SDL_Rect _RectDestination) {

		if (m_boActif) {

			SDL_RenderCopy(_Renderer, m_pTextureSprite, &m_RectSource, &_RectDestination);		// On rends le cadre actuel dans la fenêtre.
			//SDL_RenderPresent(_Renderer);
		}
	}

	// Procédure permettant de déplacer le cadre d'affichage de un vers la droit où le retourne au départ voulue...
	// En sotrie : Si l'animation à finie(true) une boucle...
	bool ModifierAnnimation() {

		// Délai fini et sprite actif...
		if (m_pTimerDelay->IsDone() && m_boActif) {

			m_pTimerDelay->Start();
			m_RectSource.x += m_RectSource.w; // Déplacement du cadre.

			// Atteint la fin...
			if (m_RectSource.x >= (m_RectSource.w * m_uiPositionBouclageFin)) {

				// Bouclage...
				if (m_boBoucle) {

					m_RectSource.x = m_uiPositionBouclageDebut * m_RectSource.w; // Retour au départ voulu.
				}

				else {

					m_boActif = false;

				}
				
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	// Méthodes...

	void DefinirPositionDeBouclage(unsigned int _uiPositionBouclageDebut, unsigned int _uiPositionBouclageFin) {
		m_uiPositionBouclageDebut = _uiPositionBouclageDebut;
		m_uiPositionBouclageFin = _uiPositionBouclageFin;
	}

	void DefinirActif(bool _boActif) {

		m_boActif = _boActif;
	}

	void DefinirboBoucle(bool _boBoucle) {

		m_boBoucle = _boBoucle;
	}

	void DefinirEtage(unsigned int _uiEtage) {

		m_uiEtage = _uiEtage;
		m_RectSource.y = m_RectSource.h * m_uiEtage;
	}

	// Accesseur ...

	SDL_Rect ObtenirRectSource(void) {

		return m_RectSource;
	}

	unsigned int ObtenirEtage(void) {

		return m_uiEtage;
	}

	bool IsActif(void) {

		return m_boActif;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSurfaceSprite;
	}
};