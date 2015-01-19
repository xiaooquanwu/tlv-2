/** \file
 * Copyright:
 *
 * Description: TLV stands for tag-length-value or type-length-value.
 * Here BER encoding is implemented
 *
 * BER-TLV is a recursive structure, it defined as:
 *   1. tlv is either a primitive TLV, that its content is a data value
 *   2. Or tlv is a constructed TLV, that its content is one or more TLVs
 *
 * Constrains:
 *   1. Tag is at most 2 bytes
 *   2. long format length is at most 2 octets (totally 3 octets including the
 *   initial octet)
 */

#include <stdbool.h>
#include <stdint.h>

/** Defines values of tag class */
typedef enum {
  /** universal class */
  TAG_CLASS_UNI = 0,
  /** application class */
  TAG_CLASS_APP = 1,
  /** context specific class */
  TAG_CLASS_CON = 2,
  /** private class */
  TAG_CLASS_PRI = 3
} TagClass_t;


/** type for tag number
 * Note: Only support 2 bytes tag, so 1 bytes tag num is enough as the
 * max is 127 */
typedef uint8_t TagNum_t;

/** type for length
 * Note: at most 2 bytes for length encoding is support,
 * max is 0x7FFF (b1111111 1111111) */
typedef uint16_t Length_t;

/** type for tlv object */
typedef struct {
  /** tag class */
  TagClass_t tagClass;
  /** b6 is 0 on primitive, 1 on constructed */
  uint8_t isConstructed;
  /** tag number */
  TagNum_t tagNum;
  /** Length */
  Length_t length;
  /** points to the whole encoded tlv object in the buffer */
  uint8_t *ptr;
  /** points to the value part of the encoded tlv object in the buffer */
  uint8_t *value;
} Tlv_t;

/**
 * Getters of tagClass field of tlv object
 * @param tlv address of tlv object
 * @return the value of tag class
 */
static inline TagClass_t TlvTagClass(Tlv_t *tlv)
{
  return tlv->tagClass;
}

/**
 * Getters of isConstructed field of tlv object
 * @param tlv address of tlv object
 * @return the value of isConstructed field
 */
static inline uint8_t TlvIsConstructed(Tlv_t *tlv)
{
  return tlv->isConstructed;
}

/**
 * Getters of tagNum field of tlv object
 * @param tlv address of tlv object
 * @return the value of tagNum field
 */
static inline TagNum_t TlvTagNum(Tlv_t *tlv)
{
  return tlv->tagNum;
}

/**
 * Getters of len field of tlv object
 * @param tlv address of tlv object
 * @return the value of length field
 */
static inline Length_t TlvLength(Tlv_t *tlv)
{
  return tlv->length;
}

/**
 * Getters of ptr field of tlv object
 * @param tlv address of tlv object
 * @return the value of ptr field
 */
static inline uint8_t* TlvPtr(Tlv_t *tlv)
{
  return tlv->ptr;
}

/**
 * Getters of value field of tlv object
 * @param tlv address of tlv object
 * @return the value of value field
 */
static inline uint8_t* TlvValue(Tlv_t *tlv)
{
  return tlv->value;
}

/**
 * Helper of tlv object.
 * @param tlv address of tlv object
 * @return the end address of of the encoded TLV object in the buffer
 */
static inline uint8_t* TlvEnd(Tlv_t *tlv)
{
  return TlvValue(tlv) + TlvLength(tlv);
}

/**
 * Parse the data in buffer to a TLV object. If the buffer contains more than
 * one TLV objects at the same level in the buffer, only the first TLV object
 * is parsed. This function should be called before any other TLV function
 * calls.
 * @param buffer the input buffer having encoded data
 * @param length the length of the input buffer
 * @param tlv the output TLV object
 * @return true on successful parsing, otherwise false
 */
extern bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv);


/**
 * Locate a TLV encoded data object in buffer of given length (if recursively
 * in depth-first order)
 * @param buffer the input buffer
 * @param length the length of input buffer
 * @param tag tag (up to 2 bytes) to find, we define the less significant byte
 * is the first byte and the other byte is the second byte.
 * @param recursive search sub-nodes or not
 * @param tlv A pointer to the found TLV object
 * @return true if the tag is found, otherwise false
 */
extern bool TlvSearchTag(const uint8_t *buffer, size_t length, uint16_t tag,
                         bool recursive, Tlv_t *tlv);
