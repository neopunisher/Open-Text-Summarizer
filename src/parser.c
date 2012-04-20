/*
 *  parser.c
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
#include <strings.h>
#include "libots.h"

#define BUFFER_SIZE (1024*8)

int
ots_match_post (const char *aWord,const char *post)
{
  int i, wlen, plen;
  

  wlen = strlen (aWord);
  plen = strlen (post);

  if (plen > wlen) return 0;

  for (i = 0; i < plen; i++)
    if (aWord[wlen - plen + i] != post[i])
      return 0;		/* no match */

  return 1;		/*word match */
}

void
ots_parse_file (FILE * stream, OtsArticle * Doc )
{
  unsigned char fread_buffer[BUFFER_SIZE];
  unsigned char *buffer;
  size_t nread, total_read, avail_size;

  buffer = g_new0 (unsigned char, BUFFER_SIZE);

  avail_size = BUFFER_SIZE;
  total_read = nread = 0;
  while ((nread =
	  fread (fread_buffer, sizeof (unsigned char), sizeof (fread_buffer),
		 stream)) > 0)
    {
      if (nread + total_read > avail_size)
	{
	  avail_size *= 2;
	  buffer = g_renew (unsigned char, buffer, avail_size);
	}

      strncpy (buffer + total_read, fread_buffer, nread);
      total_read += nread;
    }

  ots_parse_stream (buffer, total_read, Doc);
  g_free (buffer);
}





int
ots_parser_should_break(const char *aWord,const OtsStemRule * rule)
{
  GList *li;
  char *postfix;
  int toBreak=0;
  
  for (li = (GList *) rule->ParserBreak; li != NULL; li = li->next)
    {
      postfix=li->data;
      if (ots_match_post (aWord, postfix) )
      {  
         toBreak=1;
     		break; 
      }
      
    }


  for (li = (GList *) rule->ParserDontBreak; li != NULL; li = li->next)
    {
      postfix=li->data;
      if (ots_match_post (aWord, postfix) )
      {
         toBreak=0;
     		break; 
      }
      
    }
return toBreak;
}
      


void
ots_parse_stream(const unsigned char *utf8, size_t len, OtsArticle * Doc)	/*parse the unicode stream */
{

  OtsSentence *tmpLine = ots_append_line (Doc);
  OtsStemRule * rule=Doc->stem;
  gunichar uc;
  int index = 0;
  char *s = (char *) utf8;
  GString *word_buffer = g_string_new (NULL);

   
while ((*s) && (index < len))
    {
  uc = g_utf8_get_char (s);      
     
      if (!g_unichar_isspace (uc))	/* space is the end of a word */
		{

		g_string_append_unichar(word_buffer,uc); 

		} 
		else 
		{
		  
		  if (0<word_buffer->len)
				{
				ots_append_word (tmpLine, word_buffer->str);
  				
  				if (ots_parser_should_break(word_buffer->str,rule)) {
  					tmpLine = ots_append_line (Doc);	/* Add a new Line */
  					}
  	
  				g_string_assign (word_buffer, "");
  				
  				}
		
		if (uc=='\n') {ots_append_word (tmpLine,"\n");}
		else 
		{ots_append_word (tmpLine," ");}
		
		g_string_assign (word_buffer,"");
		}

      s = g_utf8_next_char (s);

      index++;
    }


		if (0<word_buffer->len) /*final flush*/
				{
				ots_append_word (tmpLine, word_buffer->str);
				g_string_assign (word_buffer, "");
  				}
  				 
 

  g_string_free (word_buffer, TRUE);
}
