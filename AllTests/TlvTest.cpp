#include "CppUTest/TestHarness.h"

extern "C"
{
#include "tlv.h"
}

const uint8_t tlv1Data[] =  {
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

TEST_GROUP(TLV)
{
  void setup()
  {

  }
  void teardown()
  {

  }
};


TEST(TLV, ParseTagClassBITSuccessfully)
{
  uint8_t uni[] = { 0x00 }, pri[] = { 0xC0 };
  Tlv_t tlv;

  CHECK(TlvParse(uni, sizeof(uni), &tlv));
  LONGS_EQUAL(TAG_CLASS_UNI, TlvTagClass(&tlv));

  CHECK(TlvParse(pri, sizeof(pri), &tlv));
  LONGS_EQUAL(TAG_CLASS_PRI, TlvTagClass(&tlv));
}

TEST(TLV, ParsePorCBITSuccessfully)
{
  uint8_t primitive[] = { 0x00 }, constructed[] = { 0x20 };
  Tlv_t tlv;

  CHECK(TlvParse(primitive, sizeof(primitive), &tlv));
  CHECK(!TlvIsConstructed(&tlv));

  CHECK(TlvParse(constructed, sizeof(constructed), &tlv));
  CHECK(TlvIsConstructed(&tlv));
}

TEST(TLV, ParseTagNumberBITSuccessfully)
{
  uint8_t tagNum1[] = {0x01};   // minimum tag number
  uint8_t tagNum30[] = {0x1E};  // max tag number expressed in 1 byte
  uint8_t tagNum31[] = {0x1F, 0x1F}; // minimum tag number expressed in 2bytes
  uint8_t tagNum127[] = {0x1F, 0x7F}; // maximum tag number expressed in 2 bytes
  Tlv_t tlv;

  CHECK(TlvParse(tagNum1, sizeof(tagNum1), &tlv));
  LONGS_EQUAL(1, TlvTagNum(&tlv));

  CHECK(TlvParse(tagNum30, sizeof(tagNum30), &tlv));
  LONGS_EQUAL(30, TlvTagNum(&tlv));

  CHECK(TlvParse(tagNum31, sizeof(tagNum31), &tlv));
  LONGS_EQUAL(31, TlvTagNum(&tlv));

  CHECK(TlvParse(tagNum127, sizeof(tagNum127), &tlv));
  LONGS_EQUAL(127, TlvTagNum(&tlv));

}

TEST(TLV, ParseTlv1DataSuccessfully)
{
  Tlv_t tlv;
  // parse
  CHECK(TlvParse(tlv1Data, sizeof(tlv1Data), &tlv));
  // tag class
  LONGS_EQUAL(TAG_CLASS_APP, TlvTagClass(&tlv));
  // Primitive or constructed
  CHECK(TlvIsConstructed(&tlv));
  // tag number
  LONGS_EQUAL(0x10, TlvTagNum(&tlv));
}
