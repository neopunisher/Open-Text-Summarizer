/*
 *  grader-tc.c
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


/*Grader - Term count algorithm*/
/*This is non-normelized term frequency algorithm without using inverse document frequency database */

#define NUM_KEY_WORDS 100 /* use first n key words only */

int
ots_get_article_word_count (const OtsArticle * Doc)
{
  GList *li;
  int articleWC;
  articleWC = 0;

  if (Doc==NULL) return 0;

  for (li = (GList *) Doc->lines; li != NULL; li = li->next)
    {
      articleWC += ((OtsSentence *) li->data)->wc;
    }

  return articleWC;
}


/*take this line and add each word to the "wordStat" list
* this list will hold all of the words in the article and the number 
* of times they appeared in the article.
*/

static void 
ots_line_add_wordlist(OtsArticle * Doc,const OtsSentence * aLine) 
{
  GList *li;
  if ((aLine==NULL) ||(NULL==Doc)) { return;}
  
      for (li = (GList *) aLine->words; li != NULL; li = li->next)	/* for each word in the sentence Do: */
			if (li->data && strlen (li->data)) ots_add_wordstat (Doc, (char *)li->data);
		    
  return;
}

static void 
ots_create_wordlist(OtsArticle * Doc)
{
GList *line;
if (Doc==NULL) return;

for (line = (GList *) Doc->lines; line != NULL; line = line->next)	
	{ 
	OtsSentence * aLine=line->data;
	if (aLine)
	ots_line_add_wordlist(Doc,aLine);
	}
}


	

static int
keyVal (const int n)			/* Ugly , I know */
{
  if (n == 1) return 3;
  if (n == 2) return 2;
  if (n == 3) return 2;
  if (n == 4) return 2;
  return 1;
}


static void
ots_grade_line (GList *impList, OtsSentence * aLine,
		OtsStemRule * rule)
{
  GList *li;
  GList *di;
  int n;
  char *tmp_stem;

  if ((aLine==NULL)||(rule==NULL)||(impList==NULL)) return;

  for (li = (GList *) aLine->words; li != NULL; li = li->next)	/* for each word */
    {
      n = 0;
      tmp_stem = ots_stem_strip ((unsigned char *) li->data, rule);

      for (di = (GList *) impList;
	   ((di != NULL) && (n < NUM_KEY_WORDS)); di = di->next)
	{
	  n++;
	if ((NULL!=((OtsWordEntery *) di->data)->stem) && (NULL!=tmp_stem))
	  if (0 == strcmp ((((OtsWordEntery *) di->data)->stem), tmp_stem)) 
	    {
	      /* debug:  
	      if (0!=strcmp((((OtsWordEntery *) di->data)->word),li->data)) 
	      printf("[%s][%s] stem[%s]\n",(((OtsWordEntery *) di->data)->word),li->data,tmp);*/

	      aLine->score += (((OtsWordEntery *) di->data)->occ) * keyVal (n);
	    }

	}

      g_free (tmp_stem);
    }

}


void
ots_create_title_tc(OtsArticle * Doc)
{

	char *tmp;
	char *word;
	int i;
    	GString *title;
	if (NULL==Doc) return;

	title=g_string_new(NULL);
	
	for (i=0;i<5;i++)
	{
	word = ots_word_in_list(Doc->ImpWords,i);
	if (word) g_string_append(title,word); else break;
	if (i<4) g_string_append(title,",");
	}
	
	tmp=title->str;
	if (NULL!=title) g_string_free(title,FALSE);
	Doc->title=tmp;
}


void
ots_grade_doc_tc (OtsArticle * Doc)
{

  GList *li;
  if (NULL==Doc) return;  
  ots_create_wordlist(Doc);
 
  
   Doc->ImpWords=ots_union_list (Doc->wordStat, Doc->dict);	   /* subtract from the Article wordlist all the words in the dic file (on , the , is...) */
   Doc->ImpWords=ots_sort_list (Doc->ImpWords);			  /* sort the list , top 3 is what the article talks about (SARS , virus , cure ... ) */

   /*to print wordlist: ots_print_wordlist (stdout, Doc->ImpWords);*/

  if (0 == Doc->lineCount) return;

  for (li = (GList *) Doc->lines; li != NULL; li = li->next)
    {
    if (li->data)
      ots_grade_line (Doc->ImpWords, (OtsSentence *) li->data, Doc->stem);
    }


  ots_create_title_tc(Doc);
}
