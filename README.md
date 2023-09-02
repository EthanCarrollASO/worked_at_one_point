# worked_at_one_point

my_function.so is the shared library file. It can be created by running the following command:
gcc -fPIC -shared -o my_functions.so find_pixel_trace.c

main_c.ipynb is how you call the c functions from py. It should not be messed around with till you can first call the c funciton from c.

from_c.c is the file that calls the c funciton from c. This is the file that is throwing the seg fault erros.

find_pixel_trace.c is the original c file that I have made changes to.

HERE IS THE BUG REPORT OF THE PROBLEM I GAVE UP ONE

the bugs I am getting are totally insane and inconsistent. I print the same value twice in a row and both times the value displayed is different.
This code displays this output.

##############################################################################

printf("%p: %f\n", &(a_p->struct_s[0]), a_p->struct_s[0]);   
printf("%p\n", &(a_p->struct_s[0]));

_________________________
0x56186ff266f0: 0.000000
0x4033ab7d75d48b18    

################################################################################

But interestingly, this code produces the right output.
printf("%p\n", &(a_p->struct_s[0]) );
printf("%p\n", &(a_p->struct_s[0])); 

________________________________________
0x558e665396f0
0x558e665396f0

###################################################################################

Its almost certainly a malloc() error which admittedly I could learn more about. I just thought I would tell you so you would know why im losing my mind. Especially since I swear the code worked fine 2 days ago and that printing the other two arrays in a_p produce no segmentation faults. 
