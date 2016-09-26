#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

int main(int argc, char **argv){
    if (argc == ARGC_NUM){
        int counter = 0;
        printf("DICTIONARY GENERATOR\nNow gathering words...\n");
        char **string = get_words(argv[1], atoi(argv[3]), &counter);
        if (string != NULL){
            printf("Got total of %d words, now ordenating on alphabetical order...\n", counter);
            merge_sort(string, 0, counter - 1);
            if(dump_dictionary(argv[2], string, counter)){
                printf("Succesfully dumped dictionary on %s file, program is now exiting...\n", argv[2]);
                string_destroy(string, counter);
                return SUCCESS;
            }
            string_destroy(string, counter);
        }
    } else printf("program usage: %s <input_file> <output_file> <min_word_lenght>\n", argv[0]);
    return FAILURE;
}