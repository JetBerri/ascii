#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256

// Flags de Visualización:
//   - Bit 0 (1): Mostrar valores en formato hexadecimal.
//   - Bit 1 (2): Mostrar caracteres ASCII.
//   - Bit 2 (4): Mostrar línea vertical de caracteres ASCII entre los valores.
char *help = "KZ Cat by Cursed\nv1.00\nkzh [options] filename\n-H Display hex\n-a Display ASCII\n-h This message\n";
char *invf = "Error: File doesn't exist or cannot be opened.\n";

void printHexAndAscii(char *buffer, int offset, int length, char flags) {
    printf("%4x: ", offset);

    for (int i = 0; i < length; i++) {
        if (flags & 1) {
            printf("%.2x ", buffer[i]);
        }

        if (flags & 2) {
            char c = buffer[i];
            if ((c > 128) || (c < 31)) {
                printf(".");
            } else {
                printf("%c", c);
            }
        }
    }

    if (flags == 3) {
        printf("|");
        for (int i = 0; i < 16 - length; i++) {
            printf("-- ");
        }
        printf("|");
    }

    printf("\n");
}

void processFile(FILE *file, char flags) {
    if (file == NULL) {
        perror(invf);
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];
    int readBytes = 0;
    int offset = 0;

    while ((readBytes = fread(buffer, 1, BUF_SIZE, file)) > 0) {
        for (int i = 0; i < readBytes; i += 16) {
            printHexAndAscii(buffer + i, offset + i, (readBytes - i < 16) ? readBytes - i : 16, flags);
        }

        offset += readBytes;
    }

    if (ferror(file)) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [options] filename\n", argv[0]);
        return EXIT_FAILURE;
    } else if (argc == 2) {
        char *path = argv[1];
        if (strcmp(path, "-h") == 0) {
            printf("%s", help);
            return EXIT_SUCCESS;
        }

        FILE *file = fopen(path, "r");
        processFile(file, 3);
        fclose(file);
    } else if (argc == 3) {
        char *op = argv[1];
        char *path = argv[2];
        char flags = 0;

        if (strcmp("-h", op) == 0) {
            printf("%s", help);
            return EXIT_SUCCESS;
        } else if (strcmp("-H", op) == 0) {
            flags += 1;
        } else if (strcmp("-a", op) == 0) {
            flags += 2;
        } else if (strcmp("-Ha", op) == 0 || strcmp("-aH", op) == 0) {
            flags += 3;
        } else {
            fprintf(stderr, "Error: Invalid arguments. Use -h for help.\n");
            return EXIT_FAILURE;
        }

        FILE *file = fopen(path, "r");
        processFile(file, flags);
        fclose(file);
    } else {
        fprintf(stderr, "Error: Invalid number of arguments. Use -h for help.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
