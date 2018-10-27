/*
// File:          CK_MAL.C
// 
// Prefix:        NONE
// 
// Description:   Own implementation of malloc with added checking 
//                and diagnostics.
//
*/

#ifndef _MSC_VER  /* uses borland specific fns */

#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <dos.h>
#include <assert.h>
#include <stdlib.h>
#include "printf.h"

typedef struct s_mh
{
  long guard1;
  char file[16];      /* owner file */
  int line;       /* owner line number */
  void huge *block;   /* ptr to allocated block */
  unsigned long size; /* size in bytes of block inc header */
  int InUse;        /* true if used, false if free */
  struct s_mh huge *next; /* linked list of memory headers */
  char pad[8];
  long guard2;
}MH;

void DumpHeap(void);

static MH huge *MemChain = NULL;


static void InitMalloc(void)
{
  unsigned long available;

  available = farcoreleft();

  MemChain = farmalloc(available);
  if(MemChain == NULL)
    {
    PRSI_printf("Unable to allocate space for heap\n");
    return;
    }


  MemChain->block = (void *)(MemChain+1);
  MemChain->size = available-sizeof(MH);
  MemChain->InUse = 0;

  strcpy(MemChain->file,"FREE");
  MemChain->line = 0;

  MemChain->next = NULL;

  MemChain->guard1 = 0x42424242l;
  MemChain->guard2 = 0x42424242l;

  return;
}


static void *NormalisePointer(void *ptr)
{
  unsigned long addr;
  void *nptr;
  unsigned int noff,nseg;

  addr = (unsigned long)FP_SEG(ptr) << 4;
  addr += (unsigned long)FP_OFF(ptr);

  nseg = (unsigned int) (addr >> 4);
  noff = (unsigned int) (addr & 0x0000000f);

  nptr = MK_FP(nseg,noff);

  return(nptr);

}


void DumpHeap(void)
{
  MH huge *here;
  long freemem;
  int freeblocks;
  int count;
  int i;

  freemem = 0;
  freeblocks = 0;
  count = 0;
  
  if(MemChain == NULL)
    {
    PRSI_printf("Heap not initialised in DumpHeap()");
    return;
    }

  count = 0;
  for (here = MemChain;here != NULL;here = here->next)
    ++count;

  PRSI_printf("Heap has %d items\n",count);

  for (here = MemChain;here != NULL;here = here->next)
    {
    for (i=0;here->file[i] != '\0';++i)
      {
      if(i > sizeof(here->file))
        {
        PRSI_printf("Heap node corrupt (filename)\n");
        PRSI_printf("File: %.15s\n",here->file);
        exit(1);
        }
      }

    if(here->size > 655360l)
      {
      PRSI_printf("Heap node corrupt (size)\n");
      PRSI_printf("Node size is %lu\n",here->size);
      exit(1);
      }

    if(here->InUse)
      {
      PRSI_printf("%lu bytes at %p,belonging to %s,%d\n",
        here->size,
        here->block,
        here->file,here->line);
      }
    else
      {
      PRSI_printf("%lu bytes FREE at %p\n",
        here->size,
        here->block);

      freemem += here->size-sizeof(MH);
      ++freeblocks;
      }

    ++count;
    if(count == 20)
      {
      getch();
      count = 0;
      }

    }
  PRSI_printf("%lu bytes of free memory in %d blocks\n",freemem,freeblocks);
  getch();
  exit(1);
  return;
}


static void CheckGuard(const char *file,int line)
{
  MH huge *here;

  for (here = MemChain;here != NULL; here = here->next)
    {

    /* Check the guard blocks for the current node */
    if(here->guard1 != 0x42424242l ||
      here->guard2 != 0x42424242l)
      {
      PRSI_printf("Guard over-write before %s,line %d\n",file,line);
      if(here->guard1 != 0x42424242l)
        {
        PRSI_printf("Over-run of block before %p\n",here->block);
        here->guard1 = 0x42424242l;
        }
      if(here->guard2 != 0x42424242l)
        {
        PRSI_printf("Under-run of block at %p\n",here->block);
        here->guard2 = 0x42424242l;
        }

      DumpHeap();
      getch();
      exit(1);
      }
    }
  return;
}


