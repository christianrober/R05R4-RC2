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

#include "player.h"
#include "decode.h"
#include "command.h"
#include "interface.h"
#include "playlist.h"
#include "ls.h"
#include "listen.h"
#include "log.h"
#include "utils.h"
#include "directory.h"
#include "volume.h"
#include "playerData.h"
#include "permission.h"
#include "sig_handlers.h"
#include "mpm.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define clearPlayerPid() do { \
		mpm_set_id(MPM_PLAYER,0); \
		mpm_free(MPM_PLAYER); \
	} while (0)

static void resetPlayerMetadata() {
        PlayerControl * pc = &(getPlayerData()->playerControl);

        if(pc->metadataState == PLAYER_METADATA_STATE_READ) {
                pc->metadataState = PLAYER_METADATA_STATE_WRITE;
        }
}

void resetPlayer() {
	clearPlayerPid();
	getPlayerData()->playerControl.stop = 0;
	getPlayerData()->playerControl.play = 0;
	getPlayerData()->playerControl.pause = 0;
	getPlayerData()->playerControl.lockQueue = 0;
	getPlayerData()->playerControl.unlockQueue = 0;
	getPlayerData()->playerControl.state = PLAYER_STATE_STOP;
	getPlayerData()->playerControl.queueState = PLAYER_QUEUE_UNLOCKED;
	getPlayerData()->playerControl.seek = 0;
        getPlayerData()->playerControl.metadataState = 
                        PLAYER_METADATA_STATE_WRITE;
	if(mpm_get_id(MPM_DECODE)>0)
		kill(mpm_get_id(MPM_DECODE),SIGTERM);
	mpm_set_id(MPM_DECODE,0);
}

void player_sigChldHandler(pid_t pid, int status) {
	if(mpm_get_id(MPM_PLAYER)==pid) {
		DEBUG("SIGCHLD caused by player process\n");
		if(WIFSIGNALED(status) && WTERMSIG(status)!=SIGTERM &&
				WTERMSIG(status)!=SIGINT) 
		{
			ERROR("player process died from signal: %i\n",
					WTERMSIG(status));
		}
		resetPlayer();
	}
	else if(pid==mpm_get_id(MPM_DECODE) && mpm_get_id(MPM_PLAYER)<=0)
	{
		if(WIFSIGNALED(status) && WTERMSIG(status)!=SIGTERM) {
			ERROR("(caught by master parent) "
					"decode process died from a "
					"non-TERM signal: %i\n",
					WTERMSIG(status));
		}
		mpm_set_id(MPM_DECODE,0);
		mpm_free(MPM_DECODE);
	}
}

int player_task(void *playercontrol) {
	PlayerControl * pc = (PlayerControl *) playercontrol;

	unblockSignals();
	
	setSigHandlersForDecoder();

	mpm_enter(MPM_PLAYER);	

		fprintf(stderr,"In player.c player_task func :\r\n");
	while(1) {
		if(pc->play) decode();
		else if(pc->stop) pc->stop = 0;
		else if(pc->pause) pc->pause = 0;
		else if(pc->closeAudio) {
			closeAudioDevice();
			pc->closeAudio = 0;
			kill(getppid(),SIGUSR1);
		}
		else if(pc->lockQueue) {
			pc->queueLockState = PLAYER_QUEUE_LOCKED;
			pc->lockQueue = 0;
		}
		else if(pc->unlockQueue) {
			pc->queueLockState = PLAYER_QUEUE_UNLOCKED;
			pc->unlockQueue = 0;
		}
		else if(pc->cycleLogFiles) {
			myfprintfCloseAndOpenLogFile();
			pc->cycleLogFiles = 0;
		}
		else my_usleep(10000);
	}
		fprintf(stderr,"In player.c player_task func NOW Exiting  :\r\n");
	
	return EXIT_SUCCESS;
}

int playerInit() {
	int rc;
	PlayerControl * pc = &(getPlayerData()->playerControl);
	blockSignals();
	rc = mpm_spawn(MPM_PLAYER,(void *) pc);
	unblockSignals();
	if (rc<=0) {
		ERROR("return code from mpm_spawn(player_task): %d\n",rc);
		exit(-1);
	}

	return 0;
}

