#include "core/resource/ILoader.h"
#include "core/resource/Resource.h"

bool load(Resource* res)
{
	unsigned char* buffer = NULL;
    ssize_t size = 0;
    size_t readsize;
    const char* mode = "rb";

	Image* img = NULL;

	ResouceState state = res->getState();
	if(state == LOADING) return false;
	
	img = dynamic_castM<Image*>(res);
	if(img)
	{
		if(state == LOADED) img->unload();
		Uri* uri = img->getUri();
		do
    	{
        	// Read the file from hardware
        	//std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
        	FILE *fp = fopen(_filename, mode);
        	if (!fp) {
            	FKLOG("openfile error");
        	}
        	FK_BREAK_IF(!fp);
        	fseek(fp,0,SEEK_END);
        	size = ftell(fp);
        	fseek(fp,0,SEEK_SET);
        
        	buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
        
        	readsize = fread(buffer, sizeof(unsigned char), size, fp);
        	fclose(fp);

    	} while (0);
    
    	if (nullptr == buffer || 0 == readsize)
    	{
        	std::string msg = "Get data from file(";
        	msg.append(_filename).append(") failed!");
        	FKLOG("%s", msg.c_str());
			return nullptr;
    	}

		img->initWithImageData(buffer, readsize);
		return true;
	}
	
	return false;
	
}

Loader* createLoader()
{
	
}

Resource* createRes(Uri* uri)
{
	Image *res = new Image();
	
	return res;
}

