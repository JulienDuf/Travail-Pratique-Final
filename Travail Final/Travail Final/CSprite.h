
// 420-202-RE : Travail final
// Classe qui représente un sprite.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Modifier le 18 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Ajout de la procédure ModifierAnnimation.
//				  -Ajout d'accesseur.
//Fin des modifications.
class CSprite {

private:

	SDL_Surface* m_pSurfaceSprite;		// pointeur de surface SDL qui pointe la surface qui contient l'image du sprite.

	SDL_Texture* m_pTextureSprite;		// pointeur de texture SDL qui pointe sur la texture qui contient l'image du sprite.
	
	SDL_Rect m_RectSource;				// Rectangle qui représente la position et les dimensions du rectangle qui contiendra l'image que l'on veut afficher
	SDL_Rect m_RectDestination;			// Rectangle qui représente la position et les dimensions du rectangle ou on veut afficher l'image.

	CTimer* m_pTimerDelay;				// Pointeur de minuterie qui indique le temps de délai entre chaque cadre.

	unsigned int m_uiCurrentFrame;			// Variable entière positive qui indique le cadre ou l'on est rendu dans l'affichage du sprite.
	unsigned int m_uiNbrFrames;			// Variable Entière positive qui indique le nombre de cadres dans le sprite.
	unsigned int m_uiAnimation;		// Quel annimation on veut afficher.

	bool m_boBoucle;					// Variable booléenne qui indique si le sprite doit continuer à boucler ( Si oui = true ).
	bool m_boActif;						// Variable booléenne qui indique si le sprite est en animation ( si oui = true ).
	

public:


	// Constructeur...
	// Paramètre :  _SurfaceSprite : Surface du sprite.
	//				_Texture : Texture du sprite.
	//			    _RectDestination : Destination du sprite.
	//			    _uiNbrFrames : Le nombre de frames qu'à le sprite.
	//			    _uiDelay : Le delai avant de changer de frames donc la vitesse de l'annimation.
	//			    _boBoucle : Si le sprite est en boucle donc qu'il ne recommence pas l'animation.
	//				_uiNbrEtage : Le nombre d'étage que comporte le sprite.
	//				_Renderer : Le renderer de la fenetre.
	// Retour : Rien.
	CSprite(SDL_Surface* _SurfaceSprite, SDL_Texture* _Texture, SDL_Rect _RectDestination, unsigned int _uiNbrFrames, unsigned int _uiDelay, bool _boBoucle, bool _boActif, unsigned int _uiNbrEtage) {

		m_pSurfaceSprite = _SurfaceSprite;		// La texture de notre sprite est entrée en paramètre.

		m_pTextureSprite = _Texture;

		m_pTimerDelay = new CTimer(_uiDelay);		// Le délai de notre minuterie est entré en paramètre.
		m_pTimerDelay->Start();						// On démarre la minuterie.

		m_uiCurrentFrame = 0;					// On commence au premier cadre, donc à la position 0.
		m_uiNbrFrames = _uiNbrFrames;			// Le nombre de cadres est entré en paramètre.
		m_uiAnimation = 0;

		m_boBoucle = _boBoucle;						// Par défaut l'animation ne boucle pas. ( faux mais on ne sent sert pas dans l'atelier... )
		m_boActif = _boActif;						// Par défaut l'animation n'est pas active.  ( faux mais on ne s'en serts pas dans l'atelier... )


		m_RectSource.w = m_pSurfaceSprite->w;
		m_RectSource.h = m_pSurfaceSprite->h / _uiNbrEtage;
		m_RectSource.w = m_RectSource.w / m_uiNbrFrames;									// On mets la largeur de notre rectangle source égale à la largeur d'un cadre.
		m_RectSource.x = 0;								// On commence notre animation...
		m_RectSource.y = 0;								// Au premier cadre.
		
		m_RectDestination = _RectDestination;
		m_RectDestination.w = m_RectSource.w;			// Les dimensions de ce que l'on affiche...
		m_RectDestination.h = m_RectSource.h;			// Seront égales à ceux d'un cadre de notre sprite.

	}

	// Procédure qui rends le sprite dans la fenêtre.
	// Prends en paramètre le rendeur de la fenêtre.
	void Render(SDL_Renderer* _Renderer, SDL_Rect _RectDestination) {
		if (m_boActif) {
			
			SDL_RenderCopy(_Renderer, m_pTextureSprite, &m_RectSource, &_RectDestination);		// On rends le cadre actuel dans la fenêtre.
			
		}

	}
	// Procédure permettant de modifier l'annimation à afficher.
	// Paramètre : chrD : La direction du mouvement de l'annimation. Option : 'G' : gauche, 'D': droite.
	//			   _uiAnnimation: L'annimation qu'on veut qui joue.
	void ModifierAnnimation(void) {
		//Dans ce cas c'est pour le saut.
		if (((m_uiCurrentFrame == m_uiNbrFrames - 1) && (!m_boBoucle) && m_uiAnimation == 0) || (m_RectSource.x == 0 && (!m_boBoucle) && m_uiAnimation == 1)) {
			m_boActif = false;
		}

		if (m_boActif && m_pTimerDelay->IsDone()) { // Si l'annimation est active.// Si la minuterie du délai est terminée on peut passer au prochain cadre.

			m_RectSource.y = m_uiAnimation * m_RectSource.h;


			if (m_uiAnimation == 0)			// Si c'est une animation qui va de gauche à droite.
				m_uiCurrentFrame++;			// On va au prochain cadre.

			else                            // Si c'est une animation qui va de droite à gauche.
				m_uiCurrentFrame--;				

			m_RectSource.x = (m_uiCurrentFrame % m_uiNbrFrames) * m_RectSource.w;		// La position de notre rectangle source se modifie pour englober le prochain cadre.


			m_pTimerDelay->Start();			// on recommence la minuterie.


		}
	}

	// Accesseur ...

	SDL_Rect ObtenirRectSource(void) {
		return m_RectSource;
	}
	SDL_Rect ObtenirRectDestination(void) {
		return m_RectDestination;
	}
	void DefinirActif(bool _boActif) {
		m_boActif = _boActif;
		
	}

	unsigned int ObtenirAnimation(void) {

		return m_uiAnimation;

	}
	// Sert a definir quel annimation dans le sprite.
	// _uiAnimation c'est sois 0 ou 1.
	void DefinirAnimation(unsigned int _uiAnimation) {
		m_uiAnimation = _uiAnimation;
		if (m_uiAnimation == 0) {
			m_uiCurrentFrame = 0;
			m_RectSource.x = 0;
		}
		else {
			m_RectSource.x = m_RectSource.w * (m_uiNbrFrames - 1);
			m_uiCurrentFrame = ((m_uiNbrFrames * 10000));
		}
	}

	bool IsActif(void) {
		if (m_boActif)
			return true;
		else
			return false;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSurfaceSprite;

	}
	

	//Destructeur...
	~CSprite(void) {

		delete m_pTimerDelay;

	}


};