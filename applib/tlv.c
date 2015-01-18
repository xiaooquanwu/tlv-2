#include <assert.h>
#include "tlv.h"

#define TAGCLASS_MASK 0xC0      /* b1100 0000 */
#define TAGCLASS_SHIFT 6

#define ISCONSTRUCTED_MASK 0x20 /* b0010 0000 */
#define ISCONSTRUCTED_SHIFT 5

#define TAGNUM_B1_MASK 0x1F     /* b0001 0000
                                   tag number bits mask in the first byte */
#define TAGNUM_B1_SHIFT 0

#define getTagClass(first_byte) ((first_byte & TAGCLASS_MASK) >> TAGCLASS_SHIFT)
#define getIsConstructed(first_byte) ((first_byte & ISCONSTRUCTED_MASK) >> \
                                       ISCONSTRUCTED_SHIFT)
#define getTagNumOfB1(first_byte) ((first_byte & TAGNUM_B1_MASK) >> TAGNUM_B1_SHIFT)


bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv)
{
  assert(length);

  const uint8_t *octet = buffer;
  tlv->tagClass = getTagClass(*octet);
  tlv->isConstructed = getIsConstructed(*octet);
  tlv->tagNum = getTagNumOfB1(*octet);
  return true;
}
