/**
 * @file BitStream.c
 * @brief Implements the routines for bitstream manipulation
 * @author Makarand Kulkarni
 * @internal BitStreamShow
 *           BitStreamCreate
 *           BitStreamDelete
 *           BitStreamPutByte
 *           BitStreamGetByte
 *	     BitStreamFill
 *
 * Copyright (c) 2017, Makarand Kulkarni under GPLv3 License
 */

#include "BitStream.h"

/**
 * @ingroup BitStream
 * @fn void BitSreamShow(BitStream* bs, const char* fmt)
 * @brief Show contents of bit stream as hex array
 *
 * @param [in]	bs\n
 *  	pointer to bitstream to show
 * @returns void
 */
void BitStreamShow(BitStream* bs) {
   uint16_t i;
   if (bs != NULL && bs->array != NULL) {
      for (i = 0; i < (bs->nbits + BITS_PER_BYTE - 1)/BITS_PER_BYTE; i++)
         printf("%02x ", bs->array[i]);
   }
}

/**
 * @ingroup Bitstream
 * @fn BitStream* BitStreamCreate(uint16_t nbits)
 * @brief Creates a object of type BitStream and allocates space to hold nbits
 *
 * @param [in] nbits\n
 * 	number of bits to hold in bit stream
 * @returns pointer to newly created bit stream object, NULL on failure
 */
BitStream* BitStreamCreate(uint16_t nbits) {
   
   BitStream *bs = (BitStream *)malloc(sizeof(BitStream));
   if (bs != NULL) {
      bs->array = (uint8_t *)calloc((nbits + BITS_PER_BYTE - 1)/BITS_PER_BYTE,
    	sizeof(uint8_t));
      if (NULL == bs->array) {
         free(bs);
         bs = NULL;
      } else {
	 memset(bs->array, (nbits + BITS_PER_BYTE - 1)/BITS_PER_BYTE, '\0');
         bs->nbits = nbits;
      }
   }
   return bs;
}

/**
 * @ingroup BitStream
 * @fn void BitStreamDelete(BitStream* bs)
 *
 * @brief Deletes the bit stream object, frees the memory holding bits
 *
 * @param [in] bs\n
 * 	pointer to bit stream object
 * @returns none
 */
void BitStreamDelete(BitStream* bs) {
   if (bs != NULL) {
      if (bs->array != NULL) {
         free(bs->array);
      }
      free(bs);
   }
}

/**
 * @ingroup BitStream
 * @fn uint16_t BitStreamPutByte(BitStream* bs, uint8_t byte, uint16_t offset,\n 
 * 	uint16_t nbits) 
 *
 * @brief inserts maximum 1 byte of data in bit stream at offset (in bits) nbits
 *
 * If the number of bits inserted are less than BITS_PER_BYTE (8) then the bits
 * are shifted left so that they are aligned in network order, i.e, inserting 3 
 * bits (101b) will cause the bits to be inserted in the specified byte as follows
 * 	- 101xxxxxb
 * where 'x' indicates the existing bits in stream
 *
 * @param [in] bs\n
 * 	Bit stream in which the bits are to be inserted
 * @param [in] byte\n
 * 	Bits to be inserted in bit stream (maximum 8)
 * @param [in] offset\n
 * 	offset in bits at which the above byte is to be inserted
 * @param [in] nbits\n
 * 	number of bits to insert, by default is should be 8, else the shift as
 * 	explained above applies
 * @returns Number of bits inserted. Insertion fails while inserting bits 
 * 	beyond the size of bit stream
 */
uint16_t BitStreamPutByte(BitStream* bs, uint8_t byte, uint16_t offset, 
	uint16_t nbits) {

   uint16_t curBits;
   uint8_t mask;
   uint16_t bitsCopied = 0;

   DECL_BYTE_OFFSET(i);
   DECL_BITS_OFFSET(j);

   if (nbits < 8) byte = byte << (BITS_PER_BYTE - nbits);

   curBits = MIN((BITS_PER_BYTE - j), nbits);

   mask = (0xFF >> j) & (0xFF << (BITS_PER_BYTE - (j + curBits)));

   bs->array[i] = (bs->array[i] & ~mask) | ((byte >> j) & mask);

   bitsCopied += curBits;

   byte = byte << curBits;

   if (curBits < nbits) {
      curBits = nbits - curBits;

      mask = 0xFF << (BITS_PER_BYTE - curBits);

      bs->array[i + 1] = (bs->array[i + 1] & ~mask) | (byte & mask);

      bitsCopied += curBits;
   }
   return bitsCopied;
}

