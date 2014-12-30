#ifndef _FK_IMAGELOADER_H_
#define _FK_IMAGELOADER_H_

#include "core/resource/ILoader.h"

class Uri;
class Resource;

class ImageLoader : public ILoader
{
	public:
		ImageLoader();
		~ImageLoader();
		virtual bool load(Resource* res) override;
		virtual Loader* createLoader() override;
		virtual Resource* createRes(Uri* uri) override;
}

#endif
