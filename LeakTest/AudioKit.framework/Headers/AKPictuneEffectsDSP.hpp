//
//  AKPictuneEffectsDSP.hpp
//  pictune
//
//  Created by Pictune on 11/5/20.
//  Copyright © 2020 Pictune. All rights reserved.
//

#pragma once

#import <AVFoundation/AVFoundation.h>
#import "AKParameterRamp.hpp"

typedef NS_ENUM (AUParameterAddress, AKPictuneEffectsParameter) {
    AKPictuneEffectsParameterGain,
    AKPictuneEffectsParameterRampDuration,
    AKPictuneEffectsParameterRampType
};

#ifndef __cplusplus

AKDSPRef createPictuneEffectsDSP(int channelCount, double sampleRate);

#else

#import "AKDSPBase.hpp"
#import "AKSoundpipeDSPBase.hpp"
#import "PfxS1DSPCompressor.hpp"


struct AKPictuneEffectsDSP : AKSoundpipeDSPBase {
private:
    struct InternalData;
    std::unique_ptr<InternalData> data;
    sp_data *sp = nullptr;

public:
    // BITCRUSHER
    float bitcrushIncr = 1.f;
    float bitcrushIndex = 0.f;
    float bitcrushSampleIndex = 0.f;
    float bitcrushValue = 0.f;
    
    // PHASER
    sp_phaser *phaser0;
    
    // DELAY
    sp_vdelay *delayL;
    sp_vdelay *delayR; // this seems to be used for stereo
    sp_vdelay *delayRR;
    sp_vdelay *delayFillIn;
    sp_crossfade *delayCrossfadeL;
    sp_crossfade *delayCrossfadeR;
    
    // REVERB
//    sp_revsc *reverbCostello;
//    PfxS1Compressor mCompReverbWet;
//    sp_crossfade *revCrossfadeL;
//    sp_crossfade *revCrossfadeR;
    
    AKPictuneEffectsDSP();

    void setParameter(AUParameterAddress address, float value, bool immediate) override;
    float getParameter(AUParameterAddress address) override;
    void process(AUAudioFrameCount frameCount, AUAudioFrameCount bufferOffset) override;
    void start() override;
    void stop() override;
    void init(int channelCount, double sampleRate) override;
    void deinit() override;
    int spSampleRate();
};

#endif
