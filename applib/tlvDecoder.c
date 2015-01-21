#include "tlvDecoder.h"

#define LENGTH_MASK 0x7F        /* b0111 1111 */
#define LENGTH_FMT_MASK 0x80        /* b1000 0000 */

#define getLength(octet) (octet & LENGTH_MASK)
#define lengthIsShortFmt(octet) (!(octet & LENGTH_FMT_MASK))

/* cur is uint8 *, the value of octet that cur points to is returned
   and cur is moved to next as well */
#define nextOctet(cur) (*((cur)++))

/**
 * parse the tag part of encoding buffer pointed by *pcur
 * @param pcur the pointer of pointer to the current position of the buffer
 * @param end the pointer to one more than the last octet in the buffer
 * @param tlv tlv object
 * @return true on success, false on error. pcur is moved to the next
 * not consumed octet, tlv is updated with parsed tag information
 */
static bool parseTag(const uint8_t **pcur, const uint8_t *end, Tlv_t *tlv)
{
  uint8_t octet;
  Tag_t tag;

  if (*pcur >= end) return false;

  tlv->ptr = (uint8_t *)(*pcur);
  octet = nextOctet(*pcur);

  tag.tagClass = getTagClass(octet);
  tag.isConstructed = getIsConstructed(octet);

  tag.tagNum = getTagNumOfB1(octet);
  if (!tagNumIsIn1Byte(TagTagNum(&tag))) {
    if (*pcur == end) return false;
    octet = nextOctet(*pcur);
    /* as only max two bytes tag is supported */
    if (isNotLastByteOfTagNum(octet)) return false;
    tag.tagNum = getTagNumOfB2(octet);
  }

  tlv->tag = tag;
  return true;
}

/**
 * parse the length part of encoding buffer pointed by *pcur
 * @param pcur the pointer of pointer to the current position of the buffer
 * @param end the pointer to one more than the last octet in the buffer
 * @param tlv tlv object
 * @return true on success, false on error. pcur is moved to the next
 * not consumed octet, tlv is updated with parsed length information
 */
static bool parseLength(const uint8_t **pcur, const uint8_t *end, Tlv_t *tlv)
{
  uint8_t octet = 0;

  if (*pcur >= end) return false;
  octet = nextOctet(*pcur);

  if (lengthIsShortFmt(octet)) {
    tlv->dataLen = getLength(octet);
    return true;
  }

  /* the following is handling long format */

  /* only 1 is supported, hard coded here because the octet
     handling is also hard code here */
  if (getLength(octet) != 1) return false;

  if (*pcur >= end) return false;
  octet = nextOctet(*pcur);
  tlv->dataLen = octet;

  return true;
}

/**
 * parse the value part of encoding buffer pointed by *pcur.
 * @param pcur the pointer of pointer to the current position of the buffer
 * @param end the pointer to one more than the last octet in the buffer
 * @param tlv tlv object, tlv object should have valid length value
 * @return true on success, false on error. pcur is moved to the next
 * not consumed octet, tlv is updated with parsed length information
 */
static bool parseValue(const uint8_t **pcur, const uint8_t *end, Tlv_t *tlv)
{
  /* if not all content are in the buffer */
  if ((*pcur + TlvDataLen(tlv)) > end) return false;

  tlv->value = (uint8_t *)(*pcur);
  return true;
}

/**
 * Skip all leading zero bytes, that is moving pcur to the first non-zero byte,
 * Or end of the buffer
 * @param pcur the pointer of pointer to the current position of the buffer
 * @param end the pointer to one more than the last octet in the buffer
 * @return true on success, false if the end of buffer is met
 */
static bool skipZeroBytes(const uint8_t **pcur, const uint8_t *end)
{
  while (!**pcur && (*pcur < end)) (*pcur)++;

  if (*pcur < end) return true;

  /* the end of the buffer is reached */
  return false;
}

bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv)
{
  const uint8_t *cur = NULL, *end = NULL;
  /* length at least 2 bytes because one for tag and one for length */
  if (!buffer || (length < 2) || !tlv) return false;

  cur = buffer;
  end = buffer + length;

  if (!skipZeroBytes(&cur, end)) return false;
  if (!parseTag(&cur, end, tlv)) return false;
  if (!parseLength(&cur, end, tlv)) return false;
  if (!parseValue(&cur, end, tlv)) return false;
  tlv->dataCapacity = (size_t)TlvDataLen(tlv);
  return true;
}

bool TlvSearchTag(const uint8_t *buffer, size_t length, uint16_t tag,
                         bool recursive, Tlv_t *tlv)
{
  const uint8_t *  cur = buffer;
  size_t  left = length;
  const uint8_t *  end = buffer + left;

  while (cur < end) {
    if (!TlvParse(cur, left, tlv)) return false;
    if (TagIsMatch(tag, tlv->tag)) return true;

    /* move cur to the end of the tlv object. It points to the
       first unparsed octet */
    cur = TlvEnd(tlv);
    left = (size_t)(end - cur);

    /* search depth first */
    if (recursive && TagIsConstructed(&tlv->tag)) {
      if (TlvSearchTag(TlvValue(tlv), TlvDataLen(tlv),
                      tag, true, tlv)) {
        return true;
      }
    }
  }

  return false;
}
