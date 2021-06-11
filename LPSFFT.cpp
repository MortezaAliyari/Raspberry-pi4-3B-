//
// File: LPSFFT.cpp
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
#include "LPSFFT.h"

// Function for MATLAB Function: '<S1>/FindIndex'
void LPSAN_V57ModelClass::FFTImplementationCallback_getba(creal_T y[131072],
  int32_T yoff, const creal_T reconVar1[65536], const creal_T reconVar2[65536],
  const int32_T wrapIndex[65536], int32_T hnRows)
{
  int32_T i;
  int32_T iterVar;
  int32_T temp1_re_tmp;
  int32_T temp2_re_tmp_tmp;
  int32_T tmp;
  iterVar = hnRows / 2;
  LPSFFT_B.temp1_re = y[yoff].re;
  LPSFFT_B.temp1_im = y[yoff].im;
  LPSFFT_B.temp2_re = y[yoff].re * reconVar1[0].im + y[yoff].im * reconVar1[0].
    re;
  LPSFFT_B.temp2_im = -y[yoff].im;
  y[yoff].re = ((y[yoff].re * reconVar1[0].re - y[yoff].im * reconVar1[0].im) +
                (LPSFFT_B.temp1_re * reconVar2[0].re - LPSFFT_B.temp2_im *
                 reconVar2[0].im)) * 0.5;
  y[yoff].im = ((LPSFFT_B.temp1_re * reconVar2[0].im + LPSFFT_B.temp2_im *
                 reconVar2[0].re) + LPSFFT_B.temp2_re) * 0.5;
  tmp = yoff + hnRows;
  y[tmp].re = ((LPSFFT_B.temp1_re * reconVar2[0].re - LPSFFT_B.temp1_im *
                reconVar2[0].im) + (LPSFFT_B.temp1_re * reconVar1[0].re -
    -LPSFFT_B.temp1_im * reconVar1[0].im)) * 0.5;
  y[tmp].im = ((LPSFFT_B.temp1_re * reconVar2[0].im + LPSFFT_B.temp1_im *
                reconVar2[0].re) + (LPSFFT_B.temp1_re * reconVar1[0].im +
    -LPSFFT_B.temp1_im * reconVar1[0].re)) * 0.5;
  for (i = 1; i < iterVar; i++) {
    temp1_re_tmp = yoff + i;
    LPSFFT_B.temp1_re = y[temp1_re_tmp].re;
    LPSFFT_B.temp1_im = y[temp1_re_tmp].im;
    temp2_re_tmp_tmp = yoff + wrapIndex[i];
    LPSFFT_B.temp2_re = y[temp2_re_tmp_tmp - 1].re;
    LPSFFT_B.temp2_im = y[temp2_re_tmp_tmp - 1].im;
    y[temp1_re_tmp].re = ((LPSFFT_B.temp1_re * reconVar1[i].re -
      LPSFFT_B.temp1_im * reconVar1[i].im) + (LPSFFT_B.temp2_re * reconVar2[i].
      re - -LPSFFT_B.temp2_im * reconVar2[i].im)) * 0.5;
    y[temp1_re_tmp].im = ((LPSFFT_B.temp1_re * reconVar1[i].im +
      LPSFFT_B.temp1_im * reconVar1[i].re) + (LPSFFT_B.temp2_re * reconVar2[i].
      im + -LPSFFT_B.temp2_im * reconVar2[i].re)) * 0.5;
    temp1_re_tmp = tmp + i;
    y[temp1_re_tmp].re = ((LPSFFT_B.temp1_re * reconVar2[i].re -
      LPSFFT_B.temp1_im * reconVar2[i].im) + (LPSFFT_B.temp2_re * reconVar1[i].
      re - -LPSFFT_B.temp2_im * reconVar1[i].im)) * 0.5;
    y[temp1_re_tmp].im = ((LPSFFT_B.temp1_re * reconVar2[i].im +
      LPSFFT_B.temp1_im * reconVar2[i].re) + (LPSFFT_B.temp2_re * reconVar1[i].
      im + -LPSFFT_B.temp2_im * reconVar1[i].re)) * 0.5;
    y[temp2_re_tmp_tmp - 1].re = ((reconVar1[wrapIndex[i] - 1].re *
      LPSFFT_B.temp2_re - reconVar1[wrapIndex[i] - 1].im * LPSFFT_B.temp2_im) +
      (reconVar2[wrapIndex[i] - 1].re * LPSFFT_B.temp1_re -
       reconVar2[wrapIndex[i] - 1].im * -LPSFFT_B.temp1_im)) * 0.5;
    y[temp2_re_tmp_tmp - 1].im = ((reconVar1[wrapIndex[i] - 1].im *
      LPSFFT_B.temp2_re + reconVar1[wrapIndex[i] - 1].re * LPSFFT_B.temp2_im) +
      (reconVar2[wrapIndex[i] - 1].im * LPSFFT_B.temp1_re +
       reconVar2[wrapIndex[i] - 1].re * -LPSFFT_B.temp1_im)) * 0.5;
    temp1_re_tmp = (temp2_re_tmp_tmp + hnRows) - 1;
    y[temp1_re_tmp].re = ((reconVar2[wrapIndex[i] - 1].re * LPSFFT_B.temp2_re -
      reconVar2[wrapIndex[i] - 1].im * LPSFFT_B.temp2_im) +
                          (reconVar1[wrapIndex[i] - 1].re * LPSFFT_B.temp1_re -
      reconVar1[wrapIndex[i] - 1].im * -LPSFFT_B.temp1_im)) * 0.5;
    y[temp1_re_tmp].im = ((reconVar2[wrapIndex[i] - 1].im * LPSFFT_B.temp2_re +
      reconVar2[wrapIndex[i] - 1].re * LPSFFT_B.temp2_im) +
                          (reconVar1[wrapIndex[i] - 1].im * LPSFFT_B.temp1_re +
      reconVar1[wrapIndex[i] - 1].re * -LPSFFT_B.temp1_im)) * 0.5;
  }

  if (iterVar != 0) {
    temp1_re_tmp = yoff + iterVar;
    LPSFFT_B.temp1_re = y[temp1_re_tmp].re;
    LPSFFT_B.temp1_im = y[temp1_re_tmp].im;
    LPSFFT_B.temp2_re = LPSFFT_B.temp1_re * reconVar2[iterVar].re;
    LPSFFT_B.temp2_im = LPSFFT_B.temp1_re * reconVar1[iterVar].re;
    y[temp1_re_tmp].re = ((LPSFFT_B.temp2_im - LPSFFT_B.temp1_im *
      reconVar1[iterVar].im) + (LPSFFT_B.temp2_re - -LPSFFT_B.temp1_im *
      reconVar2[iterVar].im)) * 0.5;
    LPSFFT_B.d = LPSFFT_B.temp1_re * reconVar2[iterVar].im;
    LPSFFT_B.temp1_re *= reconVar1[iterVar].im;
    y[temp1_re_tmp].im = ((LPSFFT_B.temp1_re + LPSFFT_B.temp1_im *
      reconVar1[iterVar].re) + (LPSFFT_B.d + -LPSFFT_B.temp1_im *
      reconVar2[iterVar].re)) * 0.5;
    tmp += iterVar;
    y[tmp].re = ((LPSFFT_B.temp2_re - LPSFFT_B.temp1_im * reconVar2[iterVar].im)
                 + (LPSFFT_B.temp2_im - -LPSFFT_B.temp1_im * reconVar1[iterVar].
                    im)) * 0.5;
    y[tmp].im = ((LPSFFT_B.d + LPSFFT_B.temp1_im * reconVar2[iterVar].re) +
                 (LPSFFT_B.temp1_re + -LPSFFT_B.temp1_im * reconVar1[iterVar].re))
      * 0.5;
  }
}

