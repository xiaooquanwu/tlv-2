/** \file
 * Copyright:
 *
 */
#ifndef D_tag_H
#define D_tag_H

#include <stdbool.h>
#include <stdint.h>

#define TAGCLASS_MASK 0xC0      /* b1100 0000 */
#define TAGCLASS_SHIFT 6

#define ISCONSTRUCTED_MASK 0x20 /* b0010 0000 */
#define ISCONSTRUCTED_SHIFT 5

#define TAGNUM_B1_MASK 0x1F     /* b0001 1111
                                   tag number bits mask in the first byte */
#define TAGNUM_MULTIBYTES_LEADING 0x1F   /* b11111 in the leading bytes if
                                            tag number is more than one byte */
#define TAGNUM_B2_MASK 0x7F     /* b0111 1111 */
#define TAGNUM_b8 0x80       /* bit 8 in a byte */

#define getTagClass(octet) ((octet & TAGCLASS_MASK) >> TAGCLASS_SHIFT)
#define getIsConstructed(octet) ((octet & ISCONSTRUCTED_MASK) >> \
                                       ISCONSTRUCTED_SHIFT)
#define getTagNumOfB1(octet) (octet & TAGNUM_B1_MASK)
#define tagNumIsIn1Byte(tagNum) (tagNum < TAGNUM_MULTIBYTES_LEADING)
#define getTagNumOfB2(octet) (octet & TAGNUM_B2_MASK)
#define isNotLastByteOfTagNum(octet) (octet & TAGNUM_b8)
#define tagNumSetAsLast(octet) (octet & TAGNUM_B2_MASK)

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

typedef enum {
  IS_PRIM = 0,
  IS_CONS = 1
} PorC_t;

/** type for tag number
 * Note: Only support 2 bytes tag, so 1 bytes tag num is enough as the
 * max is 127 */
typedef uint8_t TagNum_t;

typedef struct {
  /** tag class */
  TagClass_t tagClass;
  /** b6 is 0 on primitive, 1 on constructed */
  uint8_t isConstructed;
  /** tag number */
  TagNum_t tagNum;
} Tag_t;

/**
 * Getters of tagClass field of tag object
 * @param tag address of tag object
 * @return the value of tag class
 */
static inline TagClass_t TagTagClass(Tag_t *tag)
{
  return tag->tagClass;
}

/**
 * Getters of isConstructed field of tag object
 * @param tag address of tag object
 * @return the value of isConstructed field
 */
static inline uint8_t TagIsConstructed(Tag_t *tag)
{
  return tag->isConstructed;
}

/**
 * Getters of tagNum field of tag object
 * @param tag address of tag object
 * @return the value of tagNum field
 */
static inline TagNum_t TagTagNum(Tag_t *tag)
{
  return tag->tagNum;
}

/**
 * Encode tag to uint16_t in Big Endian as it is sent to receiver
 * @param tag the tag object
 * @return the encoded tag in uint16_t, the lowest octet is 0 if
 * only one octet is needed
 */
extern uint16_t TagToUint16(Tag_t tag);

/**
 * Decode tag from uint16_t in Big Endian to Tag_t
 * @param tag the encoded uint16_t tag
 * @return the decoded tag object
 */
extern Tag_t TagFromUint16(uint16_t tag);

/**
 * Compare the input encoded tag with the tag object
 * @param encoded the input encoded tag, it should be encoded in a way that
 * lower octet is the first octet.
 * @param tag the input tag object
 * @return true if tags are the same, otherwise false
 */
extern bool TagIsMatch(uint16_t encoded, Tag_t tag);

#endif
