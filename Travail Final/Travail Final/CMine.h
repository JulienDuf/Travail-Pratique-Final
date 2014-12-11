//Classe qui gere les mines.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Rect m_RectDestination; //position de la mine sur la map
	SDL_Rect m_RectExplosion; //Position de l'explosion
	SDL_Surface* m_pSurface; //Texture du pack
	SDL_Texture* m_pTexture; // Texture du pack
	CSprite* m_pSpriteExplosion; //Sprite de l'explosion de la mine
	bool m_boStable; //Si la mine touche par terre.
	double m_dAnglemine; //L'angle de la mine.

	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY);
	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY);

public:
	CMine(CGestionaire<SDL_Surface*>* _pGestionnaireSurface, SDL_Renderer* _Renderer, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY)){
		//initialisation de la texture.
		m_pSurface = _pGestionnaireSurface->ObtenirDonnee("pSurfaceMine");		
		m_pTexture = SDL_CreateTextureFromSurface(_Renderer, m_pSurface);
		
		
		//initialisation de la position de la mine
		m_RectDestination.h = m_pSurface->h;
		m_RectDestination.w = m_pSurface->w;
		m_RectDestination.y = 0;
		m_RectDestination.x = rand() % 1366;

		m_RectExplosion.h = 300;
		m_RectExplosion.w = 260;
		m_RectExplosion.x = m_RectDestination.x;
		m_RectExplosion.y = 0;

		m_pSpriteExplosion = new CSprite(_pGestionnaireSurface->ObtenirDonnee("pSurfaceExplosion"), SDL_CreateTextureFromSurface(_Renderer, _pGestionnaireSurface->ObtenirDonnee("pSurfaceExplosion")), m_RectExplosion, 9, 300, false, false, 1);

		m_pCollisionMap = _CollisionMap;
		m_pMapDestruction = _MapDestruction;

		m_boStable = false;
		m_dAnglemine = 0;
	}


	bool Use(CPlayer* _pPlayer) {

		if (_pPlayer != nullptr) 
			_pPlayer->SetHealth(0.0f);


		m_RectExplosion.x = (m_RectDestination.x + (m_RectDestination.w/2)) - (m_RectExplosion.w/2);
		m_RectExplosion.y = (m_RectDestination.y + m_RectDestination.h) - (m_RectExplosion.h/2);
		m_pSpriteExplosion->DefinirActif(true);
		m_pMapDestruction(45, m_RectDestination.x + m_RectDestination.w / 2, m_RectDestination.y + m_RectDestination.h);


		
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
			SDL_RenderCopyEx(_Renderer, m_pTexture, NULL, &m_RectDestination, m_dAnglemine, NULL, SDL_FLIP_NONE);
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

	SDL_Surface* GetSurface(){
		return m_pSurface;
	}

	SDL_Rect GetRectDestination(){
		return m_RectDestination;
	}

	bool IsStable(void) {

		return m_boStable;
	}

};