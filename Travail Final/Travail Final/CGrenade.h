/*
Classe qui gere les grenades et leur rebondissement
Crée par Samuel Rambaud le 25 novembre 2014
Modifiée par Gabriel Beaudry
-Ajout de la description(BlitText, ModifierMunition, Load de la description, ShowDescription)
-Ajout du ReactToEvent et ShowProjectile (Celles que Julien avait fait dans CMissile, modifiées)
-Ajout des méthodes et accesseurs:
DefinirActif
ObtenirSprite
ObtenirMunition
UpdateDescription
ObtenirVecteurVitesse
ObtenirRectDestination
ObtenirSurface
DefinirAngle
DefinirPosition
EstLancer
*/

class CGrenade: public CProjectile {
private:

	bool m_boGrenadeLancer,
		m_boDirectionRotation, // Sens de la rotation dépendant de la direction du lancer
		m_boRotation,
		m_boShowDescription;

	int m_iAngle; // Angle du lancement de la grenade

	unsigned int m_uiForce, // Force du lancement de la grenade
		m_uiMunition;

	CBarrePuissance *m_pBarrePuissance; // Barre de puissance de la grenade
	CVecteur2D *m_pVecteurVitesseGrenade;

	CTimer* m_pTimerExplosion;

	SDL_Point m_PointRotation;
	SDL_Texture *m_pTextureGrenade; //Texture de la grenade
	SDL_Surface *m_pSurfaceGrenade, // Surface de base
		*m_pSurfaceGrenadeRotation; // Surface d'affichage
	SDL_Rect m_RectDestinationGrenade; 

	// Pointeurs de fonciton
	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY); // La destruction de la map.
	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY); // Procédure déterminant
	SDL_Surface *(*m_pRotation)(SDL_Surface* _pSurfaceRotation, float _fAngle); // Rotation

	CLabel *m_pLblDescription; // La descripton du missile.
	string m_strDescription[8];

	// Fonction qui permet de blit du texte dans une Surface de couleur
	// Param1; Tableau de string.
	// Param2: Nombre d'élément dans le tableau.
	// Param3: Couleur de la surface dans laquelle on blit le texte.
	// Retourne la surface finie.
	SDL_Surface *BlitText(string _strTexte[], unsigned int _uiNombreElementTableau, SDL_Color _Couleur) {

		SDL_Surface *pSurfaceBlitin,
			*pSurfaceBlitSource;

		SDL_Rect Rect = { 0, 0, 0, 0 };

		pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[0].c_str(), { 0, 0, 0 });
		unsigned int uiH = pSurfaceBlitSource->h;

		pSurfaceBlitin = SDL_CreateRGBSurface(pSurfaceBlitSource->flags, 300, pSurfaceBlitSource->h * _uiNombreElementTableau, pSurfaceBlitSource->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_FillRect(pSurfaceBlitin, NULL, SDL_MapRGB(pSurfaceBlitin->format, 255, 255, 255));

		SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);

		for (int i = 1; i < _uiNombreElementTableau; i++) {
			pSurfaceBlitSource = TTF_RenderText_Blended(pGestionnaireFont->ObtenirDonnee("pFontDescription"), _strTexte[i].c_str(), { 0, 0, 0 });
			Rect.y = uiH * i;
			SDL_BlitSurface(pSurfaceBlitSource, NULL, pSurfaceBlitin, &Rect);
		}

		return pSurfaceBlitin;
	}

	char chr[3]; // Char pour mettre à jour les munitions, s'il n'était pas en variable de classe, le jeu plantait.

	// Procédure permettant de mettre à jour le nombre de munition dans le label de description.
	// Param1; Renderer pour mettre à jour la surface.
	void MiseajourMunition(SDL_Renderer* _pRenderer) {
		SDL_itoa(m_uiMunition, chr, 10);
		m_strDescription[0] = "";
		m_strDescription[0].append("Munition : ");
		m_strDescription[0].append(chr);
		m_pLblDescription->ModifierTexture(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 7, { 0, 0, 0 })), 0);
	}

