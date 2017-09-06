//
//  MaximilianDemo.hpp
//  CoreAudioPlayer
//
//  Created by James on 06/09/2017.
//  Copyright © 2017 James Alvarez. All rights reserved.
//

#ifndef MaximilianDemo_hpp
#define MaximilianDemo_hpp

#include <stdio.h>

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif



EXTERNC void MaximilianDemoPlay(double *left, double *right);



#endif /* MaximilianDemo_hpp */
