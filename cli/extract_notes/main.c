//
//  main.c
//  extract_notes
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pcre.h"

#pragma mark - Regex

void print_match(const char *match, int length) {
    
    char *buffer = malloc(length + 1), *insert_point = &buffer[0], *ascii_buffer = malloc(3 + 1);
    const char *temp = match;
    
    buffer[length] = ascii_buffer[4] = 0;
    
    while (1) {
        
        const char *p = strstr(temp, "\\");
        if (p == NULL) {
            strcpy(insert_point, temp);
            break;
        }
        
        memcpy(ascii_buffer, p+1, 3);
        if (p-1 > match && *(p-1) == '\\')
            ascii_buffer[0] = '9';
        long c = strtol(ascii_buffer, NULL, 8);
        
        memcpy(insert_point, temp, p - temp); // point to the match at p
        insert_point += p - temp;
        
        if (c > 0 && c <= 128) {
            
            memcpy(insert_point, &c, 1); // replace occurence and move right
            insert_point += 1;
            
            temp = p + 4; // skip substring
            
        }
        else if (*(p+1) == '0') // 000 sequence
            temp = p + 4;
        else
            temp = p + 1;
        
    }
    
    // printf("%s\n", match);
    printf("%s\n", buffer);
    
}

void print_matches(char *string, const char *regex, int max_matches /* ignored */, int str_length) {
    
    pcre *compile = NULL;
    pcre_extra *opt_compile = NULL;
    const char *error_string = NULL, *substring = NULL;
    int sub_offset[30], matches, error_offset, string_offset = 0, found_match = 1;
    
    compile = pcre_compile(regex, PCRE_DOTALL | PCRE_MULTILINE, &error_string, &error_offset, NULL);
    if (compile != NULL) {
        opt_compile = pcre_study(compile, 0, &error_string);
        if (error_string == NULL) {
            while (found_match) {
                printf("---\n");
                // printf("%i aa \n", string_offset);
                matches = pcre_exec(compile, opt_compile, string, str_length, string_offset, 0, sub_offset, 30);
                if (matches >= 2) {
#if DEBUG
                    // printf("#matches = %i\n", matches);
#endif
                    for (int i=1 ; i<2 ; i++) {
                        pcre_get_substring(string, sub_offset, matches, i, &substring);
                        print_match(substring, (int)strlen(substring));
                        if (sub_offset[2*i+1] > string_offset)
                            string_offset = sub_offset[2*i+1];
                    }
                }
                else
                    found_match = 0;
            }
            pcre_free_substring(substring);
        }
    }
    else {
        printf("error :( ('%s')\n", error_string);
        exit(1);
    }
    
    pcre_free(compile);
    if(opt_compile != NULL) {
#ifdef PCRE_CONFIG_JIT
        pcre_free_study(opt_compile);
#else
        pcre_free(opt_compile);
#endif
    }
    
}

#pragma mark - Main

int main(int argc, const char * argv[]) {
    
    if (argc == 2) {
        
        char *buffer = 0;
        long file_size = 0, read = 1;
        FILE *file = fopen (argv[1], "rb");
        
        if (file) { // Read file to buffer
            
            fseek(file, 0, SEEK_END);
            file_size = ftell(file);
            fseek(file, 0, SEEK_SET);
            buffer = malloc(file_size * sizeof(char) + 1);
            if (buffer)
                read = fread(buffer, 1, file_size, file);
            buffer[file_size] = 0;
            
            fclose (file);
            
        }
        
        if (buffer && file_size == read) { // Success reading, now filter with regular expression
            print_matches(buffer, "<< /Type /Annot /Popup[^>>]*/Contents\\s\\((.*?)(\\))\\s/M[^>>]*>>", 3*(int)(file_size/10), (int)file_size);
            free(buffer);
            return EXIT_SUCCESS;
        }
        
    }
    
    printf("error :( (make sure you use one argument, the pdf file's url)\n");
    return EXIT_FAILURE;
    
}