/*
 *  grader-tc.h
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

#ifndef HAVE_GRADERTC_H
#define HAVE_GRADERTC_H


#include <glib.h>
#include "libots.h"

G_BEGIN_DECLS 


typedef struct
{
  gchar *word;    /* the word */
  gchar *stem;    /*stem of the word*/
  gint occ;			/* how many times have we seen this word in the text? */
} OtsWordEntery;

/*Word list manipulations*/
void ots_free_wordlist (GList *aList);



OtsWordEntery *ots_copy_wordEntery (OtsWordEntery * obj);
OtsWordEntery *ots_new_wordEntery (unsigned const char *wordString);
OtsWordEntery *ots_new_wordEntery_strip (unsigned const char *wordString,const OtsStemRule *rule);
void ots_free_wordEntery (OtsWordEntery * WC);

GList *ots_sort_list (GList* aList);
GList *ots_union_list (const GList *aLst, const GList * bLst);

char *ots_word_in_list (const GList *aList,const int index);
char *ots_stem_in_list (const GList *aList,const int index);
void ots_add_wordstat (OtsArticle * Doc,unsigned const char *wordString);


/*grader*/

void ots_grade_doc_tc (OtsArticle * Doc);

G_END_DECLS



#endif /* HAVE_GRADERTC_H */
