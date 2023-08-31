#include "find_pixel_trace.h" 
#include <stdio.h>

int main(){

    struct return_vals a;
    struct return_vals *a_p = &a;

    a_p = find_pixel_trace(19, 23.2, 1.1, 10.4);


    int i =0;
    int size = a_p->size;
    for(i=0;i<size;i++){
        //printf("%d\n", a_p->struct_xpix[i]);
        //printf("%d\n", a_p->struct_ypix[i]);
        //printf("%f\n", a_p->struct_s[i]);
    }

    printf("\nFROM MAIN\n========================================\n");
    printf("%p\n", &(a_p->struct_s[0]) );
    printf("%p\n", &(a_p->struct_s[0]));                                //Why does printing the same value twice give two differient results?

    //printf("%p\n", (a_p->struct_s[1]));
    //printf("%p\n", &(a_p->struct_s[2]));
    
    return 0;
}
