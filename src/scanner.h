#define SCANNER_H
#ifdef SCANNER_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // Logical operators tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOF_TOKEN
} TokenType;

typedef struct
{
    TokenType type;
    char *lexeme;
    void *literal;
    int line;
} Token;

typedef struct
{
    const char *source;
    Token **tokens;
    int start;
    int current;
    int line;
    int capacity;
    int tokenCount;
} Scanner;

bool isAtEnd(Scanner *scanner);
char advance(Scanner *scanner);
char *substring(Scanner *scanner);

Token *newTokenLitteral(TokenType type, const char *lexeme, void *literal, int line);
Token **addTokenLitteral(Scanner *scanner, TokenType type, void *literal);
Token *newToken(TokenType type, const char *lexeme, int line);
Token **addToken(Scanner *scanner, TokenType type);
void freeToken(Token *token);
void printToken(const Token *token);

Scanner *createScanner(const char *source);
void freeScanner(Scanner *scanner);
Token **scanTokens(Scanner *scanner);
void scanToken(Scanner *scanner);

#endif