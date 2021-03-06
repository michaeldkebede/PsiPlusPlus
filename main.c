/**
 * This program is a lexical analyzer for Psi++ language.
 * Tokens are generated by the lexical analyzer
 * from the 'code.psi' file and written to the 'code.lex' file.
 **/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "functions.h"

// The main program logic
int main(void)
{
    // Open source file
    FILE *fptrSrc = fopen("code.psi", "r");
    if (fptrSrc == NULL)
    {
       printf("Error reading source code file.\nExiting...\n");
       return 1;
    }

    // Open object file
    FILE *fptrObj = fopen("code.lex", "w");
    if (fptrObj == NULL)
    {
       printf("Error opening object file.\nExiting...\n");
       return 1;
    }

    /* Read characters from source file and determine whether they are tokens.
       Start classifying tokens.
    */
    for(char c = fgetc(fptrSrc); c != EOF;)
    {
        // Possible beginning of string literal
        if (c == '"')
            c = checkIfStringLiteral(c, fptrSrc, fptrObj);

        // Possible beginning of comment
        else if (c == '{')
            c = checkIfComment(c, fptrSrc, fptrObj);

        // Possible beginning of identifier (this includes keywords)
        else if (isalpha(c))
            c = checkIfIdentifier(c, fptrSrc, fptrObj);

        // Possible beginning of int
        else if (isdigit(c))
            c = checkIfInt(c, fptrSrc, fptrObj);

        // Possible Operator or operator beginning
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == ':')
            c = checkIfOperator(c, fptrSrc, fptrObj);

        // End of line marker
        else if (c == '.')
        {
            fputs("EndOfLine\n", fptrObj);
            c = fgetc(fptrSrc);
        }

        // Brackets
        else if (c == '(' || c == ')' || c == '[' || c == ']')
        {
            if (c == '(')
                fputs("LeftPar\n", fptrObj);
            else if (c == ')')
                fputs("RightPar\n", fptrObj);
            else if (c == '[')
                fputs("LeftSquareBracket\n", fptrObj);
            else if (c == ']')
                fputs("RightSquareBracket\n", fptrObj);
            c = fgetc(fptrSrc);
        }

        // Ignore blank spaces
        else if (isspace(c))
            c = fgetc(fptrSrc);

        // If a char exits outside the above groupings' definition, print error
        else
        {
            printf("Error. Unidentified char in code.psi file.\nExiting...\n");
            return 1;
        }
    }

    // Close code.psi and code.lex file.
    fclose(fptrSrc);
    fclose(fptrObj);

    // Indicate success
    return 0;
}
