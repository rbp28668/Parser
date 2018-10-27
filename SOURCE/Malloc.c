/* 
// File:          MALLOC.C
// 
// Prefix:        NONE
// 
// Description:   Special memory allocation routines. In particular
//                code to allocate blocks of memory and parcel them
//                out by sequential sub-allocations.
//
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <dos.h>
#include <assert.h>
#include "error.h"
#include "malloc.h"

#define LUMP (16*1024)

#define NHANDLES 40
#define MAXHANDLE NHANDLES-1
#define CHECK_HANDLE(i) (i<0)||(i>MAXHANDLE)||(handles[i]==NULL)

#define NO_MORE_HANDLES -1

#define FALSE 0
#define TRUE ~0

typedef struct s_store
  {
  size_t          bytesleft;  /* number of bytes left in the block */
  struct s_store* next;       /* next in chain of blocks */
  char*           here;       /* allocation position */
  }STORE;

static STORE *handles[NHANDLES];    /* array of handles */


/*****************************************************************/
/** GET_HANDLE returns a valid handle or an error code if there **/
/** are no handles left. (cribbed from tile.c)      **/
/*****************************************************************/
static int get_handle(void)
{
  int i;
  int found = FALSE;        /* haven't found a free handle yet */
  
  for(i=0;i<NHANDLES;++i)     
    {
    if (handles[i] == NULL)   /*have found free handle*/
      {
      found=TRUE;             /*flag as found */
      break;                  /*don't need to search rest of array*/
      }
    }
  if (found)
    return(i);      /* pass back valid handle */
  else
    return(NO_MORE_HANDLES);
}

/*****************************************************************/
/** PRSI_BlockCreate creates a new block and assigns a handle.  **/
/** the handle can then be used in BlockMalloc and BlockFree  **/
/*****************************************************************/
int PRSI_BlockCreate(void)
{
  int handle = -1;
  STORE *pStore = NULL;
  
  if((handle=get_handle()) == NO_MORE_HANDLES)
    {
    PRS_Error(IDS_ERR_MALLOC_NO_MORE_HANDLES);
    return(handle);
    }
    
  pStore = malloc(LUMP);
  if(pStore == NULL) 
    {
    PRS_Error(IDS_ERR_MALLOC_ALLOCATE_BLOCK);
    return(-1);
    }
    
  handles[handle] = pStore;
  pStore->next = NULL;  
  pStore->bytesleft = LUMP - sizeof(STORE);
  pStore->here = ((char*)pStore) + sizeof(STORE);
  
  return(handle);
}

/*****************************************************************/
/** PRSI_BlockMalloc is a malloc variant for large collections of **/
/** small objects that are built up, used, and can then be  **/
/** freed as one entity. BlockMalloc takes a handle returned  **/
/** from BlockMalloc and uses the handle to distingish  **/
/** different blocks            **/
/*****************************************************************/
void *PRSI_BlockMalloc(int handle, size_t bytes)
{
  STORE *pStore = NULL;
  char *here = NULL;
  
  if (bytes==0 || bytes > LUMP-sizeof(STORE))
    return(NULL);
  
  if(CHECK_HANDLE(handle))
    {
    PRS_Error(IDS_ERR_MALLOC_INVALID_HANDLE);
    return(NULL);
    }
    
  if(handles[handle]->bytesleft < bytes)
    {
    pStore = malloc(LUMP);
    if (pStore == NULL)
      {
      PRS_Error(IDS_ERR_MALLOC_INSUFFICIENT_MEMORY);
      return(NULL);
      }
      
    pStore->next = handles[handle];               /* link block to head of chain */
    handles[handle] = pStore;
    pStore->here = ((char*)pStore) + sizeof(STORE); /* start of free space */
    pStore->bytesleft = LUMP - sizeof(STORE);     /* and its length */
    }
  pStore = handles[handle];                       /* get current block */
  here = pStore->here;                            /* current free space */
  pStore->here += bytes;                          /* allocate a bit of it */
  pStore->bytesleft -= bytes;                     /* and drop the free count */
  
  return((void*)here);
}

/*****************************************************************/
/** PRSI_BlockFree frees all store associcated with a given handle  **/
/*****************************************************************/
void PRSI_BlockFree(int handle)
{
  STORE *here,*there;
  if (CHECK_HANDLE(handle))
    {
    PRS_Error(IDS_ERR_MALLOC_ILLEGAL_FREE_HANDLE);
    return;
    }
  here=handles[handle];
  while(here!=NULL)
    {
    there=here;
    here=here->next;
    free(there);
    }
  handles[handle]=NULL;
  return;
}


/*****************************************************************
* PRSI_BlockMark
* This returns a pointer to the next location that could be
* allocated by BlockMalloc for a particular handle.  This can then
* be used with BlockFreeFrom to roll back a failed operation
* freeing all the store used by that operation.  Note that the
* pointer returned by BlockMark may not be the one returned by
* the next call to BlockMalloc if there are fewer bytes remaining
* in the block than BlockMalloc requests.
*****************************************************************/
void *PRSI_BlockMark(int handle)
{
  STORE *pStore = NULL;
  
  if (CHECK_HANDLE(handle))
    {
    PRS_Error(IDS_ERR_MALLOC_INVALID_HANDLE);
    return(NULL);
    }
  
  pStore=handles[handle];         /* get current block */
  return (void*)pStore->here;     /* current free space */
}


/*****************************************************************
* PRSI_BlockFreeFrom frees all the store allocated after a specific
* pointer.  Note that the latest blocks are added to the front
* of the list so we count them up as they are traversed: these can
* then be freed easily.
*****************************************************************/
void PRSI_BlockFreeFrom (int handle,void *where)
{
  STORE *pStore = NULL;
  int count = 0;
  int found = FALSE;
  
  assert(where != NULL);
  
  if(CHECK_HANDLE(handle))
    {
    PRS_Error(IDS_ERR_MALLOC_INVALID_HANDLE_FREE_FROM);
    return;
    }

#if defined _MSDOS /* 16 bit 80x86 so segmented addressing*/
  for (pStore=handles[handle]; pStore != NULL; pStore = pStore->next)
    {
    if((_FP_SEG(where) ==  _FP_SEG(pStore))
      && (_FP_OFF(where) > _FP_OFF(pStore))
      && (_FP_OFF(where) <= _FP_OFF(pStore) + LUMP))
      {
      pStore->here = where;  /* start of free space */
      pStore->bytesleft = (int)(_FP_OFF(pStore) + LUMP - _FP_OFF(where));
      found = TRUE;
      break;
      }
    else
      ++count;
    }

#else  /* assume linear address */
  for (pStore=handles[handle]; pStore != NULL; pStore = pStore->next)
    {
    if(  (where > pStore )
	  && (where <= pStore + LUMP))
      {
      pStore->here = where;  /* start of free space */
      pStore->bytesleft = (int)(pStore + LUMP - where);
      found = TRUE;
      break;
      }
    else
      ++count;
    }
#endif

  /* check for any blocks preceding (allocated after) the one
  // we are freeing and free those as well */
  if (found)
    {
    while(count--)
      {
      pStore = handles[handle];
      handles[handle] = pStore->next;
      free(pStore);
      }
    }
  else
    {
    PRS_Error(IDS_ERR_MALLOC_PTR_NOT_IN_BLOCK);
    }
  return;
}

