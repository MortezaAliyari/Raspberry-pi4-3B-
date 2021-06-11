//
// File: LPSFFT.h
//
// Code generated for Simulink model 'LPSFFT'.
//
// Model version                  : 3.6
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Tue May 11 10:11:12 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_LPSFFT_h_
#define RTW_HEADER_LPSFFT_h_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"

// Model Code Variants

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM_LPSFFT_T RT_MODEL_LPSFFT_T;

// Block signals (default storage)
typedef struct {
  creal_T reconVar1[65536];
  creal_T reconVar2[65536];
  real_T costab[65537];
  real_T sintab[65537];
  real_T costab1q[32769];
  real_T hcostab[32768];
  real_T hsintab[32768];
  int32_T wrapIndex[65536];
  int32_T bitrevIndex[65536];
  real_T sintab_tmp;
  real_T costab_tmp;
  real_T temp_re;
  real_T temp_im;
  real_T twid_re;
  real_T twid_im;
  real_T temp1_re;
  real_T temp1_im;
  real_T temp2_re;
  real_T temp2_im;
  real_T d;
  int32_T i;
} B_LPSFFT_T;

// External inputs (root inport signals with default storage)
typedef struct {
  real_T V[131072]={0};                    // '<Root>/V'
  real_T I[131072]={0};                    // '<Root>/I'
} ExtU_LPSFFT_T;

// External outputs (root outports fed by signals with default storage)
typedef struct {
  creal_T Vfft[131072];                // '<Root>/Vfft'
  creal_T Ifft[131072];                // '<Root>/Ifft'
  real_T Vfftreal[131072];             // '<Root>/Vfftreal'
  real_T Vfftimag[131072];             // '<Root>/Vfftimag'
  real_T Ifftreal[131072];             // '<Root>/Ifftreal'
  real_T Ifftimag[131072];             // '<Root>/Ifftimag'
  real_T NV;                           // '<Root>/NV'
  real_T NI;                           // '<Root>/NI'
} ExtY_LPSFFT_T;

// Real-time Model Data Structure
struct tag_RTM_LPSFFT_T {
  const char_T *errorStatus;
};

// Class declaration for model LPSFFT
class LPSAN_V57ModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU_LPSFFT_T LPSFFT_U;

  // External outputs
  ExtY_LPSFFT_T LPSFFT_Y;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  LPSAN_V57ModelClass();

  // Destructor
  ~LPSAN_V57ModelClass();

  // Real-Time Model get method
  RT_MODEL_LPSFFT_T * getRTM();

  // private data and function members
 private:
  // Block signals
  B_LPSFFT_T LPSFFT_B;

  // Real-Time Model
  RT_MODEL_LPSFFT_T LPSFFT_M;

  // private member function(s) for subsystem '<Root>'
  void FFTImplementationCallback_getba(creal_T y[131072], int32_T yoff, const
    creal_T reconVar1[65536], const creal_T reconVar2[65536], const int32_T
    wrapIndex[65536], int32_T hnRows);
  void FFTImplementationCallback_doHal(const real_T x[131072], int32_T xoffInit,
    creal_T y[131072], const real_T costab[65537], const real_T sintab[65537]);
  void LPSFFT_fft(const real_T x[131072], creal_T y[131072]);
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'LPSFFT'
//  '<S1>'   : 'LPSFFT/Algorithm'
//  '<S2>'   : 'LPSFFT/Algorithm/FindIndex'

#endif                                 // RTW_HEADER_LPSFFT_h_

//
// File trailer for generated code.
//
// [EOF]
//
