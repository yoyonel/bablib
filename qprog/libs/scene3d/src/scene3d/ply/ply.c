/*

The interface routines for reading and writing PLY polygon files.

Greg Turk

---------------------------------------------------------------

A PLY file contains a single polygonal _object_.

An object is composed of lists of _elements_.  Typical elements are
vertices, faces, edges and materials.

Each type of element for a given object has one or more _properties_
associated with the element type.  For instance, a vertex element may
have as properties the floating-point values x,y,z and the three unsigned
chars representing red, green and blue.

-----------------------------------------------------------------------

Copyright (c) 1998 Georgia Institute of Technology.  All rights reserved.   
  
Permission to use, copy, modify and distribute this software and its   
documentation for any purpose is hereby granted without fee, provided   
that the above copyright notice and this permission notice appear in   
all copies of this software and that you do not sell the software.   
  
THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,   
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY   
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.   

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ply.h"

char *type_names[] = {  /* names of scalar types */
"invalid",
"int8", "int16", "int32", "uint8", "uint16", "uint32", "float32", "float64",
};

char *old_type_names[] = {  /* old names of types for backward compatability */
"invalid",
"char", "short", "int", "uchar", "ushort", "uint", "float", "double",
};

int ply_type_size[] = {
  0, 1, 2, 4, 1, 2, 4, 4, 8
};

#define NO_OTHER_PROPS  -1

#define DONT_STORE_PROP  0
#define STORE_PROP       1

#define OTHER_PROP       0
#define NAMED_PROP       1

/* returns 1 if strings are equal, 0 if not */
int equal_strings(char *, char *);

/* find an element in a plyfile's list */
PlyElement *find_element(PlyFile *, char *);

/* find a property in an element's list */
PlyProperty *find_property(PlyElement *, char *, int *);

/* write to a file the word describing a PLY file data type */
void write_scalar_type (FILE *, int);

/* read a line from a file and break it up into separate words */
char **get_words(FILE *, int *, char **);

/* write an item to a file */
void write_binary_item(FILE *, int, unsigned int, double, int);
void write_ascii_item(FILE *, int, unsigned int, double, int);

/* add information to a PLY file descriptor */
void add_element(PlyFile *, char **, int);
void add_property(PlyFile *, char **, int);
void add_comment(PlyFile *, char *);
void add_obj_info(PlyFile *, char *);

/* copy a property */
void copy_property(PlyProperty *, PlyProperty *);

/* store a value into where a pointer and a type specify */
void store_item(char *, int, int, unsigned int, double);

/* return the value of a stored item */
void get_stored_item( void *, int, int *, unsigned int *, double *);

/* return the value stored in an item, given ptr to it and its type */
double get_item_value(char *, int);

/* get binary or ascii item and store it according to ptr and type */
void get_ascii_item(char *, int, int *, unsigned int *, double *);
void get_binary_item(FILE *, int, int *, unsigned int *, double *);

/* get a bunch of elements from a file */
void ascii_get_element(PlyFile *, char *);
void binary_get_element(PlyFile *, char *);

/* memory allocation */
static char *my_alloc(int, int, char *);


/*************/
/*  Writing  */
/*************/


/******************************************************************************
Given a file pointer, get ready to write PLY data to the file.

Entry:
  fp         - the given file pointer
  nelems     - number of elements in object
  elem_names - list of element names
  file_type  - file type, either ascii or binary

Exit:
  returns a pointer to a PlyFile, used to refer to this file, or NULL if error
******************************************************************************/

PlyFile *ply_write(
  FILE *fp,
  int nelems,
  char **elem_names,
  int file_type
)
{
  int i;
  PlyFile *plyfile;
  PlyElement *elem;

  /* check for NULL file pointer */
  if (fp == NULL)
    return (NULL);

  /* create a record for this object */

  plyfile = (PlyFile *) myalloc (sizeof (PlyFile));
  plyfile->file_type = file_type;
  plyfile->num_comments = 0;
  plyfile->num_obj_info = 0;
  plyfile->num_elem_types = nelems;
  plyfile->version = 1.0;
  plyfile->fp = fp;
  plyfile->other_elems = NULL;

  /* tuck aside the names of the elements */

  plyfile->elems = (PlyElement **) myalloc (sizeof (PlyElement *) * nelems);
  for (i = 0; i < nelems; i++) {
    elem = (PlyElement *) myalloc (sizeof (PlyElement));
    plyfile->elems[i] = elem;
    elem->name = strdup (elem_names[i]);
    elem->num = 0;
    elem->nprops = 0;
  }

  /* return pointer to the file descriptor */
  return (plyfile);
}


