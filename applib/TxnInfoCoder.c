#include "TxnInfoCoder.h"
#include "tlvEncoder.h"
#include "tlvDecoder.h"
#include "tag.h"
#include <string.h>
#include <stdio.h>

bool TxnRef_encode(const char value[], Tlv_t *tlv)
{
  uint16_t tag;
  tag = TagFieldsToUint16(TxnRef_tagClass, TxnRef_isPorC, TxnRef_tagNum);
  if (!TlvAddData(tlv, tag,
                  (uint8_t *)value,
                  strlen(value))) return false;
  return true;
}

bool Amount_encode(const int32_t value, Tlv_t *tlv)
{
  uint16_t tag;
  tag = TagFieldsToUint16(Amount_tagClass, Amount_isPorC, Amount_tagNum);
  Amount_Type amount = Amount_HToNE(value);
  if (!TlvAddData(tlv, tag,
                  (uint8_t *)&amount,
                  Amount_Size)) return false;
  return true;
}

bool TxnInfo_t_encode(uint8_t *buffer, size_t length,
                       const TxnInfo_t *value, Tlv_t *tlv)
{
  uint16_t tag;

  tag = TagFieldsToUint16(TxnInfo_tagClass, TxnInfo_isPorC, TxnInfo_tagNum);
  if (!TlvCreate(tlv, tag, buffer, length)) return false;

  if (!TxnRef_encode(value->txnRef, tlv)) return false;
  if (!Amount_encode(value->amount, tlv)) return false;

  return true;
}

bool TxnRef_decode(char value[], Tlv_t *tlv)
{
  Tlv_t child;
  uint16_t tag;

  tag = TagFieldsToUint16(TxnRef_tagClass, TxnRef_isPorC, TxnRef_tagNum);
  if (!TlvSearchTag(TlvValue(tlv),
                    TlvDataLen(tlv),
                    tag, true, &child)) return false;
  memcpy(value, TlvValue(&child), TlvDataLen(&child));
  return true;
}

bool Amount_decode(int32_t *value, Tlv_t *tlv)
{
  Tlv_t child;
  uint16_t tag;

  tag = TagFieldsToUint16(Amount_tagClass, Amount_isPorC, Amount_tagNum);
  if (!TlvSearchTag(TlvValue(tlv),
                    TlvDataLen(tlv),
                    tag, true, &child)) return false;
  Amount_Type amount;
  memcpy(&amount, TlvValue(&child), Amount_Size);
  *value = Amount_NEToH(amount);

  return true;
}

bool TxnInfo_t_decode(TxnInfo_t *value, Tlv_t *tlv)
{
  memset(value, 0, sizeof(*value));

  if (!TxnRef_decode(value->txnRef, tlv)) return false;
  if (!Amount_decode(&value->amount, tlv)) return false;

  return true;
}
