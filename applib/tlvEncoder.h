/** \file
 * Copyright:
 *
 * Description: tlv encoder interface
 */
#ifndef D_tlvEncoder_H
#define D_tlvEncoder_H

#include <stdbool.h>
#include "tlv.h"

/**
 * Create a TLV container
 * @param tlv [IN]Pointer to an un-initialized TLV structure. Upon return, it is
 * Initialized to represent a TLV container with given tag
 * @param tag [IN]The tag of TLV container
 * @param buffer [IN]The buffer to store entire TLV object
 * @param length [IN]The Length of the buffer
 * @return true on success, false on failure
 * @note the maximum data value length is 255, so max two bytes for length and we
 * alway use two bytes in creating
 */
extern bool TlvCreate(Tlv_t* tlv, uint16_t tag,
                      uint8_t* buffer, size_t length);

/**
 * Add TLV data to a TLV container
 * @param tlv [IN]Pointer to a TLV container
 * @param tag [IN]The tag of the data
 * @param value [IN]The Data value
 * @param valueLen [IN]The data value length
 * @return true on success, false on failure
 */
extern bool TlvAddData(Tlv_t* tlv, uint16_t tag,
                const uint8_t* value, size_t valueLen);


/**
 * Add a TLV object to a TLV container
 * @param tlv [IN]Pointer to a TLV container
 * @param childTlv [IN]Pointer to a TLV object
 * @return true on success, false on failure
 * @note The TLV container and the TLV object shouldn't
 * overlap in the buffer since memory copied from child
 * TLV object's' buffer to the TLV container's buffer
 */
extern bool TlvAdd(Tlv_t* tlv, const Tlv_t* childTlv);

#endif
