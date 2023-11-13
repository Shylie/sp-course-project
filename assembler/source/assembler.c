#include <commonlib.h>
#include <assemblerlib.h>

#include <stdio.h>

static void each(int* element, size_t element_size, void* ud)
{
	printf("%d\n", *element);
}

struct data
{
	int a;
	float b;
};

int main(int argc, char** argv)
{
	assemble((const char* []) { "basic.txt", NULL });

	/* Conversions test. */

	int choice;
	char input[100]; // Buffer for user input 

	while (1) {
		printf("\nMenu:\n");
		printf("1. Hexadecimal to Binary\n");
		printf("2. Decimal to Binary\n");
		printf("3. Binary to Hexadecimal\n");
		printf("4. Decimal to Hexadecimal\n");
		printf("5. Binary to Decimal\n");
		printf("6. Hexadecimal to Decimal\n\n");
		printf("7. Exit\n\n");
		printf("Enter your choice: ");

		scanf("%d", &choice);

		if (choice == 7)
			break;

		switch (choice) {
		case 1:
			scanf("%s", input);
			char *result1 = hexadecimal_to_binary(input);
			printf("%s\n", result1);
      free(result1);
			break;
		case 2:
			scanf("%d", &choice);
			char *result2 = decimal_to_binary(&choice);
			printf("%s\n", result2);
      free(result2);
			break;
		case 3:
			scanf("%s", input);
			char *result3 = binary_to_hexadecimal(input);
			printf("%s\n", result3);
      free(result3);
			break;
		case 4:
			scanf("%d", &choice);
			char *result4 = decimal_to_hexadecimal(&choice);
			printf("%s\n", result4);
      free(result4);
			break;
		case 5:
			scanf("%s", input);
			int result5 = binary_to_decimal(input);
			printf("%d\n", result5);
			break;
		case 6:
			scanf("%s", input);
			int result6 = hexadecimal_to_decimal(input);
			printf("%d\n", result6);
			break;
		default:
			break;
		}
	}



	return 0;
}
