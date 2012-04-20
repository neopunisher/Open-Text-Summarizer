/*
 *  ots.c
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

#include <glib.h>
#include <glib-object.h>

#include "libots.h"
#include "ots-config.h"

void print_about(FILE * stream,OtsArticle * Doc);

int main(int argc, char **argv)
{
  char *dictionary_file = "en";	/* if not told otherwise, assume we're using english */
  const char *input_file = NULL;
  char *output_file = NULL;

  FILE *input_stream = stdin;	/*by default read from stdin */
  FILE *output_stream = stdout;	/*by default read from stdout */

  OtsArticle *Art;

  int sumPercent = 20;	 	/* if not told otherwise highlight 20% of the  document */

  int c,n_args=0;

  int html = FALSE;
  int keywords = FALSE;
  int about = FALSE;
  int version = FALSE;
	
  const char *const *args;

	GOptionContext *context = NULL;
	GError *error = NULL;
	
	const GOptionEntry options[] = {
		{"ratio"   , 'r', 0, G_OPTION_ARG_INT   , &sumPercent, "summarization % [default = 20%]", "<int>"},
		{"dic"     , 'd', 0, G_OPTION_ARG_STRING, &dictionary_file, "dictionary to use", "<string>"},
		{"out"     , 'o', 0, G_OPTION_ARG_STRING, &output_file, "output file [default = stdout]", "<string>"},
		{"html"    , 'h', 0, G_OPTION_ARG_NONE  , &html, "output as html", NULL},
		{"keywords", 'k', 0, G_OPTION_ARG_NONE  , &keywords, "only output keywords", NULL},
		{"about"   , 'a', 0, G_OPTION_ARG_NONE  , &about, "only output the summary", NULL},
		{"version" , 'v', 0, G_OPTION_ARG_NONE  , &version, "show version information", NULL},
		{ G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &args, NULL, "[file.txt | stdin]" },
		{NULL}
  };

	context = g_option_context_new(" - Open Text Summarizer");
	g_option_context_add_main_entries(context, options, NULL);

        /* Parse command line */
        if (!g_option_context_parse (context, &argc, &argv, &error))
        {
                g_option_context_free (context);

                g_print ("%s\n", error->message);
                g_error_free (error);
                exit (1);
        }

	/* print version number */
  if (version)
    {
		printf("%s\n", PACKAGE_STRING);
		g_option_context_free(context);
      exit (0);
    }

  if (args)
    while (args[n_args] != NULL)
      n_args++;

  if (n_args > 1)
    {
		g_option_context_free(context);
      return 1;
    }

	if (n_args == 1 && g_file_test (args[0], G_FILE_TEST_EXISTS) == TRUE)
    input_file = args[0];

  if (input_file)
    {
      input_stream = fopen (input_file, "r");
      if (!input_stream)
	{
			g_option_context_free(context);
	  perror ("Couldn't load input file");
	  return 1;
	}
    }

  if (output_file)
    {
      output_stream = fopen (output_file, "w");
      if (!output_stream)
	{
	  if (input_file)
	    fclose (input_stream);
			g_option_context_free(context);
	  perror ("Couldn't load output file");
	  return 1;
	}
    }

  Art = ots_new_article ();
	
  if (!ots_load_xml_dictionary (Art, dictionary_file))
    {
   
      ots_free_article (Art);
		if (input_file)
			fclose(input_stream);
		if (output_file)
			fclose(output_stream);
		g_option_context_free(context);
      perror ("Couldn't load dictionary");
      return 1;
    }


  ots_parse_file (input_stream, Art);	/* read article from stdin , put it in struct Article */
  ots_grade_doc (Art);					/* grade each sentence (how relevent is it to the text) */
  ots_highlight_doc (Art, sumPercent);	/* highlight what we are going to print 0% - 100% of the words */


	if (html)
		ots_print_HTML(output_stream, Art);	/* print article in html form */
	else if (keywords)
		printf("deprecated\n");	/* print_keywords(output_stream,Art); */
	else if (about)
		print_about(output_stream, Art);
	else
		ots_print_doc(output_stream, Art);	/* print article in text */

  ots_free_article (Art);
 

  if (input_file)
    fclose (input_stream);

  if (output_file)
    fclose (output_stream);

  return 0;
}


void print_about(FILE * stream, OtsArticle * Doc)
{
    fprintf (stream, "Article talks about \"%s\"\n",Doc->title);
}    