public:

	// Constructeur de CGrenade
	// Param1; Emplacement dans le chemin d'arborescence.
	// Param2: Render pour créer la texture du label.
	// Param3: Procédure de la destruction de la map.
	// Param4: Procédure de la collision avec la map.
	// Param5: Fonction pour la rotation de la grenade.
	CGrenade(string _strEmplacement, SDL_Renderer* _pRenderer, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY), SDL_Surface* _Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle)){

		m_boShowDescription = false;
		m_boRotation = false;
		m_boGrenadeLancer = false;

		m_iAngle = 0;
		m_uiForce = 0;

		// Initialisation de la description de l'arme et des munitions...
		m_strDescription;
		string strEmplacement(_strEmplacement);
		string strMunition;
		int i = strEmplacement.length();

		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionGrenade.txt");
		ifstream FichierDescription;
		FichierDescription.open(strEmplacement);
		if (FichierDescription.is_open()) {
			char chrtmp[55];
			FichierDescription.getline(chrtmp, 75);
			for (int i = 11; chrtmp[i] != -52; i++) {
				strMunition += chrtmp[i];
			}
			m_uiMunition = SDL_atoi(strMunition.c_str());
			m_strDescription[0] = chrtmp;
			for (int i = 1; i < 8; i++) {
				FichierDescription.getline(chrtmp, 75);
				m_strDescription[i] = chrtmp;
			}
		}

		FichierDescription.close();

		m_pLblDescription = new CLabel(SDL_CreateTextureFromSurface(_pRenderer, BlitText(m_strDescription, 7, { 0, 0, 0 })), { 0, 0, 0, 0 });

		m_pSurfaceGrenadeRotation = m_pSurfaceGrenade = pGestionnaireSurface->ObtenirDonnee("pSurfaceGrenade");

		m_pTextureGrenade = SDL_CreateTextureFromSurface(_pRenderer, m_pSurfaceGrenade);

		m_RectDestinationGrenade = { 0, 0, m_pSurfaceGrenade->w, m_pSurfaceGrenade->h };

		m_pBarrePuissance = new CBarrePuissance();

		m_pTimerExplosion = new CTimer(5000);

		m_pMapDestruction = _MapDestruction;
		m_pCollisionMap = _CollisionMap;
		m_pRotation = _Rotation;

		m_PointRotation = { m_RectDestinationGrenade.w, m_RectDestinationGrenade.h / 2 };
	}
	
	void ReactionColision(int iX, int iY) {
		m_pMapDestruction(50, iX, iY);
		m_boGrenadeLancer = false;

		delete m_pVecteurVitesseGrenade;
		m_pVecteurVitesseGrenade = nullptr;
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
	
		if (m_boGrenadeLancer) {
			//if (m_pTimerExplosion->IsDone()) {
				if (m_boRotation) {
					if (m_boDirectionRotation)
						m_iAngle = m_iAngle + 1 % 360;
					else {
						m_iAngle--;
						if (m_iAngle <= -360)
							m_iAngle = 0;
					}
					m_pSurfaceGrenadeRotation = m_pRotation(m_pSurfaceGrenade, m_iAngle);
					m_pTextureGrenade = SDL_CreateTextureFromSurface(_pRenderer, m_pSurfaceGrenadeRotation);
				}
				SDL_RenderCopy(_pRenderer, m_pTextureGrenade, NULL, &m_RectDestinationGrenade);
			//}
			//else
			//{
				// Explosion
			//}
		}
		else {

			_RectPlayerDestination.x -= 22;
			_RectPlayerDestination.y -= 10;
			m_pBarrePuissance->AfficherBarre(_pRenderer, _RectPlayerDestination);
		}
	}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription) {
			MiseajourMunition(_pRenderer);
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}
	
	void ReactToEvent(SDL_Event* _pEvent) {
	
		m_pBarrePuissance->ReactToEvent(_pEvent);

		if (!m_boGrenadeLancer) {

			switch (_pEvent->type) {

			case SDL_KEYDOWN:

				if (_pEvent->key.keysym.scancode == SDL_SCANCODE_SPACE) {

					m_iAngle = m_pBarrePuissance->ObtenirAngle();

					if (m_iAngle <= 90 || m_iAngle >= 270)
						m_boDirectionRotation = false;
					else
						m_boDirectionRotation = true;

					m_uiForce = (m_pBarrePuissance->ObtenirForce() + 3) * 100;
					m_boGrenadeLancer = true;
					m_pVecteurVitesseGrenade = new CVecteur2D((float)m_uiForce, (float)m_iAngle);

					m_pBarrePuissance->ObtenirPosition(&m_RectDestinationGrenade.x, &m_RectDestinationGrenade.y);
					m_RectDestinationGrenade.y -= m_RectDestinationGrenade.h;
					m_pTimerExplosion->Start();
				}
				break;
			}
		}
	}

	void DefinirActif(bool _boActif) {
		
	}

	CSprite* ObtenirSprite(string _strNom) { 
		
		return nullptr; 
	}

	unsigned int ObtenirMunition() {

		return m_uiMunition; 
	}

	void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) {

		m_boShowDescription = _boShow;
		int uiW,
			uiH;
		m_pLblDescription->GetTextureDimension(0, &uiW, &uiH);
		if (_RectPositionDescription.x + uiW >= 1366)
			m_pLblDescription->SetRectDestinationX(_RectPositionDescription.x - uiW);
		else
			m_pLblDescription->SetRectDestinationX(_RectPositionDescription.x);

		if (_RectPositionDescription.y + uiH >= 768)
			m_pLblDescription->SetRectDestinationY(_RectPositionDescription.y - uiH);
		else
			m_pLblDescription->SetRectDestinationY(_RectPositionDescription.y);
	}

	CVecteur2D* ObtenirVecteurVitesse() {

		return m_pVecteurVitesseGrenade;
	}

	SDL_Rect* ObtenirRectDestination() {

		return &m_RectDestinationGrenade;
	}

	SDL_Surface* ObtenirSurface(void) {

		return m_pSurfaceGrenadeRotation;
	}

	void DefinirAngle(double _dAngle) {


	}

	void DefinirPosition(SDL_Rect _Rect) {

		m_RectDestinationGrenade = _Rect;
	}

	bool EstLancer() {

		return m_boGrenadeLancer;
	}
};