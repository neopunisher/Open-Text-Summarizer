/*
 *  highlighter
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

/*After the grader has graded the article and each
 sentence has a score the highlighter will select
 some of the sentences*/

static int
ots_highlight_max_line (OtsArticle * Doc)
{
  GList *li;
  int max = 0;
  for (li = (GList *) Doc->lines; li != NULL; li = li->next)
    {
      if (0 == (((OtsSentence *) li->data)->selected))	/* if not selected , count me in */
	max = MAX (((OtsSentence *) li->data)->score, max);

    }

  for (li = (GList *) Doc->lines; li != NULL; li = li->next)
    {

      if ((((OtsSentence *) li->data)->score == max) && (((OtsSentence *) li->data)->selected == 0))	/* if score==max && not selected before ,select me; */
	{
	  ((OtsSentence *) li->data)->selected = 1;
	  return ((OtsSentence *) li->data)->wc;
	}
    }

  return 0;
}


/* todo: impement this

void
ots_highlight_doc_wordcount (OtsArticle * Doc, int wordCount)

void
ots_highlight_doc_linecount (OtsArticle * Doc, int wordCount)



void
ots_highlight_doc_soft (OtsArticle * Doc, int percent) //blur selection by avrage of near sentences , will mark blocks
*/

void
ots_highlight_doc (OtsArticle * Doc, int percent)
{
  int i;
  double ratio;
  int wordCount;

  if (0 == Doc->lineCount)
    return;

  if (percent > 100)
    percent = 100;
  else if (percent < 0)
    percent = 0;

  ratio = ((double) (percent)) / (100.0);

  wordCount = ots_get_article_word_count (Doc);

  for (i = 0; i < (ratio * (double) wordCount);)
    {
      i += ots_highlight_max_line (Doc);
    }
}

void
ots_highlight_doc_lines (OtsArticle * Doc, int lines)
{
  int i;
  int lineCount;
  int tmp; 
  
  if (0 == Doc->lineCount) return;

  lineCount = Doc->lineCount;
  i=0;
  while ((i<lines)&&(i<lineCount))
  {
  i++;
  tmp=ots_highlight_max_line (Doc);
  }
  
}

void ots_highlight_doc_words (OtsArticle * Doc, int words)
{
  int i;
  int docWordCount;

  if (0 == Doc->lineCount) return;

  docWordCount = ots_get_article_word_count (Doc);
	
	i=0;
  while ((i < docWordCount) && (i <= words))
    {
      i += ots_highlight_max_line (Doc);
    }
}
