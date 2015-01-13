#ifndef _FK_IMAGELOADER_H_
#define _FK_IMAGELOADER_H_

#include "core/resource/ILoader.h"

FLAKOR_NS_BEGIN

class ImageLoader : public ILoader
{
	public:
		ImageLoader();
		~ImageLoader();

		virtual bool load(Resource* res) override;
		virtual ILoader* createLoader() override;
		virtual Resource* createRes(Uri* uri) override;
};

FLAKOR_NS_END

#endif
