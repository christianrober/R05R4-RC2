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

#include "decode.h"

#include "file_buffer.h"
#include "player.h"
#include "playerData.h"
#include "utils.h"
#include "pcm_utils.h"
#include "audio.h"
#include "path.h"
#include "log.h"
#include "sig_handlers.h"
#include "ls.h"
#include "utf8.h"
#include "mpm.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void decodeSigHandler(int sig) {
	if (sig == SIGCHLD) {
		int status;
		if (mpm_get_id(MPM_DECODE) && mpm_get_id(MPM_DECODE)
	    			== wait3(&status, WNOHANG, NULL)) {
			if (WIFSIGNALED(status) && WTERMSIG(status) != SIGTERM
					&& WTERMSIG(status) != SIGINT) {
				ERROR("decode process died from signal: %i\n",
				      WTERMSIG(status));
			}
			mpm_set_id(MPM_DECODE,0);
			mpm_free(MPM_DECODE);
		}
	} else if (sig == SIGTERM) {
		if (mpm_get_id(MPM_DECODE) > 0)
			kill(mpm_get_id(MPM_DECODE), SIGTERM);
		mpm_free(MPM_DECODE);
		exit(EXIT_SUCCESS);
	}
}

void stopDecode(DecoderControl * dc) {
	if(mpm_get_id(MPM_DECODE)>0 && 
			(dc->start || dc->state!=DECODE_STATE_STOP)) 
	{
		dc->stop = 1;
		while(mpm_get_id(MPM_DECODE)>0 && dc->stop)
		{
//			fprintf(stderr,"In stop Decode func sleep:\r\n");
		//	 my_usleep(10000);
		}
		clear_cur_stream();
		buffer_reset(0);
	}
}

void quitDecode(PlayerControl * pc, DecoderControl * dc) {
	stopDecode(dc);
        pc->metadataState = PLAYER_METADATA_STATE_READ;
	pc->state = PLAYER_STATE_STOP;
        dc->seek = 0;
	pc->play = 0;
	pc->stop = 0;
	pc->pause = 0;
	kill(getppid(),SIGUSR1);
}

int calculateCrossFadeChunks(PlayerControl * pc, AudioFormat * af) {
	long chunks;

	if(pc->crossFade<=0) return 0;

	chunks = (af->sampleRate*af->bits*af->channels/8.0/CHUNK_SIZE);
	chunks = (chunks*pc->crossFade+0.5);

	if(chunks>(buffered_chunks-buffered_before_play)) {
		chunks = buffered_chunks-buffered_before_play;
	}

	if(chunks<0) chunks = 0;

	return (int)chunks;
}

#define playSilenceOrSleep() \
	if(isAudioDeviceOpen()) { \
		playAudio(silence, CHUNK_SIZE); \
	} \
	else my_usleep(10000);

#define handleDecodeStart() \
        if(decodeWaitedOn) { \
                if(dc->state!=DECODE_STATE_START && mpm_get_id(MPM_DECODE) > 0 && \
                                dc->error==DECODE_ERROR_NOERROR) \
                { \
                        decodeWaitedOn = 0; \
	                if(openAudioDevice(&(cb->audioFormat))<0) { \
		                strncpy(pc->erroredUrl, pc->utf8url, \
                                                MAXPATHLEN); \
		                pc->erroredUrl[MAXPATHLEN] = '\0'; \
		                pc->error = PLAYER_ERROR_AUDIO; \
				ERROR("problems opening audio device while playing \"%s\"\n", pc->utf8url); \
		                quitDecode(pc,dc); \
		                return; \
	                } \
	                pc->totalTime = dc->totalTime; \
	                pc->sampleRate = dc->audioFormat.sampleRate; \
	                pc->bits = dc->audioFormat.bits; \
	                pc->channels = dc->audioFormat.channels; \
                } \
                else if(dc->state!=DECODE_STATE_START || mpm_get_id(MPM_DECODE) <= 0) { \
		        strncpy(pc->erroredUrl, pc->utf8url, MAXPATHLEN); \
		        pc->erroredUrl[MAXPATHLEN] = '\0'; \
		        pc->error = PLAYER_ERROR_FILE; \
		        quitDecode(pc,dc); \
		        return; \
                } \
                else { \
			my_usleep(10000); \
                        continue; \
                } \
        }