// Function for MATLAB Function: '<S1>/FindIndex'
void LPSAN_V57ModelClass::FFTImplementationCallback_doHal(const real_T x[131072],
  int32_T xoffInit, creal_T y[131072], const real_T costab[65537], const real_T
  sintab[65537])
{
  int32_T b_i;
  int32_T iDelta;
  int32_T iheight;
  int32_T ihi;
  int32_T istart;
  int32_T iy;
  int32_T j;
  int32_T ju;
  int32_T temp_re_tmp;
  boolean_T tst;
  for (iDelta = 0; iDelta < 32768; iDelta++) {
    ju = ((iDelta + 1) << 1) - 2;
    LPSFFT_B.hcostab[iDelta] = costab[ju];
    LPSFFT_B.hsintab[iDelta] = sintab[ju];
  }

  for (iDelta = 0; iDelta < 65536; iDelta++) {
    LPSFFT_B.temp_re = costab[iDelta];
    LPSFFT_B.temp_im = sintab[iDelta];
    LPSFFT_B.reconVar1[iDelta].re = LPSFFT_B.temp_im + 1.0;
    LPSFFT_B.reconVar1[iDelta].im = -LPSFFT_B.temp_re;
    LPSFFT_B.reconVar2[iDelta].re = 1.0 - LPSFFT_B.temp_im;
    LPSFFT_B.reconVar2[iDelta].im = LPSFFT_B.temp_re;
    if (iDelta + 1 != 1) {
      LPSFFT_B.wrapIndex[iDelta] = 65537 - iDelta;
    } else {
      LPSFFT_B.wrapIndex[0] = 1;
    }
  }

  ju = 0;
  iy = 1;
  for (iDelta = 0; iDelta < 65535; iDelta++) {
    LPSFFT_B.bitrevIndex[iDelta] = iy;
    iy = 65536;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju + 1;
  }

  LPSFFT_B.bitrevIndex[65535] = iy;
  ju = xoffInit;
  for (iDelta = 0; iDelta < 65536; iDelta++) {
    iy = LPSFFT_B.bitrevIndex[iDelta];
    y[iy - 1].re = x[ju];
    y[iy - 1].im = x[ju + 1];
    ju += 2;
  }

  for (iDelta = 0; iDelta <= 65535; iDelta += 2) {
    LPSFFT_B.temp_re = y[iDelta + 1].re;
    LPSFFT_B.temp_im = y[iDelta + 1].im;
    y[iDelta + 1].re = y[iDelta].re - y[iDelta + 1].re;
    y[iDelta + 1].im = y[iDelta].im - y[iDelta + 1].im;
    y[iDelta].re += LPSFFT_B.temp_re;
    y[iDelta].im += LPSFFT_B.temp_im;
  }

  iDelta = 2;
  ju = 4;
  iy = 16384;
  iheight = 65533;
  while (iy > 0) {
    for (b_i = 0; b_i < iheight; b_i += ju) {
      temp_re_tmp = b_i + iDelta;
      LPSFFT_B.temp_re = y[temp_re_tmp].re;
      LPSFFT_B.temp_im = y[temp_re_tmp].im;
      y[temp_re_tmp].re = y[b_i].re - LPSFFT_B.temp_re;
      y[temp_re_tmp].im = y[b_i].im - LPSFFT_B.temp_im;
      y[b_i].re += LPSFFT_B.temp_re;
      y[b_i].im += LPSFFT_B.temp_im;
    }

    istart = 1;
    for (j = iy; j < 32768; j += iy) {
      LPSFFT_B.twid_re = LPSFFT_B.hcostab[j];
      LPSFFT_B.twid_im = LPSFFT_B.hsintab[j];
      b_i = istart;
      ihi = istart + iheight;
      while (b_i < ihi) {
        temp_re_tmp = b_i + iDelta;
        LPSFFT_B.temp_re = y[temp_re_tmp].re * LPSFFT_B.twid_re - y[temp_re_tmp]
          .im * LPSFFT_B.twid_im;
        LPSFFT_B.temp_im = y[temp_re_tmp].im * LPSFFT_B.twid_re + y[temp_re_tmp]
          .re * LPSFFT_B.twid_im;
        y[temp_re_tmp].re = y[b_i].re - LPSFFT_B.temp_re;
        y[temp_re_tmp].im = y[b_i].im - LPSFFT_B.temp_im;
        y[b_i].re += LPSFFT_B.temp_re;
        y[b_i].im += LPSFFT_B.temp_im;
        b_i += ju;
      }

      istart++;
    }

    iy /= 2;
    iDelta = ju;
    ju += ju;
    iheight -= iDelta;
  }

  FFTImplementationCallback_getba(y, 0, LPSFFT_B.reconVar1, LPSFFT_B.reconVar2,
    LPSFFT_B.wrapIndex, 65536);
}

