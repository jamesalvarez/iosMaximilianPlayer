//
//  MaximilianDemo.cpp
//  CoreAudioPlayer
//
//  Created by James on 06/09/2017.
//  Copyright © 2017 James Alvarez. All rights reserved.
//

#include "MaximilianDemo.hpp"

#include "maximilian.h"

maxiOsc myOsc;
maxiEnvelope myEnv;
int counter;

void setup() {
}

void play(double *output) {
    
    
    if (counter ==0 || counter % 8820==0) {
        myEnv.trigger(true);
    }
    
    
    counter++;
    
    double out = myEnv.ar(0.01,1);
    
    output[0]=myOsc.sinewave(440)*out;
    
    output[1]=output[0];
    
}
