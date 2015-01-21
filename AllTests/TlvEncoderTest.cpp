#include "CppUTest/TestHarness.h"

extern "C"
{
#include "tlvEncoder.h"
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

TEST(TLVEncoder, FailToCreateTLVContainerUsingShortBuffer)
{
  Tlv_t tlv;
  const size_t bufferLen = 2;
  uint8_t buffer[bufferLen];
  CHECK(!TlvCreate(&tlv, 0x7000, buffer, bufferLen));
}

TEST(TLVEncoder, FailToAddDataUsingShortBuffer)
{
  Tlv_t tlv;
  const size_t bufferLen = 6;
  uint8_t buffer[bufferLen];
  uint8_t data[10];

  CHECK(TlvCreate(&tlv, 0x7000, buffer, bufferLen));
  CHECK(!TlvAddData(&tlv, 0x7100, data, sizeof(data)));
}

TEST(TLVEncoder, CreateTLVContainerSuccessfully)
{
  Tlv_t tlv;
  const size_t bufferLen = 50;
  uint8_t buffer[bufferLen];
  uint8_t expected[]={0x70,0x81,0x00};

  CHECK(TlvCreate(&tlv, 0x7000, buffer, bufferLen));
  POINTERS_EQUAL(buffer, TlvPtr(&tlv));
  LONGS_EQUAL(bufferLen - (TlvValue(&tlv) - TlvPtr(&tlv)),
              TlvDataCapacity(&tlv));
  LONGS_EQUAL(TAG_CLASS_APP, TagTagClass(&tlv.tag));
  CHECK(TagIsPorC(&tlv.tag));
  LONGS_EQUAL(0x10, TagTagNum(&tlv.tag));
  LONGS_EQUAL(0, TlvDataLen(&tlv));
  POINTERS_EQUAL(&buffer[3], TlvValue(&tlv));
  //tag
  BYTES_EQUAL(expected[0], buffer[0]);
  //length
  BYTES_EQUAL(expected[1], buffer[1]);
  BYTES_EQUAL(expected[2], buffer[2]);
}

TEST(TLVEncoder, AddDataToTLVContainerSuccessfully)
{
  Tlv_t tlv;
  const size_t bufferLen = 50;
  uint8_t buffer[bufferLen];
  uint8_t data1 = 0x18;
  uint8_t data2[2] = {0x19, 0x67};
  uint8_t expected[]={0x70,0x81,0x09,
                      0x71,0x81,0x01,0x18,
                      0x72,0x81,0x02,0x19, 0x67};

  CHECK(TlvCreate(&tlv, 0x7000, buffer, bufferLen));
  // 1. add one data
  CHECK(TlvAddData(&tlv, 0x7100, &data1, sizeof(data1)));
  //tag of child
  BYTES_EQUAL(expected[3], buffer[3]);
  //length of child
  BYTES_EQUAL(expected[4], buffer[4]);
  BYTES_EQUAL(expected[5], buffer[5]);
  //data
  BYTES_EQUAL(expected[6], buffer[6]);

  // 2. add two data
  CHECK(TlvAddData(&tlv, 0x7200, data2, sizeof(data2)));
  //tag of child
  BYTES_EQUAL(expected[7], buffer[7]);
  //length of child
  BYTES_EQUAL(expected[8], buffer[8]);
  BYTES_EQUAL(expected[9], buffer[9]);
  // data
  BYTES_EQUAL(expected[10], buffer[10]);
  BYTES_EQUAL(expected[11], buffer[11]);

  // 3. father
  //tag of father
  BYTES_EQUAL(expected[0], buffer[0]);
  //length of father
  BYTES_EQUAL(expected[1], buffer[1]);
  BYTES_EQUAL(expected[2], buffer[2]);
}

TEST(TLVEncoder, AddTLVObjectToTLVContainerSuccessfully)
{
  Tlv_t container, child;
  const size_t bufferLen = 50;
  uint8_t buffer1[bufferLen];
  uint8_t buffer2[bufferLen];
  uint8_t data = 0x18;
  uint8_t expected[]={0x70,0x81,0x07, // container tag length
                      0x71,0x81,0x04,
                      0x72,0x81,0x01,0x18}; // child

  CHECK(TlvCreate(&container, 0x7000, buffer1, bufferLen));
  // 1. create child tlv object
  CHECK(TlvCreate(&child, 0x7100, buffer2, bufferLen));
  CHECK(TlvAddData(&child, 0x7200, &data, sizeof(data)));
  // 2. add child tlv object to container
  CHECK(TlvAdd(&container, &child));
  for (unsigned int i = 0; i < sizeof(expected); i++)
    BYTES_EQUAL(expected[i], buffer1[i]);
}
