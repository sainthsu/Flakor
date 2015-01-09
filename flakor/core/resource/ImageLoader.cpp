#include "core/resource/ImageLoader.h"
#include "core/resource/Resource.h"

bool load(Resource* res)
{
	Image* img = NULL;

	ResouceState state = res->getState();
	if(state == LOADING) return false;
	
	img = dynamic_cast<Image*>(res);
	if(img)
	{
		if(state == LOADED) img->unload();
		Uri* uri = img->getUri();
		BitData* data = BitData::createFromUri(uri);
		img->initWithImageData(data->getBytes(), data->getSize());
		return true;
	}
	
	return false;
	
}

Loader* createLoader()
{
    ImageLoader* loader = new ImageLoader();
    return loader;
}

Resource* createRes(Uri* uri)
{
	Image *res = new Image();
	res->setUri(uri);
	return res;
}

