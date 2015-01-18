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

#define getTagClass(octet) ((octet & TAGCLASS_MASK) >> TAGCLASS_SHIFT)
#define getIsConstructed(octet) ((octet & ISCONSTRUCTED_MASK) >> \
                                       ISCONSTRUCTED_SHIFT)
#define getTagNumOfB1(octet) (octet & TAGNUM_B1_MASK)
#define tagNumIsSingleByte(octet) (getTagNumOfB1(octet) != TAGNUM_B1_2BYTES)
#define getTagNumOfB2(octet) (octet & TAGNUM_B2_MASK)

bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv)
{
  assert(length);
  uint8_t octet = buffer[0];

  tlv->tagClass = getTagClass(octet);
  tlv->isConstructed = getIsConstructed(octet);

  if (tagNumIsSingleByte(octet)) {
    tlv->tagNum = getTagNumOfB1(octet);
  } else {
    tlv->tagNum = getTagNumOfB2(buffer[1]);
  }

  return true;
}
