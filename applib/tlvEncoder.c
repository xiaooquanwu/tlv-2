/** \file
 * Copyright:
 *
 * Description: tlv encoder implementation
 */
#include "tlvEncoder.h"
#include <string.h>
#include <assert.h>

#define LENGTH_MAX_VALUE 255
#define LENGTH_MAX_BYTES 2
/* 0x80 means see the sequent bytes, 0x01 means 1 sequent byte */
#define LENGTH_B1_LEADING (0x80 | 0x01)

static bool writeTag(uint16_t tag, uint8_t **pcur, uint8_t *end)
{
  uint8_t *ptag = (uint8_t *)&tag;

  if (*pcur >= end) return false;
  *(*pcur)++ = ptag[1];
  if (ptag[0]) {
    if (*pcur >= end) return false;
    *(*pcur)++ = ptag[0];
  }
  return true;
}

static bool writeLen(Length_t length, uint8_t **pcur, uint8_t *end)
{
  /* as uint8_t is max 255, so it  makes nonsense but for document */
  if (length > LENGTH_MAX_VALUE) return false;

  if (*pcur >= end) return false;
  *(*pcur)++ = LENGTH_B1_LEADING;
  if (*pcur >= end) return false;
  *(*pcur)++ = length;
  return true;
}

static bool addTlvContainerLength(Length_t value, Tlv_t* tlv)
{
  if ((TlvDataLen(tlv) + value) > LENGTH_MAX_VALUE) return false;

  uint8_t *plen = tlv->ptr +
    ((tagNumIsIn1Byte(TagTagNum(&tlv->tag))) ? 1 : 2);

  tlv->dataLen += value;
  writeLen(TlvDataLen(tlv), &plen, plen + LENGTH_MAX_BYTES);
  return true;
}

bool TlvCreate(Tlv_t* tlv, uint16_t tag, uint8_t* buffer, size_t length)
{
  uint8_t *cur = buffer, *end = buffer + length;

  tlv->ptr = buffer;

  tlv->tag = TagFromUint16(tag);
  if (!writeTag(tag, &cur, end)) return false;

  tlv->dataLen = 0;
  if(!writeLen(0, &cur, end)) return false;

  tlv->value = cur;

  /* the remaining space for the data is capacity */
  tlv->dataCapacity = (size_t) (end - cur);

  return true;
}

bool TlvAddData(Tlv_t* tlv, uint16_t tag,
                const uint8_t* value, size_t valueLen)
{
  Tlv_t childTlv;
  uint8_t *childBuffer = TlvEnd(tlv);
  size_t childLen = TlvDataCapacity(tlv) - TlvDataLen(tlv);

  /* create an child container inside the container */
  if (!TlvCreate(&childTlv, tag, childBuffer, childLen)) return false;

  /* fill in data */
  if (valueLen >= TlvDataCapacity(&childTlv)) return false;
  memcpy(TlvValue(&childTlv), value, valueLen);

  /* update length */
  addTlvContainerLength((Length_t)valueLen, &childTlv);
  addTlvContainerLength((Length_t)TlvTotalLen(&childTlv), tlv);

  return true;
}
