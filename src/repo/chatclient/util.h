/*******************************************************************************
 * Name          : util.h
 * Author        : Brian S. Borowski
 * Version       : 1.1
 * Date          : April 24, 2020
 * Last modified : April 26, 2021
 * Description   : Helpful utility functions for chat server/client.
 ******************************************************************************/
#ifndef UTIL_H_
#define UTIL_H_

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_LEN  1024
#define MAX_NAME_LEN 20
#define BUFLEN       MAX_MSG_LEN + MAX_NAME_LEN + 4
                     // +4 = '[' before name, "]: " after name

enum parse_string_t { OK, NO_INPUT, TOO_LONG };

/* Functions that should be used in client and server. */
bool parse_int(const char *input, int *i, const char *usage);
int get_string(char *buf, const size_t sz);

/**
 * Determines if the string input represent a valid integer.
 */
bool is_integer(const char *input) {
    int start = 0, len = strlen(input), i;

    if (len >= 1 && input[0] == '-') {
        if (len < 2) {
            return false;
        }
        start = 1;
    }
    for (i = start; i < len; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Attempts to convert the input string into the integer i.
 * Returns true if the conversion was successful, false otherwise.
 */
bool parse_int(const char *input, int *i, const char *usage) {
    long long long_long_i;

    if (strlen(input) == 0) {
        fprintf(stderr,
                "Error: Invalid input '' received for %s.\n", usage);
        return false;
    }
    if (is_integer(input) && sscanf(input, "%lld", &long_long_i) == 1) {
        *i = (int)long_long_i;
        if (long_long_i != *i) {
            fprintf(stderr, "Error: Integer overflow for %s.\n", usage);
            return false;
        }
    } else {
        fprintf(stderr, "Error: Invalid input '%s' received for %s.\n", input,
                usage);
        return false;
    }
    return true;
}

/**
 * Reads a string fron STDIN up to sz characters. If more than sz characters
 * are supplied, the function consumes them.
 * Returns OK, NO_INPUT, or TOO_LONG.
 */
int get_string(char *buf, const size_t sz) {
    ssize_t bytes_read = read(STDIN_FILENO, buf, sz);
    if (bytes_read < 0) {
        fprintf(stderr,
                "Warning: Failed to read message from keyboard. %s.\n",
                strerror(errno));
        return NO_INPUT;
    } else if (bytes_read == 0) {
        return NO_INPUT;
    }

    // If it is too long, there'll be no new line character.
    // Consume all remaining characters on the line.
    char ch = 0;
    bool extra = false;
    if (buf[bytes_read - 1] != '\n') {
        extra = true;
        while (read(STDIN_FILENO, &ch, 1) > 0 && ch != '\n');
    }
    if (extra) {
        return TOO_LONG;
    }
    // Replace new line character with '\0'.
    buf[bytes_read - 1] = '\0';
    if (strlen(buf) == 0) {
        return NO_INPUT;
    }
    return OK;
}

#endif
