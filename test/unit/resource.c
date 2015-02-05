#include "core/resource/ResourceManager.h"
#include "core/resource/Image.h"

int main(int argc,char** argv)
{
	ResourceManager *mgr = ResourceManager::thisManager();
	Image *image = ResourceManager::createResource("file:///home/saint/Pictures/list_title.png",ResourceManager::IMAGE_NAME);

	image->load(true);
	return 1;
}
