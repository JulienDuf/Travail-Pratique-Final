
// 420-202-RE : Travail final
// Classe qui représente un sprite.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
// Modifier le 18 novembre par Xavier St-Georges (xavierst-georges@hotmail.com)
// Modification : -Ajout de la procédure ModifierAnnimation.
//				  -Ajout d'accesseur.
//Fin des modifications.
class CSprite {

private:

	SDL_Surface* m_pSurfaceSprite;		// pointeur de surface SDL qui pointe la texture qui contient l'image du sprite.
	
	SDL_Rect m_RectSource;				// Rectangle qui représente la position et les dimensions du rectangle qui contiendra l'image que l'on veut afficher
	SDL_Rect m_RectDestination;			// Rectangle qui représente la position et les dimensions du rectangle ou on veut afficher l'image.

	CTimer* m_pTimerDelay;				// Pointeur de minuterie qui indique le temps de délai entre chaque cadre.

	unsigned int m_uiCurrentFrame;			// Variable entière positive qui indique le cadre ou l'on est rendu dans l'affichage du sprite.
	unsigned int m_uiNbrFrames;			// Variable Entière positive qui indique le nombre de cadres dans le sprite.

	bool m_boBoucle;					// Variable booléenne qui indique si le sprite doit continuer à boucler ( Si oui = true ).
	bool m_boActif;						// Variable booléenne qui indique si le sprite est en animation ( si oui = true ).
	bool m_boEnMvt;						// Variable booléenne qui indique si le sprite est en mouvemant ou non.

	char m_chrD;						// Variable qui indique la direction du mouvement.


public:


	// Constructeur...
	CSprite(SDL_Surface* _SurfaceSprite, unsigned int _uiNbrFrames, unsigned int _uiDelay) {

		m_pSurfaceSprite = _SurfaceSprite;		// La texture de notre sprite est entrée en paramètre.

		m_pTimerDelay = new CTimer(_uiDelay);		// Le délai de notre minuterie est entré en paramètre.
		m_pTimerDelay->Start();						// On démarre la minuterie.

		m_uiCurrentFrame = 0;					// On commence au premier cadre, donc à la position 0.
		m_uiNbrFrames = _uiNbrFrames;			// Le nombre de cadres est entré en paramètre.

		m_boBoucle = false;						// Par défaut l'animation ne boucle pas. ( faux mais on ne sent sert pas dans l'atelier... )
		m_boActif = false;						// Par défaut l'animation n'est pas active.  ( faux mais on ne s'en serts pas dans l'atelier... )
		m_boEnMvt = false;						// Par défaut l'animation n'est pas en mouvement.

		m_RectSource.w = m_pSurfaceSprite->w;
		m_RectSource.h = m_pSurfaceSprite->h;
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
		if (m_boActif) {
			SDL_Texture* pTextureTmp = SDL_CreateTextureFromSurface(_Renderer, m_pSurfaceSprite);
			SDL_RenderCopy(_Renderer, pTextureTmp, &m_RectSource, &m_RectDestination);		// On rends le cadre actuel dans la fenêtre.
			SDL_DestroyTexture(pTextureTmp);
		}

	}
	// Procédure permettant de modifier l'annimation à afficher.
	// Paramètre : chrD : La direction du mouvement de l'annimation. Option : 'G' : gauche, 'D': droite, 'H' : en haut.
	//			   _uiAnnimation: L'annimation qu'on veut qui joue.
	void ModifierAnnimation(char chrD, unsigned int _uiAnnimation) {
		if (m_boActif) { // Si l'annimation est active.
			if (chrD == 'N') { // Si l'annimation ne bouge pas.

				m_RectSource.x = 0;
				m_RectSource.y = _uiAnnimation * m_RectSource.h;
				m_uiCurrentFrame = 0;
			}

			else if (m_pTimerDelay->IsDone()) {			// Si la minuterie du délai est terminée on peut passer au prochain cadre.
				if (chrD == 'D') {
					m_chrD = 'D';
					if (!m_boEnMvt) {
						m_RectSource.x = 0;
						m_RectSource.y = _uiAnnimation * m_RectSource.h;
						m_uiCurrentFrame = 0;
						m_boActif = true;
						m_RectDestination.x += 15;

					}

					else if (m_boEnMvt) {
						m_uiCurrentFrame++;			// On va au prochain cadre.
						m_RectDestination.x += 15;
					}


				}
				else if (chrD == 'G') {
					m_chrD = 'G';
					if (!m_boEnMvt) {
						m_RectSource.x = m_RectSource.w * (m_uiNbrFrames - 1);
						m_RectSource.y = _uiAnnimation * m_RectSource.h;
						m_uiCurrentFrame = ((m_uiNbrFrames * 10000) - 1);
						m_boActif = true;
						m_RectDestination.x -= 15;

					}
					else if (m_boEnMvt) {
						m_uiCurrentFrame--;
						m_RectDestination.x -= 15;
					}

				}
				else if (chrD == 'H') {
					if (!m_boEnMvt) {
						m_RectSource.x = 0;
						m_RectSource.y = _uiAnnimation * m_RectSource.h;
						m_uiCurrentFrame = 0;
						m_boActif = true;

					}
					else if (m_boEnMvt) {
						m_uiCurrentFrame++;			// On va au prochain cadre.
						if (m_uiCurrentFrame >= 4 && m_uiCurrentFrame < 6) {
							m_RectDestination.x += 5;
							m_RectDestination.y -= 5;
						}


					}
				}
				m_RectSource.x = (m_uiCurrentFrame % m_uiNbrFrames) * m_RectSource.w;		// La position de notre rectangle source se modifie pour englober le prochain cadre.


				m_pTimerDelay->Start();			// on recommence la minuterie.

			}
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

	void DefinirDirection(char _chrD) {
		m_chrD = _chrD;
	}
	char ObtenirDirection(void) {
		return m_chrD;
	}
	bool IsSteady(void) {
		if (!m_boActif)
			return true;
		else
			return false;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSurfaceSprite;

	}
	void DefinirEnMvt(bool _enMvt) {
		m_boEnMvt = _enMvt;
	}

	//Destructeur...
	~CSprite(void) {

		delete m_pTimerDelay;

	}


};