static void *BasicMalloc(size_t nbytes,const char *file,int line)
{
  MH huge *here,huge *new;
  unsigned long TotalNeeded;

  if(MemChain == NULL)
    InitMalloc();

  TotalNeeded = nbytes;
  TotalNeeded += sizeof(MH);  /* allow space for new header */
  TotalNeeded = (TotalNeeded+3)&~3ul; /* long align */

  if(TotalNeeded > 65535ul)
    return(NULL);


  CheckGuard(file,line);

  /** Run through the chain of blocks looking for a free one **/
  for (here = MemChain;here != NULL; here = here->next)
    {
    if(!here->InUse && here->size >= TotalNeeded) /* found a block */
      {

      new = (MH huge *)((char huge *)here+TotalNeeded); /* new header after block */

      new->block = (void huge *)(new+1);          /* block lies after header */
      new->block = NormalisePointer(new->block);
      new->size = here->size - TotalNeeded;         /* includes header size */
      strcpy(new->file,"FREE");
      new->line = 0;
      new->InUse = 0;                   /* contains rest of free block */
      new->next = here->next;
      new->guard1 = 0x42424242l;
      new->guard2 = 0x42424242l;

      /* Save owner info */
      strncpy(here->file,file,sizeof(here->file));
      here->file[sizeof(here->file)-1] = '\0';
      here->line = line;

      here->size = TotalNeeded-sizeof(MH);
      here->next = new;
      here->InUse = 1;

      return(here->block);
      }
    }
  return(NULL);
}


void *DiagMalloc(size_t nbytes,const char *file,int line)
{
  void *mem;

  mem = BasicMalloc(nbytes,file,line);
  if(mem == NULL)
    {
    PRSI_printf("Malloc failed on %u bytes at %s,line %d\n",nbytes,file,line);
    DumpHeap();
    }

  return(mem);
}

void *DiagCalloc(size_t nbytes,size_t nobj,const char *file,int line)
{
  unsigned long TotalNeeded;
  void *mem;

  TotalNeeded = (unsigned long)nbytes * (unsigned long)nobj;
  TotalNeeded += sizeof(MH);  /* allow space for header */
  TotalNeeded = (TotalNeeded+3)&~3ul; /* long align */

  if(TotalNeeded > 65535ul)
    return(NULL);

  mem = BasicMalloc(nbytes*nobj,file,line);
  if(mem == NULL)
    {
    PRSI_printf("Calloc failed on %u objects of %u bytes at %s,line %d\n",
      nobj,nbytes,file,line);
    DumpHeap();
    }
  else
    {
    memset(mem,0,nbytes*nobj);
    }

  return(mem);
}


void DiagFree(void *ptr,const char *file,int line)
{
  MH  huge *here,huge *prev;

  if(MemChain == NULL)
    {
    PRSI_printf("Call to free with uninitialised memory chain\n");
    getch();
    }

  if(ptr == NULL)
    return;

  ptr = NormalisePointer(ptr);
  prev = NULL;

  CheckGuard(file,line);

  for (here = MemChain;here != NULL;here = here->next)
    {

    if((void huge *)here->block == (void huge *)ptr)  /* found the right block */
      {
      if(here->InUse)     /* and its in use */
        {
        here->InUse = 0;  /* mark as free */
        strcpy(here->file,"FREE");
        here->line = 0;

        /* Try and coalesce with following block */
        if(here->next != NULL && !here->next->InUse)
          {
          here->size += here->next->size; /* coalesce sizes */
          here->size += sizeof(MH);   /* allowing for free header*/
          here->next = here->next->next;  /* and unlink next block */
          }

        /* and try and coalesce with previous block */
        if(prev != NULL && !prev->InUse)
          {
          prev->size += here->size;
          prev->size += sizeof(MH);
          prev->next = here->next;
          }

        return;
        }
      else  /* freeing a block not in use */
        {
        PRSI_printf("Duplicate free on block at %s,line %d\n",file,line);
        getch();
        return;
        }

      }
    prev = here;
    }

  PRSI_printf("Freeing an unallocated block at %s,line %d\n",file,line);
  PRSI_printf("Pointer is %p\n",ptr);
  getch();
  return;
}

#if 1

void *malloc(size_t nbytes)
{
  return(DiagMalloc(nbytes,"UNKNOWN",0));
}

void *calloc(size_t nbytes,size_t nobj)
{
  return(DiagCalloc(nbytes,nobj,"UNKNOWN",0));
}

void *realloc(void *ptr,size_t nbytes)
{
  PRSI_printf("REALLOC\n");
  getch();
  ++nbytes;

  ptr=NULL;
  return(ptr);
}

void free(void *ptr)
{
  DiagFree(ptr,"UNKNOWN",0);
  return;
}

#endif

#endif /* ndef _MSC_VER */
