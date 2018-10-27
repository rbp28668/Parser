/* 
// File:        MALLOC.H
// 
// Prefix:  
// 
// Description: Header for special, and block memory allocation
//
*/

#ifndef _PMALLOC_H

/* Suppress diag malloc etc if NDEBUG or Windows */
#if defined _Windows && !defined NOCHECK  /* Borland */
#define NOCHECK
#endif

#if defined _WINDOWS && !defined NOCHECK  /* MS */
#define NOCHECK
#endif

#if defined NDEBUG && !defined NOCHECK 
#define NOCHECK
#endif

#include <stdlib.h> /* for size_t */
extern int PRSI_BlockCreate(void);
extern void *PRSI_BlockMalloc(int handle, size_t bytes);
extern void PRSI_BlockFree(int handle);
extern void *PRSI_BlockMark(int handle);
extern void PRSI_BlockFreeFrom (int handle,void *where);

#if !defined NOCHECK

/* These are in ck_mal.c for malloc checking */
#define malloc(size) DiagMalloc(size,__FILE__,__LINE__)
#define calloc(nbytes,nobj) DiagCalloc(nbytes,nobj,__FILE__,__LINE__)
#define free(ptr) DiagFree(ptr,__FILE__,__LINE__)
#endif

extern void *DiagMalloc(size_t nbytes,const char *file,int line);
extern void *DiagCalloc(size_t nbytes,size_t nobj,const char *file,int line);
extern void DiagFree(void *ptr,const char *file,int line);
extern void DumpHeap(void);

#define _PMALLOC_H
#endif
