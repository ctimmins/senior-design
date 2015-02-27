#include "utils.h"

char calculateCheckSum(char *res) {
    int length, sum = 0;
    length = strlen(res);
    int i;
    for (i=0; i < length; ++i){
        sum += res[i];
    }
    
    return sum % 63 + 33;
    
}