#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void* handle_e = dlopen("./libe.so", RTLD_LAZY);
    void* handle_e_series = dlopen("./libe_series.so", RTLD_LAZY);
    void* handle_translation_binary = dlopen("./libtranslation_binary.so", RTLD_LAZY);
    void* handle_translation_ternary = dlopen("./libtranslation_ternary.so", RTLD_LAZY);

    if (!handle_e || !handle_e_series || !handle_translation_binary || !handle_translation_ternary) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    typedef float (*EFunc)(int);
    typedef char* (*TranslationFunc)(long);

    EFunc E = (EFunc)dlsym(handle_e, "E");
    EFunc E_series = (EFunc)dlsym(handle_e_series, "E");
    TranslationFunc translation_binary = (TranslationFunc)dlsym(handle_translation_binary, "translation");
    TranslationFunc translation_ternary = (TranslationFunc)dlsym(handle_translation_ternary, "translation");

    EFunc current_E = E;
    TranslationFunc current_translation = translation_binary;

    char command[256];
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);

        if (command[0] == '0') {
            if (current_E == E) {
                current_E = E_series;
                current_translation = translation_ternary;
            } else {
                current_E = E;
                current_translation = translation_binary;
            }
            printf("Switched implementation\n");
        } else if (command[0] == '1') {
            int x;
            sscanf(command, "1 %d", &x);
            printf("E(%d) = %f\n", x, current_E(x));
        } else if (command[0] == '2') {
            long x;
            sscanf(command, "2 %ld", &x);
            char* result = current_translation(x);
            printf("Translation(%ld) = %s\n", x, result);
            free(result);
        } else if (command[0] == 'q') {
            break;
        }
    }

    dlclose(handle_e);
    dlclose(handle_e_series);
    dlclose(handle_translation_binary);
    dlclose(handle_translation_ternary);
    return 0;
}
