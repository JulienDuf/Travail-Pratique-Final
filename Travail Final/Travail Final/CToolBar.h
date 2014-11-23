// 420-202-RE : Travail final
// Classe d'une bo�te qui contient plusieurs objets s�lectionnables(un � la fois)
// Cr��e par Gabriel Beaudry le 18 novembre 2014(gabriel.bdry@gmail.com)

class CToolBar {
private:

	SDL_Texture* m_pTextureToolBar; // Texture du fond de la toolbar.
	CListeDC<SDL_Texture*>* m_pListeObjet; // Liste des textures repr�sentant les objet pour l'affichage.
	SDL_Rect m_RectToolBarDestination, // Rect de destination de la toolbar.
		m_RectTextureDestination; // Rect de destination de la ToolBar pour chaque texture.
	CScrollBar *m_pScrollBar; // Barre de d�filement permettant de manipuler l'affichage.

	unsigned int m_iPositionListe, // Position dans la liste de l'objet en s�lection.
		m_iPositionAffichage, // Position de l'affichage lors qu'il y a une scrollbar par rapport � la vraie taille de la toolbar et non celle affich�e.
		m_uiMinimumSpace; // Espace minimum entre deux objets

public:

	// Constructeur de ToolBar...
	// Param1: Rect de destination de la ToolBar, les objets sont sous forme de carr�, 
	//			donc le mieux est de donner un multiple du nombre d'objet que tu donne.
	// Param2: Couleur du fond de la ToolBar.
	// Param3: Renderer pour le fond.
	// Param4: Largeur voulue pour les objets.
	// Param5: Hauteur voulue pour les objets.
	// Param6: Espace minimum entre chaque objet.
	// Param7: Nombre de texture(objet) de la ToolBar.
	// Param8: Textures des objets de la ToolBar.
	CToolBar(SDL_Rect _RectDestination, SDL_Color _Color, SDL_Renderer* _pRenderer, unsigned int _uiObjectWidth, unsigned int _uiObjectHeight, unsigned int _uiMinimumSpace, unsigned int argc, ...) {

		m_RectToolBarDestination = _RectDestination;

		// Cr�ation et initialisation de la texture background de la ToolBar...
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
		
		m_iPositionListe = argc + 1;
		m_pListeObjet->AllerATrieur(0);

		// Initialisation de mon rect pour les textures individuelles...
		// Si la ToolBar est pleine il y a ajout d'une barre de d�filement...
		if ((_uiObjectWidth + _uiMinimumSpace) * m_pListeObjet->ObtenirCompte() + _uiMinimumSpace > m_RectToolBarDestination.w) {
			m_pScrollBar = new CScrollBar({ m_RectToolBarDestination.x, m_RectToolBarDestination.y + m_RectToolBarDestination.h - 18, m_RectToolBarDestination.w, 18 }, (_uiObjectWidth + _uiMinimumSpace) * m_pListeObjet->ObtenirCompte(), _pRenderer);
			m_RectTextureDestination.y = (m_RectToolBarDestination.h / 2 - _uiObjectHeight / 2) - 9;
		}
		else
			m_RectTextureDestination.y = (m_RectToolBarDestination.h / 2 - _uiObjectHeight / 2);
		m_RectTextureDestination.x = m_uiMinimumSpace;
		m_RectTextureDestination.w = _uiObjectWidth;
		m_RectTextureDestination.h = _uiObjectHeight;
	}

	// Fonction permettant d'obtenir la position de l'objet selectionn�...
	// En sortie:
	// La position de l'objet s�lectionn�.
	unsigned int ObtenirPositionObjetSelection() {
		return m_iPositionListe;
	}

