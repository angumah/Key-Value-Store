#include "kvstore.h"


// Exercise 4: finish implementing kvstore
// TODO: define your synchronization variables here
// (hint: don't forget to initialize them)
pthread_mutex_t store_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t busy = PTHREAD_COND_INITIALIZER;
int writers = 0;
int readers = 0;

int size = 0;

/* read a key from the key-value store.
 *
 * if key exists, set "val" to be the value and return 0.
 * if key doesn't exist, return 1.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */
int kv_read(kvstore_t *kv, char *key, int *val) {
    pthread_mutex_lock(&store_mutex);
    
    // Check if any writers are active
    while (writers > 0) {
        pthread_cond_wait(&busy, &store_mutex);
    }
    
    readers++;
    int index = 0;
    key_entry_t *currEntry = &kv->keys[index];
    int *currVal = NULL;
    
    // Find the key in the kvstore
    while (currEntry->stat == 1 && index < TABLE_MAX) {
        if (strcmp(currEntry->key, key) == 0) {
            currVal = &kv->values[index];
            break;
        }
        index++;
        currEntry = &kv->keys[index];
    }
    
    // delete this later cuz printing is slow
    printf("[INFO] read key[%s]\n", key);
   
    // If the key was found, assign the value to val and return 0
    if (currVal != NULL) {
        *val = *currVal;
        readers--;
        pthread_mutex_unlock(&store_mutex);
        return 0;
    }
    
    // If the key was not found, return 1
    readers--;
    pthread_mutex_unlock(&store_mutex);
    return 1;
}



/* write a key-value pair into the kv-store.
 *
 * - if the key exists, overwrite the old value.
 * - if key doesn't exit,
 *     -- insert one if the number of keys is smaller than TABLE_MAX
 *     -- return failure if the number of keys equals TABLE_MAX
 * - return 0 for success; return 1 for failures.
 *
 * notes:
 * - the input "key" locates somewhere out of your control, you must copy the
 *   string to kv-store's own memory. (hint: use strcpy)
 *
 * NOTE: kv-store must be implemented as a monitor.
 */

int kv_write(kvstore_t *kv, char *key, int val) {
    pthread_mutex_lock(&store_mutex);
    
    // Check if any readers are active
    while (readers > 0) {
        pthread_cond_wait(&busy, &store_mutex);
    }
    
    writers++;

    int index = 0;
    key_entry_t *currEntry = &kv->keys[index];
    int currValIndex = -1; // Initialize to an invalid index

    // Find the key in the kvstore
    while (currEntry->stat == 1 && index < TABLE_MAX) {
        if (strcmp(currEntry->key, key) == 0) {
            currValIndex = index; // Update the index where the key was found
            break;
        }
        index++;
        currEntry = &kv->keys[index];
    }
    
    // If the key was found, update its value
    if (currValIndex != -1) {
        kv->values[currValIndex] = val;
    } 
    // If the key was not found, find an empty slot and insert the key-value pair
    else {
        while (index < TABLE_MAX && kv->keys[index].stat != 0) {
            index++;
        }
        if (index >= TABLE_MAX) {
            writers--;
            pthread_mutex_unlock(&store_mutex);
            return 1; // Table is full
        }
        strncpy(kv->keys[index].key, key, sizeof(kv->keys[index].key) - 1);
        kv->keys[index].key[sizeof(kv->keys[index].key) - 1] = '\0'; // Ensure null-termination
        kv->keys[index].stat = 1;
        kv->values[index] = val;
    }
    
    writers--;

    // Signal that the operation is completed
    pthread_cond_broadcast(&busy);
    
    // delete this later cuz printing is slow
    printf("[INFO] write key[%s]=val[%d]\n", key, val);
    
    pthread_mutex_unlock(&store_mutex);
    return 0;
}



/* delete a key-value pair from the kv-store.
 *
 * - if the key exists, delete it.
 * - if the key doesn't exits, do nothing.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */
void kv_delete(kvstore_t *kv, char *key) {
    pthread_mutex_lock(&store_mutex);
    
    // Check if any readers or writers are active
    while (readers > 0 || writers > 0) {
        pthread_cond_wait(&busy, &store_mutex);
    }
    
    int index = 0;
    key_entry_t *currEntry = &kv->keys[index];
    
    // Find the key in the kvstore
    while (currEntry->stat == 1 && index < TABLE_MAX) {
        if (strcmp(currEntry->key, key) == 0) {
            // Mark the entry as deleted
            currEntry->stat = 0;
            break;
        }
        index++;
        currEntry = &kv->keys[index];
    }
    
    // delete this later cuz printing is slow
    printf("[INFO] delete key[%s]\n", key);
    
    pthread_mutex_unlock(&store_mutex);
}


/* increase the value of a key
 *
 * - if the key exists, increase "val" on top of the old value.
 * - if key doesn't exit,
 *     -- insert one if the number of keys is smaller than TABLE_MAX
 *     -- return failure if the number of keys equals TABLE_MAX
 * - return 0 for success; return 1 for failures.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */
int kv_increase(kvstore_t *kv, char *key, int val) {
    pthread_mutex_lock(&store_mutex);
    
    // Check if any readers or writers are active
    while (readers > 0 || writers > 0) {
        pthread_cond_wait(&busy, &store_mutex);
    }
    
    int index = 0;
    key_entry_t *currEntry = &kv->keys[index];
    
    // Find the key in the kvstore
    while (currEntry->stat == 1 && index < TABLE_MAX) {
        if (strcmp(currEntry->key, key) == 0) {
            // Increment the value associated with the key
            kv->values[index] += val;
            break;
        }
        index++;
        currEntry = &kv->keys[index];
    }
    
    // If the key doesn't exist, add it
    if (index >= TABLE_MAX) {
        strncpy(currEntry->key, key, sizeof(currEntry->key));
        currEntry->stat = 1;
        kv->values[index] = val;
    }
    
    // delete this later cuz printing is slow
    printf("[INFO] increase key[%s]+=val[%d]\n", key, val);
    
    pthread_mutex_unlock(&store_mutex);
    return 0;
}


// print kv-store's contents to stdout
// note: use any format that you like; this is mostly for debugging purpose
void kv_dump(kvstore_t *kv) {
    /* TODO: your code here */
    pthread_mutex_lock(&store_mutex);
    while(writers > 0){
    	pthread_cond_wait(&busy, &store_mutex);
    }
    for (int index = 0; index < TABLE_MAX; index++) {
        key_entry_t *currEntry = &kv->keys[index];
        if (currEntry->stat == 1) {
            printf("Key: [%s] | Value: [%d]\n", currEntry->key, kv->values[index]);
        }
    }
    pthread_mutex_unlock(&store_mutex);
}
