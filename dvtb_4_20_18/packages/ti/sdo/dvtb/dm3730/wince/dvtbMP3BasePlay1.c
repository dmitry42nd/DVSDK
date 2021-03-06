/*
 * dvtbMP3BasePlay1.c
 *
 * XDM1.0 MP3Base Decode functionality
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "dvtbMP3BasePlay1.h"
#include "dvtbAudPlay1.h"
//#include "dvtbALSAAudio.h"
#include "dvtbMP3BaseDec1.h"
#include "dvtbMemory.h"
#include "dvtbDefs.h"
#include "dvtbUtils.h"

/***************************************************Play Thread Start **********************************************************/

//#define TEST_SEM

AudioBufDesc audioDesc[AUD_OUTBUF_COUNT];

static PlayTskState tskPlayState = DELETED;
static int availableInBuff = 0;

static SemThread_Handle sem_in;
static SemThread_Handle sem_out;
static SemThread_Handle sem_block;

void
dvtb_mp3Play(DvevmStThreadInfo *T)
{
	int aud_outBuffer_index = 0;
	int status = DVEVM_ST_SUCCESS;
	int nBytes;

	while (1)
	{
		SemThread_pend(sem_in, SemThread_FOREVER);
		if ((tskPlayState == EXITING) && (0 == availableInBuff))
			break;

		SemThread_pend(sem_block, SemThread_FOREVER);
		availableInBuff -= 1;
		SemThread_post(sem_block);

		if (T->useAlgorithm)
			nBytes = audioDesc[aud_outBuffer_index].aud_bufSize;
		else
			nBytes = AUD_FRAME_BUFF_SIZE;

#ifdef TEST_SEM
		status =  dvtb_fileWrite(T->targetFp, audioDesc[aud_outBuffer_index].aud_outBuffer, nBytes);
#else
//		status = dvtb_audioOut(&T->g.aud, audioDesc[aud_outBuffer_index].aud_outBuffer, nBytes);
#endif

		if (DVEVM_ST_FAIL == status)
		{
			SYS_ERROR("Audio play-back failed\n");
		}

		aud_outBuffer_index++;
		if (AUD_OUTBUF_COUNT <= aud_outBuffer_index)
			aud_outBuffer_index = 0;

		SemThread_post(sem_out);
	}
	tskPlayState = DELETED;
	dvtb_waitThread();
}
/***************************************************Play Thread End **********************************************************/
void
dvtb_mp3baseDec1ProcessEndMessage(DvevmStBool useAlgorithm, DvevmStBool fileBased, int nFrames)
{
	if (DVEVM_ST_FALSE == fileBased)//Driver based
	{
		if (useAlgorithm)
			SYS_OUT("MP3 [Decode]+[Play] completed for <%d> frames\n", nFrames - 1);
		else
			SYS_OUT("MP3 [Play] completed for <%d> frames\n", nFrames - 1);
	}
	else//File based
		SYS_OUT("MP3 [Decode]+[dump] completed for <%d> frames\n", nFrames - 1);
}

void
dvtb_mp3baseDec1InitPtrs(DvevmStMP3BaseDec1Info *ad)
{
	int i =0;

	ad->hTime = NULL;
	ad->ceHdl = NULL;
	ad->adecHdl = NULL;

	for (i = 0; i < MAX_XDM_BUFS; i++)
	{
		ad->inBuf.bufs[i] = NULL;
		ad->outBuf.bufs[i] = NULL;
	}
}

DvevmStRetCode
dvtb_mp3baseDec1HanInit(DvevmStMP3BaseDec1Info *ad, char *engName)
{
	DvevmStRetCode retCode = DVEVM_ST_SUCCESS;

	do //Dummy loop for easy clean up
	{
		if (DVEVM_ST_FAIL == dvtb_ceInit(engName, &ad->ceHdl))
		{
			SYS_ERROR("Unable to initialize Codec Engine <%s>\n", engName);
			retCode = DVEVM_ST_FAIL;
			break;
		}


		if (DVEVM_ST_FAIL == dvtb_mp3baseDec1Init(ad))
		{
			SYS_ERROR("Unable to initialize Audio Decoder <%s>\n", ad->adecName);
			retCode = DVEVM_ST_FAIL;
			break;
		}
	}while(DVEVM_ST_FALSE);

	return retCode;
}

void
dvtb_mp3baseDec1HanClose(DvevmStMP3BaseDec1Info *ad, char *engName)
{
	dvtb_mp3baseDec1Close(ad);
	SYS_OUT("Audio Decoder <%s> closed\n", ad->adecName);

	if (DVEVM_ST_FAIL == dvtb_ceDeInit(&ad->ceHdl))
		SYS_ERROR("Unable to close Engine <%s>\n", engName);
	else
		SYS_OUT("Engine <%s> closed\n", engName);
}

