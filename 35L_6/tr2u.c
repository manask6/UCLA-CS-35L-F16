#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
  // First we want to make sure arg-count is exactly 2
  if(argc!=3)
    {
      fprintf(stderr, "Please input TWO byte strings");
      exit(1);
    }

  // If we have the right # of arguments, we can make from and to
  char* from = argv[1];
  char* to = argv[2];

  // But we still need to make sure they're the same length
  if(strlen(to) != strlen(from))
    {
      fprintf(stderr, "Byte strings must be same size");
      exit(1);
    }

  // If from has any repeat chars, the program will make no sense
  for(int i = 0; i<strlen(from); i++)
    {
      for(int j = i+1; j<strlen(from); j++)
	{
	  if(from[i] == from[j])
	    {
	      fprintf(stderr, "'From' arg should have no duplicate chars");
	      exit(1);
	    }
	}
    }

  // So that's all the error cases.
  // Now for the kernel:

  // c is the current character in file
  char c[1];
  ssize_t s = read(0,c,1);
  
  // b is whether there's no match in from or not
  int b = 1;
  
  while(s>0)
    {
      for(size_t i = 0; i<strlen(from); i++)
	{
	  if(c[0] == from[i])
	    {
	      c[0] = to[i];
	      write(1,c,1);
	      b=0;
	      break;
	    }

	  else
	    b=1;   
	}

      if(b)
	write(1,c,1);
      s = read(0,c,1);
      
    }
  return 0;
}
