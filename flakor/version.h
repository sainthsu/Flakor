#ifndef _FK_VERSION_H_
#define _FK_VERSION_H_

//0x00 HI ME LO
// 00   00 01 00
#define FLAKOR_VERSION 0x00000100

FLAKOR_NS_BEGIN

int getFlakorVersion()
{
       return FLAKOR_VERSION;
}

FLAKOR_NS_END

#endif
