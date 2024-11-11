#include "scanner.h"

// Functions prototypes declarations
void run(const char *input);
void runFile(const char *path);
void runPrompt();

uint8_t *readFileBytes(const char *filename, size_t *fileSize);

void error(int line, char *message);
void report(int line, char *message, char *where);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Usage: jlox [script]");
        return 1;
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt();
    }

    return 0;
}

void run(const char *source)
{
    Scanner *scanner = createScanner(source);
    if (!scanner)
        return;

    Token **tokens = scanTokens(scanner);

    for (int i = 0; i < scanner->tokenCount; i++)
    {
        printToken((const Token *)tokens[i]);
    }
}

void runFile(const char *path)
{
    size_t fileSize;
    uint8_t *bytes = readFileBytes(path, &fileSize);

    if (bytes)
    {
        char *str = (char *)bytes;
        run(str);
        free(bytes);
    }
}

void runPrompt()
{
    size_t bufferSize = 1024;
    char *line = malloc(bufferSize);

    if (line == NULL)
    {
        perror("Error allocating memory");
        return;
    }

    while (1)
    {
        printf("> ");

        if (fgets(line, bufferSize, stdin) == NULL)
        {
            break;
        }

        size_t len = strlen(line);
        if (len == bufferSize - 1 && line[len - 1] != '\n')
        {
            bufferSize *= 2;                  // Double the buffer size
            line = realloc(line, bufferSize); // Reallocate memory
            if (line == NULL)
            {
                perror("Error reallocating memory");
                return;
            }
            continue; // Try reading again with the new buffer
        }

        // Remove newline character if present
        if (line[len - 1] == '\n')
        {
            line[len - 1] = '\0'; // Null-terminate the string
        }

        run(line);
    }

    free(line);
}

uint8_t *readFileBytes(const char *filename, size_t *fileSize)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a buffer for the file content
    uint8_t *buffer = malloc(*fileSize);
    if (!buffer)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the file contents into the buffer
    size_t bytesRead = fread(buffer, 1, *fileSize, file);
    if (bytesRead != *fileSize)
    {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;
}