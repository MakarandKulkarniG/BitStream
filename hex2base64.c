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
   char* hexbuf    = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
   uint16_t offset = 0;
   uint8_t byte    = 0;
   BitStream* bs   = NULL;

   uint16_t size = (strlen(hexbuf) + 1) > 1;

   bs = BitStreamCreate(size * BITS_PER_BYTE);

   if (BitStreamFillAscii(bs, hexbuf) > 0) {
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
   }
   return 0;
}
