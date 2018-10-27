/* 
// File:        MUSTHAVE.C
// 
// Prefix:      NONE
// 
// Description: code to check for a specific token
//
*/
#include <stdio.h>
#include "prsif.h"
#include "lex.h"
#include "error.h"
#include "musthave.h"
#include "malloc.h"

/* MustHave
/* compares the next token in the input stream with an expected
*  token. If they are different MustHave produces an error message
*  and returns True, otherwise MustHave returns False.
*/
int PRSI_MustHave(int tok)
{
  int actual = PRSI_LexGetToken();

  if(actual != tok)
    {
    PRS_Error(IDS_ERR_MUSTHAVE,
      PRSI_LexTranslateToken(tok),PRSI_LexTranslateToken(actual));
    }
  return(actual != tok);
}

