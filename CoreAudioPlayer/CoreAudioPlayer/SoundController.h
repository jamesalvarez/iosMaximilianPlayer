//
//  SoundController.h
//  CoreAudioPlayer
//
//  Created by James on 02/09/2016.
//  Copyright © 2016 James Alvarez. All rights reserved.
//

#ifndef SoundController_h
#define SoundController_h

#include <stdio.h>
#include <AudioToolbox/AudioToolbox.h>




// A struct to hold information about output status

typedef struct CAPAudioOutput
{
    AudioUnit outputUnit;
    double startingFrameCount;
} CAPAudioOutput;


void CAPStartAudioOutput (CAPAudioOutput *player);
void CAPDisposeAudioOutput(CAPAudioOutput *output);

#endif /* SoundController_h */
