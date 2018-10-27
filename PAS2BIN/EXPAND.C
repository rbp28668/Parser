/*
// File:        EXPAND.C
// 
// Prefix:      NONE
// 
// Description: Expands a wildcard agument into a null-terminated
//              list of strings.
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <string.h>
#include "expand.h"
#include "malloc.h"

#define LUMP 32      /* no of names for each malloc*/
#define NAMELEN 13       /* no of bytes for each name */

#define new(x) (x*)malloc(sizeof(x))

typedef struct namelist
{
  char name[NAMELEN * LUMP];
  char *here;
  int count;
  struct namelist *next;
}NAME;


/****************************************************************
* Expand
* This takes a filename with wildcards and expands it into 
* a NULL terminated list of all the matching files.
* This list is allocated in a single block of heap and can
* be freed with a single call to free().
* Parameters:  path is the filespec to be expanded.
* Returns:    NULL if no files match path, otherwise
*             a list of strings containing matching
*             filenames, terminated by NULL.
****************************************************************/
char **Expand(char *path)
{
  struct find_t work;
  NAME *list,*this;
  int nfiles;
  char *here,*start,**ptr;
  int i;

  /** Run through the files making a linked list of all the ones found**/
  nfiles=0;
  list=NULL;
  if(_dos_findfirst(path,_A_NORMAL|_A_RDONLY,&work))
    {
    return(NULL);
    }

  this=new(NAME);
  this->here=this->name;
  this->count=0;
  this->next=list;
  list=this;

  do
    {
    if(this->count == LUMP)
      {
      this=new(NAME);
      this->here=this->name;
      this->count=0;
      this->next=list;
      list=this;
      }

    strcpy(this->here,work.name);
    this->here+=NAMELEN;
    ++(this->count);
    ++nfiles;
    }
  while(!_dos_findnext(&work));
  
  /* Now we have a list of the files make the names into an **/
  /** array of strings **/

  start=malloc((nfiles+1)*sizeof(char *)+nfiles*NAMELEN);
  ptr=(char**) start;
  here=start+(nfiles+1)*sizeof(char*);

  while(list!=NULL)
    {
    list->here=list->name;
    for(i=0;i<list->count;++i)
      {
      strcpy(here,list->here);  /* Get the filename */
      *ptr=here;          /* set up its string pointer */
      ++ptr;             /* and bump ptrs for next time */
      here+=NAMELEN;
      list->here += NAMELEN;
      }
    this=list;          /* run up the list */
    list=list->next;
    free(this);          /* deleting it as you go */
    }
  *ptr=NULL;            /* terminate the list */
  return((char **)start);
}

