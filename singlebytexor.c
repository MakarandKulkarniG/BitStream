#include "BitStream.h"

/**
 * the cryptopals crypto challenges
 *
 * Set 1 / Challenge 3
 *
 * Single-byte XOR cipher
 * 
 * The hex encoded string:
 *   1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736
 * ... has been XOR'd against a single character. Find the key, decrypt the 
 * message.
 *
 * How? Devise some method for "scoring" a piece of English plaintext. 
 * Character frequency is a good metric. Evaluate each output and choose 
 * the one with the best score.
 * 
 * Achievement Unlocked
 *
 * You now have our permission to make "ETAOIN SHRDLU" jokes on Twitter.
 */
int main() {
   BitStream *cipher, *clear;

   uint16_t offset = 0;
   uint8_t  byte = 0;
   BitStream* key;

   int 	    i;
   cipher = BitStreamCreateAscii("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");

   if (cipher) {
     BitStreamShow(cipher);
     
     for (i = 0; i < 255; i++) {
	key = BitStreamCreate(BITS_PER_BYTE);
	if (key) {
	   BitStreamPutByte(key, i, 0, BITS_PER_BYTE);
	   
           clear = BitStreamExclusiveOr(cipher, key);
	   BitStreamShow(clear);

	   BitStreamDelete(clear);
        }
	BitStreamDelete(key);
     }
   }
}
