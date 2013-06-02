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

#ifndef OUTPUT_BUFFER_H
#define OUTPUT_BUFFER_H

#include "mpd_types.h"
#include "decode.h"
#include "audio.h"
#include "inputStream.h"
#include "metadataChunk.h"
#include "replayGain.h"

#define OUTPUT_BUFFER_DC_STOP   -1
#define OUTPUT_BUFFER_DC_SEEK   -2

#define BUFFERED_METACHUNKS	25

typedef struct _OutputBuffer {
	MetadataChunk metadataChunks[BUFFERED_METACHUNKS];
	mpd_sint8 metaChunkSet[BUFFERED_METACHUNKS];
        AudioFormat audioFormat;
	char * volatile chunks;
	unsigned * volatile times;
	mpd_uint16 * volatile chunkSize;
	mpd_uint16 * volatile bitRate;
	mpd_sint16 volatile begin;
	mpd_sint16 volatile end;
	mpd_sint8 * volatile metaChunk;
	volatile mpd_sint8 acceptMetadata;
} OutputBuffer;

void clearOutputBuffer(OutputBuffer * cb);

void flushOutputBuffer(OutputBuffer * cb);

/* we send inStream for buffering the inputStream while waiting to
   send the next chunk */
int sendDataToOutputBuffer(
		OutputBuffer * cb, 
		InputStream * inStream,
                DecoderControl * dc, 
		int seekable, 
		char * data, 
		long datalen, 
                unsigned time, 
		mpd_uint16 bitRate, 
		ReplayGainInfo * replayGainInfo);

int copyMpdTagToOutputBuffer(OutputBuffer * cb, MpdTag * tag);

void clearAllMetaChunkSets(OutputBuffer * cb);

#endif
