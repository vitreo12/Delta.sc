#include "SC_PlugIn.h"

static InterfaceTable *ft;

struct Delta : public Unit 
{
    float prevSample = 0.0f;
};

static void Delta_next_a_a(Delta* unit, int inNumSamples);
static void Delta_next_a_k(Delta* unit, int inNumSamples);
static void Delta_next_k_k(Delta* unit, int inNumSamples);

static void Delta_Ctor(Delta* unit);

void Delta_Ctor(Delta* unit) 
{
    if(INRATE(0) == calc_FullRate && INRATE(1) == calc_FullRate)
        SETCALC(Delta_next_a_a);
    else if(INRATE(0) == calc_BufRate && INRATE(1) == calc_BufRate)
        SETCALC(Delta_next_k_k);
    else if(INRATE(0) == calc_FullRate && INRATE(1) == calc_BufRate)
        SETCALC(Delta_next_a_k);
    else
        SETCALC(Delta_next_k_k);
       
    //calc one sample
    Delta_next_a_k(unit, 1);
}

void Delta_next_a_a(Delta* unit, int inNumSamples) 
{
    float* in     = IN(0);
    float* thresh = IN(1);

    float* out = OUT(0);

    float prevSample = unit->prevSample;

    for(int i = 0; i < inNumSamples; i++)
    {
        float in_val     = in[i];
        float thresh_val = thresh[i];

        float out_val = 0.0f;

        if(abs(in_val - prevSample) >= thresh_val)
            out_val = 1.0f;

        out[i] = out_val;

        prevSample = in_val;
    }

    unit->prevSample = prevSample;
}

void Delta_next_a_k(Delta* unit, int inNumSamples) 
{
    float* in = IN(0);
    
    float thresh_val = IN0(1);

    float* out = OUT(0);

    float prevSample = unit->prevSample;

    for(int i = 0; i < inNumSamples; i++)
    {
        float in_val = in[i];

        float out_val = 0.0f;

        if(abs(in_val - prevSample) >= thresh_val)
            out_val = 1.0f;

        out[i] = out_val;

        prevSample = in_val;
    }

    unit->prevSample = prevSample;
}

void Delta_next_k_k(Delta* unit, int inNumSamples) 
{
    float in_val = IN0(0);
    
    float thresh_val = IN0(1);

    float* out = OUT(0);

    float out_val = 0.0f;

    if(abs(in_val - unit->prevSample) >= thresh_val)
        out_val = 1.0f;

    for(int i = 0; i < inNumSamples; i++)
        out[i] = out_val;

    unit->prevSample = in_val;
}

PluginLoad(DeltaUGens) 
{
    ft = inTable; 
    DefineSimpleUnit(Delta);
}
