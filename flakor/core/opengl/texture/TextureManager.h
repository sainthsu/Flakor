#ifndef _FK_TEXUREMANAGER_H_
#define _FK_TEXUREMANAGER_H_

#include <set>

FLAKOR_NS_BEGIN

class TextureManager
{
	public:

		loadTexture(Texture2D* texture);
		reloadTexture();
		unloadTexture(Texture2D* texture);
	protected:

		std::unordered_map<std::string, Texture2D*> _textures;

		std::set<Texture2D> texturesLoaded;
		std::set<Texture2D> texturesToBeLoaded;
		std::set<Texture2D> texturesToUnloaded;
}

FLAKOR_NS_END

#endif
