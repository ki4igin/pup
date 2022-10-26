/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ParalaxCalc_fixpt.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 23-Mar-2021 21:16:28
 */

#ifndef PARALAXCALC_FIXPT_H
#define PARALAXCALC_FIXPT_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

/* Function Declarations */
extern void ParalaxCalc_fixpt(
    const uint32_T az_in,
    const int32_T  el_in,
    const uint32_T r_in,
    const int32_T  x_base,
    const int32_T  y_base,
    const int32_T  z_base,
    uint32_T*      az_out,
    int32_T*       el_out,
    uint32_T*      r_out);
extern void ParalaxCalc_fixpt_initialize(void);

#ifdef __cplusplus
}
#endif
#endif

/*
 * File trailer for ParalaxCalc_fixpt.h
 *
 * [EOF]
 */
