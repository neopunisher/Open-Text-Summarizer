/*
 *  stemmer.c
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

#define MAX_PREFIX_SIZE 256

OtsStemRule *
new_stem_rule ()
{
  OtsStemRule *rule = g_new0 (OtsStemRule, 1);
  return rule;
}

void
free_stem_rule (OtsStemRule *rule)
{

if (rule != NULL)
    {
      g_list_foreach (rule->RemovePre, (GFunc) g_free, NULL);
      g_list_free (rule->RemovePre);
      g_list_foreach (rule->RemovePost, (GFunc) g_free, NULL);
      g_list_free (rule->RemovePost);

      g_list_foreach (rule->step1_pre, (GFunc) g_free, NULL);
      g_list_free (rule->step1_pre);      
      g_list_foreach (rule->step1_post, (GFunc) g_free, NULL);
      g_list_free (rule->step1_post);

      g_list_foreach (rule->synonyms, (GFunc) g_free, NULL);
      g_list_free (rule->synonyms);
      g_list_foreach (rule->manual, (GFunc) g_free, NULL);
      g_list_free (rule->manual);

      g_list_foreach (rule->ParserBreak, (GFunc) g_free, NULL);
      g_list_free (rule->ParserBreak);
      g_list_foreach (rule->ParserDontBreak, (GFunc) g_free, NULL);
      g_list_free (rule->ParserDontBreak);

      g_list_foreach (rule->ReplaceChars, (GFunc) g_free, NULL);
      g_list_free (rule->ReplaceChars);           
 
      g_free (rule);
    } 
  return;
}

static void
ots_stem_break (unsigned const char *comp,unsigned char *part_a,unsigned char *part_b)	/*given already alocated part_a and b */
{				/*example "red|blue" */
  int i, j, clen;
  i = 0;
  j = 0;
  
  if (comp==NULL) return;
  if (part_a==NULL) return;
  if (part_b==NULL) return;

  clen = strlen (comp);


  part_a[0] = 0;
  part_b[0] = 0;

  while ((i < clen) && (i < MAX_PREFIX_SIZE) && (comp[i] != '|'))
    {
      part_a[i] = comp[i];
      i++;
    }
  part_a[i] = 0;

  i++;				/*skip the | mark */
  while (i < clen && (j < MAX_PREFIX_SIZE))
    {
      part_b[j] = comp[i];
      i++;
      j++;
    }
  part_b[j] = 0;
  return;
}


static unsigned char *
ots_stem_remove_pre (unsigned const char *aWord,unsigned const char *pre,unsigned const char *new)
{
  int i, plen, wlen, nlen;
  unsigned char *new_str = NULL;

  if (aWord==NULL) return NULL;

  plen = strlen (pre);
  wlen = strlen (aWord);
  nlen = strlen (new);

  for (i = 0; i < plen; i++)
    if (aWord[i] != pre[i])
      return NULL;		/*no match */

  new_str = g_new0 (char, wlen + nlen +5);
  for (i = 0; i <= nlen; i++)
    new_str[i] = new[i];

  for (i = nlen; i <= nlen + wlen - plen; i++)
    new_str[i] = aWord[i + plen - nlen];

  new_str[i + 1] = 0;
  return new_str;
}



static unsigned char *
ots_stem_remove_post (unsigned const char *aWord,unsigned const char *post,unsigned const char *new)
{
  unsigned int i, wlen, plen, nlen;
  unsigned char *new_str = NULL;

  if ((NULL==aWord)||(NULL==post)||(NULL==new)) return NULL;

  wlen = strlen (aWord);
  plen = strlen (post);
  nlen = strlen (new);

  if (plen>wlen) return NULL;


  for (i = 0; i < plen; i++)
    if (aWord[wlen - plen + i]!= post[i])
      return NULL;		/* no match */

  new_str = g_new0 (char, wlen + nlen +5);

  for (i = 0; i <= wlen - plen; i++)	/*place word */
    new_str[i] = aWord[i];

  for (i = 0; i <= nlen; i++)	/*place newfix */
    new_str[wlen - plen + i] = new[i];

  return new_str;		/*word replaced */
}  
  


static unsigned char *
ots_stem_replace_word (unsigned const char *aWord,unsigned const char *old,unsigned const char *new)
{

  if (aWord==NULL) return NULL;
  
if ((aWord)&&(0 == strcmp (aWord, old)))
    {
      return g_strdup (new);
    }
  else
    {
      return NULL;
    }

}





unsigned char *
ots_stem_format (unsigned const char *aWord, const OtsStemRule * rule)
{
  GList *li;
  unsigned char *rep = NULL;
  unsigned char *normWord = NULL;

  if (aWord==NULL) return NULL;
  
  normWord = g_utf8_strdown (aWord, -1);	/*lowercase the word */

  char *prefix;
  char *newfix;

  prefix = g_new0 (char, MAX_PREFIX_SIZE);
  newfix = g_new0 (char, MAX_PREFIX_SIZE);

 for (li = (GList *) rule->step1_pre; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_remove_pre (normWord, prefix, newfix);
      if (NULL != rep)
		{
	  		g_free (normWord);
	  		normWord = rep;
	  		rep = NULL;
		}
    }


 for (li = (GList *) rule->step1_post; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_remove_post(normWord, prefix, newfix);
      if (NULL != rep)
		{
	  		g_free (normWord);
	  		normWord = rep;
	  		rep = NULL;
		}
    }

  g_free (prefix);
  g_free (newfix);

  return normWord;
}








unsigned char *
ots_stem_strip (unsigned const char *aWord,const OtsStemRule * rule)
{
  GList *li;
  unsigned char *rep = NULL;

  unsigned char *prefix;
  unsigned char *newfix;
  unsigned char *normWord=NULL;

  prefix = g_new0 (char, MAX_PREFIX_SIZE);
  newfix = g_new0 (char, MAX_PREFIX_SIZE);

  if (aWord==NULL) return NULL;

  normWord = ots_stem_format (aWord,rule);


  for (li = (GList *) rule->manual; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_replace_word (normWord, prefix, newfix);
      if (NULL != rep)
	{
	  g_free (normWord);
	  normWord = rep;
	  rep = NULL;
	  break;
	}
    }




  for (li = (GList *) rule->RemovePre; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_remove_pre (normWord, prefix, newfix);
      if (NULL != rep)
	{
	  g_free (normWord);
	  normWord = rep;
	  rep = NULL;
	  break;
	}
    }


  for (li = (GList *) rule->RemovePost; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_remove_post (normWord, prefix, newfix);
      if (NULL != rep)
	{
	  g_free (normWord);
	  normWord = rep;
	  rep = NULL;
	  break;
	}

    }


  for (li = (GList *) rule->synonyms; li != NULL; li = li->next)
    {
      ots_stem_break (li->data, prefix, newfix);
      rep = ots_stem_replace_word (normWord, prefix, newfix);
      if (NULL != rep)
	{
	  g_free (normWord);
	  normWord = rep;
	  rep = NULL;
	  break;
	}
    }


  g_free (prefix);
  g_free (newfix);


  if (strlen(normWord)<3)  /*stem is two letter long. thats not right. N(eed)==N(ation) ?*/
		{ 
		g_free(normWord);  
		normWord = ots_stem_format (aWord,rule);	/*lowercase the word */	
		}


  return normWord;
}
