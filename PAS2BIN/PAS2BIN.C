

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "prsif.h"
#include "lex.h"
#include "expand.h"

#define NO_ERROR 0
#define USAGE_ERROR 1
#define CREATE_ERROR 2
#define PARSE_ERROR 3
#define WRITE_ERROR 4

static FILE *infp = NULL;

char *InputFunction(char *buff,int n)
{
  char *str;
  str = fgets(buff,n,infp);
  return str;
}


/**********************************************************************
* Function:   main (pas2bin)
* Converts pascal source to a binary file.
* Parameters: argc is the number of arguments.
*             argv is an array of strings:
*             argv[0] = app name
*             argv[1] = output file.
*             argv[2]..argv[argc-1] = input file(s)
* Returns:    0 for success, non-zero for failure.
**********************************************************************/
int main(int argc,char* argv[])
{
  FILE* outfp = NULL;
  int err = NO_ERROR;
  int i;
  long lVal;
  double dVal;
  int blFinished = 0;
  int tok;
  char *str = NULL;
  char** files = NULL;
  char** here = NULL;
  long now;
  struct tm* ptm = NULL;
  BIN_HEADER header;

  assert(sizeof(header) == 32);

  if(argc < 3)
    err = USAGE_ERROR;

  if(err == NO_ERROR)
    {
    outfp = fopen(argv[1],"wb");
    if(outfp == NULL)
      err = CREATE_ERROR;
    }
  
  if(err == NO_ERROR)
    {
    /* Set up the file header */
    header.lMagic = LEX_MAGIC;
    header.byteOrdering = LEX_INTEL;
    header.nSize = sizeof(header);
    header.nVerMajor = 0;
    header.nVerMinor = 0;
    header.nVerSub = 0;
  
    time(&now);
    ptm = localtime(&now);
    header.nTimeSec = ptm->tm_sec;
    header.nTimeMin = ptm->tm_min;
    header.nTimeHr = ptm->tm_hour;
    header.nDateDay = ptm->tm_mday;
    header.nDateMonth = ptm->tm_mon+1;
    header.nDateYr = ptm->tm_year+1900;
  
    for(i=0; i<sizeof(header.achSpare); ++i)
      header.achSpare[i] = 0;
  
    /* Look for any switches*/
    for(i=2; i<argc; ++i)
      {
      if(strchr("/-",*argv[i]) != NULL)
        {
        switch(argv[i][1])
          {
          case 'V':
          case 'v':
            sscanf(argv[i]+2,"%hd.%hd.%hd",
              &header.nVerMajor,
              &header.nVerMinor,
              &header.nVerSub);
  
            printf("Version set to %hd.%hd.%hd\n",
              header.nVerMajor,
              header.nVerMinor,
              header.nVerSub);
            break;
  
          default:
            fprintf(stderr,"Switch %s ignored\n",argv[i]);
            break;
          }
        }
      }
  
  
    PRSI_SetupLexAnalyser();
    PRS_InputFunction(InputFunction);
  
    /* Now write the header to the start of the file */
    if( 1 != fwrite(&header,sizeof(header),1,outfp))
      err = WRITE_ERROR;
    }
    
  for(i=2; err == NO_ERROR && i<argc; ++i)
    {
    /* skip any switches */
    if(strchr("/-",*argv[i]) != NULL)
      continue;

    files = Expand(argv[i]);
    
    for(here = files; *here!=NULL; ++here)
      {
      infp = fopen(*here,"r");
      if(infp == NULL)
        {
        fprintf(stderr,"Can't open %s\n",*here);
        continue;
        }

      printf("Processing %s\n",*here);

      blFinished = 0;
      do
        {
        tok = PRSI_LexGetToken();

        switch(tok)
          {
          case TokUndefined:
            fprintf(stderr,"Undefined token\n");
            blFinished = 1;
            break;

          case TokEof:
            blFinished = 1;
            break;

          case  TokErrNewlineInString:
          case  TokErrStringTooLong:
          case  TokErrEOFInString:
          case  TokErrFileReadInt:
          case  TokErrFileReadDouble:
          case  TokErrFileReadChar:
          case  TokErrFileReadEOF:
          case  TokErrEOFInIdent:
          case  TokErrIdentTooLong:
            err = PARSE_ERROR;
            break;

          case TokString:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            else
              {
              str = PRSI_LexGetString();
              while(*str != '\0')
                {
                if(fputc(*str,outfp) == EOF)
                  {
                  err = WRITE_ERROR;
                  break;
                  }
                ++str;
                }
              }
            if(fputc(0,outfp) == EOF)
              err = WRITE_ERROR;
            break;

          case TokInt:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            else
              {
              lVal = PRSI_LexGetLong();
              if( 1 != fwrite(&lVal,sizeof(long),1,outfp))
                {
                err = WRITE_ERROR;
                break;
                }
              }
            break;

          case TokFloat:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            else
              {
              dVal = PRSI_LexGetDouble();
              if( 1 != fwrite(&dVal,sizeof(double),1,outfp))
                err = WRITE_ERROR;
              }
            break;

          case TokChar:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            else if(fputc(PRSI_LexGetChar(),outfp) == EOF)
              err = WRITE_ERROR;
            break;

          case TokIdentifier:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            else
              {
              str = PRSI_LexGetIdentifier();
              while(*str != '\0')
                {
                if(fputc(*str,outfp) == EOF)
                  {
                  err = WRITE_ERROR;
                  break;
                  }
                ++str;
                }
              }
            if(fputc(0,outfp) == EOF)
              err = WRITE_ERROR;
            break;

          default:
            if(fputc(tok,outfp) == EOF)
              err = WRITE_ERROR;
            break;
          }/* End-SWITCH */
        }/* End-WHILE */
      while(err == NO_ERROR && !blFinished);

      if(infp != NULL)
        fclose(infp);
      }/* End-FOR */
      
    free(files);
    files = NULL;
    
    }/* End-FOR */

  if(outfp != NULL)
    fclose(outfp);
    
  switch (err)
    {
    case NO_ERROR:
      fprintf(stderr,"Success\n");
      break;

    case USAGE_ERROR:
      fprintf(stderr,
        "Usage: %s <output-file> <input-file> [input-file]\n",
        argv[0]);
      break;

    case CREATE_ERROR:
      fprintf(stderr,"Can't create %s\n",argv[1]);
      break;

    case WRITE_ERROR:
      fprintf(stderr,"Write error on %s\n",argv[1]);
      break;

    default:
      fprintf(stderr,"Unhandled error %d\n",err);
      break;
    }

  return err;
}
