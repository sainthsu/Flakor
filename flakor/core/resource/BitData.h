#ifndef _FK_BITDATA_H_
#define _FK_BITDATA_H_

#include "targetMacros.h"
#include <stdint.h> // for ssize_t on android
#include <string>   // for ssize_t on linux
#include "stdC.h" // for ssize_t on window


FLAKOR_NS_BEGIN

class Uri;
class String;
/**
  *a class to store bit data from a file
  */
class BitData
{
public:
    static const BitData Null;

    BitData();
    BitData(const BitData& data);
    BitData(BitData&& other);
    ~BitData();

    static BitData* createFromUri(Uri* uri);
    static BitData* createFromAsset(const String* filePath);
    static BitData* createFromFile(const String* filePath);

    // Assignment operator
    BitData& operator= (const BitData& other);
    BitData& operator= (BitData&& other);

    /**
    * @js NA
    * @lua NA
    */
    unsigned char* getBytes() const;
    /**
     * @js NA
     * @lua NA
     */
    ssize_t getSize() const;

    /** Copies the buffer pointer and its size.
     *  @note This method will copy the whole buffer.
     *        Developer should free the pointer after invoking this method.
     *  @see Data::fastSet
     */
    void copy(unsigned char* bytes, const ssize_t size);

    /** Fast set the buffer pointer and its size. Please use it carefully.
     *  @param bytes The buffer pointer, note that it have to be allocated by 'malloc' or 'calloc',
     *         since in the destructor of Data, the buffer will be deleted by 'free'.
     *  @note 1. This method will move the ownship of 'bytes'pointer to Data,
     *        2. The pointer should not be used outside after it was passed to this method.
     *  @see Data::copy
     */
    void set(unsigned char* bytes, const ssize_t size);

    /** Clears data, free buffer and reset data size */
    void clear();

    /** Check whether the data is null. */
    bool isNull() const;

private:
    void move(BitData& other);

protected:
    unsigned char* _bytes;
    ssize_t _size;
};

FLAKOR_NS_END

#endif // BITDATA_H

