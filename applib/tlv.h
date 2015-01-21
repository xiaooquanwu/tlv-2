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
#ifndef D_tlv_H
#define D_tlv_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "tag.h"

/** type for length
 * Note: at most 2 bytes for length encoding is support,
 * max is 0x255 (b10000001 1111111) */
typedef uint8_t Length_t;

#define LENGTH_MAX_VALUE 255
#define LENGTH_MAX_BYTES 2
/* 0x80 means see the sequent bytes, 0x01 means 1 sequent byte */
#define LENGTH_B1_LEADING (0x80 | 0x01)
#define LENGTH_MASK 0x7F        /* b0111 1111 */
#define LENGTH_FMT_MASK 0x80        /* b1000 0000 */

#define getLength(octet) (octet & LENGTH_MASK)
#define lengthIsShortFmt(octet) (!(octet & LENGTH_FMT_MASK))

/** type for tlv object */
typedef struct {
  /* tag */
  Tag_t tag;
  /** Data Length */
  Length_t dataLen;
  /** points to the whole encoded tlv object in the buffer */
  uint8_t *ptr;
  /** points to the value part of the encoded tlv object in the buffer */
  uint8_t *value;
  /** the space of buffer allowed for data in Tlv encoding */
  size_t dataCapacity;
} Tlv_t;

/**
 * Getters of tag field of tlv object
 * @param tlv address of tlv object
 * @return the value of tag field
 */
static inline Tag_t TlvTag(const Tlv_t *tlv)
{
  return tlv->tag;
}

/**
 * Getters of data length field of tlv object
 * @param tlv address of tlv object
 * @return the value of the field
 */
static inline Length_t TlvDataLen(const Tlv_t *tlv)
{
  return tlv->dataLen;
}

/**
 * Getters of ptr field of tlv object
 * @param tlv address of tlv object
 * @return the value of ptr field
 */
static inline uint8_t* TlvPtr(const Tlv_t *tlv)
{
  return tlv->ptr;
}

/**
 * Getters of value field of tlv object
 * @param tlv address of tlv object
 * @return the value of the field
 */
static inline uint8_t* TlvValue(const Tlv_t *tlv)
{
  return tlv->value;
}

/**
 * Getters of dataCapacity field of tlv object
 * @param tlv address of tlv object
 * @return the value of the field
 */
static inline size_t TlvDataCapacity(const Tlv_t *tlv)
{
  return tlv->dataCapacity;
}

/**
 * Helper of tlv object.
 * @param tlv address of tlv object
 * @return the end address of of the encoded TLV object in the buffer
 */
static inline uint8_t* TlvEnd(const Tlv_t *tlv)
{
  return TlvValue(tlv) + TlvDataLen(tlv);
}

/**
 * Helper of tlv object.
 * @param tlv address of tlv object
 * @return the total length of all encoded TLV object in the buffer
 */
static inline Length_t TlvTotalLen(const Tlv_t *tlv)
{
  return (Length_t)(TlvValue(tlv) - TlvPtr(tlv)) + TlvDataLen(tlv);
}

#endif
