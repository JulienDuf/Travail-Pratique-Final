//Classe qui gere les packs de santé.
//crée le 13 novembre par SAmuel Rambaud (Sam.Rambaud@outlook.com


class CHealthPack: public CPack{
private:
	SDL_Rect m_pPositon;
public:
	CHealthPack(){

	}

	void Use(CPlayer* _Player){
		_Player->SetHealth((_Player->GetHealth())*0.25);
	}

	void Show(){

	}

};