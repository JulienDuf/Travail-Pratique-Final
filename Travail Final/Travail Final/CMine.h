//Classe qui gere les mines.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Rect m_pRectDestination; //position de la mine sur la map
	SDL_Texture* m_pTexture; //Texture du pack

public:
	CMine(string _strSourceImage, SDL_Renderer* _Renderer){
		//initialisation de la texture.
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\Mine.png");
		m_pTexture = IMG_LoadTexture(_Renderer, strSourceImage.c_str());

		//initialisation de la position de la mine
		m_pRectDestination.h = 59;
		m_pRectDestination.w = 15;
		m_pRectDestination.y = 0;
		m_pRectDestination.x = rand() % 1366;
	}

	/*void Use(CPlayer* _Player){
		_Player->SetHealth((_Player->GetHealth())*0.25);
	}*/


	/*
	Affiche la mine sur la  map a la postion m_pRectDestination
	*/
	void Show(SDL_Renderer* _Renderer){
		SDL_RenderCopy(_Renderer, m_pTexture, NULL, &m_pRectDestination);
	}

	
	/*
	Calcule la chute de la mine lors de son apparition.
	*/
	void ChuteDebut(){
		m_pRectDestination.y += 15;
	}
};