/*
Title:				Token.c

Function:			To break up input into recognisable tokens.

Author:				Bruce with input from Martin

Date:				13 July 89

Directory:

Calling sequence:
		       void tok_setlist(TOKELEMENT toklist[]);
    		   void tok_setstream(FILE *fp);
    		   void tok_setesc(char esc);
    		   void tok_setsep(char *sep);
    		   void tok_setline(int line);
			   int tok_getline(void);
    		   int tok_get(void **tokval);

Input parameters:
				toklist:	a list of tokens and the corresponding strings
							held in an array of TOKELEMENT. This is terminated
							by a null string pointer.
				fp:			A valid file pointer for the input stream.
				esc:		A char to be used as the escape character.
				sep:		A string of characters that are to be treated as
							whitespace.
				line:		An integer which becomes the logical line number
							in the input file.
				tokval:		A pointer to a void-pointer. This is used to pass
							back any value a string might have.		
						
Output parameters:	
				tok_get:	returns an integer which should uniquly identify
							the tokens that are read in. Any associated value
							is returned via tokval.
				tok_getline:	returns an integer which is the current line
							number.

Description:	This module is a basic lexical analyser for use in simple
				parsers. By default the token grabber recognises quoted strings
				(surrounded by ""), integers, reals, identifiers and Funny
				Looking Tokens (FLT). The token grabber is called tok_get and
				returns a token code and any associated value. There is a
				collection of tok_set????? routines which alter the
				environment in which tok_get works. These can add/change
				the set of meta-characters used by tok_set. The most important
				tok_set???? routine is tok_setlist that allows you to specify
				a list of keywords and punctuation that are to be recognised
				as discrete tokens.

Non-library routines used:	None.

Possible error codes returned: EOF at end of input file.

Revision history:

@*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TOKENLIB 1
#include "token.h"
#include "segdefs.h"

#define TOK_MAXLEN 255

#define isin(c,str) (strchr(str,c)!=NULL)


/****** STATICS to define token handler working envrionment *******/
static char tokbuf[TOK_MAXLEN]={'\0'};	/*initialise to null string*/
static char *tokbufptr=tokbuf;			/*buffer ptr init to start */
static int qcount;
static char tokesc='\\';
static char *toksep="";
static char tokquote='"';
static TOKELEMENT *toklist=NULL;
static FILE *tokstream=stdin;
static int tokline=0;

/*  statics for returning values for default tokens */
static int tokint;
static float tokreal;
static char tokstring[TOK_MAXLEN];

/************ TOKREAD reads a token into the token buffer *********/
/* inf is the file from which the input is taken */
/* quote is the character that surounds a  string */
/* esc is a character that strips any special meaning from the one after*/
/* sep is a string which contains all valid non-space seperators. */
/******************************************************************/

static int tokread(FILE *inf,char quote,char esc,char *sep)
{
int c;
char *p;

qcount=0;

while((c=fgetc(inf))!=EOF)		/* skip whitespace in input */
	{
	if (c=='\n') ++tokline;
	if (!(isin(c,sep) || isspace(c))) break;
	}

p=tokbuf;

while(c!=EOF)
	{
	if (c==esc)							/* escape: include next char as literal */
		{
		if ((c=fgetc(inf))==EOF) break; /*so get next ch but bomb if EOF*/
		if (c=='\n') ++tokline;
		*p++ = (char) c;				/*stuff it*/
		c=fgetc(inf);					/*get next one for next iteration*/
		if (c=='\n') ++tokline;
		continue;						/*and go round again */
		}
	if (c==quote)
		{
		if (++qcount == 2) break;	/* stop if end of string */
		}
	else	 						/* not a quote*/
		{
		if (isin(c,sep) || isspace(c)) /*whitespace ? */
			{
			if (qcount==0)			/* if outside a string */
				break;				/* stop */
			else					/* else inside a string */
				*p++ = (char) c;	/* so include whitespace */
			}						/* endif whitespace */
		else						/* not whitespace*/
			{
			*p++ = (char) c;		/* stuff the char */
			}						/* endelse (not whitespace*/
		}							/* endelse (not a quote) */
	c=fgetc(inf);
	if (c=='\n') ++tokline;
	}
*p='\0';
return(c==EOF);
}

/******* TOK_SETLIST sets up the token list ********/
void tok_setlist(TOKELEMENT tokl[])
{
toklist=tokl;
}

/******* TOK_SETSTREAM sets up where the input comes from *************/
void tok_setstream(FILE *fp)
{
tokstream=fp;
}

/******** TOK_SETESC sets up the escape character for a token ******/
void tok_setesc(char esc)
{
tokesc=esc;
}

/********* TOK_SETSEP sets up the array of characters that are treated
********** as seperators (as well as whitespace) ******/
void tok_setsep(char *sep)
{
toksep=sep;
}
/********* TOK_SETQUOTE sets up the quote delimeter character******/
void tok_setquote(char quote)
{
tokquote=quote;
}
/********** TOK_SETLINE sets the current line number **********/
void tok_setline(int line)
{
tokline=line;
}

