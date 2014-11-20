//Classe qui gere les packs de santé.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Rect m_pPosition; //position de la mine sur la map
	SDL_Texture* m_pTexture; //Texture du pack

public:
	CMine(string* _SourceImage, SDL_Renderer* _Renderer){
		//initialisation de la texture.
		_SourceImage->append("\\Armes et Packs\\Mine.png");
		m_pTexture = IMG_LoadTexture(_Renderer, _SourceImage->c_str());

		//creation de la position de la mine
		m_pPosition.h = 59;
		m_pPosition.w = 15;
		m_pPosition.y = 0;
		m_pPosition.x = rand() % 1366;
	}

	/*void Use(CPlayer* _Player){
		_Player->SetHealth((_Player->GetHealth())*0.25);
	}*/


	/*
	Affiche la mine sur la  map a la postion m_pPosition
	*/
	void Show(SDL_Renderer* _Renderer){
		SDL_RenderCopy(_Renderer, m_pTexture, NULL, new SDL_Rect(m_pPosition));
	}

	
	/*
	Calcule la physique gerant la chute de la mine lors de son apparition.
	*/
	void PhysiqueChutePack(){
		m_pPosition.y += m_pPosition.y
	}
};