// Function for MATLAB Function: '<S1>/FindIndex'
void LPSAN_V57ModelClass::LPSFFT_fft(const real_T x[131072], creal_T y[131072])
{
  int32_T k;
  LPSFFT_B.costab1q[0] = 1.0;
  for (k = 0; k < 16384; k++) {
    LPSFFT_B.costab1q[k + 1] = cos((static_cast<real_T>(k) + 1.0) *
      4.7936899621426287E-5);
  }

  for (k = 0; k < 16383; k++) {
    LPSFFT_B.costab1q[k + 16385] = sin((32768.0 - (static_cast<real_T>(k) +
      16385.0)) * 4.7936899621426287E-5);
  }

  LPSFFT_B.costab1q[32768] = 0.0;
  LPSFFT_B.costab[0] = 1.0;
  LPSFFT_B.sintab[0] = 0.0;
  for (k = 0; k < 32768; k++) {
    LPSFFT_B.costab_tmp = LPSFFT_B.costab1q[k + 1];
    LPSFFT_B.costab[k + 1] = LPSFFT_B.costab_tmp;
    LPSFFT_B.sintab_tmp = -LPSFFT_B.costab1q[32767 - k];
    LPSFFT_B.sintab[k + 1] = LPSFFT_B.sintab_tmp;
    LPSFFT_B.costab[k + 32769] = LPSFFT_B.sintab_tmp;
    LPSFFT_B.sintab[k + 32769] = -LPSFFT_B.costab_tmp;
  }

  FFTImplementationCallback_doHal(x, 0, y, LPSFFT_B.costab, LPSFFT_B.sintab);
}