/******************************************************************************
Open a polygon file for writing.

Entry:
  filename   - name of file to read from
  nelems     - number of elements in object
  elem_names - list of element names
  file_type  - file type, either ascii or binary

Exit:
  returns a file identifier, used to refer to this file, or NULL if error
******************************************************************************/

PlyFile *open_for_writing_ply(
  char *filename,
  int nelems,
  char **elem_names,
  int file_type
)
{
  int i;
  PlyFile *plyfile;
  PlyElement *elem;
  char *name;
  FILE *fp;

  /* tack on the extension .ply, if necessary */

  name = (char *) myalloc (sizeof (char) * (strlen (filename) + 5));
  strcpy (name, filename);
  if (strlen (name) < 4 ||
      strcmp (name + strlen (name) - 4, ".ply") != 0)
      strcat (name, ".ply");

  /* open the file for writing */

  fp = fopen (name, "w");
  if (fp == NULL) {
    return (NULL);
  }

  /* create the actual PlyFile structure */

  plyfile = ply_write (fp, nelems, elem_names, file_type);
  if (plyfile == NULL)
    return (NULL);

  /* return pointer to the file descriptor */
  return (plyfile);
}


/******************************************************************************
Describe an element, including its properties and how many will be written
to the file.

Entry:
  plyfile   - file identifier
  elem_name - name of element that information is being specified about
  nelems    - number of elements of this type to be written
  nprops    - number of properties contained in the element
  prop_list - list of properties
******************************************************************************/

void element_layout_ply(
  PlyFile *plyfile,
  char *elem_name,
  int nelems,
  int nprops,
  PlyProperty *prop_list
)
{
  int i;
  PlyElement *elem;
  PlyProperty *prop;

  /* look for appropriate element */
  elem = find_element (plyfile, elem_name);
  if (elem == NULL) {
    fprintf(stderr,"element_layout_ply: can't find element '%s'\n",elem_name);
    exit (-1);
  }

  elem->num = nelems;

  /* copy the list of properties */

  elem->nprops = nprops;
  elem->props = (PlyProperty **) myalloc (sizeof (PlyProperty *) * nprops);
  elem->store_prop = (char *) myalloc (sizeof (char) * nprops);

  for (i = 0; i < nprops; i++) {
    prop = (PlyProperty *) myalloc (sizeof (PlyProperty));
    elem->props[i] = prop;
    elem->store_prop[i] = NAMED_PROP;
    copy_property (prop, &prop_list[i]);
  }
}


/******************************************************************************
Describe a property of an element.

Entry:
  plyfile   - file identifier
  elem_name - name of element that information is being specified about
  prop      - the new property
******************************************************************************/

void ply_describe_property(
  PlyFile *plyfile,
  char *elem_name,
  PlyProperty *prop
)
{
  PlyElement *elem;
  PlyProperty *elem_prop;

  /* look for appropriate element */
  elem = find_element (plyfile, elem_name);
  if (elem == NULL) {
    fprintf(stderr, "ply_describe_property: can't find element '%s'\n",
            elem_name);
    return;
  }

  /* create room for new property */

  if (elem->nprops == 0) {
    elem->props = (PlyProperty **) myalloc (sizeof (PlyProperty *));
    elem->store_prop = (char *) myalloc (sizeof (char));
    elem->nprops = 1;
  }
  else {
    elem->nprops++;
    elem->props = (PlyProperty **)
                  realloc (elem->props, sizeof (PlyProperty *) * elem->nprops);
    elem->store_prop = (char *)
                  realloc (elem->store_prop, sizeof (char) * elem->nprops);
  }

  /* copy the new property */

  elem_prop = (PlyProperty *) myalloc (sizeof (PlyProperty));
  elem->props[elem->nprops - 1] = elem_prop;
  elem->store_prop[elem->nprops - 1] = NAMED_PROP;
  copy_property (elem_prop, prop);
}


/******************************************************************************
State how many of a given element will be written.

Entry:
  plyfile   - file identifier
  elem_name - name of element that information is being specified about
  nelems    - number of elements of this type to be written
******************************************************************************/

