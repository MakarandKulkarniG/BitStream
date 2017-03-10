#include "BitStream.h"

/**
 * the cryptopals crypto challenges
 *
 * Set 1 / Challenge 4
 *
 * Detect single-byte XOR cipher
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

/**
 * @def WORDLEN_SCORE_HIGH
 * @brief maximum word length score to qualify as english text
 */
#define WORDLEN_SCORE_HIGH	6

/**
 * @def WORDLEN_SCORE_LOW
 * @brief mininum word length score to qualify as english text
 */
#define WORDLEN_SCORE_LOW	3

/**
 * @def NONPRINT_SCORE_HIGH
 * @brief upper limit on allowed non-printable characters 
 */
#define NONPRINT_SCORE_HIGH	3

/**
 * @def NONPRINT_SCORE_LOW
 * @brief for now we don't allow any non-printable characters except '\r\n'
 */
#define NONPRINT_SCORE_LOW	0

/**
 * @def struct EnglishTextScore
 *
 * @breif parameters to measure closeness of string to english text
 *
 * This structure is not yet complete neither is the function operating on it
 * as several other parameters need to be considered before being able to filter
 * out seemingly looking garbled text
 */
typedef struct EnglishTextScore {
   uint16_t WordLengthScore;/**< typically text contains 5 letters per word */

   uint16_t EtaoinScore;    /**< correlation to std etaoin histogram */

   uint16_t NonPrintScore;  /**< number of non-printable characters in string */
} EnglishTextScore;

int CountNonPrintsInStream(uint8_t * buf, uint16_t size) {
   int count = 0;
   int i = 0;
   while (i < size) {
     if (!isprint(buf[i]))
        count ++;
     i ++;
   }
   return count;
}

/**
 * @fn int CountWordsInStream(uint8_t* buf, uint16_t size)
 *
 * @brief quick and dirty word counting function 
 * 	this function find words separated by spaces alone! Multiple instances 
 * 	of spaces are taken as one separator
 * @param [in] buf\n
 * 	buffer in which we've to count words from
 * @param [in] size\n
 * 	length of the buffer in bytes
 * @returns number of words counted in the sentence
 */

int CountWordsInStream(uint8_t* buf, uint16_t size) {
   int count = 0;
   int i = 0;
   while (i < size) {
      while (i < size && buf[i] != ' ') 
	i ++;
      count ++;
      while (i < size && buf[i] == ' ')
	i ++;
   }
   return count;
}

/**
 * @fn int EnglishTextScoreCalc(EnglishTextScore* score, uint8_t *buf, 
 * 	uint16_t size) 
 *
 * @brief Calculates "score" for english language coherency
 *
 * @param [out] *score\n
 * 	pointer to score parameter structure to be filled 
 * @param [in] *buf\n
 * 	buffer to be parsed for calcualting the english language score
 * @param [in] size\n
 * 	length of the buffer in bytes
 * @returns average normalised score for the parsed text (should it be float?) 
 */

int EnglishTextScoreCalc(EnglishTextScore* score, uint8_t *buf, uint16_t size) {
    uint16_t nWords;

    score->NonPrintScore = CountNonPrintsInStream(buf, size);

    nWords = CountWordsInStream(buf, size);
    if (nWords > 0)
       score->WordLengthScore = size / nWords;
    else
       score->WordLengthScore = 100; /* out of range */

    if ((score->WordLengthScore < WORDLEN_SCORE_HIGH && score->WordLengthScore
		    > WORDLEN_SCORE_LOW) &&
        (score->NonPrintScore < NONPRINT_SCORE_HIGH && score->NonPrintScore 
	            > NONPRINT_SCORE_LOW) 
       )
       return score->WordLengthScore; 
    else 
       return (-1);
}

int main() {
   BitStream        *cipher, *clear;
   BitStream        *key;
   int 	      	    i;
   uint16_t         size;
   char             buffer[256];
   FILE	            *fp = NULL;
   EnglishTextScore score;

   fp = fopen("4.txt", "r");
   if (!fp) 
      return (-1);

   memset(buffer, '\0', sizeof(buffer));

   while (fgets(buffer, sizeof(buffer) - 1, fp)) {
     buffer[strlen(buffer) - 1] = '\0';
     cipher = BitStreamCreateAscii(buffer); 

     if (cipher) {
        for (i = 53; i < 255; i++) { /* key = 0 means clear text */
	   key = BitStreamCreate(BITS_PER_BYTE);
	   if (key) {
	      BitStreamPutByte(key, i, 0, BITS_PER_BYTE);
	   
              clear = BitStreamExclusiveOr(cipher, key);

	      size = (BitStreamGetSizeBits(clear) + BITS_PER_BYTE - 1)/
			   BITS_PER_BYTE;
	      if (EnglishTextScoreCalc(&score, BitStreamGetArray(clear),size) 
			      > 0) {
	         BitStreamShow(clear);
	      }

	      BitStreamDelete(clear);
           }
	   BitStreamDelete(key);
        }
     }
     memset(buffer, '\0', sizeof(buffer));
   }
}
