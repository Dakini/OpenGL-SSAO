#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H
#include "opengl.h"
#include "texCube.h"
#include "itexture.h"


class texCu: public iTexture, public tdogl::texCube{
	public:
			/*	Load from fileName. If filename already loaded (and target_ matches),
		don't reload. Return 0 on error. Assumes the following format for cube
		map faces:

					y+
			x-		z+		x+		z-
					y-

		*/
		static texCu * loadfromFile(
			const string &filename,
			unsigned optionMask =0);


		virtual bool reload(string filename);

private:
	texCu(const string &filename, unsigned optionMask);
};
inline texCu::texCu(const string &filename, unsigned int optionMask): iTexture(filename, optionMask){
}

#endif