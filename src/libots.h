/*
 *  libots.h
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

#ifndef HAVE_LIBOTS_H
#define HAVE_LIBOTS_H

#include <glib.h>

G_BEGIN_DECLS 

typedef struct
{ 			/* the Term Frequency data structure */
  char* word;			
  double tf; /*Also used for TF*/  
} OtsWordTF;


typedef struct
{
	/*a GList of char*  */
  GList *RemovePre;   /* (a|b)  replace string a with b */ 
  GList *RemovePost; 
  GList *step1_pre;
  GList *step1_post;
  
  GList *synonyms; 
  GList *manual; 
	
  GList *ParserBreak; 	
  GList *ParserDontBreak; 	
	
	
	/*to be implemented*/ 
  GList *ReplaceChars; 
  
} OtsStemRule;


typedef struct
{
  GList *words;			 /* a Glist of words (char*) */
  glong score;				 /*score set by the grader*/
  gboolean selected;     /*is selected?*/
  gint wc;               /*word count*/
  void *user_data;       /*pointer to the original sentence , or serial number maybe*/
} OtsSentence;


typedef struct
{
  GList *lines;			   /* a Glist of sentences (struct Sentence) */
  gint lineCount;          /*lines in the text*/         
  char *title;              /*title , auto generated*/
  
  OtsStemRule *stem;       /*stemming & parsing rules*/

 /*Term Frequency grader*/  
  GList *tf_terms; 
  GList *idf_terms; 
  

  /*Term Count grader*/  
  GList *dict; 		/* dictionary from xml*/
  GList *wordStat;	/* a wordlist of all words in the article and their occ */    
  GList *ImpWords;   /*important words - for term count grader*/
  
  
} OtsArticle;


OtsArticle *ots_new_article (void);
void ots_free_article (OtsArticle *art);

/*parser*/
void ots_parse_file (FILE * stream, OtsArticle * Doc);	/*file input */
void ots_parse_stream(const unsigned char *utf8 , size_t len ,OtsArticle *Doc); /*parse unicode stream*/

OtsSentence *ots_append_line (OtsArticle * Doc);
void ots_append_word (OtsSentence * aLine,unsigned const char *aWord);
void ots_add_wordstat (OtsArticle * Doc,unsigned const char *wordString);


/*dictionary*/
gboolean ots_load_xml_dictionary (OtsArticle * Doc,unsigned const char *name);

int ots_get_article_word_count (const OtsArticle * Doc);


/*grader*/
void ots_highlight_doc (OtsArticle * Doc, int percent);     /*example: 20%*/
void ots_highlight_doc_lines (OtsArticle * Doc, int lines); /*example: 10 lines*/
void ots_highlight_doc_words (OtsArticle * Doc, int words); /*example: 50 words*/

void ots_grade_doc (OtsArticle * Doc);

void  ots_free_OtsWordTF(OtsWordTF *obj); /*todo: put in .h file*/
OtsWordTF*  ots_new_OtsWordTF(const char* word,const double idf);


/*HTML output*/
void ots_print_HTML (FILE * stream, const OtsArticle * Doc);
unsigned char *ots_get_doc_HTML (const OtsArticle * Doc, size_t * out_len);

/*TEXT output*/
void ots_print_doc (FILE * stream, const OtsArticle * Doc);
unsigned char *ots_get_doc_text (const OtsArticle * Doc, size_t * out_len);


/*Plugin writing*/
unsigned char* ots_get_line_text (const OtsSentence *aLine, gboolean only_if_selected, size_t *out_size);
gboolean ots_is_line_selected(const OtsSentence *aLine);

/*Stemm support*/
OtsStemRule *new_stem_rule(void);
void free_stem_rule (OtsStemRule *rule);
unsigned char * ots_stem_strip (unsigned const char * aWord, const OtsStemRule *rule); /*returns newly allocated string with the root of the word*/
unsigned char *ots_stem_format (unsigned const char *aWord, const OtsStemRule * rule); /*Remove leading spaces, comas, colons, etc. */

/*Relations between texts*/

/*Returns the number of topics that two blocks of text share*/
int ots_text_relations(
const unsigned char *text1,const unsigned char *lang_code1,
const unsigned char *text2,const unsigned char *lang_code2,const int topic_num);

/*For a given text, return the list of the topics*/
char* ots_text_topics(const unsigned char *text,const unsigned char *lang_code,int topic_num);


/*For a given text, return the list of the stemmed topics*/
GList* ots_text_stem_list(const unsigned char *text,const unsigned char *lang_code,int topic_num);


/*Gives a score on the relations between two lists of topics; simmilar to the inner product*/
int ots_topic_list_score(const GList *topic_list1,const GList *topic_list2);

G_END_DECLS



#endif /* HAVE_LIBOTS_H */
