#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void* handle_e = dlopen("./libe.so", RTLD_LAZY);
    if (!handle_e) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    void* handle_translation = dlopen("./libtranslation_binary.so", RTLD_LAZY);
    if (!handle_translation) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    typedef float (*EFunc)(int);
    typedef char* (*TranslationFunc)(long);

    EFunc E = (EFunc)dlsym(handle_e, "E");
    TranslationFunc translation = (TranslationFunc)dlsym(handle_translation, "translation");

    char command[256];
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);

        if (command[0] == '1') {
            int x;
            sscanf(command, "1 %d", &x);
            printf("E(%d) = %f\n", x, E(x));
        } else if (command[0] == '2') {
            long x;
            sscanf(command, "2 %ld", &x);
            char* result = translation(x);
            printf("Translation(%ld) = %s\n", x, result);
            free(result);
        } else if (command[0] == 'q') {
            break;
        }
    }

    dlclose(handle_e);
    dlclose(handle_translation);
    return 0;
}
