#include <assert.h>
#include <stdio.h>
#include "tlv.h"

#define TAGCLASS_MASK 0xC0      /* b1100 0000 */
#define TAGCLASS_SHIFT 6

#define ISCONSTRUCTED_MASK 0x20 /* b0010 0000 */
#define ISCONSTRUCTED_SHIFT 5

#define TAGNUM_B1_MASK 0x1F     /* b0001 1111
                                   tag number bits mask in the first byte */
#define TAGNUM_B1_2BYTES 0x1F
#define TAGNUM_B2_MASK 0x7F     /* b0111 1111 */
#define TAGNUM_b8 0x80       /* bit 8 in a byte */

#define LENGTH_MASK 0x7F        /* b0111 1111 */
#define LENGTH_FMT_MASK 0x80        /* b1000 0000 */
#define LENGTH_LONGFMT_MINBYTES 1
#define LENGTH_LONGFMT_MAXBYTES 2
#define LENGTH_LONGFMT_BITS 8   /* 8 bits are used for forming a length value */

#define getTagClass(octet) ((octet & TAGCLASS_MASK) >> TAGCLASS_SHIFT)
#define getIsConstructed(octet) ((octet & ISCONSTRUCTED_MASK) >> \
                                       ISCONSTRUCTED_SHIFT)
#define getTagNumOfB1(octet) (octet & TAGNUM_B1_MASK)
#define tagNumIsMultiBytes(octet) (getTagNumOfB1(octet) == TAGNUM_B1_2BYTES)
#define getTagNumOfB2(octet) (octet & TAGNUM_B2_MASK)
#define isNotLastByteOfTagNum(octet) (octet & TAGNUM_b8)

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
  if (*pcur >= end) return false;

  tlv->ptr = (uint8_t *)(*pcur);

  octet = nextOctet(*pcur);
  tlv->tagClass = getTagClass(octet);
  tlv->isConstructed = getIsConstructed(octet);
  tlv->tagNum = getTagNumOfB1(octet);
  if (tagNumIsMultiBytes(tlv->tagNum)) {
    if (*pcur == end) return false;
    octet = nextOctet(*pcur);
    /* as only max two bytes tag is supported */
    if (isNotLastByteOfTagNum(octet)) return false;
    tlv->tagNum = getTagNumOfB2(octet);
  }
  return true;
}

static inline bool validateLengthLongFmtByteNum(uint8_t num)
{
  return (num >= LENGTH_LONGFMT_MINBYTES) && (num <= LENGTH_LONGFMT_MAXBYTES);
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
  uint8_t octet = 0, numOfBytes = 0;
  int i = 0;

  if (*pcur >= end) return false;
  octet = nextOctet(*pcur);

  if (lengthIsShortFmt(octet)) {
    tlv->length = getLength(octet);
    return true;
  }

  /* the following is handling long format */
  numOfBytes = getLength(octet);
  if (!validateLengthLongFmtByteNum(numOfBytes)) return false;
  tlv->length = 0;
  for (i = 0; i < numOfBytes; i++) {
    if (*pcur >= end) return false;
    octet = nextOctet(*pcur);
    /* concatenate the octet to length */
    tlv->length <<= LENGTH_LONGFMT_BITS;
    tlv->length |= octet;
  }
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
  if ((*pcur + tlv->length) > end) return false;

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
  return true;
}
