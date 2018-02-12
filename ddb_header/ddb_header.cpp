/* ******************************************************************** **
** @@ DDB_Header
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Dscr   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "..\shared\file.h"
#include "..\shared\file_find.h"
#include "..\shared\mmf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

/* ******************************************************************** **
** @@ struct
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

struct DDB_HEADER
{
   DWORD             _dw1;
   DWORD             _dw2;
   WORD              _w1;
   char              _pszSignature[18];
   WORD              _w3;
   WORD              _w4;
   BYTE              _by1;
   BYTE              _by2;
   BYTE              _by3;
   BYTE              _by4;
};

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD   dwKeepError = 0;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */
                  
static MMF        _MF;

static MMF*       _pMF = &_MF;  

static FILE*      _pOut = NULL;

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ PrintEntry()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void PrintHeader(const BYTE* const pBuf)
{
   DDB_HEADER*    pHdr = (DDB_HEADER*)pBuf;

   ASSERT(pHdr);

   fprintf(_pOut,"%08X  ",pHdr->_dw1);
   fprintf(_pOut,"%08X  ",pHdr->_dw2);
   fprintf(_pOut,"%04X  ",pHdr->_w1);
   fprintf(_pOut,"*  ");
   fprintf(_pOut,"%04X  ",pHdr->_w3);
   fprintf(_pOut,"%04X  ",pHdr->_w4);
   fprintf(_pOut,"%02X  ",pHdr->_by1);
   fprintf(_pOut,"%02X  ",pHdr->_by2);
   fprintf(_pOut,"%02X  ",pHdr->_by3);
   fprintf(_pOut,"%02X  ",pHdr->_by4);
}

/* ******************************************************************** **
** @@ ForEach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ForEach(const WIN32_FIND_DATA& w32fd)
{
   ASSERT(_pMF);

   if (!_pMF->OpenReadOnly(w32fd.cFileName))
   {
      perror("\a\nOpen Input File Error !\n");
      return;
   }
   
   BYTE*       pBuf   = _pMF->Buffer();
   DWORD       dwSize = _pMF->Size();

   if (!pBuf || !dwSize || (dwSize < sizeof(DDB_HEADER)))
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (IsBadReadPtr(pBuf,sizeof(DDB_HEADER)))
   {
      // Error !
      ASSERT(0);
      return;
   }

   PrintHeader(pBuf);

   fprintf(_pOut,"**  %s\n",w32fd.cFileName);

   _pMF->Close();
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ShowHelp()
{
   const char  pszCopyright[] = "-*-   DDB_Header 1.0   *   (c)   Gazlan, 2015   -*-";
   const char  pszDescript [] = "DDB files header printer";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: ddb_header.com [wildcards]\n\n");
   printf("%s\n\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int main(int argc,char** argv)  
{
   if (argc != 2)
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2)
   {
      if ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h")))
      {
         ShowHelp();
         return 0;
      }
   }

   FindFile*   pFF = new FindFile;

   if (!pFF)
   {
      return -1;
   }

   if (argc == 2)
   {
      if ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h")))
      {
         ShowHelp();
         return 0;
      }

      pFF->SetMask(argv[1]);
   }
   else
   {
      pFF->SetMask("*.*");
   }

   _pOut = fopen("ddb_header.txt","wt");

   if (!_pOut)
   {
      // Error !
      ASSERT(0);
      return 0;
   }
   
   while (pFF->Fetch())
   {
      if ((pFF->_w32fd.dwFileAttributes | FILE_ATTRIBUTE_NORMAL) && !(pFF->_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         ForEach(pFF->_w32fd);
      }
   }

   fclose(_pOut);
   _pOut = NULL;

   delete pFF;
   pFF = NULL;

   return 0;
}