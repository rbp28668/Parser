/*
Title:            GETTEXT.C

Function:         to read in text strings produced by the text compiler

Author:           Bruce

Date:             17 July 1989

Directory:

Calling sequence: text=get_text(int textfd,int index);

Input parameters: textfd: input file descriptor
          index:  file index produced by text compiler

Output parameters: char **text: pointer to an array of strings (ptrs)

Description:      This function uses index to identify a lump of text
                  to be read in from a language file. The text is returned
                  as a contiguous block of memory consisting of an array
                  of pointers to strings and the strings themselves. Hence
                  the whole block can be got rid of by one call to free.

Non-library routines used:

Possible error codes returned:  NULL if an error occured

Revision history:

@*/

#include <malloc.h>
#include <stdio.h>          /* for defn of NULL */
#include <string.h>
#include <io.h>
#include <assert.h>


static char **GetText(FILE *fp,int where)
{
  long sizes[2];
  long fpos;
  int length,nstrings;
  char* tstore = NULL;
  char* here = NULL;
  char* there = NULL;
  char* start = NULL;
  char** vect = NULL;

  assert(fp != NULL);
  assert(where > 0);
    
  /*first get the start and finish of the text */
  fpos = where * sizeof(long);
  
  if (fseek(fp,fpos,SEEK_SET))
    return(NULL);
  
  if (2 != fread(sizes,sizeof(long),2,fp))
    return(NULL);
  
  length=(int)(sizes[1]-sizes[0]);
  
  /*now get enough store to hold a temp copy of the strings*/
  if ((tstore=malloc(length))==NULL) return(NULL);
  
  /*and read the text into memory */
  if(fseek(fp,sizes[0],SEEK_SET))
    {
    free(tstore);
    return(NULL);
    }
  
  if(length != (int)fread(tstore,1,length,fp))
    {
    free(tstore);
    return(NULL);
    }
  
  /*count the number of strings read in */
  nstrings=0;
  here=tstore;
  while(here<(tstore+length))
    {
    ++nstrings;
    here += (*here)+1;
    }
  
  /* get enough memory to hold the strings & their pointers */
  if((start=malloc(sizeof(long)*(nstrings+1)+length))==NULL)
    {
    free(tstore);
    return(NULL);
    }
  
  /* and copy them across while setting up pointers to them */
  vect=(char**) start;            /* where the pointers go */
  there=start+sizeof(long)*(nstrings+1);    /* where the text goes */
  here=tstore;                /* where the text comes from */
  while (nstrings--)
    {
    strncpy(there,here+1,*here);      /* copy a string across */
    *vect++ =there;             /* set a pointer to it */
    there+=*here;             /* skip over string */
    *there++ ='\0';             /* and terminate it */
    here+= *here +1;            /* pt to next string to copy */
    }
  
  /* now terminate list of pointers with a NULL */
  *vect=NULL;
  
  free(tstore);
  return((char**) start);
}


/**********************************************************************
* Function:   PRSI_GetText
*
* Reads error strings from PARSER.ERR or builds a dummy text with
* Error nn text.
* Parameters: path is the path of the error file.
*             err is the error number
*
* Returns:    pointer to an array of strings.
**********************************************************************/
char** PRSI_GetText(char* path, int err)
{
  FILE* infp = NULL;
  char** text = NULL;

  infp = fopen(path,"rb");

  if(infp != NULL)
    {
    text = GetText(infp,err);
    fclose(infp);
    }
  return text;
}