DvevmStRetCode
dvtb_mp3baseDec1DecProcess(DvevmStMP3BaseDec1Info *ad, int *decDuration)
{
	DvevmStRetCode retCode = DVEVM_ST_SUCCESS;

	do
	{
		if (DVEVM_ST_FAIL == dvtb_mp3baseDec1Decode(ad, decDuration))
		{
			SYS_ERROR("Unable to decode <%s> \n", ad->adecName);
			retCode = DVEVM_ST_FAIL;
			break;
		}

		ad->adecCmd = XDM_GETSTATUS;
		if (DVEVM_ST_FAIL == dvtb_mp3baseDec1Control(ad))
		{
			SYS_ERROR("Unable to get the status \n");
			retCode = DVEVM_ST_FAIL;
			break;
		}
	}while(DVEVM_ST_FALSE);

	return retCode;
}

void
dvtb_Mp3baseDec1Play(DvevmStThreadInfo *T)
{
	int nBytes = DVEVM_ST_FAIL, nFrames = 0, frameSize = 0, totalBytesConsumed = 0, bytesRemaining = 0,
		buffsize = 1000000, thresh_hold = 900000, hugeChunk = 0, decDuration = 0, aud_outBuffer_index = 0,
		noOfOutChannels = 0, inFrameSize =0;
	char *buff = NULL, *inBuffer = NULL;
	unsigned int inputFileSize = 0;

	Thread_Handle thrHandle = NULL;
	Thread_Params thrParams;

	DvevmStRetCode status = DVEVM_ST_FAIL;
	DvevmStBool usecaseFailed = DVEVM_ST_FALSE, audioDeviceOpened =	DVEVM_ST_FALSE, fileBased = DVEVM_ST_TRUE;


	if (!T->sourceFp)
	{
		SYS_ERROR("Source file cannot be opened. It is a mandatory parameter\n");
		dvtb_fileClose(&T->sourceFp, &T->targetFp);
		dvtb_freeThread(T);
		dvtb_waitThread();
	}

	if (!T->targetFp)
	{
		SYS_ERROR("Target file cannot be opened. It is a mandatory parameter\n");
		dvtb_fileClose(&T->sourceFp, &T->targetFp);
		dvtb_freeThread(T);
		dvtb_waitThread();
	}

	if (DVEVM_ST_FAIL == dvtb_audDec1SemOpen(&sem_in, 0, &sem_out, AUD_OUTBUF_COUNT-2, &sem_block, 1))
	{
		SYS_ERROR("sem_open() FAILED\n");
		dvtb_fileClose(&T->sourceFp, &T->targetFp);
		dvtb_freeThread(T);
		dvtb_waitThread();

	}

//	dvtb_audDec1SemInit(sem_in, 0, sem_out, AUD_OUTBUF_COUNT-2, sem_block, 0);

	if (!T->targetFp)
		fileBased = DVEVM_ST_FALSE;

	dvtb_startMessage(DVEVM_ST_DECODE, "MP3", T->useAlgorithm, fileBased);
	dvtb_mp3baseDec1InitPtrs(&T->g.mp3basedec1);
	dvtb_audDec1InitPlayThreadBuffs(audioDesc, AUD_OUTBUF_COUNT);

	// [Decode][Play] scenario
	do
	{
		T->g.mp3basedec1.hTime = dvtb_timeCreate();
		if(NULL == T->g.mp3basedec1.hTime)
		{
			SYS_ERROR("Unable to create Time handle. \n");
			usecaseFailed = DVEVM_ST_TRUE;
			break;
		}

		if (T->useAlgorithm)
		{
			if (DVEVM_ST_FAIL == dvtb_mp3baseDec1HanInit(&T->g.mp3basedec1, T->g.eng.name))
			{
				SYS_ERROR("Unable to initialize Mp3 decode handle \n");
				usecaseFailed = DVEVM_ST_TRUE;
				break;
			}
			SYS_OUT("MP3 Decoder <%s> initialized\n", T->g.mp3basedec1.adecName);
		}

		if (DVEVM_ST_FALSE == fileBased)
		{
			if (!T->useAlgorithm)
			{
#if 0
				T->g.aud.mode = SND_PCM_STREAM_PLAYBACK;
				status = dvtb_audioSetup(&T->g.aud);
				if (DVEVM_ST_FAIL == status)
				{
					SYS_ERROR("Unable to setup Audio device <%s> for playback\n", T->g.aud.device);
					usecaseFailed = DVEVM_ST_TRUE;
					break;
				}

				audioDeviceOpened = DVEVM_ST_TRUE;
				SYS_OUT("Audio device <%s> initialized for playback\n", T->g.aud.device);
#endif
			}

			dvtb_audDec1InitThread(&thrParams);
		}

		hugeChunk = buffsize;
		//Get file Size
		if (DVEVM_ST_FAIL == dvtb_getFileSize(T->sourceFp, &inputFileSize))
		{
			SYS_ERROR("Unable to Get file size\n");
			usecaseFailed = DVEVM_ST_TRUE;
			break;
		}

		//If input file size is less than huge chunk than set hugeChunk to input file Size
		if ((int)inputFileSize < hugeChunk)
			hugeChunk = inputFileSize;

		if (DVEVM_ST_FAIL == dvtb_allocSingleBufCmem(&inBuffer, buffsize))
		{
			SYS_ERROR("Unable to allocate memory for In Buffer\n");
			usecaseFailed = DVEVM_ST_TRUE;
			break;
		}

		bytesRemaining = 0;
		if (DVEVM_ST_FAIL == dvtb_audDec1ReadInput(inBuffer, &totalBytesConsumed, &bytesRemaining, buffsize, T->sourceFp, &nBytes))
		{
			SYS_ERROR("Error in Read Input\n");
			usecaseFailed = DVEVM_ST_TRUE;
			break;
		}

		if (DVEVM_ST_FAIL == dvtb_audDec1AllocPlayThreadBuffs(audioDesc, T->g.mp3basedec1.outBuf.bufSizes[0], AUD_OUTBUF_COUNT))
		{
			SYS_ERROR("Unable to allocate Buffers for play thread \n");
			usecaseFailed = DVEVM_ST_TRUE;
			break;
		}

		aud_outBuffer_index = 0;
		T->g.mp3basedec1.outBuf.bufs[0] = audioDesc[aud_outBuffer_index].aud_outBuffer;
		if (T->useAlgorithm)
		{
			inFrameSize = T->g.mp3basedec1.inBuf.bufSizes[0];
		}
		else
			inFrameSize = AUD_FRAME_BUFF_SIZE;


		while (!feof(T->sourceFp) || bytesRemaining)
		{
			if (totalBytesConsumed > thresh_hold)
			{
				if (DVEVM_ST_FAIL == dvtb_audDec1ReadInput(inBuffer, &totalBytesConsumed, &bytesRemaining, buffsize, T->sourceFp, &nBytes))
				{
					SYS_ERROR("Error in Read Input\n");
					usecaseFailed = DVEVM_ST_TRUE;
					break;
				}

				hugeChunk = nBytes;
				if (nBytes == 0)
				{
					SYS_OUT("End of file reached. Exiting...\n");
					break;
				}
			}
			nFrames++;

			if (bytesRemaining < inFrameSize)
				nBytes = bytesRemaining;
			else
				nBytes = inFrameSize;

			buff = &inBuffer[totalBytesConsumed];
			T->g.mp3basedec1.inBuf.bufs[0] = buff;

			if (T->useAlgorithm)
			{
				T->g.mp3basedec1.inFrameSize = nBytes;
				if(DVEVM_ST_FAIL == dvtb_mp3baseDec1DecProcess(&T->g.mp3basedec1, &decDuration))
				{
					SYS_ERROR("Unable to decode frame # %d\n",	nFrames);
					if (DVEVM_ST_FALSE == fileBased)
					{
						tskPlayState = EXITING;
						SemThread_post(sem_in);
					}
					usecaseFailed = DVEVM_ST_TRUE;
					break;
				}

				frameSize = T->g.mp3basedec1.decFrameSize;
				totalBytesConsumed += frameSize;
				bytesRemaining = hugeChunk - totalBytesConsumed;

				buff = T->g.mp3basedec1.outBuf.bufs[0];

				nBytes = dvtb_audDec1getOutPutBytes(&T->g.mp3basedec1.adecStatus);
				audioDesc[aud_outBuffer_index].aud_bufSize = nBytes;
#if 0
				if (T->g.aud.sampleRate != T->g.mp3basedec1.adecStatus.sampleRate)
					T->g.aud.sampleRate = T->g.mp3basedec1.adecStatus.sampleRate;
#endif

				noOfOutChannels = (T->g.mp3basedec1.adecStatus.channelMode == 0) ? 1 : 2;


#ifdef PERF
				if(!T->g.mp3basedec1.pEnable)
				{
					SYS_PERF("Thread, %x, Frame#, %d, Decoded Frame Size(bytes), %d, DecTime(us), %d\n", (unsigned int) T, nFrames, nBytes, decDuration);
					SYS_DEBUG("Frame#, %d, Frame Size(bytes), %d, DecTime(us), %d\n", nFrames, frameSize, decDuration);
				}
#endif
			}
			else
			{
				memcpy(audioDesc[aud_outBuffer_index].aud_outBuffer, buff, nBytes);
				totalBytesConsumed += nBytes;
				bytesRemaining = hugeChunk - totalBytesConsumed;
			}
#ifndef TEST_SEM
			if (DVEVM_ST_TRUE == fileBased)
			{
				if(DVEVM_ST_FAIL ==  dvtb_fileWrite(T->targetFp, buff, nBytes))
				{
					SYS_ERROR("Error in writing to file.\n");
					usecaseFailed = DVEVM_ST_TRUE;
					break;
				}
			}
			else
#endif
			{
#if 0
				if (T->useAlgorithm && (noOfOutChannels != T->g.aud.numChannels))
				{
					SYS_ERROR("No of channels of audio driver is not set correctly \n");
					SYS_ERROR("Decoder: number of channels %d \n", noOfOutChannels);
					SYS_ERROR("Driver: number of channels %d \n", T->g.aud.numChannels);
					usecaseFailed = DVEVM_ST_TRUE;
					tskPlayState = EXITING;
					SemThread_post(sem_in);
					break;
				}
#endif
				SemThread_post(sem_in);

				SemThread_pend(sem_block, SemThread_FOREVER);
				availableInBuff += 1;
				SemThread_post(sem_block);

				status = DVEVM_ST_SUCCESS;
				//status = dvtb_audioOut(&T->g.aud, buff, nBytes);
				SemThread_pend(sem_out, SemThread_FOREVER);
			}

			aud_outBuffer_index++;

			if (AUD_OUTBUF_COUNT == aud_outBuffer_index)
				aud_outBuffer_index = 0;

			T->g.mp3basedec1.outBuf.bufs[0] = audioDesc[aud_outBuffer_index].aud_outBuffer;
			if (nFrames == (AUD_OUTBUF_COUNT-2))
			{
#ifndef TEST_SEM
				if ((DVEVM_ST_FALSE == fileBased))
#endif
				{
					if(T->useAlgorithm)
					{
#if 0
						T->g.aud.mode = SND_PCM_STREAM_PLAYBACK;
						T->g.aud.framesize = T->g.mp3basedec1.outBuf.bufSizes[0];
						status = dvtb_audioSetup(&T->g.aud);
						if (DVEVM_ST_FAIL == status)
						{
							SYS_ERROR("Unable to setup Audio device <%s> for playback\n", T->g.aud.device);
							usecaseFailed = DVEVM_ST_TRUE;
							break;
						}
						audioDeviceOpened = DVEVM_ST_TRUE;
						SYS_OUT("Audio device <%s> initialized for playback\n", T->g.aud.device);
#endif
					}

					tskPlayState = RUNNING;
					thrParams.arg = (IArg)T;
					thrParams.name = "dvtb_mp3Play";
					thrParams.priority = Thread_Priority_HIGHEST;

					if (DVEVM_ST_FAIL == dvtb_createThread((Thread_RunFxn) dvtb_mp3Play, &thrParams, &thrHandle))
					{
						SYS_ERROR("Unable to create a thread\n");
						usecaseFailed = DVEVM_ST_TRUE;
						break;
					}
				}
			}

		}

#ifdef TEST_SEM
		if (DVEVM_ST_FALSE == fileBased)
#endif
		{
			if (EXITING > tskPlayState)
			{
				tskPlayState = EXITING;
				SemThread_post(sem_in);
			}
			if(NULL != thrHandle)
			{
		 	        while (DELETED != tskPlayState)
			        {
					dvtb_sleepThread(1);
			        }
				dvtb_deleteThread(&thrHandle);
                        }

		}
	} while (DVEVM_ST_FALSE); // Dummy loop

	dvtb_mp3baseDec1ProcessEndMessage(T->useAlgorithm, fileBased, nFrames);

#if 0
	if (DVEVM_ST_TRUE == audioDeviceOpened)
	{
		dvtb_audioClose(&T->g.aud);
		SYS_OUT("Audio Device <%s> closed\n", T->g.aud.device);
	}
#endif

	if (NULL != inBuffer)
	{
		dvtb_freeSingleBufCmem(inBuffer, buffsize);
		inBuffer = NULL;
	}

	if (T->useAlgorithm)
		dvtb_mp3baseDec1HanClose(&T->g.mp3basedec1, T->g.eng.name);

	dvtb_audDec1FreePlayThreadBuffs(audioDesc, T->g.mp3basedec1.inBuf.bufSizes[0], AUD_OUTBUF_COUNT);

	dvtb_fileClose(&T->sourceFp, &T->targetFp);

	dvtb_audDec1SemClose(&sem_in,&sem_out,&sem_block);
	dvtb_timeDelete(T->g.mp3basedec1.hTime);
	dvtb_exitMessage(DVEVM_ST_DECODE, "MP3", T->useAlgorithm, fileBased, usecaseFailed);

	dvtb_freeThread(T);
	dvtb_waitThread();
}
