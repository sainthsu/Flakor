#include "target.h"
#include "core/resource/BitData.h"
#include "core/resource/Uri.h"
#include "core/resource/ResourceManager.h"
#include "base/lang/Str.h"
#include <string.h>
#include <stdio.h>

#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
#include <android/asset_manager.h>
#elif FK_TARGET_PLATFORM == FK_PLATFORM_IOS
#include "tool/utility/ios/FileUtils.h"
#endif


FLAKOR_NS_BEGIN

const BitData BitData::Null;

BitData::BitData() :
	_bytes(nullptr),
	_size(0)
{
	FKLOGINFO("In the empty constructor of BitData.");
}

BitData::BitData(BitData&& other) :
	_bytes(nullptr),
	_size(0)
{
	FKLOGINFO("In the move constructor of BitData.");
	move(other);
}

BitData::BitData(const BitData& other) :
	_bytes(nullptr),
	_size(0)
{
	FKLOGINFO("In the copy constructor of BitData.");
	copy(other._bytes, other._size);
}

BitData::~BitData()
{
	FKLOGINFO("deallocing BitData: %p", this);
	clear();
}

BitData* BitData::createFromUri(Uri* uri)
{
    if(uri->type == Uri::ASSET)
#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
        return BitData::createFromAsset(uri->realPath);
#else
    return NULL;
#endif
	else if(uri->type == Uri::BUNDLE)
#if FK_TARGET_PLATFORM == FK_PLATFORM_IOS
		return BitData::createFromBundle(uri->realPath);
#else
    return NULL;
#endif
	else if(uri->type == Uri::LOCAL)
		return BitData::createFromFile(uri->realPath);
	else
		return NULL;
}


#if FK_TARGET_PLATFORM == FK_PLATFORM_ANDROID
BitData* BitData::createFromAsset(const String* filePath)
{
	// read asset data
	unsigned char* buffer = NULL;
	ssize_t size = 0;
	BitData* data = NULL; 
	
	if(ResourceManager::getAssetManager())
		FKLOG("OPEN %s",filePath->getCString());
	AAsset* asset = AAssetManager_open(ResourceManager::getAssetManager(),
			filePath->getCString(),
			AASSET_MODE_UNKNOWN);

	if (NULL == asset) {
		FKLOG("asset is nullptr");
		return NULL;
	}

	off_t fileSize = AAsset_getLength(asset);

	/*
	if (forString)
	{
		data = (unsigned char*) malloc(fileSize + 1);
		data[fileSize] = '\0';
	}
	else*/
	{
		buffer = (unsigned char*) malloc(fileSize);
	}

	int bytesread = AAsset_read(asset, (void*)buffer, fileSize);
	size = bytesread;

	AAsset_close(asset);

	data = new BitData();
	data->set(buffer,size);

	return data;


}
#elif FK_TARGET_PLATFORM == FK_PLATFORM_IOS

BitData* BitData::createFromBundle(const String* filePath)
{
    String* file = FileUtils::getBundleFilePath(filePath);
    return BitData::createFromFile(file);
}

#endif

BitData* BitData::createFromFile(const String* filePath)
{
	unsigned char* buffer = NULL;
	ssize_t size = 0;
	size_t readsize = 0;
	const char* mode = "rb";
	BitData* data = NULL;
	const char* file = filePath->getCString();

	do
	{
		// Read the file from hardware
		FILE *fp = fopen(file, mode);
		if (!fp)
		{
			FKLOG("openfile error");
			break;
		}
		FK_BREAK_IF(!fp);
		fseek(fp,0,SEEK_END);
		size = ftell(fp);
		fseek(fp,0,SEEK_SET);

		buffer = (unsigned char*)malloc(sizeof(unsigned char) * size);

		readsize = fread(buffer, sizeof(unsigned char), size, fp);
		fclose(fp);

	} while (0);

	if (NULL == buffer || 0 == readsize)
	{
		std::string msg = "Get data from file(";
		msg.append(file).append(") failed!");
		FKLOG("%s", msg.c_str());
		return NULL;
	}

	data = new BitData();
	data->set(buffer,readsize);

	return data;

}

BitData& BitData::operator= (const BitData& other)
{
	FKLOGINFO("In the copy assignment of BitData.");
	copy(other._bytes, other._size);
	return *this;
}

BitData& BitData::operator= (BitData&& other)
{
	FKLOGINFO("In the move assignment of BitData.");
	move(other);
	return *this;
}

void BitData::move(BitData& other)
{
	_bytes = other._bytes;
	_size = other._size;

	other._bytes = nullptr;
	other._size = 0;
}

bool BitData::isNull() const
{
	return (_bytes == nullptr || _size == 0);
}

unsigned char* BitData::getBytes() const
{
	return _bytes;
}

ssize_t BitData::getSize() const
{
	return _size;
}

void BitData::copy(unsigned char* bytes, const ssize_t size)
{
	clear();

	if (size > 0)
	{
		_size = size;
		_bytes = (unsigned char*)malloc(sizeof(unsigned char) * _size);
		memcpy(_bytes, bytes, _size);
	}
}

void BitData::set(unsigned char* bytes, const ssize_t size)
{
	_bytes = bytes;
	_size = size;
}

void BitData::clear()
{
	free(_bytes);
	_bytes = nullptr;
	_size = 0;
}

FLAKOR_NS_END
