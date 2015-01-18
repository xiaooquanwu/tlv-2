/** \file
 * Copyright:
 *
 * Description: TLV stands for tag-length-value or type-length-value.
 * Here BER encoding is implemented
 *
 * TLV is a recursive structure, it defined as:
 *   1. tlv is either a primitive TLV, that its content is a data value
 *   2. Or tlv is a constructed TLV, that its content is one or more TLVs
 *
 */

#include <stdbool.h>

typedef enum {
  TAG_CLASS_UNI = 0,
  TAG_CLASS_APP = 1,
  TAG_CLASS_CONTEXT = 2,
  TAG_CLASS_PRIVATE = 3
} TagClass_t;

typedef struct {
  TagClass_t tagClass;
  bool isPrimitive;
  uint16_t tagNum;
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
 * Getters of isPrimitive field of tlv object
 * @param tlv address of tlv object
 * @return the value of isPrimitive field
 */
static inline bool TlvIsPrimitive(Tlv_t *tlv)
{
  return tlv->isPrimitive;
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
