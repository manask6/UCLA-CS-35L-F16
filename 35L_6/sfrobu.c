#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>


int COMPARISONS = 0;

// Takes two frobnicated words and returns what memcmp would return
// returns 1 if a>b, -1 if a<b, and 0 if a=b
// X^42 will unfrobnicate char x because XOR is the inverse of XOR
int frobcmp(char const *a, char const *b)
{
  COMPARISONS++;
  
  for(;;)  //We want an infinite loop (at some point it will return)
    {
      if(*a == ' ' && *b == ' ')  // If we're at the end of the words
	return 0;

      else if (*b == ' ' || (((*a^42) > (*b^42))))
	return 1;  // If a is longer or if b is bigger

      else if (*a == ' ' || (((*b^42) > (*a^42))))
	return -1; // If b is longer or if a is bigger

      a++;
      b++;  // We haven't reached an end condition yet, so iterate
    }
}



// Used with qsort function
int compare(const char** a, const char** b)
{
  const char* x = *a;
  const char* y = *b;
  return frobcmp(x,y);
}



int main(void)
{
  struct stat fStat;
  
  char* word = (char*)malloc(sizeof(char));
    // (ARRAY) Will hold each word individually
  char** allwords = (char**)malloc(sizeof(char*));
    // (ARRAY) Will hold pointers to all the words
  char* farray;
  
  size_t wordcount = 0;
    // These are to count letters and words (used in arrays)

  if(S_ISREG(fStat.st_mode))
    {
      farray = (char*)malloc(fStat.st_size * sizeof(char));
      int allwordsptr = 0;
      ssize_t state = read(0, farray, fStat.st_size);
      int count = 1;
      if(state > 0)
	{
	  for(size_t i = 0; i<fStat.st_size; i+=count)
	    {
	      count = 1;
	      if(fStat.st_size - 1 == i)
		farray[i] = ' ';
	      if(farray[i] == ' ')
		{
		  for(size_t j = i; farray[j] != ' '; j++)
		    count++;
		  allwordsptr++;
		}
	    }
	}

      allwords = (char**)malloc(allwordsptr * sizeof(char*));

      int bool = 0;
      for(size_t x = 0; x<fStat.st_size; x++)
	{
	  if(bool == 0 && farray[x]!=' ')
	    {  
	      allwords[allwordsptr] = &farray[x];
	      allwordsptr++;
	      bool = 1;
	    }
	  else if(bool == 1 && farray[x]==' ')
	    bool = 0;
	}
    }
	      

  char c[1];
  ssize_t cstate = read(0,c,1);
  char cnext[1];
  ssize_t cnextstate = read(0,cnext,1);
  int lettercount = 0;
  
  while(cstate > 0)
    // This whole loop fills the allwords array with words
    // We want to keep checking for errors or for the end of the file
    {
      word[lettercount] = c[0];  // Add current letter to word

      char* x = realloc(word, sizeof(char)*(lettercount+2));
        // The +2 at the end is in case we're at lettercount = 0
        // We'd need to expand the word array by at least 1 char each time
        // This method will leave a lot of empty space for big words,
        //  but whatever.
      
      if(x == NULL)
	{
	  free(word);
	  fprintf(stderr, "ERROR with memory allocation");
	  exit(1);
	}
      else
	word = x;
      // If there's no error, make the word array its reallocated size


      
    // IF WE'RE AT THE END OF A WORD
      if(c[0] == ' ')
	{
	  allwords[wordcount] = word;  // Put word into allwords array

	  char** y = realloc(allwords, sizeof(char*)*(wordcount+2));
	  if(y == NULL)  // Checking for errors again...
	    {
	      free(allwords);
	      fprintf(stderr, "ERROR with memory allocation");
	      exit(1);
	    }
	  else
	    {
	      allwords = y;  // Expand words array again
	      wordcount++;
	      word = NULL;
	      word = (char*)malloc(sizeof(char));  // reset word pointer
	      lettercount = -1;  // This should be 0 at end of loop
	    } 
	}

    // IF WE'RE AT THE END OF THE FILE PROPERLY
      if(cnextstate == 0 && c[0] == ' ') break;

    // IF WE'RE AT THE END OF THE FILE AND THERE'S EXTRA SPACES
      else if (c[0] == ' ' && cnext[0] == ' ')
	{
	  while(c[0] == ' ')  // Keep getchar'ing until there's no space
	    cstate = read(0,c,1);
	  cnextstate = read(0,cnext,1);
	  lettercount++;
	  continue;
	}

    // IF WE'RE AT THE END OF THE FILE BUT THERE'S NO SPACE THERE 
      else if(cnextstate == 0)
	{
	  c[0] = ' ';
	  lettercount++;
	  continue;
	}

    // We've examined all special cases, now we get the next char and loop
      c[0] = cnext[0];
      cnextstate = read(0,cnext,1);
      lettercount++;
    }  // End of while loop

  // We should now have a full array, allwords, with all the words in it

  // Let's sort allwords now:
  qsort(allwords, wordcount, sizeof(char*), compare);

  // Great, now let's output everything:
  for(size_t i = 0; i<wordcount; i++)
    {
      long size = 0;
      for(size_t j = 0;; j++)
	{
	  size++;
	  if(allwords[i][j] == ' ')
	    break;
	}
      if(write(1,allwords[i], size)==0)
	{
	  fprintf(stderr, "STDOUT ERROR");
          exit(1);
	}
    }

  // Everything's written, now let's clean up
  for(size_t i = 0; i<wordcount; i++)
    free(allwords[i]);
  free(allwords);

  fprintf(stderr, "Comparisons: %i\n", COMPARISONS);
 
  exit(0);
}
