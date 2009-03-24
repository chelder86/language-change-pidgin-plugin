/*
LANGUAGE_CHANGE
This plugin change the default language in Linux.

Developers:
The working of the plugin is simple. It get the posibles languages from /usr/lib/locale/. It allows to users select the languages they want.

Known errors:
A plugin works in execution time. Then, the plugin init in default language. Inmediately it get the select language and turn to it.

I would like if you teach me how to program it with signals. I would like it changed the default language without reboot pidin.

I thanks if you correct something what it was wrong. My english is poor. Because of that, you can correct me too.

*/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* config.h may define PURPLE_PLUGINS; protect the definition here so that we
 * don't get complaints about redefinition when it's not necessary. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#include <notify.h>
#include <plugin.h>

#include "version.h"
#include "internal.h"
#include "plugin.h"
#include "pluginpref.h"
#include "prefs.h"

#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

struct strings_list
{
  char **string_pointers;
  unsigned int number_of_strings;
} file_names;

void get_file_names(const char *directory, struct strings_list *file_names);

static PurplePluginPrefFrame *
get_plugin_pref_frame(PurplePlugin *plugin) {

	struct strings_list file_names;
	int i = 0;		
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;
	frame = purple_plugin_pref_frame_new();
	ppref = purple_plugin_pref_new_with_name_and_label(
							"/plugins/core/language_change/language",
							"Select language");
	purple_plugin_pref_set_type(ppref, PURPLE_PLUGIN_PREF_CHOICE);

	get_file_names("/usr/lib/locale/", &file_names);

while(i <  file_names.number_of_strings - 5)
{
	purple_plugin_pref_add_choice(ppref, file_names.string_pointers[i], file_names.string_pointers[i]);	
	i++;
}
	purple_plugin_pref_frame_add(frame, ppref);
	free(file_names.string_pointers);
	return frame;
}

void get_file_names(const char *directory, struct strings_list *file_names)
{
	struct dirent *d;
	struct stat buf;
	file_names->string_pointers = NULL;
	file_names->number_of_strings = 0;	
	DIR *dir;
	char file_path[32];
	if ((dir=opendir(directory)) == NULL)
	{
		/* error */
	}
	 
	while ((d=readdir(dir)) != NULL)
	{	
		/* The above line avoid infinite loop */
		if (strcmp(d->d_name,".")!=0 && strcmp(d->d_name, "..")!=0 && strlen(d->d_name) <  11)
		{		
		file_names->string_pointers = (char **) realloc( file_names->string_pointers, ++file_names->number_of_strings*sizeof(char *) );
		file_names->string_pointers[file_names->number_of_strings-1] = (char *) malloc( 11*sizeof(char) );
		
		strncpy( file_names->string_pointers[file_names->number_of_strings-1], d-> d_name, 11 );

			
			sprintf(file_path,"%s/%s",directory,d->d_name);
			if (stat(file_path, &buf) == -1)
			{
				/* error */
			}
			
		}
	}
	closedir(dir);	
}


static gboolean
plugin_load (PurplePlugin * plugin)
{
	const char *lang = purple_prefs_get_string("/plugins/core/language_change/language");
	
	
	char *lang_set = setlocale(LC_MESSAGES, lang);
	
			
		if (lang_set == NULL) /* Error message: */
		{
			char error_message[64];
			sprintf(error_message,"An error has ocurred while setting language to: %s", lang);
			purple_notify_message (plugin, PURPLE_NOTIFY_MSG_INFO, "Plugin error",
			error_message, NULL, NULL,
			NULL);	
		}
		
	return TRUE;
}

static PurplePluginUiInfo prefs_info = {
	get_plugin_pref_frame,
	0,   /* page_num (Reserved) */
	NULL, /* frame (Reserved) */
	/* Padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	"core-language_change",
	"Language change",
	"0.1",
	"Language change plugin",
	"It changes the default language of pidgin",
	"Carlos-Helder García Escobar", 
	NULL, /* web page */
	plugin_load,
	NULL,
	NULL,
	NULL,
	NULL,
	&prefs_info,
	NULL,		
	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin (PurplePlugin * plugin)
{
	purple_prefs_add_none("/plugins/core/language_change");
	purple_prefs_add_string("/plugins/core/language_change/language", "default");
}

PURPLE_INIT_PLUGIN (language_change, init_plugin, info)