int waitOnDecode(PlayerControl * pc, DecoderControl * dc, OutputBuffer * cb,
                int * decodeWaitedOn) 
{
        strncpy(pc->currentUrl, pc->utf8url, MAXPATHLEN);
        pc->currentUrl[MAXPATHLEN] = '\0';

	while(mpm_get_id(MPM_DECODE)>0 && dc->start) 
	{
//		fprintf(stderr,"In decode.c waitOnDecode func my_usleep is there:\r\n");
//	my_usleep(10000);
	}
	if(dc->start || dc->error!=DECODE_ERROR_NOERROR) {
		strncpy(pc->erroredUrl, pc->utf8url, MAXPATHLEN);
		pc->erroredUrl[MAXPATHLEN] = '\0';
		pc->error = PLAYER_ERROR_FILE;
		quitDecode(pc,dc);
		return -1;
	}

        pc->totalTime = pc->fileTime;
        pc->bitRate = 0;
        pc->sampleRate = 0;
        pc->bits = 0;
        pc->channels = 0;
        *decodeWaitedOn = 1;

	return 0;
}

int decodeSeek(PlayerControl * pc, DecoderControl * dc, OutputBuffer * cb,
                int * decodeWaitedOn, int * next) 
{
        int ret = -1;

	if(mpm_get_id(MPM_DECODE)>0) {
		if(dc->state==DECODE_STATE_STOP || dc->error || 
				strcmp(dc->utf8url, pc->utf8url)!=0) 
		{
			stopDecode(dc);
			*next = -1;
			cb->begin = 0;
			cb->end = 0;
			dc->error = 0;
			dc->start = 1;
			waitOnDecode(pc,dc,cb,decodeWaitedOn);
		}
		if(mpm_get_id(MPM_DECODE)>0 && dc->state!=DECODE_STATE_STOP && 
                                dc->seekable) 
                {
			*next = -1;
			dc->seekWhere = pc->seekWhere;
                        dc->seekError = 0;
			dc->seek = 1;
                        while(mpm_get_id(MPM_DECODE)>0 && dc->seek)
						{
						//	fprintf(stderr,"In decodeSeek Function sleep: \r\n");
			//				 my_usleep(10000);
						}
                        if(!dc->seekError) {
                                pc->elapsedTime = dc->seekWhere;
                                ret = 0;
                        }
		}
	}
	pc->seek = 0;

        return ret;
}

#define processDecodeInput() \
        if(pc->cycleLogFiles) { \
                myfprintfCloseAndOpenLogFile(); \
                pc->cycleLogFiles = 0; \
        } \
	if(pc->lockQueue) { \
		pc->queueLockState = PLAYER_QUEUE_LOCKED; \
		pc->lockQueue = 0; \
	} \
	if(pc->unlockQueue) { \
		pc->queueLockState = PLAYER_QUEUE_UNLOCKED; \
		pc->unlockQueue = 0; \
	} \
	if(pc->pause) { \
		pause = !pause; \
		if(pause) pc->state = PLAYER_STATE_PAUSE; \
		else { \
			if(openAudioDevice(NULL)<0) { \
				strncpy(pc->erroredUrl, pc->utf8url, \
                                                MAXPATHLEN); \
				pc->erroredUrl[MAXPATHLEN] = '\0'; \
				pc->error = PLAYER_ERROR_AUDIO; \
				ERROR("problems opening audio device while playing \"%s\"\n", pc->utf8url); \
				quitDecode(pc,dc); \
				return; \
			} \
			pc->state = PLAYER_STATE_PLAY; \
		} \
		pc->pause = 0; \
		kill(getppid(),SIGUSR1); \
		if(pause) closeAudioDevice(); \
	} \
	if(pc->seek) { \
		if(decodeSeek(pc,dc,cb,&decodeWaitedOn,&next) == 0) { \
		        doCrossFade = 0; \
		        nextChunk =  -1; \
                        bbp = 0; \
                } \
	} \
	if(pc->stop) { \
		quitDecode(pc,dc); \
		return; \
	}