	// Proc�dure permettant d'afficher la toolbar...
	// Param1: Renderer pour afficher.
	void ShowToolBar(SDL_Renderer* _pRenderer) {
		
		// Ajustement couleur s�lection.
		SDL_SetRenderDrawBlendMode(_pRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(_pRenderer, 51, 153, 255, 100);

		// Affichage de la ToolBar...
		SDL_RenderCopy(_pRenderer, m_pTextureToolBar, nullptr, &m_RectToolBarDestination);
		
		SDL_Rect Recttmp; // Rect de destination.
		// Boucle qui affiche toutes les textures repr�sentant les objet de la toolbar...
		for (int i = 0; i < m_pListeObjet->ObtenirCompte(); i++) {
			// Affichage avec la scrollbar diff�rent...
			if (m_pScrollBar != nullptr) {
				// Si la texture de la liste o� je suis rendu � afficher est dans la position d'affichage... 
				if ((m_RectTextureDestination.x >= m_iPositionAffichage) && (m_RectTextureDestination.x + m_RectTextureDestination.w) <= (m_iPositionAffichage + m_RectToolBarDestination.w)) {
					Recttmp = { m_RectTextureDestination.x - m_iPositionAffichage + m_RectToolBarDestination.x, m_RectTextureDestination.y + m_RectToolBarDestination.y, m_RectTextureDestination.w, m_RectTextureDestination.h };
					SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), nullptr, &Recttmp);
				}
				else
				{
					int iW, iH;
					SDL_QueryTexture(m_pListeObjet->ObtenirElementTrieur(), nullptr, nullptr, &iW, &iH);
					SDL_Rect Recttmp2; // Rect source pour le cropage.
					// V�rification si l'objet d�passe en partie � droite...
					if ((m_RectTextureDestination.x <= m_iPositionAffichage + m_RectToolBarDestination.w) && (m_RectTextureDestination.x + m_RectTextureDestination.w > m_iPositionAffichage + m_RectToolBarDestination.w)) {
						Recttmp = { m_RectTextureDestination.x - m_iPositionAffichage + m_RectToolBarDestination.x, m_RectTextureDestination.y + m_RectToolBarDestination.y, m_iPositionAffichage + m_RectToolBarDestination.w - m_RectTextureDestination.x, m_RectTextureDestination.h };
						Recttmp2 = { 0, 0, Recttmp.w * iW / m_RectTextureDestination.w, iH };
						SDL_RenderCopy(_pRenderer, m_pListeObjet->ObtenirElementTrieur(), &Recttmp2, &Recttmp);
					}
					else
					// V�rification si l'objet d�passe en partie � gauche...
					if ((m_RectTextureDestination.x + m_RectTextureDestination.w) >= m_iPositionAffichage && (m_RectTextureDestination.x < m_iPositionAffichage)) {
						Recttmp = { m_RectToolBarDestination.x, m_RectToolBarDestination.y + m_RectTextureDestination.y, m_RectTextureDestination.x + m_RectTextureDestination.w - m_iPositionAffichage, m_RectTextureDestination.h };
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
			// Si l'objet est s�lectionn�...
			if (i == m_iPositionListe) {
				SDL_RenderFillRect(_pRenderer, &Recttmp);
			}
			// Prochaine image
			m_pListeObjet->AllerSuivantTrieur();
			m_RectTextureDestination.x += m_RectTextureDestination.w + m_uiMinimumSpace;
		}
		m_RectTextureDestination.x = m_uiMinimumSpace; // Retour au d�but

		// Affichage de la scrollbar...
		if (m_pScrollBar != nullptr)
			m_pScrollBar->ShowScrollBar(_pRenderer);
	}

	// Proc�dure permettant de r�agir aux �v�nement de la toolbar(selection)...
	// Param1: Event de SDL.
	void ReactToEvent(SDL_Event* pEvent) {

		// Mise � jour de la position d'affichage et des events de la scrollingbar...
		m_iPositionAffichage = (float)m_pScrollBar->ReactToEvent(pEvent);

		// Switch pour g�rer les �v�nement de la toolbar...
		switch (pEvent->type) {
		case SDL_MOUSEBUTTONDOWN: // Click droit souris.
			// V�rification si le click de la souris est dans la toolbar...
			if ((pEvent->motion.x >= m_RectToolBarDestination.x && pEvent->motion.x <= (m_RectToolBarDestination.x + m_RectToolBarDestination.w)) && (pEvent->motion.y >= m_RectToolBarDestination.y && pEvent->motion.y <= (m_RectToolBarDestination.y + m_RectToolBarDestination.h))) {
				unsigned int uiX, uiY; // Positions X,Y pour la position de l'event click par rapport � la toolbar non pas par rapport � la window.

				// Ajustement du rectevent et de la position d'affichage si il y a une toolbar ou  non...
				if (m_pScrollBar != nullptr)
					uiX = pEvent->motion.x - m_RectToolBarDestination.x + m_iPositionAffichage;
				else
					uiX = pEvent->motion.x - m_RectToolBarDestination.x;
				uiY = pEvent->motion.y - m_RectToolBarDestination.y;

				// V�rification si le click est � la hauteur des textures dans la toolbar...
				if (uiY >= m_RectTextureDestination.y && uiY <= (m_RectTextureDestination.y + m_RectTextureDestination.h)) {
					m_iPositionListe = 0;
					// Boucle d'incr�mentation de la position de s�lection dans la liste jusqu'� ce qu'il n'y ait plus de modulo � faire...
					while (uiX >= (m_uiMinimumSpace + m_RectTextureDestination.w)) {
						m_iPositionListe++;
						uiX -= (m_uiMinimumSpace + m_RectTextureDestination.w);
					}
					// Si le click �tait entre des objets...
					if (uiX <= m_uiMinimumSpace) {
						m_iPositionListe = m_pListeObjet->ObtenirCompte() + 1; // D�selection
					}
				}
				else
				{
					m_iPositionListe = m_pListeObjet->ObtenirCompte() + 1; // D�selection
				}
			}
			else
			{
				m_iPositionListe = m_pListeObjet->ObtenirCompte() + 1; // D�selection
			}
			break;
		}	
	}
};