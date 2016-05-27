#include "itexture.h"
#include "opengl.h"

void iTexture::destroy(iTexture **ppTexture){
	assert(ppTexture !=0);
	assert(*ppTexture !=0);

	delete *ppTexture;
	*ppTexture=0;
}
bool iTexture::reloadAll(){
	bool result = true;
	for(size_t i =0; i<nInstances(); i++)
		result &=instance(i).reload("");
	glFinish();
	return result;

}