
int rand() {
        static unsigned long next = 1;
        next = next * 1103515245 + 12345;
        return (unsigned int)(next/65536) % 32768;
}

int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

typedef struct {
    int keys[100];
    int values[100];
    int size;
} Dictionary;

void Dict_insert(Dictionary* dict, int key, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->keys[i] == key) {
            dict->values[i] = value;
            return;
        }
    }
    dict->keys[dict->size] = key;
    dict->values[dict->size] = value;
    dict->size++;
}

int Dict_get_value(Dictionary* dict, int key) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->keys[i] == key) {
            return dict->values[i];
        }
    }
    return -1; // Not found
}
int Dict_get_key(Dictionary* dict, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->values[i] == value) {
            return dict->keys[i];
        }
    }
    return -1; // Not found
}

int Dict_set_value(Dictionary* dict, int key, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->keys[i] == key) {
            dict->values[i] = value;
            return 0; // Success
        }
    }
    return -1; // Key not found
}