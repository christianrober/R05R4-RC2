/* the Music Player Daemon (MPD)
 * (c)2003-2004 by Warren Dukes (shank@mercury.chem.pitt.edu)
 * This project's homepage is: http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INPUT_PLUGIN_H
#define INPUT_PLUGIN_H

#include "../config.h"
#include "inputStream.h"
#include "decode.h"
#include "outputBuffer.h"
#include "tag.h"

#define INPUT_PLUGIN_STREAM_FILE	0x01
#define INPUT_PLUGIN_STREAM_URL		0x02

typedef int (* InputPlugin_initFunc) ();

typedef void (* InputPlugin_finishFunc) ();

typedef int (* InputPlugin_streamDecodeFunc) (OutputBuffer *, DecoderControl *,
		InputStream *);

typedef int (* InputPlugin_fileDecodeFunc) (OutputBuffer *, DecoderControl *,
		char * path);

/* file should be the full path! */
typedef MpdTag * (* InputPlugin_tagDupFunc) (char * file);

typedef struct _InputPlugin {
	char * name;
	InputPlugin_initFunc initFunc;
	InputPlugin_finishFunc finishFunc;
	InputPlugin_streamDecodeFunc streamDecodeFunc;
	InputPlugin_fileDecodeFunc fileDecodeFunc;
	InputPlugin_tagDupFunc tagDupFunc;
	unsigned char streamTypes;
	char ** suffixes;
	char ** mimeTypes;
} InputPlugin;

/* individual functions to load/unload plugins */
void loadInputPlugin(InputPlugin * inputPlugin);
void unloadInputPlugin(InputPlugin * inputPlugin);

/* interface for using plugins */

InputPlugin * getInputPluginFromSuffix(char * suffix);

InputPlugin * getInputPluginFromMimeType(char * mimeType);

InputPlugin * getInputPluginFromName(char * name);

void printAllInputPluginSuffixes(FILE * fp);

/* this is where we "load" all the "plugins" ;-) */
void initInputPlugins();

/* this is where we "unload" all the "plugins" */
void finishInputPlugins();

#endif
