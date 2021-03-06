/**
 * @file  BitStream.h
 * @brief Libraries to manipulate data as bits, typically for crypto 
 * 	  functions
 */
#if !defined(_BITSTREAM_H)
#define _BITSTREAM_H

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Macro Definitions */
/**
 * @def BITS_PER_BYTE
 * @brief Number of bits in byte (always 8)
 */
#define BITS_PER_BYTE	8

/**
 * @def MIN
 * @brief Returns mininum of given 2 quantities
 */
#define MIN(a,b)	((a) < (b) ? (a) : (b))

/**
 * @def DECL_BYTE_OFFSET
 * @brief creates a variable to hold byte offset from input param "offset"
 */
#define DECL_BYTE_OFFSET(a)	\
	uint16_t a = (offset) / BITS_PER_BYTE;

/**
 * @def DECL_BITS_OFFSET
 * @brief creates a variable to hold bits offset from input param "offset"
 */
#define DECL_BITS_OFFSET(a)	\
	uint16_t a = (offset) % BITS_PER_BYTE;

/* Type Definitions */
/**
 * @struct BitStream
 * @brief This represents the bit stream for manipulation
 */
typedef struct BitStream {
   /**< @brief container for bit stream */
   uint8_t 	*array;
   /**< @brief number of bits in the container */
   uint16_t	nbits;
} BitStream;


uint16_t BitStreamGetSizeBits(BitStream *bs) ;

uint8_t* BitStreamGetArray(BitStream *bs) ;

BitStream* BitStreamCreate(uint16_t nbits) ;

BitStream* BitStreamCreateHex(const char* s) ;

BitStream* BitStreamCreateAscii(const char* s) ;

void BitStreamDelete(BitStream* bs) ;

void BitStreamShow(BitStream* bs) ;

uint16_t BitStreamPutByte(BitStream* bs, uint8_t byte, uint16_t offset, 
	uint16_t nbits) ;

uint16_t BitStreamGetByte(BitStream *bs, uint8_t *byte, uint16_t offset, 
		uint16_t nbits) ;

uint16_t BitStreamCopy(BitStream* bs, uint8_t* inp, uint16_t nbits) ;

uint16_t BitStreamCopyHex(BitStream* bs, const char* inp) ;

uint16_t BitStreamCopyAscii(BitStream* bs, const char* inp) ;

uint16_t BitStreamFill(BitStream* bs, uint8_t byte) ;

BitStream* BitStreamHex2Base64(BitStream *bs) ;

BitStream* BitStreamExclusiveOr(BitStream *bx, BitStream *by) ;
#endif /* _BITSTREAM_H */
