/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include "front.h"
//#include "parser.h"
#include <string.h>



/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static int lookup(char ch);
void printToken(int token, const char *lexeme); // Added function prototype

/******************************************************/
/* main driver */
int main(int argc, char *argv[])
{
    /* Check if a file name is provided as a command-line argument */
    // if (argc != 2) {
    //     printf("ERROR: Input file name not provided\n");
    //     return 1;  // Exit the program if no file is provided
    // }
    /* Print the required header */
    printf("Cooke Analyzer :: R11763784\n");  // Replace with your roll number
    /* Open the input data file and process its contents */
    //if((in_fp = fopen(argv[1], "r")) == NULL) {
      //  printf("ERROR: cannot open %s\n", argv[1]);
        //return 1;  // Exit the program if the file can't be opened
    if((in_fp = fopen("test.dc", "r")) == NULL) {
         printf("ERROR: cannot open %s\n", argv[1]);
        return 1;  // Exit the program if the file can't be opened
    } else {
        getChar();

        // for (int i = 0; i <50 ; i++){
        //     lex();
        // }
        do {
            lex();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
static int lookup(char ch)
{
    switch (ch) {
        case '(':
            addChar();
            nextToken = OPEN_PAREN;
            getChar();
            break;
        case ')':
            addChar();
            nextToken = CLOSE_PAREN;
            getChar();
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            getChar();
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            getChar();
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            getChar();
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            getChar();
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;
            getChar();
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            getChar();
            break;
        case '{':
            addChar();
            nextToken = OPEN_CURL;
            getChar();
            break;
        case '}':
            addChar();
            nextToken = CLOSE_CURL;
            getChar();
            break;
        case '=':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                //getChar();
                nextToken = EQUAL_OP;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '='
                nextToken = ASSIGN_OP;
            }
            break;
        case '!':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                //getChar();
                nextToken = NEQUAL_OP;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '='
                nextToken = BOOL_NOT;
            }
            break;
        case '<':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                //getChar();
                nextToken = LEQUAL_OP;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '='
                nextToken = LESSER_OP;
            }
            break;
        case '>':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                //getChar();
                nextToken = GEQUAL_OP;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '='
                nextToken = GREATER_OP;
            }
            break;
        case '&':
            addChar();
            getChar();
            if (nextChar == '&') {
                addChar();
                //getChar();
                nextToken = BOOL_AND;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '&'
                nextToken = UNKNOWN;
            }
            break;
        case '|':
            addChar();
            getChar();
            if (nextChar == '|') {
                addChar();
                //getChar();
                nextToken = BOOL_OR;
                getChar();
            } else {
                //ungetc(nextChar, in_fp);  // Put it back if it's not '|'
                nextToken = UNKNOWN;
            }
            break;
        default:
            addChar();
            nextToken = UNKNOWN;
            getChar();
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar()
{
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
static void getChar()
{
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
static void getNonBlank()
{
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex()
{
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            if ((strcmp(lexeme, "input") == 0)) {
                nextToken = KEY_IN;
            }
            else if ((strcmp(lexeme, "output") == 0)) {
                nextToken = KEY_OUT;
            }
            else if ((strcmp(lexeme, "if") == 0)) {
                nextToken = KEY_IF;
            }
            else if ((strcmp(lexeme, "else") == 0)) {
                nextToken = KEY_ELSE;
            }
            else {
                nextToken = IDENT;
            }

            //nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            return nextToken;  // Return immediately without printing EOF token
    } /* End of switch */

    printToken(nextToken, lexeme); // Print the token and lexeme
    return nextToken;
}

/*****************************************************/
/* printToken - a function to print the lexeme and token type */
void printToken(int token, const char *lexeme) {
    char *tokenName;

    switch (token) {
        case OPEN_PAREN:    tokenName = "OPEN_PAREN"; break;
        case CLOSE_PAREN:   tokenName = "CLOSE_PAREN"; break;
        case OPEN_CURL:     tokenName = "OPEN_CURL"; break;
        case CLOSE_CURL:    tokenName = "CLOSE_CURL"; break;
        case ADD_OP:        tokenName = "ADD_OP"; break;
        case SUB_OP:        tokenName = "SUB_OP"; break;
        case MULT_OP:       tokenName = "MULT_OP"; break;
        case DIV_OP:        tokenName = "DIV_OP"; break;
        case MOD_OP:        tokenName = "MOD_OP"; break;
        case SEMICOLON:     tokenName = "SEMICOLON"; break;
        case EQUAL_OP:      tokenName = "EQUAL_OP"; break;
        case ASSIGN_OP:     tokenName = "ASSIGN_OP"; break;
        case NEQUAL_OP:     tokenName = "NEQUAL_OP"; break;
        case BOOL_NOT:      tokenName = "BOOL_NOT"; break;
        case LEQUAL_OP:     tokenName = "LEQUAL_OP"; break;
        case LESSER_OP:     tokenName = "LESSER_OP"; break;
        case GEQUAL_OP:     tokenName = "GEQUAL_OP"; break;
        case GREATER_OP:    tokenName = "GREATER_OP"; break;
        case BOOL_AND:      tokenName = "BOOL_AND"; break;
        case BOOL_OR:       tokenName = "BOOL_OR"; break;
        case IDENT:         tokenName = "IDENT"; break;
        case INT_LIT:       tokenName = "INT_LIT"; break;
        case KEY_IN:        tokenName = "KEY_IN"; break;
        case KEY_OUT:       tokenName = "KEY_OUT"; break;
        case KEY_IF:        tokenName = "KEY_IF"; break;
        case KEY_ELSE:      tokenName = "KEY_ELSE"; break;
        case UNKNOWN:       tokenName = "UNKNOWN"; break;
    }

    // Print both token type and lexeme
    printf("%s %s\n", lexeme, tokenName);
}
