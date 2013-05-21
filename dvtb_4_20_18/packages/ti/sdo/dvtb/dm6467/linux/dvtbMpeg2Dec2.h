/*
 * dvtbMpeg2Dec2.h
 *
 * XDM1.2 MPEG2 Decoder Interface
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

#ifndef _DVEVM_ST_MPEG2_DEC2_H
#define _DVEVM_ST_MPEG2_DEC2_H

#include "dvtbGlobal.h"
#include "dvtbEngine.h"
#include "dvtbDefs.h"
#include <ti/sdo/ce/video2/viddec2.h>
#include <ti/sdo/codecs/mpeg2dec/imp2vdec.h>


typedef struct
{
	char vdec2Name[MAX_NAME_LEN];

	Imp2VDEC_Cmd vdec2Cmd;
	Imp2VDEC_Params vdec2Params;
	Imp2VDEC_DynamicParams vdec2DynParams;
	Imp2VDEC_InArgs vdec2InArgs;
	Imp2VDEC_OutArgs vdec2OutArgs;
	Imp2VDEC_Status vdec2Status;

	DvevmStXdmBuffer inBuf;
	DvevmStXdmBuffer outBuf;
	XDM1_BufDesc inBufDesc;
	XDM_BufDesc outBufDesc;

	DvevmStBufElement outBufArray[VIDDEC2_PLAY_MAX_BUFFERS];
	DvevmStOutBuffsInUse dispBufArray[VIDDEC2_PLAY_MAX_BUFFERS];
	int buffIdSearchStart;

	Engine_Handle ceHdl;
	VIDDEC2_Handle vdec2Hdl;

	DvevmStBool skipFrame;
	DvevmStXdmBuffer firstBuffer;
	int numFrames;
	int totBuf;
	unsigned int pEnable;
} DvevmStMpeg2Dec2Info;

void
dvtb_mpeg2Dec2InitDebug(Imp2VDEC_Params *vdp);

void
dvtb_mpeg2Dec2InArgsDebug(Imp2VDEC_InArgs *vdi);

void
dvtb_mpeg2Dec2OutArgsDebug(Imp2VDEC_OutArgs *vdo);

void
dvtb_mpeg2Dec2ControlDebug(Imp2VDEC_Status *vds);

void
dvtb_mpeg2Dec2Cleanup(DvevmStMpeg2Dec2Info *vd);

DvevmStRetCode
dvtb_mpeg2Dec2Init(DvevmStMpeg2Dec2Info *vd);

DvevmStRetCode
dvtb_mpeg2Dec2Decode(DvevmStMpeg2Dec2Info *vd, int *decDuration);

DvevmStRetCode
dvtb_mpeg2Dec2Control(DvevmStMpeg2Dec2Info *vd);

DvevmStRetCode
dvtb_mpeg2Dec2Close(DvevmStMpeg2Dec2Info *vd);

#endif