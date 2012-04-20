/*
 *  article.c
 *
 *  Copyright (C) 2003 Nadav Rotem <nadav256@hotmail.com>
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libots.h"
#include "grader-tc.h"

extern void ots_free_TF_wordlist (GList * aList);

#define MAX_WORD_LENGTH 35

/*Data structure related functions*/

OtsSentence *
ots_new_sentence (void)
{
  OtsSentence *aLine = g_new0 (OtsSentence, 1);
  aLine->words = NULL;
  aLine->wc = 0;
  aLine->selected = 0;
  aLine->score = 0;
  return aLine;
}

void
ots_free_sentence (OtsSentence * sen)
{
  if (sen != NULL)
    {
      g_list_foreach (sen->words, (GFunc) g_free, NULL);
      g_list_free (sen->words);
      g_free (sen);
    }
sen=NULL;
}

OtsArticle *
ots_new_article (void)
{
  OtsArticle *Doc;
  Doc = g_new0 (OtsArticle, 1);
  Doc->lineCount = 0;
  Doc->title = NULL;
  Doc->stem=new_stem_rule ();
  Doc->lines=NULL;
  Doc->dict = NULL;
  Doc->ImpWords = NULL;
  Doc->wordStat = NULL;
  
  Doc->tf_terms=NULL;
  return Doc;
}

void
ots_free_article (OtsArticle * art)
{
  if (NULL != art)
    {
      free_stem_rule (art->stem);
  	   ots_free_wordlist (art->dict);
  	   ots_free_wordlist (art->ImpWords);
	   ots_free_wordlist (art->wordStat);
      
      ots_free_TF_wordlist(art->tf_terms);
      
	   g_list_foreach (art->lines, (GFunc) ots_free_sentence, NULL);
      g_list_free (art->lines);
      
      if (art->title != NULL) g_free (art->title);
      g_free (art);
    } 
art=NULL;
}

OtsSentence *
ots_append_line (OtsArticle * Doc)
{
  OtsSentence *aLine = ots_new_sentence ();
  Doc->lineCount++;
  Doc->lines = g_list_append (Doc->lines, aLine);
  return aLine;
}

void
ots_append_word (OtsSentence * aLine,unsigned const char *aWord)
{
  if ((aWord == NULL) || (0==strlen(aWord)) ||(NULL==aLine)) return;
  aLine->wc++;
  aLine->words = g_list_append (aLine->words, (gpointer) g_strdup (aWord));
  return;
}


gboolean
ots_is_line_selected(const OtsSentence *aLine)
{
  if (aLine==NULL) {printf("Warning:Line=NULL\n"); return FALSE;}
  return (aLine->selected);
}