/********** TOK_GETLINE gets the current line number ***********/
int tok_getline(void)
{
return(tokline);
}

/**********************************************************************/
/********* TOK_GET the grand-daddy of them all. This is the main*******/
/********* routine that breaks the input up into proper tokens  *******/
/********* returns the token type and any value associated      *******/
/**********************************************************************/

int tok_get(void **tokval)
{
char *there;
TOKELEMENT *currtok;

/* before parsing the first 'lump' of text make sure there is something
	there to parse.......*/
if (*tokbufptr == '\0')				/* nothing there */
	{
	if(tokread(tokstream,tokquote,tokesc,toksep))	/*true if eof*/
		{
		*tokval=NULL;
		return(EOF);
		}
	tokbufptr=tokbuf;				/* reset pointer to start of buffer */
	}
/* First, is the token a string ? */
if (qcount != 0)					/* buffer contains a string */
	{
	strcpy(tokstring,tokbufptr);	/* copy into return area */
	tokbufptr+=strlen(tokbufptr);	/* skip over entire buffer */
	*tokval=tokstring;				/* pointer pts to buffer */
	return(STRING);
	}

/* Ok, so not a string, is it a number? */
if (isdigit(*tokbufptr))						/* token is a number */
	{
	there=tokbufptr;							/* copy current posn */
	while(isdigit(*there)) ++there;				/* find 1st non digit */
	if ((*there == '.')&&(isdigit(*(there+1)))) /*look for dig.dig */
		{
		if (sscanf(tokbufptr,"%f",&tokreal)) 	/*if really is a real */
			{
			/* need to skip over 9999.99e99.9 for example */
			/* do the mantissa of form 9.9e99 */
			while(isdigit(*tokbufptr))
				++tokbufptr; 					/*skip over digits*/
			if(*tokbufptr == '.')
				 ++tokbufptr;					/*skip any decimal point*/
			while(isdigit(*tokbufptr))
				++tokbufptr; 					/*skip over digits*/

			/*look for exponent of form 99.9e9*/
			if( isin(*tokbufptr,"eE") 			/* Look for exponent */
				&& isdigit(*(tokbufptr+1)))
					++tokbufptr; 				/* &skip if it is there*/

			/* Look for e+9 or e-9*/
			if( 	isin(*tokbufptr,"eE")			 
				&&	isin(*(tokbufptr+1),"+-")
 				&& 	isdigit(*(tokbufptr+2)))
 					tokbufptr+=2; 				/* &skip if it is there*/

			while(isdigit(*tokbufptr))
				++tokbufptr; 					/*skip over digits*/
 
			*tokval=&tokreal;					/* return ptr to real read in */
			return(REAL);				/* tell the world what it is */
			} 		
		}
	else		/*not a real (but still a number) */
		{
		sscanf(tokbufptr,"%d",&tokint);		/* read in the real */
		while(isdigit(*tokbufptr))++tokbufptr;	/*skip over digits*/
		
		*tokval=&tokint;				/*pointer to the value */
		return(INT);
		}
	}									/* endif isdigit() */
	
/* Ok so if we have got here then it is not a string,a real or an int*/
/* so is it in the list of tokens? */		

if(toklist!=NULL)					/* if there is a token list....*/
	{		
	currtok=toklist;				/* point to start of token list*/
	while(currtok->match !=NULL)   /* list terminated by NULL token */
		{
		if(!strncmp(tokbufptr,currtok->match,strlen(currtok->match)))
			{
			tokbufptr+=strlen(currtok->match);	/*skip over token */
			*tokval=NULL;			/* only returning token */
			return(currtok->tok);
			}
		else						/* didn't match current token */
			++currtok;				/* so try next one */
		}							/* endwhile */
	}								/* endif currtok!=NULL */

/* 	having got here the current token is an unknown so if it starts
	with a letter assume it is an
	identifier of some sort and suck it up until the first illegal
	character for an identifier */

if(isalpha(*tokbufptr))						/* must be an identifier*/
	{
	there=tokstring;
	while((*tokbufptr)&&(isalnum(*tokbufptr)||(*tokbufptr=='_')))
		{
		*there++=*tokbufptr++;
		}
	*there='\0';
	*tokval=tokstring;
	return(IDENT);
	}
	
/* Well.... What have we got here? it is not a recognised string, number, 
meta-symbol or identifier. In other words it is a Funny Looking Token (FLT)
so just gobble it (Yes please!) and throw it back to the parser to cope 
with */

strcpy(tokstring,tokbufptr);		/*copy it across*/
tokbufptr+=strlen(tokbufptr);		/*skip over it*/
*tokval=tokstring;					/*point to where it's hidden*/
return(FLT);						/*Funny Looking Token*/
}
