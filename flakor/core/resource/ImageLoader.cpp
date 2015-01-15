#include "core/resource/ImageLoader.h"
#include "core/opengl/GL.h"
#include "core/resource/Resource.h"
#include "core/resource/Image.h"
#include "core/resource/BitData.h"


FLAKOR_NS_BEGIN

ImageLoader::ImageLoader()
{
	
}

ImageLoader::~ImageLoader()
{

}

bool ImageLoader::load(Resource* res)
{
	Image* img = NULL;

	ResourceState state = res->getState();
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

ILoader* ImageLoader::createLoader()
{
    ImageLoader* loader = new ImageLoader();
    return dynamic_cast<ILoader*>(loader);
}

Resource* ImageLoader::createRes(Uri* uri)
{
	Image *res = new Image();
	res->setUri(uri);
	return res;
}

FLAKOR_NS_END