void decodeStart(PlayerControl * pc, OutputBuffer * cb, DecoderControl * dc) {
        int ret;
        InputStream inStream;
        InputPlugin * plugin = NULL;
        char * path;

        if(isRemoteUrl(pc->utf8url)) {
                path = utf8StrToLatin1Dup(pc->utf8url);
        }
	else path = strdup(rmp2amp(utf8ToFsCharset(pc->utf8url)));

	if(!path) {
		dc->error = DECODE_ERROR_FILE;
		dc->state = DECODE_STATE_STOP;
		dc->start = 0;
                return;
	}

	copyMpdTagToOutputBuffer(cb, NULL);

        strncpy(dc->utf8url, pc->utf8url, MAXPATHLEN);
	dc->utf8url[MAXPATHLEN] = '\0';

        if(openInputStream(&inStream, path) < 0) {
		dc->error = DECODE_ERROR_FILE;
		dc->state = DECODE_STATE_STOP;
		dc->start = 0;
		free(path);
                return;
        }

        dc->seekable = inStream.seekable;
        dc->state = DECODE_STATE_START;
	dc->start = 0;

        while(!inputStreamAtEOF(&inStream) && bufferInputStream(&inStream) < 0
                        && !dc->stop)
	{
		/* sleep so we don't consume 100% of the cpu */
//		fprintf(stderr,"In decode.c decodeParent decode start func\r\n");
	//	my_usleep(1000);
	}

        if(dc->stop) {
                dc->state = DECODE_STATE_STOP;
                dc->stop = 0;
		free(path);
                return;
        }

        /*if(inStream.metaName) {
		MpdTag * tag = newMpdTag();
		tag->name = strdup(inStream.metaName);
		copyMpdTagToOutputBuffer(cb, tag);
		freeMpdTag(tag);
        }*/

	/* reset Metadata in OutputBuffer */

        ret = DECODE_ERROR_UNKTYPE;
	if(isRemoteUrl(dc->utf8url)) {
		cb->acceptMetadata = 1;
		plugin = getInputPluginFromMimeType(inStream.mime);
                if(plugin == NULL) {
                        plugin = getInputPluginFromSuffix(
                                        getSuffix(dc->utf8url));
                }
                /* this is needed for bastard streams that don't have a suffix
                                or set the mimeType */
                if(plugin == NULL) {
                        plugin = getInputPluginFromName("mp3");
                }
                if(plugin && (plugin->streamTypes & INPUT_PLUGIN_STREAM_URL) &&
                                plugin->streamDecodeFunc) 
                {
                        ret = plugin->streamDecodeFunc(cb, dc, &inStream);
                }
	}
        else {
		cb->acceptMetadata = 0;
                plugin = getInputPluginFromSuffix(getSuffix(dc->utf8url));
                if(plugin && (plugin->streamTypes && INPUT_PLUGIN_STREAM_FILE))
                {
                        if(plugin->streamDecodeFunc) {
                                ret = plugin->streamDecodeFunc(cb, dc, 
                                                &inStream);
                        }
                        else if(plugin->fileDecodeFunc) {
                                closeInputStream(&inStream);
                                ret = plugin->fileDecodeFunc(cb, dc, path);
                        }
                }
        }

	if(ret<0 || ret == DECODE_ERROR_UNKTYPE) {
		strncpy(pc->erroredUrl, dc->utf8url, MAXPATHLEN);
		pc->erroredUrl[MAXPATHLEN] = '\0';
		if(ret != DECODE_ERROR_UNKTYPE) dc->error = DECODE_ERROR_FILE;
                else {
                        dc->error = DECODE_ERROR_UNKTYPE;
                        closeInputStream(&inStream);
                }
		dc->stop = 0;
		dc->state = DECODE_STATE_STOP;
	}

	free(path);
}

int decode_task() {
	OutputBuffer * cb = &(getPlayerData()->buffer);
	PlayerControl * pc = &(getPlayerData()->playerControl);
	DecoderControl * dc = &(getPlayerData()->decoderControl);
	
	unblockSignals();
	
	mpm_enter(MPM_DECODE);	
	
			fprintf(stderr,"0 In the decode_task:\r\n");
	/* CHILD TASK */
	while(1) {
		if(dc->cycleLogFiles) {
			myfprintfCloseAndOpenLogFile();
			dc->cycleLogFiles = 0;
		}
		else if(dc->start || dc->seek)
				{
					fprintf(stderr,"CAlling decodestart func :\r\n");
					 decodeStart(pc, cb, dc);
				}
		else if(dc->stop) {
			dc->state = DECODE_STATE_STOP;
			dc->stop = 0;
		}
		else{
			 my_usleep(10000);
	//		fprintf(stderr,"in the decode_task sleep:\r\n");
			}
	}
	return EXIT_SUCCESS;
	/* END OF CHILD TASK */
}

int decoderInit(PlayerControl * pc, OutputBuffer * cb, DecoderControl * dc) {
	int rc;
	blockSignals();
	rc = mpm_spawn(MPM_DECODE,NULL);
	unblockSignals();
	
	if (rc<=0) {
		ERROR("return code from mpm_spawn(): %d\n",rc);
		strncpy(pc->erroredUrl, pc->utf8url, MAXPATHLEN);
		pc->erroredUrl[MAXPATHLEN] = '\0';
		pc->error = PLAYER_ERROR_SYSTEM;
		return -1;
	}

	return 0;
}

