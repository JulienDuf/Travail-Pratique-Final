/*
420-202-RE : Travail final
Classe qui gere les packs de sant�.
cr�e le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com)
*/


class CHealthPack: public CPack{
private:
	SDL_Rect m_pRectDestination; //Position du pack sur la map
	SDL_Texture* m_pTexture; //Texture du pack
public:
	//constructeur
	CHealthPack(string _strSourceImage, SDL_Renderer* _Renderer){
		//initialisation de la texture
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\HealthPack.png");
		m_pTexture = IMG_LoadTexture(_Renderer, strSourceImage.c_str());

		//initialisation de la position du pack
		m_pRectDestination.h = 40;
		m_pRectDestination.w = 34;
		m_pRectDestination.y = 0;
		m_pRectDestination.x = rand() % 1366;
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
		SDL_RenderCopy(_Renderer, m_pTexture, NULL, &m_pRectDestination);
	}


	/*
	calcul de la chute du pack au debut de partie
	parametres:
		_Y: composante y du vecteur du vent
		_X: Composante x du vecteur du vent
	*/
	void ChuteDebut(int _Y, int _X){
		m_pRectDestination.y += 35;
		m_pRectDestination.y += _Y;
		m_pRectDestination.x += _X;
	}
};