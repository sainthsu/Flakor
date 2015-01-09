#include "core/resource/TextureLoader.h"
#include "core/resource/Resource.h"

bool load(Resource* res)
{
	Texture2D* tex = NULL;

	ResouceState state = res->getState();
	if(state == LOADING) return false;
	
	tex = dynamic_cast<Texture2D*>(res);
	if(tex)
	{
		if(state == LOADED) tex->unload();
		Uri* uri = tex->getUri();
		Image* img = new Image();
		BitData* data = BitData::createFromUri(uri);
		img->initWithImageData(data->getBytes(), data->getSize());
		
		return true;
	}
	
	return false;
	
}

Loader* createLoader()
{
    TextureLoader* loader = new TextureLoader();
    return loader;
}

Resource* createRes(Uri* uri)
{
	Texture2D *res = new Texture2D();
	res->setUri(uri);
	return res;
}

