#include "assemblerlib.h"
#include "commonlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct map bin_hex_representations;

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
  char *binary = (char *)malloc(33);

  unsigned int temp_decimal = *decimal;

  unsigned int i = 0;
  while (temp_decimal) {
    binary[i] = '0' + (temp_decimal & 1);
    temp_decimal = temp_decimal / 2;
    i++;
  }

  binary[i] = '\0';
  strrev(binary); 

  return binary;
}

char *binary_to_hexadecimal(const char *binary)
{
  char *hexadecimal = (char *)malloc(33);

  unsigned int binary_size = strlen(binary);
  unsigned int binary_pad = (4 - (binary_size % 4)) % 4;
  char *padded_binary = (char *)malloc(129);
  memset(padded_binary, '0', binary_pad);

  /* 
  Check whether map has not been initialized.
     
  if not, then DO IT.
  */
  if (!map_get(&bin_hex_representations, "0", 1)) {
    bin_hex_representations = map_new(4 * sizeof(char));

    map_set(&bin_hex_representations, "0", 1, "0000");
    map_set(&bin_hex_representations, "1", 1, "0001");
    map_set(&bin_hex_representations, "2", 1, "0010");
    map_set(&bin_hex_representations, "3", 1, "0011");
    map_set(&bin_hex_representations, "4", 1, "0100");
    map_set(&bin_hex_representations, "5", 1, "0101");
    map_set(&bin_hex_representations, "6", 1, "0110");
    map_set(&bin_hex_representations, "7", 1, "0111");
    map_set(&bin_hex_representations, "8", 1, "1000");
    map_set(&bin_hex_representations, "9", 1, "1001");
    map_set(&bin_hex_representations, "10", 2, "1010");
    map_set(&bin_hex_representations, "11", 2, "1011");
    map_set(&bin_hex_representations, "12", 2, "1100");
    map_set(&bin_hex_representations, "13", 2, "1101");
    map_set(&bin_hex_representations, "14", 2, "1110");
    map_set(&bin_hex_representations, "15", 2, "1111");
  }

  /* There's def some place for few optimizations. */
  unsigned int i;
  for (i = 0; i < binary_size + binary_pad; i += 4) {
    char *group = (char *)malloc(5);
    strncpy(group, padded_binary + i, 4);
    group[4] = '\0';

    unsigned int j;
    char *j_str = (char *)malloc(2);
    for (j = 0; j != 16; j++) {
      sprintf(j_str, "%u.%u", j / 10, j % 10);

      if (strcmp(group, map_get(&bin_hex_representations, j_str, j / 15 + 1)) == 0) {
        char *hexadecimal_digit = (char *)malloc(2);
        sprintf(hexadecimal, hexadecimal_digit);
        break;
      }
    }
  }

  map_del(&bin_hex_representations);

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

  unsigned int i;
  for (i = 0; i != length; i++)
    decimal = decimal * 2 + (binary[i] - '0');

  return decimal;
}

int hexadecimal_to_decimal(const char *hexadecimal)
{
  int decimal = 0;

  return decimal;
}
