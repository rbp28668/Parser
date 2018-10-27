/*
// File:          PSTRING.C
//
// Prefix:        NONE
//
// Description:   Internal string handling
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "malloc.h"
#include "pstring.h"

#define MINSTRING 32

/**********************************************************************
* PRS_CreateString
* Creates a new string in memory with the C string str as a template
**********************************************************************/
PSTRING PRS_CreateString(const char * str)
{
  PSTRING pstr = NULL;
  int len = 0;
  int alloc;

  assert(str != NULL);
  
  len = strlen(str);
  alloc = len+1;

  if (alloc < MINSTRING)
    alloc = MINSTRING;

  pstr = (StringT*)malloc(sizeof(StringT) + alloc);

  if (pstr != NULL)
    {
    pstr->ref = 0;    /* nothing referenced it yet */
    pstr->len = len;
    pstr->alloc = alloc;
    pstr->text = (char *)(pstr+1);
    strcpy(pstr->text,str);
    }

  return pstr;
}

/**********************************************************************
* PRS_CreateStringOfLength
* Creates a new string in memory with the given length.
**********************************************************************/
PSTRING PRS_CreateStringOfLength(int len)
{
  PSTRING pstr = NULL;
  int alloc;

  alloc = len+1;

  if (alloc < MINSTRING)
    alloc = MINSTRING;

  pstr = (StringT*)malloc(sizeof(StringT) + alloc);

  if (pstr != NULL)
    {
    pstr->ref = 0;    /* nothing referenced it yet */
    pstr->len = len;
    pstr->alloc = alloc;
    pstr->text = (char *)(pstr+1);
    pstr->text[0] = '\0';
    }

  return pstr;
}

/**********************************************************************
* PRSI_ReferenceString
* This increments the reference count of a string. 
**********************************************************************/
void PRSI_ReferenceString(PSTRING pstr)
{
  if(pstr != NULL)
    ++(pstr->ref);
}

/**********************************************************************
* PRS_DereferenceString
* This decrements the reference count and, if the string is no longer
* referenced by anyone, then it frees it.
**********************************************************************/
/*void PRS_DereferenceString(PSTRING pstr)
//{
//  if(pstr != NULL)
//    {
//    --(pstr->ref);
//    if(pstr->ref == 0)
//      free(pstr);
//    }
//}
*/
/**********************************************************************
* PRS_ModifyString
* This takes a string ptr and, if necessary, clones it. It returns
* a pointer to the original string or the clone.
**********************************************************************/
//PSTRING PRS_ModifyString(PSTRING pstr)
//{
//  PSTRING newstr = NULL;
//
//  if(pstr != NULL)
//    {
//    if(pstr->ref == 1)    /* only one owner, no need to clone */
//      newstr = pstr;
//    else                  /* need to clone string */
//      {
//      --(pstr->ref);      /* one less reference to original string
//                             as this ref now to cloned string */
//      newstr = PRS_CreateString(pstr->text); /* and create a copy to modify */
//      }
//    }
//  return newstr;
//}

/**********************************************************************
* PRS_StringContents
* Returns a pointer to the 'C' string in a string
**********************************************************************/
const char* PRS_StringContents(PSTRING pstr)
{
  char* lpsz = NULL;
  if(pstr != NULL)
    lpsz = pstr->text;

  return lpsz;
}

/**********************************************************************
* PRS_SetString
* Sets the contents of a string to a particular C string
**********************************************************************/
PSTRING PRS_SetString(PSTRING pstr, const char* str)
{
  int len = 0;

  assert(pstr);
  assert(str);
  assert(pstr->ref == 1);

  len = strlen(str);
  if(len < pstr->alloc)       /* then can modify in place */
    {
    strcpy(pstr->text, str);  /* so copy across & set length*/
    pstr->len = len;
    }
  else
    {
    free(pstr);
    pstr = PRS_CreateString(str);
    }
  return pstr;
}

/**********************************************************************
* PRSI_StrCmp
* This compares 2 strings to give their ordering. This is an internal
* function which is used by the interpreter as a CCall function for
* handling <,<=,=,>= & > for strings.
**********************************************************************/
int PRSI_StrCmp(void)
{
  PSTRING* pString = PRS_Parameters(2*sizeof(PSTRING));
  PRS_ReturnOrdinal(strcmp(PRS_StringContents(pString[0]),
                           PRS_StringContents(pString[1])));
  PRS_DeleteString(pString[1]);
  PRS_DeleteString(pString[0]);
  return 0;
}

/**********************************************************************
* PRS_CloneString
* if pstr == 0 create an empty string.
* if pstr->ref == 0 then  increment pstr->ref and return pstr
* if pstr->ref > 0 then copy pstr & return new pstr 
**********************************************************************/
PSTRING PRS_CloneString(PSTRING pstr)
{
  if(pstr == NULL)
    pstr = PRS_CreateString("");
  else if(pstr->ref == 0)
    PRSI_ReferenceString(pstr);
  else
    {
    pstr = PRS_CreateString(PRS_StringContents(pstr));
    PRSI_ReferenceString(pstr); /* make sure ref count of 1 */
    }
  return pstr;
}

/**********************************************************************
* PRS_DeleteString
**********************************************************************/
void PRS_DeleteString(PSTRING pstr)
{
  free(pstr);
}

