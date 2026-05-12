#include <stdio.h>
#include <string.h>
#define TEST_NUM 1

char errors[6][10]= {"Count", "seed", "pk", "sk", "ct", "ss"};

int main() {
    FILE* ref_kats = fopen("kat_kem.rsp", "r");
    FILE* impl_kats = fopen("kat_out.vals", "r");
    char * ref_line, * impl_line;
    size_t ref_len, impl_len;
    printf("start test\n");
    for(int i = 0; i < 1; i++) {
        printf("line %i start: \n", i);
        getline(&ref_line, &ref_len, ref_kats);
        getline(&impl_line, &impl_len, impl_kats);
        printf("%i\n", impl_line);
        //printf(impl_line);


        if(strcmp(ref_line, impl_line) == 0)
            printf("equal\n");
        else
            printf("not equal\n");
/*
        int passed = 0;

        if(ref_len == impl_len) {
            for(int c = 0; c < ref_len; c++) {
                if(ref_line[c] == impl_line[c])
                    continue;
                else {
                    printf("Issues with test %i:", i/8);
                    printf(errors[i%8]);
                    printf("\n");
                    passed = 0;
                    break;
                }
            }
            passed = 1;
        }
        if(passed) {
            printf("line %i passed\n", i);
        }
        else {
            printf("Issues with test %i:", i/8);
            printf(errors[i%8]);
            printf("\n");
        }
*/
    }

    

    return 0;
}