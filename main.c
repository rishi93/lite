/*
    getline() was officially standardized in POSIX.1-2008

    When we use -std=c99, compiler restricts headers to only pure C99 functions,
    By defining _POSIX_C_SOURCE 200809L, you tell the system to include POSIX.1-2008 functions
*/
#define _POSIX_C_SOURCE 200809L


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
    A small wrapper around the state we need to store to interact
    with getline
*/
typedef struct {
    char* buffer;
    /*
        size_t - Used for sizes of objects -> return a size in bytes
        ssize_t - Used for count of bytes or an error indication -> could be a valid
        size, or a negative error value
        
        Reference:
        https://pubs.opengroup.org/onlinepubs/007908775/xsh/systypes.h.html 
    */
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;

}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();

    /*
        A simple REPL - An infinite loop that prints the prompt, gets a line
        of input, then processes that line of input
    */
    while (1) {
        print_prompt();
        
        read_input(input_buffer);

        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s'\n", input_buffer->buffer);
        }
    }
}