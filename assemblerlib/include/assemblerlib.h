#ifndef ASSEMBLERLIB_H
#define ASSEMBLERLIB_H

// Initialize assemblerlib.
void al_init(void);
// Free all allocations made by assemblerlib.
void al_deinit(void);

/*-------------------------------
  Number systems conversion API
-------------------------------*/

/* Fight "undefined reference to strrev" compiler error. */
char *strrev(char *str);

/* To binary conversions. */
extern char *hexadecimal_to_binary(const char *hexadecimal);
extern char  *decimal_to_binary(const int *decimal);

/* To hexadecimal coversions. */
extern char *binary_to_hexadecimal(const char *binary);
extern char *decimal_to_hexadecimal(const int *decimal);

/* To decimal conversions. */
extern int binary_to_decimal(const char *binary);
extern int hexadecimal_to_decimal(const char *hexadecimal);

#endif//ASSEMBLERLIB_H
