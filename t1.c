#include <stdio.h>

#include "src/go.h"

int main() {
    Channel *out = channel();

    printf("this should be the last line\n");
    send(out,asInt(0));
    printf("should never see this\n");

    return 0;
}
