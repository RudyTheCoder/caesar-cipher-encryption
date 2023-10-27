#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

// Function to perform Caesar Cipher on a buffer
void cc(char *buffer, int shift, int reverse) {
    int length = strlen(buffer);
    char newMessage[length + 1];

    for (int i = 0; i < length; i++) {
        char character = buffer[i];
        char encrypted;

        if (isalpha(character)) {
            if (reverse) {
                // Decryption (reverse)
                if (isupper(character)) {
                    encrypted = (character - 'A' - shift + 26) % 26 + 'A';
                }
                if (islower(character)) {
                    encrypted = (character - 'a' - shift + 26) % 26 + 'a';
                }
            } else {
                // Encryption
                if (islower(character)) {
                    encrypted = ((character - 'a' + shift) % 26) + 'a';
                } else {
                    encrypted = ((character - 'A' + shift) % 26) + 'A';
                }
            }
            newMessage[i] = encrypted;
        } else {
            // Non-alphabet characters are left unchanged
            newMessage[i] = character;
        }
    }
    newMessage[length] = '\0';
    strcpy(buffer, newMessage);
}

int main(int argc, char *argv[]) {
    const int SIZE = 1024;
    int shift = 0, reverse = 0, lineNum = 0, args = argc, num = 1, eolFlag 
= 0, numRead = 0, j = 0, fd;
    char buf[SIZE];
    char c;

    // Check for no arguments
    if (argc == 1) {
        printf("Usage: %s -EnT file.txt\n", argv[0]);
        exit(0);
    }

    // Parse arguments for switches
    while (args--) {
        // Open file
        if (argv[args][0] == '-') {
            int switchLen = (int)strlen(argv[args]);
            while (switchLen--) {
                if (argv[args][switchLen] == 's') {
                    shift = atoi(argv[args + 1]);
                    args--;
                }
                if (argv[args][switchLen] == 'r') {
                    reverse = 1;
                }
                if (argv[args][switchLen] == 'n') {
                    lineNum = 1;
                }
            }
        }
    }

    // Open the file for reading
    fd = open(argv[argc - 1], O_RDONLY);

    if (fd == -1) {
        perror(argv[argc - 1]);
        exit(1);
    }

    while ((numRead = read(fd, buf, SIZE)) > 0) {
        // Call the Caesar cipher function on the buffer
        cc(buf, shift, reverse);

        // Parse the buffer
        for (j = 0; j < numRead; j++) {
            c = buf[j];
            if (lineNum && (eolFlag || num == 1)) {
                // Print line numbers when enabled
                printf("%d ", num++);
            }
            printf("%c", c);
            if (lineNum && c == '\n') {
                eolFlag = 1;
            }
        }
    }

    // Close the file
    close(fd);

    return 0;
}

