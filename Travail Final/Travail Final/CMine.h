//Classe qui gere les mines.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Point m_PointRotation; // Point de rotation de la mine.
	SDL_Rect m_RectDestination; //position de la mine sur la map
	SDL_Rect m_RectExplosion; //Position de l'explosion
	SDL_Surface* m_pSurface; //Texture du pack
	SDL_Texture* m_pTexture; // Texture du pack
	CSprite* m_pSpriteExplosion; //Sprite de l'explosion de la mine
	bool m_boStable; //Si la mine touche par terre.
	bool m_boUse; // Si la mine explose.
	double m_dAnglemine; //L'angle de la mine.
	unsigned int m_uiRayon; // Rayon d'explosion de la mine...

	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY);

public:
	CMine(string _strEmplacement, SDL_Renderer* _Renderer, void _MapDestruction(int _iRayon, int _iX, int _iY)) {
		
		// Initialisation des stats en liens avec le pack...
		string strEmplacement(_strEmplacement);
		int i = strEmplacement.length();

		// Retour à debug...
		for (int i2 = 0; i2 < 2; i2++) {
			strEmplacement.resize(--i);
			while (strEmplacement[--i] != '\\');
			strEmplacement.resize(++i);
		}

		strEmplacement.append("Armes et Packs\\DescriptionMine.txt");
		ifstream FichierDescriptionMine;
		FichierDescriptionMine.open(strEmplacement);

		// Si le fichier s'est ouvert et qu'il y de quoi...
		if (FichierDescriptionMine.is_open() && FichierDescriptionMine.gcount() != 0) { // Description mine ouvrait, mais le getline donnait une violation d'accès. Weird.
			char chrtmp[5];
			FichierDescriptionMine.getline(chrtmp, 75);
			string strRayon;
			for (int j = 11; chrtmp[j] > 47 && chrtmp[j] < 58; j++) {
				strRayon += chrtmp[j];
			}
			// Initialisation des stats...
			m_uiRayon = SDL_atoi(strRayon.c_str());
		}
		else
		{
			m_uiRayon = 45;
		}
		FichierDescriptionMine.close();
		
		//initialisation de la texture.
		m_pSurface = pGestionnaireSurface->ObtenirDonnee("pSurfaceMine");		
		m_pTexture = pGestionnaireTexture->ObtenirDonnee("pTextureMine");
		
		
		//initialisation de la position de la mine
		m_RectDestination.h = m_pSurface->h;
		m_RectDestination.w = m_pSurface->w;
		m_RectDestination.y = 0;
		m_RectDestination.x = rand() % (1366 - m_pSurface->w);

		m_RectExplosion.h = 2 * m_uiRayon + 40;
		m_RectExplosion.w = 2 * m_uiRayon + 40;
		m_RectExplosion.x = m_RectDestination.x;
		m_RectExplosion.y = 0;

		m_PointRotation = { m_pSurface->w / 2, m_pSurface->h };

		m_pSpriteExplosion = new CSprite(pGestionnaireSurface->ObtenirDonnee("pSurfaceExplosionMissile"), pGestionnaireTexture->ObtenirDonnee("pTextureExplosionMissile"), m_RectExplosion, 9, 100, false, false, 1);
		
		m_pMapDestruction = _MapDestruction;

		m_boStable = false;
		m_dAnglemine = 0;
	}


	bool Use(CPlayer* _pPlayer) {

		if (_pPlayer != nullptr) 
			_pPlayer->SetHealth(0.0f);


		m_RectExplosion.x = (m_RectDestination.x + (m_RectDestination.w/2)) - (m_RectExplosion.w/2);
		m_RectExplosion.y = (m_RectDestination.y + m_RectDestination.h) - (m_RectExplosion.h/2) - 20;
		m_pSpriteExplosion->DefinirActif(true);
		m_pMapDestruction(m_uiRayon, m_RectDestination.x + m_RectDestination.w / 2, m_RectDestination.y + m_RectDestination.h);
		m_boUse = true;
		
		
		return true;
	}

	/*
	Affiche la mine sur la  map a la postion m_pRectDestination
	*/
	void ShowPack(SDL_Renderer* _Renderer){		
		if (m_pSpriteExplosion->IsActif()){
			m_pSpriteExplosion->ModifierAnnimation();
			m_pSpriteExplosion->Render(_Renderer, m_RectExplosion);
		}
		else{
			SDL_RenderCopyEx(_Renderer, m_pTexture, NULL, &m_RectDestination, m_dAnglemine, &m_PointRotation, SDL_FLIP_NONE);
			SDL_RenderDrawRect(_Renderer, &m_RectDestination);
		}
	}

	void ModifierPosition(SDL_Rect _RectDestination) {

		m_RectDestination = _RectDestination;

	}

	void ModifierStabilePack(bool _boStable) {

		m_boStable = _boStable;
	}

	void ModifierAnlge(double _dAngle) {

		m_dAnglemine = _dAngle;
	}

	void ModifierUse(bool _boUse) {

		m_boUse = _boUse;
	}

	unsigned int GetRayon() {
		return m_uiRayon;
	}

	SDL_Surface* GetSurface(){
		return m_pSurface;
	}

	SDL_Rect GetRectDestination(){
		return m_RectDestination;
	}

	CSprite* GetSpriteExplosion(void) {

		return m_pSpriteExplosion;
	}

	bool IsStable(void) {

		return m_boStable;
	}

	bool IsUse(void) {

		return m_boUse;
	}
};