int playerPlay(FILE * fp, Song * song) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(fp==NULL) fp = stderr;

	if(playerStop(fp)<0) return -1;

	/*{
		struct stat st;
		if(stat(rmp2amp(utf8ToFsCharset(utf8file)),&st)<0) {
			strncpy(pc->erroredFile,pc->file,MAXPATHLEN);
			pc->erroredFile[MAXPATHLEN] = '\0';
			pc->error = PLAYER_ERROR_FILENOTFOUND;
			return 0;
		}
	}*/
	
        if(song->tag) pc->fileTime = song->tag->time;
        else pc->fileTime = 0;

        strncpy(pc->utf8url, getSongUrl(song), MAXPATHLEN);
	pc->utf8url[MAXPATHLEN] = '\0';

	pc->play = 1;
	if(mpm_get_id(MPM_PLAYER)==0 && playerInit()<0) {
		pc->play = 0;
		return -1;
	}
	
        resetPlayerMetadata();
	while(mpm_get_id(MPM_PLAYER)>0 && pc->play) my_usleep(1000);
	
	return 0;
}

int playerStop(FILE * fp) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)>0 && pc->state!=PLAYER_STATE_STOP) {
		pc->stop = 1;
		while(mpm_get_id(MPM_PLAYER)>0 && pc->stop) my_usleep(1000);
	}

	pc->queueState = PLAYER_QUEUE_BLANK;
	playerQueueUnlock();

	return 0;
}

void playerKill() {
	if(mpm_get_id(MPM_PLAYER)>0)
		kill(mpm_get_id(MPM_PLAYER),SIGTERM);
}

int playerPause(FILE * fp) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)>0 && pc->state!=PLAYER_STATE_STOP) {
		pc->pause = 1;
		while(mpm_get_id(MPM_PLAYER)>0 && pc->pause) my_usleep(1000);
	}

	return 0;
}

int playerSetPause(FILE * fp, int pause) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)<=0) return 0;

	switch(pc->state) {
	case PLAYER_STATE_PLAY:
		if(pause) playerPause(fp);
		break;
	case PLAYER_STATE_PAUSE:
		if(!pause) playerPause(fp);
		break;
	}

	return 0;
}

int getPlayerElapsedTime() {
	return (getPlayerData()->playerControl.elapsedTime);
}

unsigned long getPlayerBitRate() {
	return getPlayerData()->playerControl.bitRate;
}

int getPlayerTotalTime() {
	return (getPlayerData()->playerControl.totalTime);
}

int getPlayerState() {
	return getPlayerData()->playerControl.state;
}

void clearPlayerError() {
	getPlayerData()->playerControl.error = 0;
}

int getPlayerError() {
	return getPlayerData()->playerControl.error;
}

/* static vars are thread-safe for now -- eric */
char * getPlayerErrorStr() {
	static char * error = NULL;
	int errorlen = MAXPATHLEN+1024;
	PlayerControl * pc = &(getPlayerData()->playerControl);

	error = realloc(error,errorlen+1);
	memset(error,0,errorlen+1);

	switch(pc->error) {
	case PLAYER_ERROR_FILENOTFOUND:
		snprintf(error,errorlen,
				"file \"%s\" does not exist or is inaccesible",
				pc->erroredUrl);
		break;
	case PLAYER_ERROR_FILE:
		snprintf(error,errorlen,"problems decoding \"%s\"",
				pc->erroredUrl);
		break;
	case PLAYER_ERROR_AUDIO:
		snprintf(error,errorlen,"problems opening audio device");
		break;
	case PLAYER_ERROR_SYSTEM:
		snprintf(error,errorlen,"system error occured");
		break;
	case PLAYER_ERROR_UNKTYPE:
		snprintf(error,errorlen,"file type  of \"%s\" is unknown",
				pc->erroredUrl);
	default:
		break;
	}

	errorlen = strlen(error);
	error = realloc(error,errorlen+1);

	if(errorlen) return error;

	return NULL;
}

void playerCloseAudio() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)>0) {
		if(playerStop(stderr)<0) return;
		pc->closeAudio = 1;
	}
}

