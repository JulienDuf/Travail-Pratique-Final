// 420-202-RE : Travail final
// Classe d'une boîte qui contient plusieurs objets sélectionnables(un à la fois)
// Créée par Gabriel Beaudry le 18 novembre 2014(gabriel.bdry@gmail.com)
// 02 décembre 2014, Gabriel Beaudry
// -Ajout d'une variable m_uiPositionListe pour le double-click.

class CToolBar {
private:

	SDL_Texture* m_pTextureToolBar; // Texture du fond de la toolbar.
	CListeDC<SDL_Texture*>* m_pListeObjet; // Liste des textures représentant les objet pour l'affichage.
	SDL_Rect m_RectToolBarDestination, // Rect de destination de la toolbar.
		m_RectTextureDestination; // Rect de destination de la ToolBar pour chaque texture.
	CScrollBar *m_pScrollBar; // Barre de défilement permettant de manipuler l'affichage.
	CTimer *m_pTimerClick; // Timer pour le double click.

	unsigned int m_uiPositionSelection, // Position dans la liste de l'objet en sélection.
		m_uiPositionListe, // Position dans la liste du double-click.
		m_uiPositionAffichage, // Position de l'affichage lors qu'il y a une scrollbar par rapport à la vraie taille de la toolbar et non celle affichée.
		m_uiMinimumSpace, // Espace minimum entre deux objets
		m_uiPositionHover; // Position de hovering

	bool m_boShow; // Booléen qui dit si la toolbar s'affiche.
	bool m_boClick;

public:

	// Constructeur de ToolBar...
	// Param1: Rect de destination de la ToolBar, les objets sont sous forme de carré, 
	//			donc le mieux est de donner un multiple du nombre d'objet que tu donne.
	// Param2: Couleur du fond de la ToolBar.
	// Param3: Renderer pour le fond.
	// Param4: Largeur voulue pour les objets.
	// Param5: Hauteur voulue pour les objets.
	// Param6: Espace minimum entre chaque objet.
	// Param7: Nombre de texture(objet) de la ToolBar.
	// Param8: Textures des objets de la ToolBar.
	CToolBar(SDL_Rect _RectDestination, SDL_Color _Color, SDL_Renderer* _pRenderer, unsigned int _uiObjectWidth, unsigned int _uiObjectHeight, unsigned int _uiMinimumSpace, unsigned int argc, ...) {

		m_pTimerClick = new CTimer(500);

		m_boShow = false;
		m_boClick = false;

		m_RectToolBarDestination = _RectDestination;

		// Création et initialisation de la texture background de la ToolBar...
		SDL_Surface* pSurfacetmp = SDL_CreateRGBSurface(NULL, m_RectToolBarDestination.w, m_RectToolBarDestination.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pSurfacetmp, NULL, SDL_MapRGB(pSurfacetmp->format, _Color.r, _Color.g, _Color.b));
		m_pTextureToolBar = SDL_CreateTextureFromSurface(_pRenderer, pSurfacetmp);
		SDL_FreeSurface(pSurfacetmp);

		m_uiMinimumSpace = _uiMinimumSpace;

		// Creation et initialisation de la liste d'objet...
		m_pListeObjet = new CListeDC<SDL_Texture*>();

		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pListeObjet->AjouterFin(va_arg(parametres, SDL_Texture*));
		}
		va_end(parametres);
		
		m_uiPositionSelection = argc;
		m_uiPositionListe = argc;
		m_pListeObjet->AllerATrieur(0);

