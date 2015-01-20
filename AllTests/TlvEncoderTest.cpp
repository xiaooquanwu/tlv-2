#include "CppUTest/TestHarness.h"

extern "C"
{
#include "tlvDecoder.h"
}

TEST_GROUP(TLVEncoder)
{
  void setup()
  {

  }
  void teardown()
  {

  }
};

// We are going to encoding the following Data,
// The tags are defined by ourselves
//
// typedef struct
// {
//   char TxnRef[MAX_TXN_REF_LEN+1]; // Zero terminated string
//   int32_t amount;
//   uint8_t txnType;
//   uint16_t currencyCode;
// } TxnInfo_t;

IGNORE_TEST(TLVEncoder, TLVEncoderStartHere)
{
  FAIL("Encoder Start Here")
}
