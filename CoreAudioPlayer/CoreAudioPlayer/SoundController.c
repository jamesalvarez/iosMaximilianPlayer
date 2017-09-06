//
//  SoundController.c
//  CoreAudioPlayer
//
//  Created by James on 02/09/2016.
//  Copyright © 2016 James Alvarez. All rights reserved.
//


#include "SoundController.h"
#include "MaximilianDemo.hpp"




#define CAP_SAMPLE_RATE 44100
#define CAP_CHANNELS 2
#define CAP_SAMPLE_SIZE sizeof(Float32)


AudioStreamBasicDescription const CAPAudioDescription = {
    .mSampleRate        = CAP_SAMPLE_RATE,
    .mFormatID          = kAudioFormatLinearPCM,
    .mFormatFlags       = kAudioFormatFlagIsFloat,
    .mBytesPerPacket    = CAP_SAMPLE_SIZE * CAP_CHANNELS,
    .mFramesPerPacket   = 1,
    .mBytesPerFrame     = CAP_CHANNELS * CAP_SAMPLE_SIZE,
    .mChannelsPerFrame  = CAP_CHANNELS,
    .mBitsPerChannel    = 8 * CAP_SAMPLE_SIZE, //8 bits per byte
    .mReserved          = 0
};





#pragma mark - callback function -

static OSStatus CAPRenderProc(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList * ioData) {
    
    
    
    
    //CAPAudioOutput *audioOutput = (CAPAudioOutput*)inRefCon;


    
    Float32 *outputData = (Float32*)ioData->mBuffers[0].mData;
    
    
    for (UInt32 frame = 0; frame < inNumberFrames; ++frame) {
        UInt32 outSample = frame * 2;
        
        double *left = (double*)&((outputData)[outSample]);
        double *right = (double*)&((outputData)[outSample + 1]);
        MaximilianDemoPlay(left, right);

    }
    
    
    return noErr;
}

#pragma mark - error handler -

// generic error handler - if err is nonzero, prints error message and exits program.
static void CheckError(OSStatus error, const char *operation) {
    if (error == noErr) return;
    
    char str[20];
    // see if it appears to be a 4-char-code
    *(UInt32 *)(str + 1) = CFSwapInt32HostToBig(error);
    if (isprint(str[1]) && isprint(str[2]) && isprint(str[3]) && isprint(str[4])) {
        str[0] = str[5] = '\'';
        str[6] = '\0';
    } else
        // no, format it as an integer
        sprintf(str, "%d", (int)error);
    
    fprintf(stderr, "Error: %s (%s)\n", operation, str);
    
    exit(1);
}



#pragma mark - audio output functions -

void CAPStartAudioOutput (CAPAudioOutput *player) {
    OSStatus status = noErr;
    
    // Description for the output AudioComponent
    AudioComponentDescription outputcd = {
        .componentType = kAudioUnitType_Output,
        .componentSubType = kAudioUnitSubType_RemoteIO,
        .componentManufacturer = kAudioUnitManufacturer_Apple,
        .componentFlags = 0,
        .componentFlagsMask = 0
    };
    
    // Get the output AudioComponent
    AudioComponent comp = AudioComponentFindNext (NULL, &outputcd);
    if (comp == NULL) {
        printf ("can't get output unit");
        exit (-1);
    }
    
    // Create a new instance of the AudioComponent = the AudioUnit
    status = AudioComponentInstanceNew(comp, &player->outputUnit);
    CheckError (status, "Couldn't open component for outputUnit");
    
    
    // Set the stream format
    status = AudioUnitSetProperty(player->outputUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  0,
                                  &CAPAudioDescription,
                                  sizeof(CAPAudioDescription));
    CheckError (status,"kAudioUnitProperty_StreamFormat");

    
    // Set the render callback
    AURenderCallbackStruct input = {
        .inputProc = CAPRenderProc,
        .inputProcRefCon = player
    };
    
    status = AudioUnitSetProperty(player->outputUnit,
                                    kAudioUnitProperty_SetRenderCallback,
                                    kAudioUnitScope_Global,
                                    0,
                                    &input,
                                    sizeof(input));
    CheckError (status, "Could not set render callback");
    
    
    // Set the maximum frames per slice (not necessary)
    UInt32 framesPerSlice = 4096;
    status = AudioUnitSetProperty(player->outputUnit,
                                    kAudioUnitProperty_MaximumFramesPerSlice,
                                    kAudioUnitScope_Global,
                                    0,
                                    &framesPerSlice,
                                  sizeof(framesPerSlice));
    CheckError (status, "AudioUnitSetProperty(kAudioUnitProperty_MaximumFramesPerSlice");
    
    
    // Initialize the Audio Unit
    status = AudioUnitInitialize(player->outputUnit);
    CheckError (status, "Couldn't initialize output unit");
    
    
    // Start the Audio Unit (sound begins)
    status = AudioOutputUnitStart(player->outputUnit);
    CheckError (status, "Couldn't start output unit");
}

void CAPDisposeAudioOutput(CAPAudioOutput *output) {
    AudioOutputUnitStop(output->outputUnit);
    AudioUnitUninitialize(output->outputUnit);
    AudioComponentInstanceDispose(output->outputUnit);
}




