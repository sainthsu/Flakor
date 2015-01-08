#ifndef _FK_TEXTURELOADER_H_
#define _FK_TEXTURELOADER_H_

#include "core/resource/ILoader.h"

class Uri;
class Resource;

class TextureLoader : public ILoader
{
	public:
		TextureLoader();
		~TextureLoader();
		virtual bool load(Resource* res) override;
		virtual Loader* createLoader() override;
		virtual Resource* createRes(Uri* uri) override;
}

#endif
