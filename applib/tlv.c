#include <assert.h>
#include "tlv.h"

#define TAGCLASS_MASK 0xC0
#define TAGCLASS_SHIFT 6

#define get_taglass(first_byte) ((first_byte & TAGCLASS_MASK) >> TAGCLASS_SHIFT)

bool TlvParse(const uint8_t *buffer, size_t length, Tlv_t *tlv)
{
  assert(length);

  const uint8_t *octet = buffer;
  tlv->tagClass = get_taglass(*octet);

  return true;
}
