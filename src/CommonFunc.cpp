#include "CommonFunc.h"

/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
uint8_t asciiHexToInt(char asciiChar)
{
	uint8_t val = 0;
	// transform hex character to the 4bit equivalent number, using the ascii table indexes
	if (asciiChar >= '0' && asciiChar <= '9') asciiChar = asciiChar - '0';
	else if (asciiChar >= 'a' && asciiChar <= 'f') asciiChar = asciiChar - 'a' + 10;
	else if (asciiChar >= 'A' && asciiChar <= 'F') asciiChar = asciiChar - 'A' + 10;
	// shift 4 to make space for new digit, and add the 4 bits of the new digit 
	val = (val << 4) | (asciiChar & 0xF);
	return val;
}
