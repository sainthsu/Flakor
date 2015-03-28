/**
 * copyright Saint Hsu
 */
#include "core/resource/Uri.h"
#include <string.h>

FLAKOR_NS_BEGIN

const char* Uri::DEFAULT_ENCODING = "UTF-8";

Uri::Uri()
:type(0)
,origin(NULL)
,realPath(NULL)
,filename(NULL)
,scheme(NULL)
,port(0)
,path(NULL)
,fragment(NULL)
,query(NULL)
{}

Uri::~Uri()
{
	FK_SAFE_DELETE(origin);
	FK_SAFE_DELETE(realPath);
	FK_SAFE_DELETE(filename);
	FK_SAFE_DELETE(scheme);
	FK_SAFE_DELETE(path);
	FK_SAFE_DELETE(fragment);
	FK_SAFE_DELETE(query);
	port=0;
	type=0;
}

Uri* Uri::parse(String* uriString)
{
    return parse(uriString->getCString());
}

Uri* Uri::parse(const std::string&& uriString)
{
	FKLOG("uri:here %s",uriString.c_str());
    return parse(uriString.c_str());
}

Uri* Uri::parse(const char* uriString)
{
    char* position=NULL;
	char* name=NULL;

    Uri* u = new Uri();

	u->origin = String::create(uriString);
    position = strstr(uriString,"://");

    u->scheme = String::create(uriString,position-uriString);
	
    position += 3;
	FKLOG("uri:here %s",position);
    u->realPath = String::create(position);
    //暂时只支持asset和local
    name = strchr(position,'/');
    if(name != NULL)
    {
        u->filename = String::create(name+1);
    }
    else
    {
        u->filename = String::create(position);
    }

    if(u->scheme->compare("asset") == 0)
    {
        u->type = Uri::ASSET;
    }
    else if(u->scheme->compare("bundle") == 0)
    {
        u->type = Uri::BUNDLE;
    }
    else if(u->scheme->compare("local") == 0 || u->scheme->compare("file") == 0)
    {
        u->type = Uri::LOCAL;
    }
    else if(u->scheme->compare("http") == 0 || u->scheme->compare("https") == 0)
    {
        u->type = Uri::INTERNET;
    }
    else
    {
        u->type = Uri::OTHER;
    }

    return u;
}

bool Uri::isFrom(char* source)
{
		return (origin->compare(source) == 0);
}

bool Uri::equal(Uri* other)
{
	return (origin->compare(other->origin) == 0);
}

bool Uri::isRelative() const
{
    return !isAbsolute();
}

bool Uri::isAbsolute() const
{
    return true;
}


String* Uri::getScheme() const
{
    return scheme;
}

String* Uri::getSchemeSpecificPart() const
{
    return NULL;
}

String* Uri::getEncodedSchemeSpecificPart() const
{
    return NULL;
}

String* Uri::getAuthority() const
{
    return NULL;
}

String* Uri::getEncodedAuthority() const
{
    return NULL;
}

String* Uri::getHost() const
{
    return host;
}


int Uri::getPort() const
{
    return port;
}

String* Uri::getPath() const
{
    return path;
}

String* Uri::getEncodedPath() const
{
    return path;
}

String* Uri::getQuery() const
{
    return query;
}

String* Uri::getEncodedQuery() const
{
    return NULL;
}

String* Uri::getFragment() const
{
    return fragment;
}

String* Uri::getEncodedFragment() const
{
    return NULL;
}

FLAKOR_NS_END

