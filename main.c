#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "passwd.h"

int main(int argc, char** argv) {
    
    (void)generate_password(argc, argv);

    return 0;
}
