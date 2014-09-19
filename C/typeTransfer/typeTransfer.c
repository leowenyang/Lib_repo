/**
 * @file typeTransfer.c
 * @brief All the type to transfer another type
 * @author leowenyang, leowenyang@163.com
 * @version 1.0
 * @date 2014-09-19
 */

#include "typeTransfer.h"

/**
 * @brief GSM8bitTo7bit : GSM spec define 8bit data transfer 7bit data
 *
 * @param p8bitBuf : 8bit data
 * @param b8bitLen : 8bit data length
 *
 * @return : 7bit data len
 *
 *      the result (7bit data) will be save into p8bitBuf 
 */

#define GROUP_8BIT_LEN 0x08
BYTE GSM8bitTo7bit(BYTE* p8bitBuf, BYTE b8bitLen)
{
  BYTE bStart = 0;
  BYTE i, index, len;
  BYTE* pGroup;

  // len is 0
  if(!b8bitLen) return 0x00;

  for (i = 0; i <= (b8bitLen-1)/GROUP_8BIT_LEN; i++)
  {
    // get group head
    pGroup = &p8bitBuf[i*GROUP_8BIT_LEN];

    // handle 8 byte group
    index = 0;
    len = b8bitLen - i*GROUP_8BIT_LEN;
    len > GROUP_8BIT_LEN ? (len=GROUP_8BIT_LEN) : len;
    while(index < len) {
      // remove low bit 
      pGroup[index] = pGroup[index] >> index;
      index++;
      if (index == len)
      {
        // last byte is 7, padding 0001101
        if (len == (GROUP_8BIT_LEN-1)) {
          pGroup[index-1] |= 0x1A;
        }
        // 8 byte group, should delete last byte
        // no 8 byte group, move 7bit into buffer
        if (GROUP_8BIT_LEN != len) {
          p8bitBuf[bStart++] = pGroup[index-1];
        }

        // goto next 8 byte group
        break;
      }
      // add the follow byte low bit
      pGroup[index-1] |= (pGroup[index] << (GROUP_8BIT_LEN - index));
      // move 7bit data into buffer
      p8bitBuf[bStart++] = pGroup[index-1];
    }
  }
  return bStart;
}


