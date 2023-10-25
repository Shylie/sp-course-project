#include "assemblerlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strrev(char *str)
{
  char *p1, *p2;

  if (!str || !*str)
    return str;
  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
    *p1 ^= *p2;
    *p2 ^= *p1;
    *p1 ^= *p2;
  }
  return str;
}

char *hexadecimal_to_binary(const char *hexadecimal)
{
  unsigned int temp_hexadecimal;
  char *binary = (char *)malloc(33);

  sscanf(hexadecimal, "%x", &temp_hexadecimal);

  unsigned int i = 0;
  while (temp_hexadecimal) {
    binary[i] = '0' + temp_hexadecimal % 2;
    temp_hexadecimal /= 2;
    i++;
  }

  binary[i] = '\0';

  strrev(binary);

  return binary;
}

char *decimal_to_binary(const int *decimal)
{
  int temp_decimal = *decimal; /* so that we can perform bit shifting operations */
  char *binary = (char *)malloc(33); /* allocate enough memory for a 32-bit binary number */

  unsigned int i = 0;
  while (temp_decimal) {
    binary[i] = '0' + (temp_decimal & 1);
    /* temp_decimal = temp_decimal / 2; */
    temp_decimal = temp_decimal >> 1; /* shift temp_decimal by one bit thus dividing the int by 2 */
    i++;
  }

  binary[i] = '\0';
  strrev(binary); 

  return binary;
}

char *binary_to_hexadecimal(const char *binary)
{
  char *hexadecimal;

  return hexadecimal;
}

char *decimal_to_hexadecimal(const int *decimal)
{
  char *hexadecimal;

  return hexadecimal;
}

int binary_to_decimal(const char *binary)
{
  int decimal = 0;
  unsigned int length = strlen(binary);

  int i;
  for (i = 0; i != length; i++)
    decimal = decimal * 2 + (binary[i] - '0');

  return decimal;
}

int hexadecimal_to_decimal(const char *hexadecimal)
{
  int decimal = 0;

  return decimal;
}
