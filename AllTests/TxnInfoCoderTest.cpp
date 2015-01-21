#include "CppUTest/TestHarness.h"
#include <cstring>

extern "C"
{
#include "TxnInfoCoder.h"
#include "tlvDecoder.h"
}

TEST_GROUP(TxnInfoCoder)
{
  void setup()
  {

  }
  void teardown()
  {

  }
};

TEST(TxnInfoCoder, StartHere)
{
  uint8_t buffer[TxnInfo_t_size];
  TxnInfo_t value, parsedValue;
  Tlv_t tlv, parsedTlv;

  // encoding
  strcpy(value.txnRef, "abc");
  value.amount = 22;
  value.txnType = 253;
  CHECK(TxnInfo_t_encode(buffer, TxnInfo_t_size, &value, &tlv));

  // decoding
  CHECK(TlvParse(buffer, TxnInfo_t_size, &parsedTlv));
  LONGS_EQUAL(TxnInfo_tagClass, TagTagClass(&parsedTlv.tag));
  LONGS_EQUAL(TxnInfo_isPorC, TagIsPorC(&parsedTlv.tag));
  LONGS_EQUAL(TxnInfo_tagNum, TagTagNum(&parsedTlv.tag));
  CHECK(TxnInfo_t_decode(&parsedValue, &parsedTlv));
  STRCMP_EQUAL("abc", parsedValue.txnRef);
  LONGS_EQUAL(value.amount, parsedValue.amount);
  LONGS_EQUAL(value.txnType, parsedValue.txnType);
}
