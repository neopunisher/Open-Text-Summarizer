/*
 *  text.c
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

unsigned char *
ots_get_line_text (const OtsSentence * aLine, gboolean only_if_selected, size_t * out_size)
{
  GList *li;
  GString *text;
  unsigned char *utf8_data;

  if (!(aLine))
    return NULL;

  text = g_string_new (NULL);

  if (!only_if_selected || aLine->selected)
    {
      for (li = (GList *) aLine->words; li != NULL; li = li->next)	/* for each word in the sentence Do: */
			if (li->data && strlen (li->data)) /*if word exists*/
	  			g_string_append (text, (char *) li->data);
			
    }
    
  if (out_size)
    *out_size = text->len;

  utf8_data = text->str;
  g_string_free (text, FALSE);

  return utf8_data;
}

static void
ots_print_line (FILE * stream, const OtsSentence * aLine)
{
  unsigned char *utf8_txt;
  size_t len;
  utf8_txt = ots_get_line_text (aLine, TRUE, &len);
  fwrite (utf8_txt, 1, len, stream);
  g_free (utf8_txt);
}

unsigned char *
ots_get_doc_text (const OtsArticle * Doc, size_t * out_len)
{
  GList *li;
  GString *text;
  unsigned char *utf8_data;
  size_t line_len;

  text = g_string_new (NULL);

  for (li = (GList *) Doc->lines; li != NULL; li = li->next)
    {
      utf8_data = ots_get_line_text ((OtsSentence *) li->data, TRUE, &line_len);
      g_string_append_len (text, utf8_data, line_len);
      g_free (utf8_data);
    }

  if (out_len)
    *out_len = text->len;
  utf8_data = text->str;

  g_string_free (text, FALSE);
  return utf8_data;
}

void
ots_print_doc (FILE * stream, const OtsArticle * Doc)
{
  GList *li;
  for (li = (GList *) Doc->lines; li != NULL; li = li->next)	/* for each line in Article Do: */
    ots_print_line (stream, (OtsSentence *) li->data);
  fputc ('\n', stream);
}
