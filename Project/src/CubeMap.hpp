#ifndef		__CUBEMAP__
#define		__CUBEMAP__

#include "image.hpp"

class CubeMap {
public:
	Image *textures[6];

	void set_face(Image* texture, int face) {
		textures[face] = texture;
	}	

	Image *front() {
		return textures[0];
	} 

	Image *right() {
		return textures[1];
	}

	Image *left() {
		return textures[2];
	}

	Image *back() {
		return textures[3];
	}

	Image *top() {
		return textures[4];
	}

	Image *bottom() {
		return textures[5];
	}
};

#endif