/**
 * @ingroup BitStream
 * @fn uint16_t BitStreamGetByte(BitStream* bs, uint8_t *byte, uint16_t offset,\n 
 * 	uint16_t nbits) 
 *
 * @brief fetches maximum 1 byte of data in bit stream at offset (in bits) nbits
 *
 * If the number of bits requested are less than BITS_PER_BYTE (8) then the bits
 * are shifted right so that they are aligned in host order, i.e, requesting 3 
 * bits at offset 0, will fetch the bits as 00000xxx where xxx are the fetched 
 * bits
 *
 * @param [in] bs\n
 * 	Bit stream in which the bits are to be inserted
 * @param [out] *byte\n
 * 	Bits fetched from the stream (maxium 8)
 * @param [in] offset\n
 * 	offset in bits from which the above byte is to be fetched
 * @param [in] nbits\n
 * 	number of bits to insert, by default is should be 8, else the shift as
 * 	explained above applies
 * @returns Number of bits fetched. Retrieval fails while fetching bits 
 * 	beyond the size of bit stream
 */
uint16_t BitStreamGetByte(BitStream *bs, uint8_t *byte, uint16_t offset, 
		uint16_t nbits) {

   uint16_t curBits;
   uint8_t mask;
   uint16_t bitsCopied = 0;

   DECL_BYTE_OFFSET(i);
   DECL_BITS_OFFSET(j);

   if (offset + nbits > bs->nbits)
	   return (0);

   curBits = MIN((BITS_PER_BYTE - j), nbits);

   mask = (0xFF >> j) & (0xFF << (BITS_PER_BYTE - (j + curBits)));

   *byte = (bs->array[i] & mask) << j;

   bitsCopied += curBits;

   if (curBits < nbits) {
      curBits = nbits - curBits;

      mask = 0xFF << (BITS_PER_BYTE - curBits);

      *byte |= (bs->array[i + 1] & mask) >> (BITS_PER_BYTE - j);

      bitsCopied += curBits;
   }

   if (nbits < 8) *byte = *byte >> (BITS_PER_BYTE - nbits);

   return bitsCopied;
}

/**
 * @ingroup BitStream
 * @fn uint16_t BitStreamFill(BitStream* bs, uint8_t* inp, uint16_t nbits) 
 *
 * @brief fills the bytes from input buffer into bit stream
 *
 * @param [in,out] bs\n
 * 	bit stream to fill data in
 * @param [in] *inp\n
 * 	pointer to the data to be copied
 * @param [in] nbits\n
 * 	size of input data in bits
 * @returns number of bits copied into bit stream
 */
uint16_t BitStreamFill(BitStream* bs, uint8_t* inp, uint16_t nbits) {
   uint16_t bitsCopied = 0;
   
   while (bitsCopied < bs->nbits) {
      bitsCopied += BitStreamPutByte(bs, *inp++, bitsCopied, BITS_PER_BYTE);
   }

   return bitsCopied;
}


/**
 * Test purposes only, delete later
 */
int main() {
   uint16_t offset = 0;
   uint8_t byte;

   BitStream* bs = BitStreamCreate(24);
   BitStreamFill(bs, "Man", 24); 

   /*
   offset += BitStreamPutByte(bs, 19, offset, 6);
   offset += BitStreamPutByte(bs, 22, offset, 6);
   offset += BitStreamPutByte(bs, 5, offset, 6);
   offset += BitStreamPutByte(bs, 46, offset, 6);

   BitStreamShow(bs);
    */

   while (BitStreamGetByte(bs, &byte, offset, 6) > 0) {
	   switch (byte) {
	   case 0 ... 25:
		   printf("%c", 'A'+byte);
		   break;
	   case 26 ... 51:
		   printf("%c", 'a'+(byte-26));
		   break;
	   case 52 ... 61:
		   printf("%c", '1'+(byte-52));
		   break;
	   case 62: 
		   printf("+");
		   break;
	   case 63:
		   printf("/");
		   break;
	   default: 
		   printf("*");
	   }
	   offset += 6;
   }
   return 0;
}
