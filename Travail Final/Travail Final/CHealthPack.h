//Classe qui gere les packs de santé.
//crée le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com)


class CHealthPack: public CPack{
private:
	SDL_Rect m_pPosition; //Position du pack sur la map
	SDL_Texture* m_pTexture; //Texture du pack
public:
	//constructeur
	CHealthPack(string* _SourceImage, SDL_Renderer* _Renderer){
		//initialisation de la texture
		_SourceImage->append("\\Armes et Packs\\HealthPack.png");
		m_pTexture = IMG_LoadTexture(_Renderer, _SourceImage->c_str());

		//creation de la position du pack
		m_pPosition.h = 40;
		m_pPosition.w = 34;
		m_pPosition.y = 0;
		m_pPosition.x = rand() % 1366;
	}

	/*
	Utilisation du pack de vie
	parametre:
		_player: Joueur utilisant le pack.
	*/
	//void Use(CPlayer* _Player){
	//	_Player->SetHealth(_Player->GetHealth() + ((_Player->GetHealth())*0.25));
	//}

	/*
	Affichage du pack de vie sur la map a la position m_pPosition
	*/
	void Show(SDL_Renderer* _Renderer){
		SDL_RenderCopy(_Renderer, m_pTexture, NULL, new SDL_Rect(m_pPosition));
	}

};