//Classe qui gere les mines.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Rect m_pRectDestination; //position de la mine sur la map
	SDL_Surface* m_pSurface; //Texture du pack
	bool m_boCollision; // Si la mine touche par terre.
	bool m_boActivation; // Si la mine doit exploser

	void(*m_pCollisionMap)(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY);
	void(*m_pMapDestruction)(int _iRayon, int _iX, int _iY);
	//void(*m_pDetectionCollisionJoueur)(CListeDC<CTeam*>* _pTeamList, CPack* _Pack);

public:
	CMine(string _strSourceImage, SDL_Renderer* _Renderer, void _MapDestruction(int _iRayon, int _iX, int _iY), void _CollisionMap(SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY)){
		//initialisation de la texture.
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\mine.png");
		m_pSurface = IMG_Load(strSourceImage.c_str());

		//initialisation de la position de la mine
		m_pRectDestination.h = m_pSurface->h;
		m_pRectDestination.w = m_pSurface->w;
		m_pRectDestination.y = 0;
		m_pRectDestination.x = rand() % 1366;

		m_pCollisionMap = _CollisionMap;
		m_pMapDestruction = _MapDestruction;

		m_boCollision = false;
		m_boActivation = false;
	}


	/*
	Affiche la mine sur la  map a la postion m_pRectDestination
	*/
	void ShowPack(SDL_Renderer* _Renderer){
		
		if (!m_boCollision)
			ModifierPosition();
		SDL_Texture* pTexture = SDL_CreateTextureFromSurface(_Renderer, m_pSurface);
		SDL_RenderCopy(_Renderer, pTexture, NULL, &m_pRectDestination);
		SDL_DestroyTexture(pTexture);

	}

	void Use(){

	}

	void ModifierPosition(void) {

		int iX, iY;

		SDL_Rect RectTmp = m_pRectDestination;
		RectTmp.y += 9.8;
		
		m_pCollisionMap(m_pSurface, RectTmp, &iX, &iY);

		if (iX != 0 && iY != 0) {

			RectTmp.y -= ((RectTmp.y + RectTmp.h) - iY);
			m_pRectDestination = RectTmp;
			m_boCollision = true;
		}

		else
			m_pRectDestination = RectTmp;

	}

	SDL_Surface* GetSurface(){
		return m_pSurface;
	}

	SDL_Rect GetRectDestination(){
		return m_pRectDestination;
	}
};