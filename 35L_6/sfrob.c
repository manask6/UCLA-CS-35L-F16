#include<stdio.h>
#include<stdlib.h>



// Takes two frobnicated words and returns what memcmp would return
// returns 1 if a>b, -1 if a<b, and 0 if a=b
// X^42 will unfrobnicate char x because XOR is the inverse of XOR
int frobcmp(char const *a, char const *b)
{
  for(;;)  // We want an infinite loop (at some point it will return)
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
  char* word = (char*)malloc(sizeof(char));
    // (ARRAY) Will hold each word individually
  char** allwords = (char**)malloc(sizeof(char*));
    // (ARRAY) Will hold pointers to all the words

  
  char thischar = getchar();
  char nextchar = getchar();
    // We need two variables to get one char at a time to check EOF

  
  int lettercount = 0;
  int wordcount = 0;
    // These are to count letters and words (used in arrays)

  
  while(thischar != EOF && !ferror(stdin))
    // This whole loop fills the allwords array with words
    // We want to keep checking for errors or for the end of the file
    {
      word[lettercount] = thischar;  // Add current letter to word

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
      if(thischar == ' ')
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
      if(thischar == ' ' && nextchar == EOF) break;

    // IF WE'RE AT THE END OF THE FILE AND THERE'S EXTRA SPACES
      else if (thischar == ' ' && nextchar == ' ')
	{
	  while(thischar == ' ')  // Keep getchar'ing until there's no space
	    thischar= getchar();
	  nextchar = getchar();
	  lettercount++;
	  continue;
	}

    // IF WE'RE AT THE END OF THE FILE BUT THERE'S NO SPACE THERE 
      else if(nextchar == EOF)
	{
	  thischar = ' ';
	  lettercount++;
	  continue;
	}

    // We've examined all special cases, now we get the next char and loop
      thischar = nextchar;
      nextchar = getchar();
      lettercount++;
    }  // End of while loop

  // We should now have a full array, allwords, with all the words in it

  // Let's sort allwords now:
  qsort(allwords, wordcount, sizeof(char*), compare);

  // Great, now let's output everything:
  for(size_t i = 0; i<wordcount; i++)
      for(size_t j = 0;; j++)
	{
	  if(putchar(allwords[i][j]) == EOF)  // Writes AND error checks
	    {
	      fprintf(stderr, "STDOUT ERROR");
	      exit(1);
	    }
	  else if(allwords[i][j] == ' ') break;
	}

  // Everything's written, now let's clean up
  for(size_t i = 0; i<wordcount; i++)
    free(allwords[i]);
  free(allwords);

  exit(0);
}
