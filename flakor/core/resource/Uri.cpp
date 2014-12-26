/**
 * copyright Saint Hsu
 */
#include "core/resource/Uri.h"

FLAKOR_NS_BEGIN

Uri* Uri::parse(String uriString)
{}

Uri* Uri::parse(std::string uriString)
{}

Uri* parse(char* uriString)
{}

bool Uri::isRelative()
{
}

bool Uri::isAbsolute()
{
}


String Uri::getScheme()
{}

String Uri::getSchemeSpecificPart()
{}

String Uri::getEncodedSchemeSpecificPart()
{}

String Uri::getAuthority()
{}

String Uri::getEncodedAuthority()
{}

String Uri::getHost()
{}


int Uri::getPort()
{}

String Uri::getPath()
{}

String Uri::getEncodedPath()
{}

String Uri::getQuery()
{}

String Uri::getEncodedQuery()
{}

String Uri::getFragment()
{}

String Uri::getEncodedFragment()
{}

FLAKOR_NS_END

