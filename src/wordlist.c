/*
 *  wordlist.c
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

/*word lists manipulations , mainly for grader-tc */

OtsWordEntery *
ots_new_wordEntery_strip(unsigned const char *wordString,const OtsStemRule *rule) /*for real text use*/
{
  OtsWordEntery *aWord = g_new0 (OtsWordEntery, 1);
  aWord->occ = 1;
  aWord->word = ots_stem_format(wordString,rule);  
  aWord->stem = ots_stem_strip(wordString,rule);
  return aWord;
}

OtsWordEntery *
ots_new_wordEntery (unsigned const char *wordString) /*for dictionary use only, no formating here*/
{
  OtsWordEntery *aWord = g_new0 (OtsWordEntery, 1);
  aWord->occ = 1;
  aWord->word = g_strdup (wordString);  
  aWord->stem = g_strdup (wordString);
  return aWord;
}


void
ots_free_wordEntery (OtsWordEntery * WC)
{
  if (WC != NULL)
    {
      if (NULL!=WC->word) g_free (WC->word);
      if (NULL!=WC->stem) g_free (WC->stem);
      g_free (WC);
    }
}

void
ots_free_wordlist (GList * aList)
{  
  if (aList != NULL)
    {
  		 	g_list_foreach(aList,(GFunc)ots_free_wordEntery , NULL);
    		g_list_free(aList);
    } 
}

OtsWordEntery *
ots_copy_wordEntery (OtsWordEntery * obj)
{
  OtsWordEntery *aWord;
  if (obj == NULL) { return NULL;}
  aWord = g_new (OtsWordEntery, 1);
  aWord->occ = obj->occ;
  aWord->word = g_strdup (obj->word);  
  if (NULL!=obj->stem)
  {aWord->stem = g_strdup (obj->stem);} else {aWord->stem=NULL;}
  return aWord;
}

static int
ots_sort_handler (OtsWordEntery * node1, OtsWordEntery * node2)
{
  if (node1->occ > node2->occ)
    return -1;
  if (node1->occ < node2->occ)
    return 1;
  return 0;
}

GList *
ots_sort_list (GList* aList)
{ 
  GList *newList; 
  newList = g_list_sort (aList, (GCompareFunc) ots_sort_handler);	/* sort article */
  return newList;
}

GList *
ots_union_list (const GList *aLst, const GList * bLst)
{
  GList *li;
  GList *di;
  int insert;
  GList *newLst=NULL;
  
  for (li = (GList *) aLst; li != NULL; li = li->next)
    {
      insert = 1;
      for (di = (GList *) bLst; di != NULL; di = di->next)
		{
		 if(( li->data) && (di->data) && (((OtsWordEntery *) li->data)->word) && (((OtsWordEntery *) di->data)->word)) /*all defined?*/
	    if (0 == g_strncasecmp ((((OtsWordEntery *) li->data)->word), /*fix me: unicode issue?*/
				  (((OtsWordEntery *) di->data)->word), 10))
	    insert = 0;		/* if word in B */

		}
      if (insert == 1) 
	if ((li->data))
      newLst = g_list_append (newLst,ots_copy_wordEntery ((OtsWordEntery *) li->data));
    }
    
    return newLst;
}


char *
ots_word_in_list (const GList *aList,const int index)	/* return the String value of the n'th word */
{
  OtsWordEntery *obj = NULL;
  
  GList *item =(GList *)g_list_nth ((GList *)aList, index);
  if (item != NULL) obj = item->data;
  if (obj == NULL)
    {
      return NULL;
    }
  else
    return obj->word;
}

char *
ots_stem_in_list (const GList *aList,const int index)	/* return the String value of stem of the n'th word */
{
  OtsWordEntery *obj = NULL;

  GList *item =(GList *)g_list_nth ((GList *)aList, index);
  if (item != NULL) obj = item->data;
  if (obj == NULL)
    {
      return NULL;
    }
  else
    return obj->stem;
}

/*Adds a word to the word count of the article*/
void
ots_add_wordstat (OtsArticle * Doc,
		  unsigned const char *wordString)
{
  GList *li;
  OtsWordEntery *stat;
  OtsStemRule * rule=Doc->stem;
  char *tmp = NULL;

  if (NULL==wordString) return;
  if (NULL==Doc) return;
    
  if (0==strlen(wordString)) return;
  if (0==strcmp(wordString," ")) return;
  if (0==strcmp(wordString,"\n")) return;
  if (0==strcmp(wordString,"\t")) return;
  
  if (wordString)
  tmp = ots_stem_strip (wordString, rule);

  for (li = (GList *) Doc->wordStat; li != NULL; li = li->next)	/* search the word in current wordlist */
    {
      if (li->data)
      if (0 == strcmp (tmp, ((OtsWordEntery *) li->data)->stem))
	{
	  ((OtsWordEntery *) li->data)->occ++;	/* occurred in another place in the text now; */
	  g_free (tmp);
	
		/*printf for debug*/
	    /* 
	    if (0!=strcmp(((OtsWordEntery *) li->data)->word,wordString)  )
	     printf("[%s]==[%s]\n",((OtsWordEntery *) li->data)->word,wordString);
	    */

	  return;
	}
    }

  stat = ots_new_wordEntery_strip (wordString, rule);	/* if not in list , Add  stem  it to the list */
  if ((stat)) 
  Doc->wordStat = g_list_prepend (Doc->wordStat, stat);
  g_free (tmp);
  return;
}		  




void
ots_print_wordlist (FILE * stream, const GList * aList)
{
  GList *li;
  for (li = (GList *) aList; li != NULL; li = li->next)
    fprintf (stream, "Word[%d][%s]\n", ((OtsWordEntery *) li->data)->occ,
	    		 			((OtsWordEntery *) li->data)->word);
}