void element_count_ply(
  PlyFile *plyfile,
  char *elem_name,
  int nelems
)
{
  int i;
  PlyElement *elem;
  PlyProperty *prop;

  /* look for appropriate element */
  elem = find_element (plyfile, elem_name);
  if (elem == NULL) {
    fprintf(stderr,"element_count_ply: can't find element '%s'\n",elem_name);
    exit (-1);
  }

  elem->num = nelems;
}


/******************************************************************************
Signal that we've described everything a PLY file's header and that the
header should be written to the file.

Entry:
  plyfile - file identifier
******************************************************************************/

void header_complete_ply(PlyFile *plyfile)
{
  int i,j;
  FILE *fp = plyfile->fp;
  PlyElement *elem;
  PlyProperty *prop;

  fprintf (fp, "ply\n");

  switch (plyfile->file_type) {
    case PLY_ASCII:
      fprintf (fp, "format ascii 1.0\n");
      break;
    case PLY_BINARY_BE:
      fprintf (fp, "format binary_big_endian 1.0\n");
      break;
    case PLY_BINARY_LE:
      fprintf (fp, "format binary_little_endian 1.0\n");
      break;
    default:
      fprintf (stderr, "ply_header_complete: bad file type = %d\n",
               plyfile->file_type);
      exit (-1);
  }

  /* write out the comments */

  for (i = 0; i < plyfile->num_comments; i++)
    fprintf (fp, "comment %s\n", plyfile->comments[i]);

  /* write out object information */

  for (i = 0; i < plyfile->num_obj_info; i++)
    fprintf (fp, "obj_info %s\n", plyfile->obj_info[i]);

  /* write out information about each element */

  for (i = 0; i < plyfile->num_elem_types; i++) {

    elem = plyfile->elems[i];
    fprintf (fp, "element %s %d\n", elem->name, elem->num);

    /* write out each property */
    for (j = 0; j < elem->nprops; j++) {
      prop = elem->props[j];
      if (prop->is_list == PLY_LIST) {
        fprintf (fp, "property list ");
        write_scalar_type (fp, prop->count_external);
        fprintf (fp, " ");
        write_scalar_type (fp, prop->external_type);
        fprintf (fp, " %s\n", prop->name);
      }
      else if (prop->is_list == PLY_STRING) {
        fprintf (fp, "property string");
        fprintf (fp, " %s\n", prop->name);
      }
      else {
        fprintf (fp, "property ");
        write_scalar_type (fp, prop->external_type);
        fprintf (fp, " %s\n", prop->name);
      }
    }
  }

  fprintf (fp, "end_header\n");
}


/******************************************************************************
Specify which elements are going to be written.  This should be called
before a call to the routine ply_put_element().

Entry:
  plyfile   - file identifier
  elem_name - name of element we're talking about
******************************************************************************/

void put_element_setup_ply(PlyFile *plyfile, char *elem_name)
{
  PlyElement *elem;

  elem = find_element (plyfile, elem_name);
  if (elem == NULL) {
    fprintf(stderr, "put_element_setup_ply: can't find element '%s'\n", elem_name);
    exit (-1);
  }

  plyfile->which_elem = elem;
}


/******************************************************************************
Write an element to the file.  This routine assumes that we're
writing the type of element specified in the last call to the routine
put_element_setup_ply().

Entry:
  plyfile  - file identifier
  elem_ptr - pointer to the element
******************************************************************************/

