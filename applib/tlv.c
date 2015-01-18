#include <assert.h>
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

#define getTagClass(octet) ((octet & TAGCLASS_MASK) >> TAGCLASS_SHIFT)
#define getIsConstructed(octet) ((octet & ISCONSTRUCTED_MASK) >> \
                                       ISCONSTRUCTED_SHIFT)
#define getTagNumOfB1(octet) (octet & TAGNUM_B1_MASK)
#define tagNumIsSingleByte(octet) (getTagNumOfB1(octet) != TAGNUM_B1_2BYTES)
#define getTagNumOfB2(octet) (octet & TAGNUM_B2_MASK)
#define isNotLastByteOfTagNum(octet) (octet & TAGNUM_b8)
#define getLength(octet) (octet & LENGTH_MASK)

bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv)
{
  const uint8_t *cur = NULL, *end = NULL;
  uint8_t octet;
  /* length at least 2 bytes because one for tag and one for length */
  if (!buffer || (length < 2) || !tlv) return false;

  cur = buffer;
  end = buffer + length;

  /* parse tag */
  octet = *(cur++);
  tlv->tagClass = getTagClass(octet);
  tlv->isConstructed = getIsConstructed(octet);
  tlv->tagNum = getTagNumOfB1(octet);
  if (!tagNumIsSingleByte(tlv->tagNum)) {
    if (cur == end) return false;
    octet = *(cur++);
    /* as only max two bytes tag is supported */
    if (isNotLastByteOfTagNum(octet)) return false;
    tlv->tagNum = getTagNumOfB2(octet);
  }

  /* parse length */
  if (cur == end) return false;
  octet = *(cur++);
  tlv->length = getLength(octet);
  return true;
}
