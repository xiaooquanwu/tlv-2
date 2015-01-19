/**
 * Copyright:
 *
 * The example parser parsed the test data and printed the result
 *
 * */

#include <stdio.h>
#include "tlv.h"

uint8_t tlv1Data[] =  {
  0x70,0x43,0x5F,0x20,0x1A,0x56,0x49,0x53,
  0x41,0x20,0x41,0x43,0x51,0x55,0x49,0x52,
  0x45,0x52,0x20,0x54,0x45,0x53,0x54,0x20,
  0x43,0x41,0x52,0x44,0x20,0x32,0x39,0x57,
  0x11,0x47,0x61,0x73,0x90,0x01,0x01,0x00,
  0x10,0xD1,0x01,0x22,0x01,0x11,0x43,0x87,
  0x80,0x89,0x9F,0x1F,0x10,0x31,0x31,0x34,
  0x33,0x38,0x30,0x30,0x37,0x38,0x30,0x30,
  0x30,0x30,0x30,0x30,0x30,0x90,0x00
};

uint8_t tlv2Data[] =  {
  0x00, 0x00,
  0x70,0x81,0x83,0x90,0x81,0x80,0x6F,0xC4,
  0x63,0xDD,0xD0,0x2A,0x73,0xB3,0x5C,0x84,
  0xDA,0xA7,0x26,0xEE,0x4D,0x3F,0x25,0x32,
  0x66,0x22,0xF1,0xD8,0x2A,0x07,0x48,0x11,
  0xAE,0x2B,0x1B,0x9A,0x67,0xCB,0x58,0xD9,
  0x55,0x73,0x5E,0xE6,0x35,0xD5,0x71,0xF3,
  0x9B,0x5C,0xE0,0xF6,0x4D,0x71,0xAF,0x73,
  0x2D,0x83,0xF3,0x7E,0x2B,0xD5,0x6D,0x67,
  0x22,0x13,0x76,0xC9,0x9B,0x14,0x3B,0x05,
  0x30,0xF2,0xFC,0xEA,0xB2,0xFE,0x63,0x50,
  0xC6,0x2F,0xCE,0xA0,0xC1,0x63,0xE4,0xBD,
  0x84,0xEC,0xB8,0x43,0x42,0xD0,0x5E,0xBF,
  0xB6,0x8F,0x6A,0x9E,0x49,0x96,0xD2,0xCA,
  0xB9,0x63,0x96,0x2E,0x54,0x8A,0x5B,0xEE,
  0xF5,0xEF,0xFF,0xD0,0x19,0x55,0xB9,0x2A,
  0xB5,0x06,0x4B,0xAC,0xB0,0xC8,0xBC,0x3E,
  0x1C,0x40,0x28,0x6D,0xFE,0xFC
};

uint8_t tlv3Data[] =  {
  0xFF,0xFF,
  0x70,0x0E,0x5A,0x08,0x47,0x61,0x73,0x90,
  0x01,0x01,0x00,0x10,0x5F,0x34,0x01,0x01,
  0x90,0x00
};

uint8_t tlv4Data[] =  {
  0x70,0x59,0x60,0x14,0x57,0x12,0x47,0x61,
  0x73,0x90,0x01,0x01,0x00,0x10,0xD1,0x01,
  0x22,0x01,0x11,0x43,0x87,0x80,0x89,0x90,
  0x5F,0x20,0x1A,0x56,0x49,0x53,0x41,0x20,
  0x41,0x43,0x51,0x55,0x49,0x52,0x45,0x52,
  0x20,0x54,0x45,0x53,0x54,0x20,0x43,0x41,
  0x52,0x44,0x20,0x32,0x39,0x57,0x11,0x47,
  0x61,0x73,0x90,0x01,0x01,0x00,0x10,0xD1,
  0x01,0x22,0x01,0x11,0x43,0x87,0x80,0x89,
  0x9F,0x1F,0x10,0x31,0x31,0x34,0x33,0x38,
  0x30,0x30,0x37,0x38,0x30,0x30,0x30,0x30,
  0x30,0x30,0x30,0x90,0x00
};

char *tagClassStr[] = {
  "Universal",
  "Application",
  "Context-Specific",
  "Private"
};

char *pOrCStr[] = {
  "Primitive",
  "Constructed"
};

static inline void printIndent(uint16_t level)
{
  int i = 0;
  for(i =0; i< level; i++) printf("  ");
}

static void printTagClass(TagClass_t tagClass, uint16_t level)
{
  printIndent(level);
  printf("Class: %d - %s\n", tagClass, tagClassStr[tagClass]);
}

static void printTagNum(TagNum_t tagNum, uint16_t level)
{
  printIndent(level);
  printf("Tag Number: %d\n", tagNum);
}

static void printLength(Length_t len, uint16_t level)
{
  printIndent(level);
  printf("Content length: %d\n", len);
}

static void printIsConstructed(bool isConstructed, uint16_t level)
{
  printIndent(level);
  printf("Primitive Or Constructed: %d - %s\n",
         isConstructed, pOrCStr[isConstructed]);
}

static void printData(uint8_t data[], Length_t len, uint16_t level)
{
  int i = 0;

  printIndent(level);
  for (i = 0; i < len; i++) {
    printf("%.2x", data[i]);
  }
  printf("\n");
}

static void printTlv(Tlv_t *tlv, uint16_t level)
{
  printTagClass(tlv->tagClass, level);
  printTagNum(tlv->tagNum, level);
  printLength(tlv->length, level);
  printIsConstructed(tlv->isConstructed, level);

  /* Print data value if it's primitive */
  if (!tlv->isConstructed) {
    printf("\n");
    return;
  }
}

/**
 * It traverses a parsed TLV in a depth-first way and
 * print the TLV fields as well
 * @param tlv a parsed tlv object
 * @param level recursive level, increase 1 one more depth is searched
 */
static void traverseTvl(Tlv_t *tlv, uint16_t level)
{
  uint8_t *cur = NULL, *end = NULL;
  Length_t left = 0;
  Tlv_t childTlv;

  printTlv(tlv, level);

  if (!tlv->isConstructed) {
    return;
  }
  /* one more depth */
  level++;

  cur = tlv->value;
  /* the left octets unhandled in the TLV. Nonzero means there are still TVLs */
  left = tlv->length;
  end = cur + left;

  do {
    if (!TlvParse(cur, left, &childTlv)) {
      printf("Failed to parse\n");
      return;
    }
    traverseTvl(&childTlv, level);
    cur = childTlv.value + childTlv.length;
    left = end - cur;
  } while(left);
}

static void parseTestData(uint8_t *data, size_t len, char *name)
{
  Tlv_t tlv;

  printf("\n%s: length=%lu\n", name, len);
  printData(data, len, 0);
  if (!TlvParse(data, len, &tlv)) {
    printf("Failed to parse %s\n", name);
    return;
  }
  traverseTvl(&tlv, 1);
}

int main()
{
  Tlv_t tlv;

  parseTestData(tlv1Data, sizeof(tlv1Data), "tlv1Data");
  parseTestData(tlv2Data, sizeof(tlv2Data), "tlv2Data");
  parseTestData(tlv3Data, sizeof(tlv3Data), "tlv3Data");
  parseTestData(tlv4Data, sizeof(tlv4Data), "tlv4Data");
}