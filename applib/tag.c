#include "tag.h"

uint16_t TagFieldsToUint16(TagClass_t tagClass,
                           PorC_t isPorC, TagNum_t tagNum)
{
  uint16_t encoded = 0;
  uint8_t *octet = (uint8_t *)&encoded;

  octet[1] |= tagClass << TAGCLASS_SHIFT;
  octet[1] |= isPorC << ISCONSTRUCTED_SHIFT;
  if (tagNumIsIn1Byte(tagNum)) {
    octet[1] |= tagNum;
  } else {
    octet[1] |= TAGNUM_MULTIBYTES_LEADING;
    octet[0] |= tagNumSetAsLast(tagNum);
  }
  return encoded;
}

uint16_t TagToUint16(Tag_t tag)
{
  return TagFieldsToUint16(tag.tagClass, tag.isPorC, tag.tagNum);
}

Tag_t TagFromUint16(uint16_t tag)
{
  Tag_t tlvTag;

  uint8_t *octet = (uint8_t *)&tag;

  tlvTag.tagClass = getTagClass(octet[1]);
  tlvTag.isPorC = getIsPorC(octet[1]);

  tlvTag.tagNum = getTagNumOfB1(octet[1]);
  if (!tagNumIsIn1Byte(TagTagNum(&tlvTag))) {
    /* as only max two bytes tag is supported */
    /* if (isNotLastByteOfTagNum(octet[0])) return false; */
    tlvTag.tagNum = getTagNumOfB2(octet[0]);
  }
  return tlvTag;
  /* return true; */
}

bool TagIsMatch(uint16_t encoded, Tag_t tag)
{
  return encoded == TagToUint16(tag);
}
