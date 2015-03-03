#include "utils.h"

char calculateCheckSum(char *res, int len) {
    int sum = 0;
    int length = len;
    int i;
    for (i=0; i < length; ++i){
        sum += res[i];
    }
    
    return sum % 26 + 97;
    
}