void handleMetadata(OutputBuffer * cb, PlayerControl * pc, int * previous,
		int * currentChunkSent, MetadataChunk * currentChunk) 
{
	if(cb->begin!=cb->end) {
		int meta = cb->metaChunk[cb->begin];
		if( meta != *previous ) {
			DEBUG("player: metadata change\n");
			if( meta >= 0 && cb->metaChunkSet[meta]) {
				DEBUG("player: new metadata from decoder!\n");
				memcpy(currentChunk, 
					cb->metadataChunks+meta,
					sizeof(MetadataChunk));
				*currentChunkSent = 0;
				cb->metaChunkSet[meta] = 0;
			}
		}
		*previous = meta;
	}
	if(!(*currentChunkSent) && pc->metadataState == 
			PLAYER_METADATA_STATE_WRITE)
	{
		*currentChunkSent = 1;
		memcpy(&(pc->metadataChunk), currentChunk, 
				sizeof(MetadataChunk));
		pc->metadataState = PLAYER_METADATA_STATE_READ;
		kill(getppid(), SIGUSR1);
	}
}

void advanceOutputBufferTo(OutputBuffer * cb, PlayerControl * pc, 
	int * previous, int * currentChunkSent, MetadataChunk * currentChunk,
	int to) 
{
	while(cb->begin!=to) {
		handleMetadata(cb, pc, previous, currentChunkSent, 
				currentChunk);
		cb->begin++;
		if(cb->begin>=buffered_chunks) {
			cb->begin = 0;
		}
	}
}