void put_element_ply(PlyFile *plyfile, void *elem_ptr)
{
  int i,j,k;
  FILE *fp = plyfile->fp;
  PlyElement *elem;
  PlyProperty *prop;
  char *item;
  char *elem_data;
  char **item_ptr;
  int list_count;
  int item_size;
  int int_val;
  unsigned int uint_val;
  double double_val;
  char **other_ptr;

  elem = plyfile->which_elem;
  elem_data = (char *) elem_ptr;
  other_ptr = (char **) (((char *) elem_ptr) + elem->other_offset);

  /* write out either to an ascii or binary file */

  if (plyfile->file_type == PLY_ASCII) {

    /* write an ascii file */

    /* write out each property of the element */
    for (j = 0; j < elem->nprops; j++) {

      prop = elem->props[j];

      if (elem->store_prop[j] == OTHER_PROP)
        elem_data = *other_ptr;
      else
        elem_data = (char *) elem_ptr;

      if (prop->is_list == PLY_LIST) {  /* list */
        item = elem_data + prop->count_offset;
        get_stored_item ((void *) item, prop->count_internal,
                         &int_val, &uint_val, &double_val);
        write_ascii_item (fp, int_val, uint_val, double_val,
                          prop->count_external);
        list_count = uint_val;
        item_ptr = (char **) (elem_data + prop->offset);
        item = item_ptr[0];
        item_size = ply_type_size[prop->internal_type];
        for (k = 0; k < list_count; k++) {
          get_stored_item ((void *) item, prop->internal_type,
                           &int_val, &uint_val, &double_val);
          write_ascii_item (fp, int_val, uint_val, double_val,
                            prop->external_type);
          item += item_size;
        }
      }
      else if (prop->is_list == PLY_STRING) {  /* string */
	char **str;
        item = elem_data + prop->offset;
	str = (char **) item;
	fprintf (fp, "\"%s\"", *str);
      }
      else {                                  /* scalar */
        item = elem_data + prop->offset;
        get_stored_item ((void *) item, prop->internal_type,
                         &int_val, &uint_val, &double_val);
        write_ascii_item (fp, int_val, uint_val, double_val,
                          prop->external_type);
      }
    }

    fprintf (fp, "\n");
  }
  else {

    /* write a binary file */

    /* write out each property of the element */
    for (j = 0; j < elem->nprops; j++) {
      prop = elem->props[j];
      if (elem->store_prop[j] == OTHER_PROP)
        elem_data = *other_ptr;
      else
        elem_data = (char *) elem_ptr;
      if (prop->is_list == PLY_LIST) {   /* list */
        item = elem_data + prop->count_offset;
        item_size = ply_type_size[prop->count_internal];
        get_stored_item ((void *) item, prop->count_internal,
                         &int_val, &uint_val, &double_val);
        write_binary_item (fp, int_val, uint_val, double_val,
                           prop->count_external);
        list_count = uint_val;
        item_ptr = (char **) (elem_data + prop->offset);
        item = item_ptr[0];
        item_size = ply_type_size[prop->internal_type];
        for (k = 0; k < list_count; k++) {
          get_stored_item ((void *) item, prop->internal_type,
                           &int_val, &uint_val, &double_val);
          write_binary_item (fp, int_val, uint_val, double_val,
                             prop->external_type);
          item += item_size;
        }
      }
      else if (prop->is_list == PLY_STRING) {   /* string */
	int len;
	char **str;
        item = elem_data + prop->offset;
	str = (char **) item;

	/* write the length */
	len = strlen(*str) + 1;
	fwrite (&len, sizeof(int), 1, fp);

	/* write the string, including the null character */
	fwrite (*str, len, 1, fp);
      }
      else {                   /* scalar */
        item = elem_data + prop->offset;
        item_size = ply_type_size[prop->internal_type];
        get_stored_item ((void *) item, prop->internal_type,
                         &int_val, &uint_val, &double_val);
        write_binary_item (fp, int_val, uint_val, double_val,
                           prop->external_type);
      }
    }

  }
}






/*************/
/*  Reading  */
/*************/



/******************************************************************************
Given a file pointer, get ready to read PLY data from the file.

Entry:
  fp - the given file pointer

Exit:
  nelems     - number of elements in object
  elem_names - list of element names
  returns a pointer to a PlyFile, used to refer to this file, or NULL if error
******************************************************************************/

