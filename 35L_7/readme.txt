CS 35L Homework 7 readme.txt
Manas Kumar
004580689

Below are the time results from the "make clean check" command.
These results show the times for various levels of threading (1,2,4,8):

time ./srt 1-test.ppm >1-test.ppm.tmp
real    1m0.445s
user    1m0.471s
sys     0m0.002s

time ./srt 2-test.ppm >2-test.ppm.tmp
real    0m30.413s
user    1m0.515s
sys     0m0.006s

time ./srt 4-test.ppm >4-test.ppm.tmp
real    0m15.251s
user    1m0.582s
sys     0m0.007s

time ./srt 8-test.ppm >8-test.ppm.tmp
real    0m8.224s
user    1m3.766s
sys     0m0.001s

The biggest difference is with the real times of each test. Each increase in
the number of threads basically seems to halve the real time of the program.
This makes sense, as the kernel of the program should take half the time when
it is split into twice the pieces and those pieces are run in parallel. In 
short, this new implementation very much improves performance for multi-core
machines.



I did run into many issues when making this implementation. One of the
notable problems was the fact that pthread_create only takes one function and
that function can only take one argument. This was a problem because the
kernel of the main function that I was trying to thread used many variables
and splitting that section into many parallel mini-sections that went into a
seperate function that only took one variable was very confusing. I ended up
addressing this by declaring some variables outside the main function, 
declaring some other variables inside the new threaded function itself, and
splitting the main for loop into sections by thread number (so say the main
for loop went from i=0 to i=19 and we had 4 cores, the first thread would
deal with 0 to 4, the second would deal with 5 to 9, etc.).

The other big problem was dealing with the printing colors, since splitting
the image processing into multiple threads could lead to this printing being
done out of order. To fix this, I had to mess with the order of processing
and printing myself a bit to have the main function do the printing instead 
of the threaded function. To accomplish this I made a 3D global variable 
array that kept track of width, height, and color (hence the name WHCarray), 
and the main function used this array to print after the threaded function
was out of scope. This does cut down on the amount of threading, but I wasn't
sure how else to solve this issue.

Also for the longest time I forgot that I had to also link with the -lpthread
library by editing the Makefile, so that confused me for a while until I
realized where the error was coming from.

Also also I didn't use malloc anywhere, and the instructors on Piazza said 
we had to do that for some arrays. As far as I can tell this doesn't affect 
my program, but if there was something I was supposed to do with malloc that
I didn't, then that would be another problem I ran into.