// Model step function
void LPSAN_V57ModelClass::step()
{
  // Outputs for Atomic SubSystem: '<Root>/Algorithm'
  // MATLAB Function: '<S1>/FindIndex' incorporates:
  //   Inport: '<Root>/I'
  //   Inport: '<Root>/V'

  LPSFFT_fft(LPSFFT_U.V, LPSFFT_Y.Vfft);
  LPSFFT_fft(LPSFFT_U.I, LPSFFT_Y.Ifft);

  // End of Outputs for SubSystem: '<Root>/Algorithm'
  for (LPSFFT_B.i = 0; LPSFFT_B.i < 131072; LPSFFT_B.i++) {
    // Outputs for Atomic SubSystem: '<Root>/Algorithm'
    // Outport: '<Root>/Vfftreal' incorporates:
    //   MATLAB Function: '<S1>/FindIndex'

    LPSFFT_Y.Vfftreal[LPSFFT_B.i] = LPSFFT_Y.Vfft[LPSFFT_B.i].re;

    // Outport: '<Root>/Vfftimag' incorporates:
    //   MATLAB Function: '<S1>/FindIndex'

    LPSFFT_Y.Vfftimag[LPSFFT_B.i] = LPSFFT_Y.Vfft[LPSFFT_B.i].im;

    // Outport: '<Root>/Ifftreal' incorporates:
    //   MATLAB Function: '<S1>/FindIndex'

    LPSFFT_Y.Ifftreal[LPSFFT_B.i] = LPSFFT_Y.Ifft[LPSFFT_B.i].re;

    // Outport: '<Root>/Ifftimag' incorporates:
    //   MATLAB Function: '<S1>/FindIndex'

    LPSFFT_Y.Ifftimag[LPSFFT_B.i] = LPSFFT_Y.Ifft[LPSFFT_B.i].im;

    // End of Outputs for SubSystem: '<Root>/Algorithm'
  }

  // Outputs for Atomic SubSystem: '<Root>/Algorithm'
  // Outport: '<Root>/NV' incorporates:
  //   MATLAB Function: '<S1>/FindIndex'

  LPSFFT_Y.NV = 131072.0;

  // Outport: '<Root>/NI' incorporates:
  //   MATLAB Function: '<S1>/FindIndex'

  LPSFFT_Y.NI = 131072.0;

  // End of Outputs for SubSystem: '<Root>/Algorithm'
}

// Model initialize function
void LPSAN_V57ModelClass::initialize()
{
  // (no initialization code required)
}

// Model terminate function
void LPSAN_V57ModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
LPSAN_V57ModelClass::LPSAN_V57ModelClass() :
  LPSFFT_U(),
  LPSFFT_Y(),
  LPSFFT_B(),
  LPSFFT_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
LPSAN_V57ModelClass::~LPSAN_V57ModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_LPSFFT_T * LPSAN_V57ModelClass::getRTM()
{
  return (&LPSFFT_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
