//Classe qui gere les packs de santé.
//crée le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com)


class CHealthPack: public CPack{
private:
	SDL_Rect m_pPosition; //Position du pack sur la map
	SDL_Texture* m_pTexture; //Texture du pack
public:
	CHealthPack(const char* _Destination, SDL_Renderer* _Renderer){
		m_pTexture IMG_LoadTexture(_Renderer, _Destination + "\\Armes et Packs\\HealthPack.jpg");
		m_pPosition.h = 100;
		m_pPosition.w = 100;
		
	}

	void Use(CPlayer* _Player){
		_Player->SetHealth(_Player->GetHealth() + ((_Player->GetHealth())*0.25));
	}

	void Show(SDL_Renderer* _Renderer){

	}

};