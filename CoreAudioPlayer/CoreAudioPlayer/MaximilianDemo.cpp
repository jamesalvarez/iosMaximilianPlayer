//
//  MaximilianDemo.cpp
//  CoreAudioPlayer
//
//  Created by James on 06/09/2017.
//  Copyright © 2017 James Alvarez. All rights reserved.
//

#include "MaximilianDemo.hpp"
#include "maximilian.h"

struct MaximilianDemo {
    maxiOsc myOsc;
    maxiEnvelope myEnv;
    int counter;
};

MaximilianDemo demo;


void MaximilianDemoPlay(double *left, double *right) {
    
    
    if (demo.counter ==0 || demo.counter % 8820==0) {
        demo.myEnv.trigger(true);
    }
    
    
    demo.counter++;
    
    double out = demo.myEnv.ar(0.01,1);
    
    *left = demo.myOsc.sinewave(440)*out;
    
    *right = *left;
    
}