void decodeParent(PlayerControl * pc, DecoderControl * dc, OutputBuffer * cb) {
	int pause = 0;
	int quit = 0;
	int bbp = buffered_before_play;
	int doCrossFade = 0;
	int crossFadeChunks = 0;
	int fadePosition;
	int nextChunk = -1;
	int test;
        int decodeWaitedOn = 0;
	char silence[CHUNK_SIZE] = { 0 };
	int previousMetadataChunk = -1;
	MetadataChunk currentMetadataChunk;
	int currentChunkSent = 1;
	int end;
	int next = -1;

//		fprintf(stderr,"In decode.c decodeParent func :\r\n");
	if(waitOnDecode(pc,dc,cb,&decodeWaitedOn)<0) return;

        pc->elapsedTime = 0;
	pc->state = PLAYER_STATE_PLAY;
	pc->play = 0;
	kill(getppid(),SIGUSR1);

	while(mpm_get_id(MPM_DECODE)>0 && cb->end-cb->begin<bbp && 
				cb->end!=buffered_chunks-1 &&
				dc->state!=DECODE_STATE_STOP) 
	{
		processDecodeInput();
		if(quit) return;
		playSilenceOrSleep();
	}

	while(!quit) {
		processDecodeInput();
                handleDecodeStart();
		handleMetadata(cb, pc, &previousMetadataChunk, 
				&currentChunkSent, &currentMetadataChunk);
		if(dc->state==DECODE_STATE_STOP && 
			pc->queueState==PLAYER_QUEUE_FULL &&
			pc->queueLockState==PLAYER_QUEUE_UNLOCKED) 
		{
			next = cb->end;
			dc->start = 1;
			pc->queueState = PLAYER_QUEUE_DECODE;
			kill(getppid(),SIGUSR1);
		}
		if(next>=0 && doCrossFade==0 && !dc->start && 
				dc->state!=DECODE_STATE_START) 
		{
			nextChunk = -1;
			if(isCurrentAudioFormat(&(cb->audioFormat))) {
				doCrossFade = 1;
				crossFadeChunks = 
				calculateCrossFadeChunks(pc,
                                        &(cb->audioFormat));
				if(!crossFadeChunks ||
						pc->crossFade>=dc->totalTime) 
				{
					doCrossFade = -1;
				}
			}
			else doCrossFade = -1;
		}

		/* copy thse to locale variables to prevent any potential
			race conditions and weirdness */
		end = cb->end;

		if(pause) my_usleep(10000);
		else if(cb->begin!=end && cb->begin!=next) {
			if(doCrossFade==1 && next>=0 &&
					((next>cb->begin && 
					(fadePosition=next-cb->begin)
					<=crossFadeChunks) || 
					(cb->begin>next &&
					(fadePosition=next-cb->begin+
					buffered_chunks)<=crossFadeChunks)))
			{
				if(nextChunk<0) {
					crossFadeChunks = fadePosition;
				}
				test = end;
				if(end < cb->begin) test+=buffered_chunks;
				nextChunk = cb->begin+crossFadeChunks;
				if(nextChunk<test) {
					if(nextChunk>=buffered_chunks)
					{
						nextChunk -=  buffered_chunks;  
					}
					pcm_mix(cb->chunks+cb->begin*CHUNK_SIZE,
							cb->chunks+nextChunk*
							CHUNK_SIZE,
							cb->chunkSize[
								cb->begin],
							cb->chunkSize[
								nextChunk],
							&(cb->audioFormat),
							((float)fadePosition)/
							crossFadeChunks);
					if(cb->chunkSize[nextChunk]>
							cb->chunkSize[cb->begin]
							)
					{
						cb->chunkSize[cb->begin]
								= cb->chunkSize
								[nextChunk];
					}
				}
				else {
					if(dc->state==DECODE_STATE_STOP)
					{
						doCrossFade = -1;
					}
					else continue;
				}
			}
			pc->elapsedTime = cb->times[cb->begin];
			pc->bitRate = cb->bitRate[cb->begin];
			pcm_volumeChange(cb->chunks+cb->begin*
				CHUNK_SIZE,
				cb->chunkSize[cb->begin],
				&(cb->audioFormat),
				pc->softwareVolume);
			if(playAudio(cb->chunks+cb->begin*CHUNK_SIZE,
				cb->chunkSize[cb->begin])<0) 
			{
				quit = 1;
			}
			if( cb->begin+1 >= buffered_chunks ) {
				cb->begin = 0;
			}
			else cb->begin++;
		}
		else if(next==cb->begin) {
			if(doCrossFade==1 && nextChunk>=0) {
				nextChunk = cb->begin+crossFadeChunks;
				test = cb->end;
				if(end < cb->begin) test+=buffered_chunks;
				if(nextChunk<test) {
					if(nextChunk>=buffered_chunks)
					{
						nextChunk -= buffered_chunks;
					}
					advanceOutputBufferTo(cb, pc, 
						&previousMetadataChunk,
						&currentChunkSent, 
						&currentMetadataChunk, 
						nextChunk);
				}	
			}
			while(pc->queueState==PLAYER_QUEUE_DECODE ||
					pc->queueLockState==PLAYER_QUEUE_LOCKED)
			{
				processDecodeInput();
				if(quit) {
					quitDecode(pc,dc);
					return;
				}
		fprintf(stderr,"In decode.c decodeParent while loop with my_usleep\r\n");
//				my_usleep(10000);
			}
			if(pc->queueState!=PLAYER_QUEUE_PLAY) {
				quit = 1;
				break;
			}
			else {
				next = -1;
				if(waitOnDecode(pc,dc,cb,&decodeWaitedOn)<0) {
                                        return;
                                }
				nextChunk = -1;
				doCrossFade = 0;
				crossFadeChunks = 0;
				pc->queueState = PLAYER_QUEUE_EMPTY;
				kill(getppid(),SIGUSR1);
			}
		}
		else if(mpm_get_id(MPM_DECODE)<=0 || 
				(dc->state==DECODE_STATE_STOP && !dc->start)) 
		{
			quit = 1;
			break;
		}
		else {
			if(playAudio(silence, CHUNK_SIZE) < 0) quit = 1;
		}
	}

	quitDecode(pc,dc);
}

/* decode w/ buffering
 * this will fork another process
 * child process does decoding
 * parent process does playing audio
 */
void decode() {
	OutputBuffer * cb;
	PlayerControl * pc;
	DecoderControl * dc;

//		fprintf(stderr,"In decode.c decode func :\r\n");
	cb = &(getPlayerData()->buffer);

	clearAllMetaChunkSets(cb);
	cb->begin = 0;
	cb->end = 0;
	pc = &(getPlayerData()->playerControl);
	dc = &(getPlayerData()->decoderControl);
	dc->error = 0;
        dc->seek = 0;
        dc->stop = 0;
	dc->start = 1;
        
	if(mpm_get_id(MPM_DECODE)<=0) {
		if(decoderInit(pc,cb,dc)<0) return;
	}

        decodeParent(pc, dc, cb);
//		fprintf(stderr,"Exiting In decode.c decode func :\r\n");

}
