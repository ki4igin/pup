/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ParalaxCalc_fixpt_types.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 23-Mar-2021 21:16:28
 */

#ifndef PARALAXCALC_FIXPT_TYPES_H
#define PARALAXCALC_FIXPT_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_uint96m_T
#define typedef_uint96m_T

typedef struct {
  uint32_T chunks[3];
} uint96m_T;

#endif                                 /*typedef_uint96m_T*/

#ifndef typedef_uint64m_T
#define typedef_uint64m_T

typedef struct {
  uint32_T chunks[2];
} uint64m_T;

#endif                                 /*typedef_uint64m_T*/

#ifndef typedef_int96m_T
#define typedef_int96m_T

typedef struct {
  uint32_T chunks[3];
} int96m_T;

#endif                                 /*typedef_int96m_T*/

#ifndef typedef_int64m_T
#define typedef_int64m_T

typedef struct {
  uint32_T chunks[2];
} int64m_T;

#endif                                 /*typedef_int64m_T*/
#endif

/*
 * File trailer for ParalaxCalc_fixpt_types.h
 *
 * [EOF]
 */