PlyFile *ply_read(FILE *fp, int *nelems, char ***elem_names)
{
  int i,j;
  PlyFile *plyfile;
  int nwords;
  char **words;
  int found_format = 0;
  char **elist;
  PlyElement *elem;
  char *orig_line;

  /* check for NULL file pointer */
  if (fp == NULL)
    return (NULL);

  /* create record for this object */

  plyfile = (PlyFile *) myalloc (sizeof (PlyFile));
  plyfile->num_elem_types = 0;
  plyfile->comments = NULL;
  plyfile->num_comments = 0;
  plyfile->obj_info = NULL;
  plyfile->num_obj_info = 0;
  plyfile->fp = fp;
  plyfile->other_elems = NULL;
  plyfile->rule_list = NULL;

  /* read and parse the file's header */

  words = get_words (plyfile->fp, &nwords, &orig_line);
  if (!words || !equal_strings (words[0], "ply"))
    return (NULL);

  while (words) {

    /* parse words */

    if (equal_strings (words[0], "format")) {
      if (nwords != 3)
        return (NULL);
      if (equal_strings (words[1], "ascii"))
        plyfile->file_type = PLY_ASCII;
      else if (equal_strings (words[1], "binary_big_endian"))
        plyfile->file_type = PLY_BINARY_BE;
      else if (equal_strings (words[1], "binary_little_endian"))
        plyfile->file_type = PLY_BINARY_LE;
      else
        return (NULL);
      plyfile->version = atof (words[2]);
      found_format = 1;
    }
    else if (equal_strings (words[0], "element"))
      add_element (plyfile, words, nwords);
    else if (equal_strings (words[0], "property"))
      add_property (plyfile, words, nwords);
    else if (equal_strings (words[0], "comment"))
      add_comment (plyfile, orig_line);
    else if (equal_strings (words[0], "obj_info"))
      add_obj_info (plyfile, orig_line);
    else if (equal_strings (words[0], "end_header"))
      break;

    /* free up words space */
    free (words);

    words = get_words (plyfile->fp, &nwords, &orig_line);
  }

  /* create tags for each property of each element, to be used */
  /* later to say whether or not to store each property for the user */

  for (i = 0; i < plyfile->num_elem_types; i++) {
    elem = plyfile->elems[i];
    elem->store_prop = (char *) myalloc (sizeof (char) * elem->nprops);
    for (j = 0; j < elem->nprops; j++)
      elem->store_prop[j] = DONT_STORE_PROP;
    elem->other_offset = NO_OTHER_PROPS; /* no "other" props by default */
  }

  /* set return values about the elements */

  elist = (char **) myalloc (sizeof (char *) * plyfile->num_elem_types);
  for (i = 0; i < plyfile->num_elem_types; i++)
    elist[i] = strdup (plyfile->elems[i]->name);

  *elem_names = elist;
  *nelems = plyfile->num_elem_types;

  /* return a pointer to the file's information */

  return (plyfile);
}


/******************************************************************************
Open a polygon file for reading.

Entry:
  filename - name of file to read from

Exit:
  nelems     - number of elements in object
  elem_names - list of element names
  file_type  - file type, either ascii or binary
  version    - version number of PLY file
  returns a file identifier, used to refer to this file, or NULL if error
******************************************************************************/

PlyFile *ply_open_for_reading(
  char *filename,
  int *nelems,
  char ***elem_names,
  int *file_type,
  float *version
)
{
  FILE *fp;
  PlyFile *plyfile;
  char *name;

  /* tack on the extension .ply, if necessary */

  name = (char *) myalloc (sizeof (char) * (strlen (filename) + 5));
  strcpy (name, filename);
  if (strlen (name) < 4 ||
      strcmp (name + strlen (name) - 4, ".ply") != 0)
      strcat (name, ".ply");

  /* open the file for reading */

  fp = fopen (name, "r");
  if (fp == NULL)
    return (NULL);

  /* create the PlyFile data structure */

  plyfile = ply_read (fp, nelems, elem_names);

  /* determine the file type and version */

  *file_type = plyfile->file_type;
  *version = plyfile->version;

  /* return a pointer to the file's information */

  return (plyfile);
}


/******************************************************************************
Get information about a particular element.

Entry:
  plyfile   - file identifier
  elem_name - name of element to get information about

Exit:
  nelems   - number of elements of this type in the file
  nprops   - number of properties
  returns a list of properties, or NULL if the file doesn't contain that elem
******************************************************************************/

PlyProperty **get_element_description_ply(
  PlyFile *plyfile,
  char *elem_name,
  int *nelems,
  int *nprops
)
{
  int i;
  PlyElement *elem;
  PlyProperty *prop;
  PlyProperty **prop_list;

  /* find information about the element */
  elem = find_element (plyfile, elem_name);
  if (elem == NULL)
    return (NULL);

  *nelems = elem->num;
  *nprops = elem->nprops;

  /* make a copy of the element's property list */
  prop_list = (PlyProperty **) myalloc (sizeof (PlyProperty *) * elem->nprops);
  for (i = 0; i < elem->nprops; i++) {
    prop = (PlyProperty *) myalloc (sizeof (PlyProperty));
    copy_property (prop, elem->props[i]);
    prop_list[i] = prop;
  }

  /* return this duplicate property list */
  return (prop_list);
}


