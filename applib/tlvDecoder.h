/** \file
 * Copyright:
 *
 * Description: tlv decoder interface
 */
#ifndef D_tlvDecoder_H
#define D_tlvDecoder_H

#include "tlv.h"

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

#endif
