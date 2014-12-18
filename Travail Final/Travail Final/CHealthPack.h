//Classe qui gere les packs de santé.
//crée le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com)


class CHealthPack: public CPack{
private:
	SDL_Rect m_RectDestination; //Position du pack sur la map
	SDL_Texture* m_pTexture; //Texture du pack
	SDL_Surface* m_pSurface; //Surface du pack
	bool m_boStable; //Si le pack touche par terre.
	bool m_boUse; // Si le pack est use.
	double m_dAnglePack; //L'angle de le pack.

public:
	//constructeur
	CHealthPack(){
		//initialisation de la texture
		m_pTexture = pGestionnaireTexture->ObtenirDonnee("pTextureHealthPack");
		m_pSurface = pGestionnaireSurface->ObtenirDonnee("pSurfaceHealthPack");

		//initialisation de la position du pack
		m_RectDestination.h = m_pSurface->h;
		m_RectDestination.w = m_pSurface->w;
		m_RectDestination.y = 0;
		m_RectDestination.x = rand() % (1366 - m_pSurface->w);

		m_boStable = false;
		m_dAnglePack = 0;
	}

	/*
	Utilisation du pack de vie
	parametre:
		_player: Joueur utilisant le pack.
	*/
	bool Use(CPlayer* _Player){
		_Player->SetHealth(_Player->GetHealth() + ((_Player->GetHealth())*0.25));

		if (_Player->GetHealth() > 1)
			_Player->SetHealth(1);

		m_boUse = true;

		return false;
	}

	/*
	Affichage du pack de vie sur la map a la position m_pRectDestination
	*/
	void ShowPack(SDL_Renderer* _Renderer){
		SDL_RenderCopyEx(_Renderer, m_pTexture, NULL, &m_RectDestination, m_dAnglePack, NULL, SDL_FLIP_NONE);
	}

	void ModifierPosition(SDL_Rect _RectDestination) {

		m_RectDestination = _RectDestination;
	}

	void ModifierStabilePack(bool _boStable) {

		m_boStable = _boStable;
	}

	void ModifierAnlge(double _dAngle) {

		m_dAnglePack = _dAngle;
	}

	void ModifierUse(bool _boUse) {

	}

	SDL_Surface* GetSurface(void) {

		return m_pSurface;
	}

	SDL_Rect GetRectDestination(void) {
		
		return m_RectDestination;
	}

	CSprite* GetSpriteExplosion(void) {

		return nullptr;
	}

	bool IsStable(void) {

		return m_boStable;
	}

	bool IsUse(void) {
		return m_boUse;
	}
};