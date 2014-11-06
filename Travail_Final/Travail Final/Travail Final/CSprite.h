
// 420-202-RE : Travail final
// Classe qui représente un sprite.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CSprite {

private:

	SDL_Texture* m_pTextureSprite;		// pointeur de texture SDL qui pointe la texture qui contient l'image du sprite.
	
	SDL_Rect m_RectSource;				// Rectangle qui représente la position et les dimensions du rectangle qui contiendra l'image que l'on veut afficher
	SDL_Rect m_RectDestination;			// Rectangle qui représente la position et les dimensions du rectangle ou on veut afficher l'image.

	CTimer* m_pTimerDelay;				// Pointeur de minuterie qui indique le temps de délai entre chaque cadre.

	unsigned int m_uiCurrentFrame;			// Variable entière positive qui indique le cadre ou l'on est rendu dans l'affichage du sprite.
	unsigned int m_uiNbrFrames;			// Variable Entière positive qui indique le nombre de cadres dans le sprite.

	bool m_boBoucle;						// Variable booléenne qui indique si le sprite doit continuer à boucler ( Si oui = true ).
	bool m_boActif;						// Variable booléenne qui indique si le sprite est en animation ( si oui = true ).



public:


	// Constructeur...
	CSprite(SDL_Texture* _TextureSprite, unsigned int _uiNbrFrames, unsigned int _uiDelay) {

		m_pTextureSprite = _TextureSprite;		// La texture de notre sprite est entrée en paramètre.

		m_pTimerDelay = new CTimer(_uiDelay);		// Le délai de notre minuterie est entré en paramètre.
		m_pTimerDelay->Start();						// On démarre la minuterie.

		m_uiCurrentFrame = 0;					// On commence au premier cadre, donc à la position 0.
		m_uiNbrFrames = _uiNbrFrames;			// Le nombre de cadres est entré en paramètre.

		m_boBoucle = false;						// Par défaut l'animation ne boucle pas. ( faux mais on ne sent sert pas dans l'atelier... )
		m_boActif = false;						// Par défaut l'animation n'est pas active.  ( faux mais on ne s'en serts pas dans l'atelier... )

		SDL_QueryTexture(_TextureSprite, NULL, NULL, &m_RectSource.w, &m_RectSource.h);		// On va chercher la largeur et la hauteur de notre image pour le mettre dans le rectangle source.
		m_RectSource.w = m_RectSource.w / m_uiNbrFrames;									// On mets la largeur de notre rectangle source égale à la largeur d'un cadre.
		m_RectSource.x = 0;								// On commence notre animation...
		m_RectSource.y = 0;								// Au premier cadre.

		m_RectDestination.x = 0;						// On va afficher l'image à gauche...
		m_RectDestination.y = 0;						// En haut de l'écran.
		m_RectDestination.w = m_RectSource.w;			// Les dimensions de ce que l'on affiche...
		m_RectDestination.h = m_RectSource.h;			// Seront égales à ceux d'un cadre de notre sprite.

	}

	// Procédure qui rends le sprite dans la fenêtre.
	// Prends en paramètre le rendeur de la fenêtre.
	void Render(SDL_Renderer* _Renderer) {

		if (m_pTimerDelay->IsDone()) {			// Si la minuterie du délai est terminée on peut passer au prochain cadre.

			m_uiCurrentFrame++;			// On va au prochain cadre.

			m_RectSource.x = (m_uiCurrentFrame % m_uiNbrFrames) * m_RectSource.w;		// La position de notre rectangle source se modifie pour englober le prochain cadre.

			m_pTimerDelay->Start();			// on recommence la minuterie.

		}

		SDL_RenderCopy(_Renderer, m_pTextureSprite, &m_RectSource, &m_RectDestination);		// On rends le cadre actuel dans la fenêtre.


	}

	//Destructeur...
	~CSprite(void) {

		delete m_pTimerDelay;

	}


};