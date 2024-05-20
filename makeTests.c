#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Include for random number generation

// Define the operations and keys
char *operations[] = {"R", "W", "I", "D"};
char *keys[] = {"key1", "key2", "key3", "key4", "key5"};

// Function to generate a random integer within a range
int rand_range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}



int large(){
	// Seed the random number generator
    srand(time(NULL));

    // Open the file for writing
    FILE *file = fopen("large.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Generate approximately 10,000 operations
    int num_operations = 10000;
    for (int i = 0; i < num_operations; i++) {
        // Choose a random operation and key
        char *op = operations[rand_range(0, 3)];
        char *key = keys[rand_range(0, 4)];
        
        // Generate a line with the chosen operation and key
        if (op[0] == 'R' || op[0] == 'D') {
            fprintf(file, "%s,%s\n", op, key);
        } else {
            // For write and increase operations, generate a random value
            int value = rand_range(1, 100);
            fprintf(file, "%s,%s,%d\n", op, key, value);
        }
    }

    // Close the file
    fclose(file);

    return 0;
}

int pressure(){
    // Open the file for writing
    FILE *file = fopen("pressure.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Generate approximately 1,000 keys
    int num_keys = 1000;
    int opCount = 0;
    int queueCount = 0;
    
    char key[20];
    while (opCount <= num_keys) {
    	while (queueCount < 200){
    		int value = rand_range(1, 100);
    		sprintf(key, "key%d", queueCount);
    		fprintf(file, "W,%s,%d\n", key, value);
    		queueCount++;
    		opCount++;
    	} 
    	while (queueCount > 0) {
    		sprintf(key, "key%d", queueCount);
    		fprintf(file, "D,%s\n", key);
    		queueCount--;
    		opCount++;
    	}
    	
    }

    // Close the file
    fclose(file);

    return 0;
}


int main() {
	if(large() != 0){
		perror("[Error] making large.txt file");
		return 1;
	}
	
	if(pressure() != 0){
		perror("[Error] making pressure.txt file");
		return 1;
	}
	return 0;
}


