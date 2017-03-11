#include "BitStream.h"

/**
 * the cryptopals crypto challenges
 *
 * Set 1 / Challenge 1
 *
 * Convert hex to base64
 *
 * The string:
 *   49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f
 *   7573206d757368726f6f6d
 * should produce:
 *   SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
 *
 * So go ahead and make that happen. You'll need to use this code for the rest 
 * of the exercises.
 *
 * Cryptopals Rule
 * Always operate on raw bytes, never on encoded strings. Only use hex and 
 * base64 for pretty-printing.
 *    
 */
int main() {
   BitStream *hex, *base64;
   uint16_t offset = 0;
   uint8_t  byte = 0;

   if ((hex = BitStreamCreateHex("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d")) != NULL) {
      base64 = BitStreamHex2Base64(hex);
      BitStreamShow(hex);
      printf("\n");
      if (base64 != NULL) 
	      BitStreamShow(base64);

      BitStreamDelete(hex);
      BitStreamDelete(base64);
   }
   return 0;
}