/******************************************************************************
Specify which properties of an element are to be returned.  This should be
called before a call to the routine get_element_ply().

Entry:
  plyfile   - file identifier
  elem_name - which element we're talking about
  nprops    - number of properties
  prop_list - list of properties
******************************************************************************/

void get_element_setup_ply(
  PlyFile *plyfile,
  char *elem_name,
  int nprops,
  PlyProperty *prop_list
)
{
  int i;
  PlyElement *elem;
  PlyProperty *prop;
  int index;

  /* find information about the element */
  elem = find_element (plyfile, elem_name);
  plyfile->which_elem = elem;

  /* deposit the property information into the element's description */
  for (i = 0; i < nprops; i++) {

    /* look for actual property */
    prop = find_property (elem, prop_list[i].name, &index);
    if (prop == NULL) {
      fprintf (stderr, "Warning:  Can't find property '%s' in element '%s'\n",
               prop_list[i].name, elem_name);
      continue;
    }

    /* store its description */
    prop->internal_type = prop_list[i].internal_type;
    prop->offset = prop_list[i].offset;
    prop->count_internal = prop_list[i].count_internal;
    prop->count_offset = prop_list[i].count_offset;

    /* specify that the user wants this property */
    elem->store_prop[index] = STORE_PROP;
  }
}


/******************************************************************************
Specify a property of an element that is to be returned.  This should be
called (usually multiple times) before a call to the routine ply_get_element().
This routine should be used in preference to the less flexible old routine
called ply_get_element_setup().

Entry:
  plyfile   - file identifier
  elem_name - which element we're talking about
  prop      - property to add to those that will be returned
******************************************************************************/

void ply_get_property(
  PlyFile *plyfile,
  char *elem_name,
  PlyProperty *prop
)
{
  PlyElement *elem;
  PlyProperty *prop_ptr;
  int index;

  /* find information about the element */
  elem = find_element (plyfile, elem_name);
  plyfile->which_elem = elem;

  /* deposit the property information into the element's description */

  prop_ptr = find_property (elem, prop->name, &index);
  if (prop_ptr == NULL) {
    fprintf (stderr, "Warning:  Can't find property '%s' in element '%s'\n",
             prop->name, elem_name);
    return;
  }
  prop_ptr->internal_type  = prop->internal_type;
  prop_ptr->offset         = prop->offset;
  prop_ptr->count_internal = prop->count_internal;
  prop_ptr->count_offset   = prop->count_offset;

  /* specify that the user wants this property */
  elem->store_prop[index] = STORE_PROP;
}


/******************************************************************************
Read one element from the file.  This routine assumes that we're reading
the type of element specified in the last call to the routine
ply_get_element_setup().

Entry:
  plyfile  - file identifier
  elem_ptr - pointer to location where the element information should be put
******************************************************************************/

void ply_get_element(PlyFile *plyfile, void *elem_ptr)
{
  if (plyfile->file_type == PLY_ASCII)
    ascii_get_element (plyfile, (char *) elem_ptr);
  else
    binary_get_element (plyfile, (char *) elem_ptr);
}


/******************************************************************************
Extract the comments from the header information of a PLY file.

Entry:
  plyfile - file identifier

Exit:
  num_comments - number of comments returned
  returns a pointer to a list of comments
******************************************************************************/

char **get_comments_ply(PlyFile *plyfile, int *num_comments)
{
  *num_comments = plyfile->num_comments;
  return (plyfile->comments);
}


/******************************************************************************
Extract the object information (arbitrary text) from the header information
of a PLY file.

Entry:
  plyfile - file identifier

Exit:
  num_obj_info - number of lines of text information returned
  returns a pointer to a list of object info lines
******************************************************************************/

char **get_obj_info_ply(PlyFile *plyfile, int *num_obj_info)
{
  *num_obj_info = plyfile->num_obj_info;
  return (plyfile->obj_info);
}


/******************************************************************************
Make ready for "other" properties of an element-- those properties that
the user has not explicitly asked for, but that are to be stashed away
in a special structure to be carried along with the element's other
information.

Entry:
  plyfile - file identifier
  elem    - element for which we want to save away other properties
******************************************************************************/

