#define BLANC32BIT 4294967295
#define TRANSPARENCE32BIT 16777215

void CollisionMap(SDL_Surface* _pSDLSurfaceMap,SDL_Surface* _pSDLSurface, SDL_Rect _RectDestination, int* _iX, int* _iY) {

	*_iX = 0;
	*_iY = 0;

	for (int y = _RectDestination.y; y < _RectDestination.y + _RectDestination.h; y++) {
		for (int x = _RectDestination.x; x < _RectDestination.x + _RectDestination.w; x++) {

			if ((((unsigned int*)_pSDLSurfaceMap->pixels)[y * _pSDLSurfaceMap->w + x] != TRANSPARENCE32BIT) && (((unsigned int*)_pSDLSurfaceMap->pixels)[(y - _RectDestination.y) * _pSDLSurface->w + (x - _RectDestination.x)] != TRANSPARENCE32BIT)) {

				*_iX = _RectDestination.x;
				*_iY = _RectDestination.y;
			}
		}
	}

}
/*
// Fonction qui retourne la position d'une collision.
// Param�tre: _pSDLSurfacecollision, pointe vers la surface avec laquelle on veut v�rifier les collisions avec la map.
// Param�tre: _SDLRectCollision, rectangle qui encadre l'endroit ou l'on veut v�rifier les collisions sur la map. (ex: rectangle destination d'un sprite.)
// Param�tre: _SDLRectSource, rectangle qui encadre la position sur la surface ou l'on veut v�rifier les collisions (ex : rectangle source d'un sprite.)
// Param�tre: _uiXMap, position en x dans la map ou la collision a lieu.
// Param�tre: _uiYMap, position en y dans la map ou la collision a lieu.
// Param�tre: _uiXRectCollision, Position en x dans le rectangle de collision ou la collision a eu lieu.
// Param�tre: _uiYRectCollision, position en y dans le rectangle de collision ou la collision a eu lieu.
// Retour: Rien, mais les positions en x et en y de la collision seront stock�s dans les 4 param�tres �crits plus haut.
void VerifierCollisionJoueurMap(SDL_Surface* _pSDLSurfaceMap, SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision) {
void VerifierCollisionMap(SDL_Surface* _pSDLSurfaceCollision, SDL_Rect _SDLRectCollision, SDL_Rect _SDLRectSource, unsigned int* _uiXMap, unsigned int* _uiYMap, unsigned int* _uiXRectCollision, unsigned int* _uiYRectCollision) {

	_uiXMap = 0;
	_uiYMap = 0;
	_uiXRectCollision = 0;
	_uiYRectCollision = 0;

	SDL_Surface* _pSDLSurfaceMap = nullptr;

	for (int y = _SDLRectCollision.h; y > 0; y--) {				// On parcours les pixels dans le rectangle collision en y � l'envers.

		for (int x = _SDLRectCollision.w; x > 0; x--) {			// On parcours les pixels dans le rectangle collision en x � l'envers.

			if ((((unsigned int*)_pSDLSurfaceMap->pixels)[_SDLRectCollision.x + x * y] != TRANSPARENCE32BIT) && (((unsigned int*)_pSDLSurfaceCollision->pixels)[_SDLRectSource.x + x * y] != TRANSPARENCE32BIT)) {			// Si il y a une collision entre les pixels non-transparents de la map et les pixels non-transparents du joueur...

				*_uiXMap = _SDLRectCollision.x + x;		// On stocke la position de la collision dans les variables ad�quates.
				*_uiYMap = _SDLRectCollision.y + y;
				*_uiXRectCollision = x;
				*_uiYRectCollision = y;

			}

		}

	}


}*/

