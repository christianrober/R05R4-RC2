/*
 *
 *  config.c
 *
 *       Copyright (C) Stan Seibert - July 2000
 *
 *  This file is part of libao, a cross-platform audio output library.  See
 *  README for a history of this source code.
 *
 *  libao is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  libao is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "ao_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void read_config_files (ao_config *config)
{
	char userfile[FILENAME_MAX+1];
	char *homedir = getenv("HOME");

	/* Read the system-wide config file */
	read_config_file(config, AO_SYSTEM_CONFIG);
	
	/* Read the user config file */
	if ( homedir!=NULL && 
	     strlen(homedir) <= FILENAME_MAX - strlen(AO_USER_CONFIG) )
	{
		strncpy(userfile, homedir, FILENAME_MAX);
		strcat(userfile, AO_USER_CONFIG);
		read_config_file(config, userfile);
	}
}

#define LINE_LEN 100

int read_config_file(ao_config *config, const char *config_file)
{
	FILE *fp;
	char line[LINE_LEN];
	int end;
	
	
	if ( !(fp = fopen(config_file, "r")) )
		return 0; /* Can't open file */
	
	while (fgets(line, LINE_LEN, fp)) {
		/* All options are key=value */
		
		if (strncmp(line, "default_driver=", 15) == 0) {
			free(config->default_driver);
			end = strlen(line);
			if (line[end-1] == '\n')
				line[end-1] = 0; /* Remove trailing newline */

			config->default_driver = strdup(line+15);
		}
	}

	fclose(fp);

	return 1;
}