void setup_other_props(PlyFile *plyfile, PlyElement *elem)
{
  int i;
  PlyProperty *prop;
  int size = 0;
  int type_size;

  /* Examine each property in decreasing order of size. */
  /* We do this so that all data types will be aligned by */
  /* word, half-word, or whatever within the structure. */

  for (type_size = 8; type_size > 0; type_size /= 2) {

    /* add up the space taken by each property, and save this information */
    /* away in the property descriptor */

    for (i = 0; i < elem->nprops; i++) {

      /* don't bother with properties we've been asked to store explicitly */
      if (elem->store_prop[i])
        continue;

      prop = elem->props[i];

      /* internal types will be same as external */
      prop->internal_type = prop->external_type;
      prop->count_internal = prop->count_external;

      /* list case */
      if (prop->is_list == PLY_LIST) {

        /* pointer to list */
        if (type_size == sizeof (void *)) {
          prop->offset = size;
          size += sizeof (void *);    /* always use size of a pointer here */
        }

        /* count of number of list elements */
        if (type_size == ply_type_size[prop->count_external]) {
          prop->count_offset = size;
          size += ply_type_size[prop->count_external];
        }
      }
      /* string */
      else if (prop->is_list == PLY_STRING) {
        /* pointer to string */
        if (type_size == sizeof (char *)) {
          prop->offset = size;
          size += sizeof (char *);
        }
      }
      /* scalar */
      else if (type_size == ply_type_size[prop->external_type]) {
        prop->offset = size;
        size += ply_type_size[prop->external_type];
      }
    }

  }

  /* save the size for the other_props structure */
  elem->other_size = size;
}


/******************************************************************************
Specify that we want the "other" properties of an element to be tucked
away within the user's structure.

Entry:
  plyfile - file identifier
  elem    - the element that we want to store other_props in
  offset  - offset to where other_props will be stored inside user's structure

Exit:
  returns pointer to structure containing description of other_props
******************************************************************************/

static PlyOtherProp *get_other_properties(
  PlyFile *plyfile,
  PlyElement *elem,
  int offset
)
{
  int i;
  PlyOtherProp *other;
  PlyProperty *prop;
  int nprops;

  /* remember that this is the "current" element */
  plyfile->which_elem = elem;

  /* save the offset to where to store the other_props */
  elem->other_offset = offset;

  /* place the appropriate pointers, etc. in the element's property list */
  setup_other_props (plyfile, elem);

  /* create structure for describing other_props */
  other = (PlyOtherProp *) myalloc (sizeof (PlyOtherProp));
  other->name = strdup (elem->name);
#if 0
  if (elem->other_offset == NO_OTHER_PROPS) {
    other->size = 0;
    other->props = NULL;
    other->nprops = 0;
    return (other);
  }
#endif
  other->size = elem->other_size;
  other->props = (PlyProperty **) myalloc (sizeof(PlyProperty) * elem->nprops);
  
  /* save descriptions of each "other" property */
  nprops = 0;
  for (i = 0; i < elem->nprops; i++) {
    if (elem->store_prop[i])
      continue;
    prop = (PlyProperty *) myalloc (sizeof (PlyProperty));
    copy_property (prop, elem->props[i]);
    other->props[nprops] = prop;
    nprops++;
  }
  other->nprops = nprops;

  /* set other_offset pointer appropriately if there are NO other properties */
  if (other->nprops == 0) {
    elem->other_offset = NO_OTHER_PROPS;
  }
 
  /* return structure */
  return (other);
}


/******************************************************************************
Specify that we want the "other" properties of an element to be tucked
away within the user's structure.  The user needn't be concerned for how
these properties are stored.

Entry:
  plyfile   - file identifier
  elem_name - name of element that we want to store other_props in
  offset    - offset to where other_props will be stored inside user's structure

Exit:
  returns pointer to structure containing description of other_props
******************************************************************************/

PlyOtherProp *ply_get_other_properties(
  PlyFile *plyfile,
  char *elem_name,
  int offset
)
{
  PlyElement *elem;
  PlyOtherProp *other;

  /* find information about the element */
  elem = find_element (plyfile, elem_name);
  if (elem == NULL) {
    fprintf (stderr, "ply_get_other_properties: Can't find element '%s'\n",
             elem_name);
    return (NULL);
  }

  other = get_other_properties (plyfile, elem, offset);
  return (other);
}




/*************************/
/*  Other Element Stuff  */
/*************************/





/******************************************************************************
Grab all the data for the current element that a user does not want to
explicitly read in.  Stores this in the PLY object's data structure.

Entry:
  plyfile - pointer to file

Exit:
  returns pointer to ALL the "other" element data for this PLY file
******************************************************************************/

PlyOtherElems *g