//
//  PfxS1DSPCompressor.hpp
//  AudioKit
//
//  Created by Pictune on 11/12/20.
//  Copyright © 2020 AudioKit. All rights reserved.
//

#import "AudioKit/AKSoundpipeKernel.hpp"

#ifndef PFXS1DSPCompressor_h
#define PFXS1DSPCompressor_h

struct PfxSPCompressorDeleter {
    void operator()(sp_compressor* compInst) const {
        sp_compressor_destroy(&compInst);
    }
};

struct PfxSPCompressorAllocator {
    static auto allocate() -> sp_compressor* {
        sp_compressor* compPtr;
        sp_compressor_create(&compPtr);
        return compPtr;
    }
};

struct PfxS1Compressor {

    PfxS1Compressor() {};
//    S1Compressor(S1Compressor&&) = delete;
//    S1Compressor(const S1Compressor&) = delete;

    PfxS1Compressor(sp_data* sp) :
        mSp(sp),
        mCompressorL(PfxSPCompressorAllocator::allocate()),
        mCompressorR(PfxSPCompressorAllocator::allocate())
    {
        sp_compressor_init(mSp, mCompressorR.get());
        sp_compressor_init(mSp, mCompressorL.get());
    }

    void compute(float &inL, float &inR, float &outL, float &outR) {
        configure(mCompressorR.get());
        configure(mCompressorL.get());
        compute(mCompressorR.get(), inR, outR);
        compute(mCompressorL.get(), inL, outL);
        outR *= 1.88; // compressorReverbWetMakeupGain
        outL *= 1.88; // compressorReverbWetMakeupGain
    }

private:

    void configure(sp_compressor *comp) {
        *comp->atk = 0.001; // compressorReverbWetAttack
        *comp->rel = 0.15; // compressorReverbWetRelease
        *comp->thresh = -8; // compressorReverbWetThreshold
        *comp->ratio = 13; // compressorReverbWetRatio
    }

    void compute(sp_compressor *comp, float &in, float &out) {
        sp_compressor_compute(mSp, comp, &in, &out);
    }

    // DSP Internals
    std::unique_ptr<sp_compressor, PfxSPCompressorDeleter> mCompressorR;
    std::unique_ptr<sp_compressor, PfxSPCompressorDeleter> mCompressorL;
    sp_data* mSp;
};

#endif /* PfxS1DSPCompressor_h */
