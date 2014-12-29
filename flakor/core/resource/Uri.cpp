/**
 * copyright Saint Hsu
 */
#include "core/resource/Uri.h"
#include <string.h>

FLAKOR_NS_BEGIN

Uri::Uri()
:type(0)
,realPath(NULL)
,filename(NULL)
,scheme(NULL)
,port(0)
,path(NULL)
,fregment(NULL)
,query(NULL)
{}

Uri* Uri::parse(String* uriString)
{
    return parse(uriString->getCString());
}

Uri* Uri::parse(std::string uriString)
{
    return parse(uriString->c_char());
}

Uri* parse(const char* uriString)
{
    char* position,name;

    Uri* u = new Uri();
    position = strstr(uriString,"://");
    u->scheme = new String(uriString,position-uriString);

    position += 3;
    realPath = new String(position);
    //暂时只支持asset和local
    name = strchr(position,'/');
    if(name == NULL)
    {
        u->filename = new String(name+1);
    }
    else
    {
        u->filename = new String(position);
    }

    if(u->scheme->compare("asset") == 0)
    {
        u->type = ASSET;
    }
    else if(scheme->compare("local") == 0 || scheme->compare("file") == 0)
    {
        u->type = LOCAL;
    }
    else if(scheme->compare("http") == 0 || scheme->compare("https") == 0)
    {
        u->type = INTERNET;
    }
    else
    {
        u->type = OTHER;
    }

    return u;
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

