 #include <stdio.h>
 #include "myadd.h"
 #include "mysub.h"
 int main( void )
 {
    int a = 10;
    int b = 20;
    printf("add(10, 20)=%d\n", a, b, add(a, b));
    a = 100;
    b = 20;
    printf("sub(%d,%d)=%d\n", a, b, sub(a, b));
 }