		// Initialisation de mon rect pour les textures individuelles...
		// Si la ToolBar est pleine il y a ajout d'une barre de défilement...
		if ((_uiObjectWidth + _uiMinimumSpace) * m_pListeObjet->ObtenirCompte() + _uiMinimumSpace > m_RectToolBarDestination.w) {
			m_pScrollBar = new CScrollBar({ m_RectToolBarDestination.x, m_RectToolBarDestination.y + m_RectToolBarDestination.h - 18, m_RectToolBarDestination.w, 18 }, (_uiObjectWidth + _uiMinimumSpace) * m_pListeObjet->ObtenirCompte(), _pRenderer);
			m_RectTextureDestination.y = (m_RectToolBarDestination.h / 2 - _uiObjectHeight / 2) - ((m_RectToolBarDestination.h - _uiObjectHeight) / 2);
		}
		else
			m_RectTextureDestination.y = (m_RectToolBarDestination.h / 2 - _uiObjectHeight / 2);
		m_RectTextureDestination.x = m_uiMinimumSpace;
		m_RectTextureDestination.w = _uiObjectWidth;
		m_RectTextureDestination.h = _uiObjectHeight;
	}

	// Fonction permettant d'obtenir la position de l'objet selectionné...
	// En sortie:
	// La position de l'objet sélectionné.
	unsigned int ObtenirPositionObjetSelection() {
		return m_uiPositionListe;
	}

	// Procédure permettant d'afficher la toolbar...
	// Param1: Renderer pour afficher.
	void ShowToolBar(SDL_Renderer* _pRenderer) {

		if (m_boShow) {
			// Ajustement couleur sélection.
			SDL_SetRenderDrawBlendMode(_pRenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(_pRenderer, 51, 153, 255, 100);

			// Affichage de la ToolBar...
			SDL_RenderCopy(_pRenderer, m_pTextureToolBar, nullptr, &m_RectToolBarDestination);

			SDL_Rect Recttmp; // Rect de destination.
			// Boucle qui affiche toutes les textures représentant les objet de la toolbar...
			for (int i = 0; i < m_pListeObjet->ObtenirCompte(); i++) {
				// Affichage avec la scrollbar différent...
				if (m_pScrollBar != nullptr) {
					// Si la texture de la liste où je suis rendu à afficher est dans la position d'affichage... 
					if ((m_RectTextureDestination.x >= m_uiPositionAffichage) && (m_RectTextureDestination.x + m_RectTextureDestination.w) <= (m_uiPositionAffichage + m_RectToolBarDestination.w)) {
						Recttmp = { m_RectTextureDestination.x - m_uiPositionAffichage + m_RectToolBarDestination.x, m_RectTextureDestination.y + m_RectToolBarDestination.y, m_RectTextureDestination.w, m_RectTextureDestination.h };
						SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), nullptr, &Recttmp);
					}
					else
					{
						int iW, iH;
						SDL_QueryTexture(m_pListeObjet->ObtenirElementTrieur(), nullptr, nullptr, &iW, &iH);
						SDL_Rect Recttmp2; // Rect source pour le cropage.
						// Vérification si l'objet dépasse en partie à droite...
						if ((m_RectTextureDestination.x <= m_uiPositionAffichage + m_RectToolBarDestination.w) && (m_RectTextureDestination.x + m_RectTextureDestination.w > m_uiPositionAffichage + m_RectToolBarDestination.w)) {
							Recttmp = { m_RectTextureDestination.x - m_uiPositionAffichage + m_RectToolBarDestination.x, m_RectTextureDestination.y + m_RectToolBarDestination.y, m_uiPositionAffichage + m_RectToolBarDestination.w - m_RectTextureDestination.x, m_RectTextureDestination.h };
							Recttmp2 = { 0, 0, Recttmp.w * iW / m_RectTextureDestination.w, iH };
							SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), &Recttmp2, &Recttmp);
						}
						else
						// Vérification si l'objet dépasse en partie à gauche...
						if ((m_RectTextureDestination.x + m_RectTextureDestination.w) >= m_uiPositionAffichage && (m_RectTextureDestination.x < m_uiPositionAffichage)) {
							Recttmp = { m_RectToolBarDestination.x, m_RectToolBarDestination.y + m_RectTextureDestination.y, m_RectTextureDestination.x + m_RectTextureDestination.w - m_uiPositionAffichage, m_RectTextureDestination.h };
							Recttmp2 = { (m_RectTextureDestination.w - Recttmp.w)* iW / m_RectTextureDestination.w, 0, Recttmp.w * iW / m_RectTextureDestination.w, iH };
							SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), &Recttmp2, &Recttmp);
						}
					}
				}
				else
				{
					Recttmp = { m_RectTextureDestination.x + m_RectToolBarDestination.x, m_RectTextureDestination.y + m_RectToolBarDestination.y, m_RectTextureDestination.w, m_RectTextureDestination.h };
					SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), nullptr, &Recttmp);
				}
				// Si l'objet est sélectionné...
				if (i == m_uiPositionSelection) {
					SDL_RenderFillRect(_pRenderer, &Recttmp);
				}
				// Prochaine image
				m_pListeObjet->AllerSuivantTrieur();
				m_RectTextureDestination.x += m_RectTextureDestination.w + m_uiMinimumSpace;
			}
			m_RectTextureDestination.x = m_uiMinimumSpace; // Retour au début

			// Affichage de la scrollbar...
			if (m_pScrollBar != nullptr)
				m_pScrollBar->ShowScrollBar(_pRenderer);
		}
	}

	// Procédure permettant de réagir aux événement de la toolbar(selection)...
	// Param1: Event de SDL.
	void ReactToEvent(SDL_Event* _pEvent, CPlayer* _pPlayerActif) {
		
		if (m_boShow) {
			if (_pEvent->motion.x >= 0 && _pEvent->motion.x <= 1366)
				int i = 0;
				// Vérification si le click de la souris est dans la toolbar...
				if ((_pEvent->motion.x >= m_RectToolBarDestination.x && _pEvent->motion.x <= (m_RectToolBarDestination.x + m_RectToolBarDestination.w)) && (_pEvent->motion.y >= m_RectToolBarDestination.y && _pEvent->motion.y <= (m_RectToolBarDestination.y + m_RectToolBarDestination.h))) {
					unsigned int uiX, uiY; // Positions X,Y pour la position de l'event click par rapport à la toolbar non pas par rapport à la window.

					// Ajustement du rectevent et de la position d'affichage si il y a une toolbar ou  non...
					if (m_pScrollBar != nullptr) {
						// Mise à jour de la position d'affichage et des events de la scrollingbar...
						m_uiPositionAffichage = (float)m_pScrollBar->ReactToEvent(_pEvent);
						uiX = _pEvent->motion.x - m_RectToolBarDestination.x + m_uiPositionAffichage;
					}
					else
						uiX = _pEvent->motion.x - m_RectToolBarDestination.x;
					uiY = _pEvent->motion.y - m_RectToolBarDestination.y;

					// Vérification si le click est à la hauteur des textures dans la toolbar...
					if (uiY >= m_RectTextureDestination.y && uiY <= (m_RectTextureDestination.y + m_RectTextureDestination.h)) {
						m_uiPositionSelection = 0;
						// Boucle d'incrémentation de la position de sélection dans la liste jusqu'à ce qu'il n'y ait plus de modulo à faire...
						while (uiX >= (m_uiMinimumSpace + m_RectTextureDestination.w)) {
							m_uiPositionSelection++;
							uiX -= (m_uiMinimumSpace + m_RectTextureDestination.w);
						}
						// Si le click était entre des objets...
						if (uiX <= m_uiMinimumSpace) {
							m_uiPositionSelection = m_pListeObjet->ObtenirCompte() + 1; // Déselection
							_pPlayerActif->DefinirMissileShowDescription(false);
							_pPlayerActif->DefinirJetPackShowDescription(false);
						}
						else
						{
							if (_pEvent->type == SDL_MOUSEMOTION) {
								switch (m_uiPositionSelection) {
								case 0:

									_pPlayerActif->DefinirMissileShowDescription(true);
									break;
								case 1:

									break;
								case 2:

									break;
								case 3:
									_pPlayerActif->DefinirJetPackShowDescription(true);
									break;
								}
								m_uiPositionHover = m_uiPositionSelection;
								m_uiPositionSelection = m_pListeObjet->ObtenirCompte() + 1; //Déselection
							}
							else {
								if (!m_pTimerClick->IsDone() && m_boClick) {
									m_boShow = false;
									m_uiPositionListe = m_uiPositionSelection;
								}
								else
								{
									m_pTimerClick->Start();
									m_boClick = true;
								}
							}
						}
					}
					else
					{
						_pPlayerActif->DefinirMissileShowDescription(false);
						_pPlayerActif->DefinirJetPackShowDescription(false);
						m_uiPositionSelection = m_pListeObjet->ObtenirCompte() + 1; // Déselection
					}
				}
				else
				{
					_pPlayerActif->DefinirMissileShowDescription(false);
					_pPlayerActif->DefinirJetPackShowDescription(false);
					m_uiPositionSelection = m_pListeObjet->ObtenirCompte() + 1; // Déselection
				}
		}
		else
		{
			_pPlayerActif->DefinirMissileShowDescription(false);
			_pPlayerActif->DefinirJetPackShowDescription(false);
		}
	}

	void ReverseboShow() {
		m_boShow = !m_boShow;
		m_uiPositionSelection = m_pListeObjet->ObtenirCompte();
	}
};