int queueSong(Song * song) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(pc->queueState==PLAYER_QUEUE_BLANK) {
                strncpy(pc->utf8url, getSongUrl(song), MAXPATHLEN);
		pc->utf8url[MAXPATHLEN] = '\0';

                if(song->tag) pc->fileTime = song->tag->time;
                else pc->fileTime = 0;

		pc->queueState = PLAYER_QUEUE_FULL;
		return 0;
	}

	return -1;
}

int getPlayerQueueState() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->queueState;
}

void setQueueState(int queueState) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	pc->queueState = queueState;
}

void playerQueueLock() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)>0 && pc->queueLockState==PLAYER_QUEUE_UNLOCKED)
	{
		pc->lockQueue = 1;
		while(mpm_get_id(MPM_PLAYER)>0 && pc->lockQueue) my_usleep(1000);
	}
}

void playerQueueUnlock() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(mpm_get_id(MPM_PLAYER)>0 && pc->queueLockState==PLAYER_QUEUE_LOCKED)
	{
		pc->unlockQueue = 1;
		while(mpm_get_id(MPM_PLAYER)>0 && pc->unlockQueue) my_usleep(1000);
	}
}

int playerSeek(FILE * fp, Song * song, unsigned time) {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	if(pc->state==PLAYER_STATE_STOP) {
		commandError(fp, ACK_ERROR_PLAYER_SYNC, 
                                "player not currently playing", NULL);
		return -1;
	}

 	if(strcmp(pc->utf8url, getSongUrl(song))!=0) {
                if(song->tag) pc->fileTime = song->tag->time;
                else pc->fileTime = 0;

 		strncpy(pc->utf8url, getSongUrl(song), MAXPATHLEN);
		pc->utf8url[MAXPATHLEN] = '\0';
	}

	if(pc->error==PLAYER_ERROR_NOERROR) {
                resetPlayerMetadata();
		pc->seekWhere = time;
		pc->seek = 1;
		while(mpm_get_id(MPM_PLAYER)>0 && pc->seek) my_usleep(1000);
	}

	return 0;
}

float getPlayerCrossFade() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->crossFade;
}

void setPlayerCrossFade(float crossFadeInSeconds) {
	PlayerControl * pc;
	if(crossFadeInSeconds<0) crossFadeInSeconds = 0;

	pc = &(getPlayerData()->playerControl);

	pc->crossFade = crossFadeInSeconds;
}

void setPlayerSoftwareVolume(int volume) {
	PlayerControl * pc;
	volume = (volume>1000) ? 1000 : (volume<0 ? 0 : volume);

	pc = &(getPlayerData()->playerControl);

	pc->softwareVolume = volume;
}

int getPlayerSoftwareVolume() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->softwareVolume;
}

unsigned int getPlayerSampleRate() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->sampleRate;
}

int getPlayerBits() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->bits;
}

int getPlayerChannels() {
	PlayerControl * pc = &(getPlayerData()->playerControl);

	return pc->channels;
}

void playerCycleLogFiles() {
	PlayerControl * pc = &(getPlayerData()->playerControl);
	DecoderControl * dc = &(getPlayerData()->decoderControl);

	pc->cycleLogFiles = 1;
	dc->cycleLogFiles = 1;
}

/* this actually creates a dupe of the current metadata */
/* not thread-safe, but no need to worry -- eric */
Song * playerCurrentDecodeSong() {
        static Song * song = NULL;
        static MetadataChunk * prev = NULL;
	Song * ret = NULL;
	PlayerControl * pc = &(getPlayerData()->playerControl);

        if(pc->metadataState == PLAYER_METADATA_STATE_READ) {
		DEBUG("playerCurrentDecodeSong: caught new metadata!\n");
                if(prev) free(prev);
                prev = malloc(sizeof(MetadataChunk));
		memcpy(prev, &(pc->metadataChunk), sizeof(MetadataChunk));
                if(song) freeJustSong(song);
                song = newNullSong();
                song->url = strdup(pc->currentUrl);
		song->tag = metadataChunkToMpdTagDup(prev);
                ret =  song;
        	resetPlayerMetadata();
        }

        return ret;
}
