#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    char m[] = "RJAM{Aopz_PZ_Tf_JOPWJOPWJOPW}";
    
    int len = strlen(m);
    
    

    for(int i = 0 ; i < 26 ; i++) {
        for(int k = 0 ; k < len ; k++)
            m[k] -= 1;

        printf("%d : %s\n", i, m);
    }
}