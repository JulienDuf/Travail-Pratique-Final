//Classe qui gere les packs de santé.
//crée le 13 novembre par Samuel Rambaud (Sam.Rambaud@outlook.com)

class CMine : public CPack{
private:
	SDL_Rect m_pPosition; //position de la mine sur la map
public:
	CMine(){

	}

	void Use(CPlayer* _Player){
		_Player->SetHealth((_Player->GetHealth())*0.25);
	}

	void Show(){

	}
};