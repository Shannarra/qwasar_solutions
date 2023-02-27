// #include <stdio.h>
// #include <stdlib.h>

// void fill_array(int* arr, int arrSize, char* str);
// void print_ngram(int* arr, int arrSize);

// int main(int ac, char** av)
// {
//     int arr[128] = {0};
    
//     if(ac >= 2 && av[1] != NULL) {
//         for(int i = 1; i < ac; i++) {
//             fill_array(&arr[0], 128, av[i]);
//         }
//     }
//     print_ngram(&arr[0], 128);
//     return 0;
// }

// void fill_array(int* arr, int arrSize, char* str)
// {
//     int index = 0;
//     while(str[index] != '\0') {
//         for(int i = 0; i < arrSize; i++) {
//             if(str[index] == i && str[index] != '"')
//                 arr[i]++;
//         }
//         index++;
//     }
// }

// void print_ngram(int* arr, int arrSize)
// {
//     for(int i = 0; i < arrSize; i++) {
//         if(arr[i] > 0)
//             printf("%c:%d\n", i, arr[i]);
//     }
// }


#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc <= 1) return -1;

    int dict[128] = {0};

    for (int i = 0; i < argc-1; i++) 
        for (int j = 0; j < (int)strlen(argv[i+1]); j++)
            if (argv[i+1][j] != '"')
                dict[(int)argv[i+1][j]]++;

    for (int i = 0; i < 128; i++) 
        if (dict[i] != 0)
            printf("%c:%d\n", (char)i, dict[i]);
}