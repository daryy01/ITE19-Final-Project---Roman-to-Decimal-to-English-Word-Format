#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int getValue(char roman);
int romanToDecimal(const char *roman);
int processOperation(int num1, int num2, char operator);
void numberToWords(int num, char *result);
void processFile(const char *inputFile, const char *outputFile);

int main()
{
    processFile("input.txt", "output.txt"); // Process input and output files
    printf("Processing complete. Check output.txt for results.\n");
    return 0;
}

// Get the decimal value of a single Roman numeral character
int getValue(char roman)
{
    switch (roman)
    {
    case 'I':
        return 1;
    case 'V':
        return 5;
    case 'X':
        return 10;
    case 'L':
        return 50;
    case 'C':
        return 100;
    case 'D':
        return 500;
    case 'M':
        return 1000;
    default:
        return 0; // Return 0 for invalid characters
    }
}

// Convert Roman numeral string to its decimal equivalent
int romanToDecimal(const char *roman)
{
    int total = 0;
    int prevValue = 0;

    for (int i = 0; roman[i] != '\0'; i++)
    {
        int currentValue = getValue(roman[i]);
        if (currentValue == 0)
            return -1; // Invalid Roman numeral

        if (currentValue > prevValue)
        {
            total += currentValue - 2 * prevValue; // Handle subtractive notation
        }
        else
        {
            total += currentValue;
        }

        prevValue = currentValue;
    }
    return total;
}

// Perform arithmetic operation and return the result
int processOperation(int num1, int num2, char operator)
{
    switch (operator)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0)
            return -1; // Error: Division by zero
        return num1 / num2;
    default:
        return -1; // Invalid operator
    }
}

// Convert a number to its English word representation
void numberToWords(int num, char *result)
{
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char buffer[200] = ""; // temporary array for storing the final result
    int partCount = 0;     // Keeps track of how many chunks (thousands, millions, etc.) have been processed.

    while (num > 0) // processes the number in parts of 3 digits (each chunk less than 1000). It continues until the number becomes zero.
    {
        int part = num % 1000;

        if (part > 0)
        {
            char partResult[100] = "";

            if (part / 100 > 0) // it adds the corresponding word Hundred
            {
                sprintf(partResult, "%s Hundred ", ones[part / 100]);
            }

            if (part % 100 >= 10 && part % 100 < 20) // it adds the corresponding word from the teens array.
            {
                strcat(partResult, teens[part % 100 - 10]);
            }
            else // it adds the corresponding word from the tens array.
            {
                strcat(partResult, tens[(part % 100) / 10]);
                strcat(partResult, " ");
                strcat(partResult, ones[part % 10]);
            }

            if (strlen(thousands[partCount]) > 0) // it adds the corresponding word from thousands array based on the partCount
            {
                strcat(partResult, " ");
                strcat(partResult, thousands[partCount]);
            }
            // After processing the current chunk, it concatenates this chunk into the final result:
            strcat(partResult, " ");
            strcat(partResult, buffer);
            strcpy(buffer, partResult);
        }

        num /= 1000; // the number is divided by 1000 to get the next chunk
        partCount++; // The partCount is incremented to track the place value
    }

    strcpy(result, buffer);
}

// Process input file and generate output file
void processFile(const char *inputFile, const char *outputFile)
{
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");

    if (in == NULL || out == NULL)
    {
        printf("Error opening file(s).\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), in))
    {
        char operand1[100], operand2[100], wordsResult[200];
        char operator;
        int decimal1, decimal2, result;

        // Parse the line
        if (sscanf(line, "%s %c %s", operand1, &operator, operand2) == 3)
        {
            decimal1 = romanToDecimal(operand1);
            decimal2 = romanToDecimal(operand2);

            if (decimal1 == -1 || decimal2 == -1)
            {
                fprintf(out, "Invalid Roman numeral: %s", line);
                continue;
            }

            result = processOperation(decimal1, decimal2, operator);
            if (result == -1)
            {
                fprintf(out, "Invalid operation or division by zero: %s", line);
                continue;
            }

            numberToWords(result, wordsResult);
            fprintf(out, "%s\n", wordsResult);
        }
        else
        {
            fprintf(out, "Invalid input format: %s", line);
        }
    }

    fclose(in);
    fclose(out);
}
