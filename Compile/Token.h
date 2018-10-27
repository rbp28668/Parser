/*
Title:						TOKEN.H

Function:					To provide predefined tokens for token.c

Author:						Bruce

Date:

Directory:

Calling sequence:

Input parameters:

Output parameters:

Description:

Non-library routines used:

Possible error codes returned:

Revision history:

@*/

#define  STRING 0
#define  REAL 1
#define  INT 2
#define  IDENT 3
#define  FLT 4
#define  LAST_STD_TOKEN 4

typedef struct tokel
	{
	int tok;
	char *match;
	}TOKELEMENT;


#ifdef __cplusplus
extern "C" 
{
#endif

extern	void tok_setlist(struct tokel *tokl); 
extern  void tok_setstream(FILE *fp);
extern  void tok_setesc(char esc);
extern  void tok_setsep(char *sep);
extern  void tok_setquote(char quote);
extern  void tok_setline(int line);
extern  int tok_getline(void );
extern  int tok_get(void * *tokval);

#ifdef __cplusplus
}
#endif