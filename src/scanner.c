#include "scanner.h"

bool isAtEnd(Scanner *scanner)
{
    return scanner->current >= strlen(scanner->source);
}

char advance(Scanner *scanner)
{
    return scanner->source[scanner->current++];
}

char *substring(Scanner *scanner)
{
    size_t length = scanner->current - scanner->start;
    char *text = (char *)malloc(length + 1);
    if (text == NULL)
    {
        perror("Failed to allocate memory for token text");
        exit(EXIT_FAILURE);
    }
    strncpy(text, scanner->source + scanner->start, length);
    text[length] = '\0';
    return text;
}

Token *newTokenLitteral(TokenType type, const char *lexeme, void *literal, int line)
{
    Token *token = malloc(sizeof(Token));
    if (!token)
    {
        perror("Failed to allocate memory for Token");
        return NULL;
    }
    token->type = type;
    token->lexeme = strdup(lexeme);
    token->literal = literal;
    token->line = line;
    return token;
}

Token **addTokenLitteral(Scanner *scanner, TokenType type, void *literal)
{
    if (scanner->tokenCount >= scanner->capacity)
    {
        scanner->capacity = scanner->capacity ? scanner->capacity * 2 : 4;
        scanner->tokens = realloc(scanner->tokens, scanner->capacity * sizeof(Token *));
        if (!scanner->tokens)
        {
            perror("Failed to reallocate memory for tokens array");
            exit(EXIT_FAILURE);
        }
    }

    const char *text = substring(scanner);
    scanner->tokens[scanner->tokenCount] = newTokenLitteral(type, text, literal, scanner->line);
    if (scanner->tokens[scanner->tokenCount] == NULL)
    {
        fprintf(stderr, "Failed to create new token\n");
        exit(EXIT_FAILURE);
    }

    (scanner->tokenCount)++;
    return scanner->tokens;
}

Token *newToken(TokenType type, const char *lexeme, int line)
{
    Token *token = malloc(sizeof(Token));
    if (!token)
    {
        perror("Failed to allocate memory for Token");
        return NULL;
    }
    token->type = type;
    token->lexeme = strdup(lexeme);
    token->literal = NULL;
    token->line = line;
    return token;
}

Token **addToken(Scanner *scanner, TokenType type)
{
    if (scanner->tokenCount >= scanner->capacity)
    {
        scanner->capacity = scanner->capacity ? scanner->capacity * 2 : 4;
        scanner->tokens = realloc(scanner->tokens, scanner->capacity * sizeof(Token *));
        if (!scanner->tokens)
        {
            perror("Failed to reallocate memory for tokens array");
            exit(5);
        }
    }

    const char *text = substring(scanner);
    scanner->tokens[scanner->tokenCount] = newToken(type, text, scanner->line);
    if (scanner->tokens[scanner->tokenCount] == NULL)
    {
        fprintf(stderr, "Failed to create new token\n");
        exit(6);
    }

    (scanner->tokenCount)++;
    return scanner->tokens;
}

void freeToken(Token *token)
{
    if (token)
    {
        free(token->lexeme);
        free(token);
    }
}

void printToken(const Token *token)
{
    printf("Token: Type=%d, Lexeme=%s, Line=%d\n", token->type, token->lexeme, token->line);
}

Scanner *createScanner(const char *source)
{
    Scanner *scanner = malloc(sizeof(Scanner));
    if (!scanner)
    {
        perror("Failed to allocate memory for Scanner");
        return NULL;
    }
    scanner->source = source;
    scanner->tokenCount = 0;
    scanner->capacity = 4;
    scanner->current = 0;
    scanner->start = 0;
    scanner->line = 1;

    scanner->tokens = malloc(scanner->capacity * sizeof(Token *));
    if (!scanner->tokens)
    {
        perror("Failed to allocate memory for tokens array");
        free(scanner);
        return NULL;
    }

    return scanner;
}

void freeScanner(Scanner *scanner)
{
    if (scanner)
    {
        for (int i = 0; i < scanner->tokenCount; i++)
        {
            freeToken(scanner->tokens[i]);
        }
        free(scanner->tokens);
        free(scanner);
    }
}

Token **scanTokens(Scanner *scanner)
{
    while (!isAtEnd(scanner))
    {
        scanner->start = scanner->current;
        scanToken(scanner);
    }

    return scanner->tokens;
}

void scanToken(Scanner *scanner)
{
    char c = advance(scanner);
    switch (c)
    {
    case '(':
        addToken(scanner, LEFT_PAREN);
        break;
    case ')':
        addToken(scanner, RIGHT_PAREN);
        break;
    case '{':
        addToken(scanner, LEFT_BRACE);
        break;
    case '}':
        addToken(scanner, RIGHT_BRACE);
        break;
    case ',':
        addToken(scanner, COMMA);
        break;
    case '.':
        addToken(scanner, DOT);
        break;
    case '-':
        addToken(scanner, MINUS);
        break;
    case '+':
        addToken(scanner, PLUS);
        break;
    case ';':
        addToken(scanner, SEMICOLON);
        break;
    case '*':
        addToken(scanner, STAR);
        break;
    default:
        break;
    }
}