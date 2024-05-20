#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h" 
#include "kvstore.h"// Include your kvstore header file here

int main() {
    // Initialize your kvstore
    kvstore_t kv;
    // Initialize your kvstore with appropriate initialization function if needed

    // Open the large.txt file for reading
    FILE *file = fopen("large.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Buffer to store each line read from the file
    char line[100];

    // Read and process each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Parse the operation and key/value from the line
        char op, key[32];
        int value;
        sscanf(line, "%c,%31[^,],%d", &op, key, &value);

        // Perform the corresponding operation on the kvstore
        switch (op) {
            case 'R':
                if (kv_read(&kv, key, &value) == 0) {
                    printf("Read key [%s] with value [%d]\n", key, value);
                } else {
                    printf("Key [%s] not found\n", key);
                }
                break;
            case 'W':
                if (kv_write(&kv, key, value) == 0) {
                    printf("Wrote key [%s] with value [%d]\n", key, value);
                } else {
                    printf("Failed to write key [%s] with value [%d]\n", key, value);
                }
                break;
            case 'I':
                if (kv_increase(&kv, key, value) == 0) {
                    printf("Increased key [%s] by value [%d]\n", key, value);
                } else {
                    printf("Failed to increase key [%s] by value [%d]\n", key, value);
                }
                break;
            case 'D':
                kv_delete(&kv, key);
                printf("Deleted key [%s]\n", key);
                break;
            default:
                printf("Invalid operation: %c\n", op);
                break;
        }
    }

    // Close the file
    fclose(file);

    return 0;
}