// Proc�dure qui d�truit la carte de jeu � l'endroit appropri�.
// Param�tre: _iRayon, variable enti�re qui contient la longueur du rayon du cercle dans lequel la carte de jeu sera d�truite.
// Param�tre: _iX, La position en X ou la carte sera d�truite.
// Param�tre: _iY, La position en Y ou la carte sera d�truite.
// Retour: Rien.
void MapDestruction(SDL_Surface* _pSDLSurfaceMap, SDL_Surface* _pSurfaceGabarie, int _iRayon, int _iX, int _iY) {

	// Variables repr�sentant la position dans la map.
	int iX = _iX - _iRayon;
	int iY = _iY - _iRayon;

	SDL_Rect RectSource; // Source du gabarie.

	// Selon le rayon mis en param�tre...
	switch (_iRayon) {

		// Si le rayon est de 50 pixels.
	case 45:
		RectSource = { 0, 0, _pSurfaceGabarie->h, _pSurfaceGabarie->h };
		break;

		// Si le rayon est de 50 pixels.
	case 50:
		RectSource = { 1 * _pSurfaceGabarie->h, 0, _pSurfaceGabarie->h, _pSurfaceGabarie->h };
		break;

		// Si le rayon est de 60 pixels.
	case 60:
		RectSource = { 2 * _pSurfaceGabarie->h, 0, _pSurfaceGabarie->h, _pSurfaceGabarie->h };
		break;
	}

	// Pour toute la grandeur du gabarie...
	for (int y = 0; y < _iRayon * 2; y++) {
		for (int x = 0; x < _iRayon * 2; x++) {

			// Si le point est dans la map...
			if (iX > 0 && iY > 0) {

				// Si le pixel du gabarie n'est pas en blanc, remplace le pixel de la map en transparent.
				if (((unsigned int*)_pSurfaceGabarie->pixels)[y * _pSurfaceGabarie->w + x] != BLANC32BIT)
					((unsigned int*)_pSDLSurfaceMap->pixels)[iY * _pSDLSurfaceMap->w + iX] = TRANSPARENCE32BIT;

				iX++; // Augmente la position en X.
			}
		}

		iY++; // Augmente la position en Y.
		iX = _iX - _iRayon; // Remet la position en X au d�part.
	}

}




void DetectionRayonExplosion(){

}

// Fonction convertissant en angle de degr� � radian.
// En entr�e:
// Param1: L'angle � convertir.
// En sortie: L'angle converti.
float DegtoRad(float _fAngle) {

	return (M_PI / 180) * _fAngle;
}

// Fontion effectuant un rotation sur une surface selon un angle.
// En entr�e: 
// Param1: La surface qui doit �tre tourn�.
// Param2: L'angle de la rotation.
// En sortie: La surface avec la rotaiton.
SDL_Surface* Rotation(SDL_Surface* _pSurfaceRotation, float _fAngle) {

	SDL_Surface * pNouvelleSurface = SDL_CreateRGBSurface(_pSurfaceRotation->flags, _pSurfaceRotation->w, _pSurfaceRotation->h, _pSurfaceRotation->format->BitsPerPixel, _pSurfaceRotation->format->Rmask, _pSurfaceRotation->format->Gmask, _pSurfaceRotation->format->Bmask, _pSurfaceRotation->format->Amask);

	double sin = sinf(DegtoRad(_fAngle));
	double cos = cosf(DegtoRad(_fAngle));

	for (int y = 0; y < _pSurfaceRotation->h; y++)
		for (int x = 0; x < _pSurfaceRotation->w; x++) {

		int RotX = (x - _pSurfaceRotation->w / 2) * cos - (y - _pSurfaceRotation->h / 2) * sin + _pSurfaceRotation->w / 2;
		int RotY = (x - _pSurfaceRotation->w / 2) * sin + (y - _pSurfaceRotation->h / 2) * cos + _pSurfaceRotation->w / 2;

		if ((RotX >= 0 && RotY >= 0 && (RotX < _pSurfaceRotation->w && RotY < _pSurfaceRotation->h)))
			((unsigned int*)pNouvelleSurface->pixels)[y * pNouvelleSurface->w + x] = ((unsigned int*)_pSurfaceRotation->pixels)[RotY * _pSurfaceRotation->w + RotX];
		}

	return pNouvelleSurface;
}