#include <ctype.h>
void normalizeWord(char* original) 
{
    for (int i = 0; i < strlen(original); i++) {
        original[i] = tolower(original[i]);
    }
}