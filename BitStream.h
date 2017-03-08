/**
 * @file  BitStream.h
 * @brief Libraries to manipulate data as bits, typically for crypto 
 * 	  functions
 */
#if !defined(_BITSTREAM_H)
#define _BITSTREAM_H

#include <stdlib.h>
#include <stdint.h>
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
#define MIN(a,b)	(a < b ? a : b)

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


#endif /* _BITSTREAM_H */
