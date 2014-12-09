//Classe qui gere les packs de santé.
//crée le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com)


class CHealthPack: public CPack{
private:
	SDL_Rect m_RectDestination; //Position du pack sur la map
	SDL_Texture* m_pTexture; //Texture du pack
public:
	//constructeur
	CHealthPack(string _strSourceImage, SDL_Renderer* _Renderer){
		//initialisation de la texture
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\HealthPack.png");
		m_pTexture = IMG_LoadTexture(_Renderer, strSourceImage.c_str());

		//initialisation de la position du pack
		m_RectDestination.h = 40;
		m_RectDestination.w = 34;
		m_RectDestination.y = 0;
		m_RectDestination.x = rand() % 1366;
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
	Affichage du pack de vie sur la map a la position m_pRectDestination
	*/
	void Show(SDL_Renderer* _Renderer){
		SDL_RenderCopy(_Renderer, m_pTexture, NULL, &m_RectDestination);
	}


	/*
	calcul de la chute du pack au debut de partie
	*/
	void ChuteDebut(){
		m_RectDestination.y += 15;
	}
};