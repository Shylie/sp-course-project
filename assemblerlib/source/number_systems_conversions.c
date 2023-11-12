#include "assemblerlib.h"
#include "commonlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *reverse(char *str)
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

  reverse(binary);

  return binary;
}

char *decimal_to_binary(const int *decimal)
{
  char *binary = (char *)malloc(33);

  unsigned int temp_decimal = *decimal;

  unsigned int i = 0;
  while (temp_decimal) {
    binary[i] = '0' + (temp_decimal & 1);
    temp_decimal = temp_decimal / 2;
    i++;
  }

  binary[i] = '\0';
  reverse(binary);

  return binary;
}

char *binary_to_hexadecimal(const char *binary)
{
  char *hexadecimal = (char *)malloc(33);

  unsigned int binary_size = strlen(binary);
  unsigned int binary_pad = (4 - (binary_size % 4)) % 4;
  char *padded_binary = (char *)malloc(129);
  memset(padded_binary, '0', binary_pad);
  strcpy(padded_binary + binary_pad, binary);

  struct array bin_hex_representations = array_new(sizeof(char *), 0);
  array_append(&bin_hex_representations, "0000");
  array_append(&bin_hex_representations, "0001");
  array_append(&bin_hex_representations, "0010");
  array_append(&bin_hex_representations, "0011");
  array_append(&bin_hex_representations, "0100");
  array_append(&bin_hex_representations, "0101");
  array_append(&bin_hex_representations, "0110");
  array_append(&bin_hex_representations, "0111");
  array_append(&bin_hex_representations, "1000");
  array_append(&bin_hex_representations, "1001");
  array_append(&bin_hex_representations, "1010");
  array_append(&bin_hex_representations, "1011");
  array_append(&bin_hex_representations, "1100");
  array_append(&bin_hex_representations, "1101");
  array_append(&bin_hex_representations, "1110");
  array_append(&bin_hex_representations, "1111");

  /* There's def some place for few optimizations. */
  unsigned int i;
  for (i = 0; i < binary_size + binary_pad; i += 4) {
    char group[5];
    strncpy(group, padded_binary + i, 4);
    group[4] = '\0';

    unsigned int j;
    for (j = 0; j < 16; j++)
      if (strcmp(group, (char *)array_at(&bin_hex_representations, j)) == 0) {
        char hexadecimal_digit[2];
        sprintf(hexadecimal_digit, "%X", j);
        strcat(hexadecimal, hexadecimal_digit);
        break;
      } 
  }

  array_del(&bin_hex_representations);

  return strdup(hexadecimal);
}

char *decimal_to_hexadecimal(const int *decimal)
{
  /* Decimal -> Binary -> Hexadecimal */
  char *binary = decimal_to_binary(decimal);

  return binary_to_hexadecimal(binary);
}

int binary_to_decimal(const char *binary)
{
  int decimal = 0;
  unsigned int length = strlen(binary);

  unsigned int i;
  for (i = 0; i != length; i++)
    decimal = decimal * 2 + (binary[i] - '0');

  return decimal;
}

int hexadecimal_to_decimal(const char *hexadecimal)
{
  char *binary = hexadecimal_to_binary(hexadecimal);

  return binary_to_decimal(binary);
}
