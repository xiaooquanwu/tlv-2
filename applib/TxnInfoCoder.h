/** \file
 * Copyright:
 *
 * Description: encoder and decoder for TxnInfo_t
 *
 */

#ifndef D_txnTypeCoder_H
#define D_txnTypeCoder_H

#include "tlv.h"
#include <stdint.h>
#include <arpa/inet.h>

#define TxnInfo_tagClass TAG_CLASS_APP
#define TxnInfo_tagNum 1
#define TxnInfo_isPorC IS_CONS

#define TxnRef_tagClass TAG_CLASS_APP
#define TxnRef_tagNum 2
#define TxnRef_isPorC IS_PRIM

#define Amount_tagClass TAG_CLASS_APP
#define Amount_tagNum 3
#define Amount_isPorC IS_PRIM
#define Amount_Type int32_t
#define Amount_Size sizeof(Amount_Type)
#define Amount_HToNE(x) ((Amount_Type)htonl((uint32_t)x))
#define Amount_NEToH(x) ((Amount_Type)ntohl((uint32_t)x))

#define TxnType_tagClass TAG_CLASS_APP
#define TxnType_tagNum 4
#define TxnType_isPorC IS_PRIM
#define TxnType_Type uint8_t
#define TxnType_Size sizeof(TxnType_Type)
#define TxnType_HToNE(x) (x)
#define TxnType_NEToH(x) (x)

#define CurrencyCode_tagClass TAG_CLASS_APP
#define CurrencyCode_tagNum 5
#define CurrencyCode_isPorC IS_PRIM
#define CurrencyCode_Type uint16_t
#define CurrencyCode_Size sizeof(CurrencyCode_Type)
#define CurrencyCode_HToNE(x) ((CurrencyCode_Type)htons((uint16_t)x))
#define CurrencyCode_NEToH(x) ((CurrencyCode_Type)ntohs((uint16_t)x))


#define MAX_TXN_REF_LEN 20

typedef struct
{
  char txnRef[MAX_TXN_REF_LEN+1]; // Zero terminated string
  int32_t amount;
  uint8_t txnType;
  uint16_t currencyCode;
} TxnInfo_t;

/* rough caculate, 2+2 is the tag octets and length octets,
   There are 4 primitive TLV for 4 fields and 1 constructed
   TLV the struct self. The calculate can be improved to a
   more accurate level, by calculate tag size and each
   primitive TLV size*/
#define TxnInfo_t_size (sizeof(TxnInfo_t) + (2 + 2) + (2 + 2) * 4)

/**
 * encoder for TxnInfo_t
 * @param buffer [in] Points to the buffer for saving encoded result
 * @param length [in] the length of buffer
 * @param value [in] the data value to be encoded
 * @param tlv [in] Point to an uninitialized tlv, returns an initialized
 * tlv after successfully encoding
 * @result true on success, false on failure
 */
extern bool TxnInfo_t_encode(uint8_t *buffer, size_t length,
                             const TxnInfo_t *value, Tlv_t *tlv);
/**
 * decoder for TxnInfo_t, usually used after TlvParse
 * @param value [out] address of TxnInfo_t to get decoded result
 * @param tlv [in] a parsed tlv object
 * @result true on success, false on failure
 */
extern bool TxnInfo_t_decode(TxnInfo_t *value, Tlv_t *tlv);

/* the following can be in their own files, put here because
   this is an eample to not distrubte too much files*/
extern bool TxnRef_encode(const char value[], Tlv_t *tlv);
/* extern bool TxnRef_encode(const TxnInfo_t *value, Tlv_t *tlv); */
extern bool Amount_encode(const Amount_Type value, Tlv_t *tlv);
extern bool TxnType_encode(const TxnType_Type value, Tlv_t *tlv);
extern bool CurrencyCode_encode(const CurrencyCode_Type value, Tlv_t *tlv);

extern bool TxnRef_decode(char value[], Tlv_t *tlv);
extern bool Amount_decode(Amount_Type *value, Tlv_t *tlv);
extern bool TxnType_decode(TxnType_Type *value, Tlv_t *tlv);
extern bool CurrencyCode_decode(CurrencyCode_Type *value, Tlv_t *tlv);

#endif
