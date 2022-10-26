/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ParalaxCalc_fixpt.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 23-Mar-2021 21:16:28
 */

/* Include Files */
#include "ParalaxCalc_fixpt.h"
#include "ParalaxCalc_fixpt_types.h"
#include <string.h>

/* Variable Definitions */
static int16_T FI_SIN_COS_LUT[256];
static int16_T b_FI_SIN_COS_LUT[256];
static uint16_T ATAN_UFRAC_LUT[257];

/* Function Declarations */
static boolean_T MultiWord2Bool(const uint32_T u[], int32_T n);
static int32_T MultiWord2sLong(const uint32_T u[]);
static uint32_T MultiWord2uLong(const uint32_T u[]);
static void MultiWordAdd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
static void MultiWordNeg(const uint32_T u1[], uint32_T y[], int32_T n);
static void MultiWordSetSignedMax(uint32_T y[], int32_T n);
static void MultiWordSetSignedMin(uint32_T y[], int32_T n);
static void MultiWordSignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[]);
static void MultiWordSub(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
static void MultiWordUnsignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[]);
static int32_T asr_s32(int32_T u, uint32_T n);
static void atan2_fi_lut_private_init(void);
static int16_T b_atan2(int32_T y, int32_T x);
static void cos_init(void);
static uint32_T div_nzp_repeat_u32_sat(uint32_T numerator, uint32_T denominator,
  uint32_T nRepeatSub);
static int32_T div_repeat_s32_sat(int32_T numerator, int32_T denominator,
  uint32_T nRepeatSub);
static int32_T mul_ssu32_loSR(int32_T a, uint32_T b, uint32_T aShift);
static uint32_T mul_u32_loSR(uint32_T a, uint32_T b, uint32_T aShift);
static void mul_wide_su32(int32_T in0, uint32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo);
static void mul_wide_u32(uint32_T in0, uint32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo);
static void sLong2MultiWord(int32_T u, uint32_T y[], int32_T n);
static void sMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n);
static int32_T sMultiWordCmp(const uint32_T u1[], const uint32_T u2[], int32_T n);
static void sMultiWordDivFloor(const uint32_T u1[], int32_T n1, const uint32_T
  u2[], int32_T n2, uint32_T b_y1[], int32_T m1, uint32_T y2[], int32_T m2,
  uint32_T t1[], int32_T l1, uint32_T t2[], int32_T l2);
static boolean_T sMultiWordEq(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
static boolean_T sMultiWordGt(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
static boolean_T sMultiWordLe(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
static boolean_T sMultiWordLt(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
static void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n);
static void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
static void sin_init(void);
static void ssuMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n);
static void uLong2MultiWord(uint32_T u, uint32_T y[], int32_T n);
static void uMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n);
static int32_T uMultiWordDiv(uint32_T a[], int32_T na, uint32_T b[], int32_T nb,
  uint32_T q[], int32_T nq, uint32_T r[], int32_T nr);
static void uMultiWordInc(uint32_T y[], int32_T n);
static void uMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n);
static void uMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
static void uMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);

/* Function Definitions */
/*
 * Arguments    : const uint32_T u[]
 *                int32_T n
 * Return Type  : boolean_T
 */
static boolean_T MultiWord2Bool(const uint32_T u[], int32_T n)
{
  int32_T i;
  boolean_T y;
  y = false;
  i = 0;
  while ((i < n) && (!y)) {
    if (u[i] != 0U) {
      y = true;
    }

    i++;
  }

  return y;
}

/*
 * Arguments    : const uint32_T u[]
 * Return Type  : int32_T
 */
static int32_T MultiWord2sLong(const uint32_T u[])
{
  return (int32_T)u[0];
}

/*
 * Arguments    : const uint32_T u[]
 * Return Type  : uint32_T
 */
static uint32_T MultiWord2uLong(const uint32_T u[])
{
  return u[0];
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void MultiWordAdd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n)
{
  int32_T carry = 0;
  int32_T i;
  uint32_T u1i;
  uint32_T yi;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i + u2[i]) + ((uint32_T)carry);
    y[i] = yi;
    if (((uint32_T)carry) != 0U) {
      carry = (int32_T)((yi <= u1i) ? 1 : 0);
    } else {
      carry = (int32_T)((yi < u1i) ? 1 : 0);
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void MultiWordNeg(const uint32_T u1[], uint32_T y[], int32_T n)
{
  int32_T carry = 1;
  int32_T i;
  uint32_T yi;
  for (i = 0; i < n; i++) {
    yi = (~u1[i]) + ((uint32_T)carry);
    y[i] = yi;
    carry = (int32_T)((yi < ((uint32_T)carry)) ? 1 : 0);
  }
}

/*
 * Arguments    : uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void MultiWordSetSignedMax(uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T n1;
  n1 = n - 1;
  for (i = 0; i < n1; i++) {
    y[i] = MAX_uint32_T;
  }

  y[n - 1] = 2147483647U;
}

/*
 * Arguments    : uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void MultiWordSetSignedMin(uint32_T y[], int32_T n)
{
  int32_T n1;
  n1 = n - 1;
  if (0 <= (n1 - 1)) {
    memset(&y[0], 0, ((uint32_T)n1) * (sizeof(uint32_T)));
  }

  y[n - 1] = 2147483648U;
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T n2
 *                uint32_T y[]
 * Return Type  : void
 */
static void MultiWordSignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[])
{
  int32_T n1m1;
  uint32_T mask;
  uint32_T ys;
  n1m1 = n1 - 1;
  if (0 <= (n1m1 - 1)) {
    memcpy(&y[0], &u1[0], ((uint32_T)n1m1) * (sizeof(uint32_T)));
  }

  mask = (1U << (31U - n2));
  if ((u1[n1 - 1] & mask) != 0U) {
    ys = MAX_uint32_T;
  } else {
    ys = 0U;
  }

  mask = (mask << 1U) - 1U;
  y[n1 - 1] = (u1[n1 - 1] & mask) | ((~mask) & ys);
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void MultiWordSub(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n)
{
  int32_T borrow = 0;
  int32_T i;
  uint32_T u1i;
  uint32_T yi;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i - u2[i]) - ((uint32_T)borrow);
    y[i] = yi;
    if (((uint32_T)borrow) != 0U) {
      borrow = (int32_T)((yi >= u1i) ? 1 : 0);
    } else {
      borrow = (int32_T)((yi > u1i) ? 1 : 0);
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T n2
 *                uint32_T y[]
 * Return Type  : void
 */
static void MultiWordUnsignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[])
{
  int32_T n1m1;
  n1m1 = n1 - 1;
  if (0 <= (n1m1 - 1)) {
    memcpy(&y[0], &u1[0], ((uint32_T)n1m1) * (sizeof(uint32_T)));
  }

  y[n1 - 1] = u1[n1 - 1] & ((1U << (32U - n2)) - 1U);
}

/*
 * Arguments    : int32_T u
 *                uint32_T n
 * Return Type  : int32_T
 */
static int32_T asr_s32(int32_T u, uint32_T n)
{
  int32_T y;
  if (u >= 0) {
    y = (int32_T)((uint32_T)(((uint32_T)u) >> n));
  } else {
    y = (-((int32_T)((uint32_T)(((uint32_T)((int32_T)(-1 - u))) >> n)))) - 1;
  }

  return y;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void atan2_fi_lut_private_init(void)
{
  static const uint16_T uv[257] = { 0U, 256U, 512U, 768U, 1024U, 1280U, 1536U,
    1792U, 2047U, 2303U, 2559U, 2814U, 3070U, 3325U, 3580U, 3836U, 4091U, 4346U,
    4600U, 4855U, 5110U, 5364U, 5618U, 5872U, 6126U, 6380U, 6633U, 6887U, 7140U,
    7392U, 7645U, 7898U, 8150U, 8402U, 8653U, 8905U, 9156U, 9407U, 9657U, 9908U,
    10158U, 10408U, 10657U, 10906U, 11155U, 11403U, 11652U, 11899U, 12147U,
    12394U, 12641U, 12887U, 13133U, 13379U, 13624U, 13869U, 14114U, 14358U,
    14601U, 14845U, 15088U, 15330U, 15572U, 15814U, 16055U, 16296U, 16536U,
    16776U, 17015U, 17254U, 17492U, 17730U, 17968U, 18205U, 18441U, 18677U,
    18913U, 19148U, 19382U, 19616U, 19850U, 20083U, 20315U, 20547U, 20779U,
    21009U, 21240U, 21469U, 21699U, 21927U, 22156U, 22383U, 22610U, 22836U,
    23062U, 23288U, 23512U, 23737U, 23960U, 24183U, 24406U, 24627U, 24849U,
    25069U, 25289U, 25509U, 25727U, 25946U, 26163U, 26380U, 26597U, 26813U,
    27028U, 27242U, 27456U, 27670U, 27882U, 28094U, 28306U, 28517U, 28727U,
    28936U, 29145U, 29354U, 29561U, 29768U, 29975U, 30180U, 30386U, 30590U,
    30794U, 30997U, 31200U, 31402U, 31603U, 31803U, 32003U, 32203U, 32401U,
    32600U, 32797U, 32994U, 33190U, 33385U, 33580U, 33774U, 33968U, 34160U,
    34353U, 34544U, 34735U, 34925U, 35115U, 35304U, 35492U, 35680U, 35867U,
    36053U, 36239U, 36424U, 36608U, 36792U, 36975U, 37158U, 37340U, 37521U,
    37701U, 37881U, 38060U, 38239U, 38417U, 38594U, 38771U, 38947U, 39123U,
    39297U, 39472U, 39645U, 39818U, 39990U, 40162U, 40333U, 40503U, 40673U,
    40842U, 41010U, 41178U, 41346U, 41512U, 41678U, 41844U, 42008U, 42172U,
    42336U, 42499U, 42661U, 42823U, 42984U, 43145U, 43304U, 43464U, 43622U,
    43780U, 43938U, 44095U, 44251U, 44407U, 44562U, 44716U, 44870U, 45024U,
    45176U, 45328U, 45480U, 45631U, 45781U, 45931U, 46080U, 46229U, 46377U,
    46525U, 46672U, 46818U, 46964U, 47109U, 47254U, 47398U, 47542U, 47685U,
    47827U, 47969U, 48111U, 48251U, 48392U, 48531U, 48671U, 48809U, 48947U,
    49085U, 49222U, 49359U, 49495U, 49630U, 49765U, 49899U, 50033U, 50167U,
    50299U, 50432U, 50563U, 50695U, 50826U, 50956U, 51086U, 51215U, 51344U,
    51472U };

  memcpy(&ATAN_UFRAC_LUT[0], &uv[0], 257U * (sizeof(uint16_T)));
}

/*
 * Arguments    : int32_T y
 *                int32_T x
 * Return Type  : int16_T
 */
static int16_T b_atan2(int32_T y, int32_T x)
{
  int32_T i;
  int32_T negThisX;
  int32_T negThisY;
  int16_T theta;
  uint16_T xfi;
  uint8_T slice_temp;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  guard1 = false;
  guard2 = false;
  if (y > 0) {
    if (x == y) {
      theta = 6434;
    } else if (x >= 0) {
      if (y <= x) {
        i = div_repeat_s32_sat(y, x, 16U);
        if (i < 0) {
          i = 0;
        } else {
          if (i > 65535) {
            i = 65535;
          }
        }

        xfi = (uint16_T)i;
      } else {
        i = div_repeat_s32_sat(x, y, 16U);
        if (i < 0) {
          i = 0;
        } else {
          if (i > 65535) {
            i = 65535;
          }
        }

        xfi = (uint16_T)i;
      }

      guard2 = true;
    } else {
      if (x <= MIN_int32_T) {
        negThisX = MAX_int32_T;
      } else {
        negThisX = -x;
      }

      if (y == negThisX) {
        theta = 19302;
      } else {
        if (y < negThisX) {
          i = div_repeat_s32_sat(y, negThisX, 16U);
          if (i < 0) {
            i = 0;
          } else {
            if (i > 65535) {
              i = 65535;
            }
          }

          xfi = (uint16_T)i;
        } else {
          i = div_repeat_s32_sat(negThisX, y, 16U);
          if (i < 0) {
            i = 0;
          } else {
            if (i > 65535) {
              i = 65535;
            }
          }

          xfi = (uint16_T)i;
        }

        guard2 = true;
      }
    }
  } else if (y < 0) {
    if (x == y) {
      theta = -19302;
    } else if (x >= 0) {
      if (y <= MIN_int32_T) {
        negThisY = MAX_int32_T;
      } else {
        negThisY = -y;
      }

      if (negThisY == x) {
        theta = -6434;
      } else {
        if (negThisY < x) {
          i = div_repeat_s32_sat(negThisY, x, 16U);
          if (i < 0) {
            i = 0;
          } else {
            if (i > 65535) {
              i = 65535;
            }
          }

          xfi = (uint16_T)i;
        } else {
          i = div_repeat_s32_sat(x, negThisY, 16U);
          if (i < 0) {
            i = 0;
          } else {
            if (i > 65535) {
              i = 65535;
            }
          }

          xfi = (uint16_T)i;
        }

        guard1 = true;
      }
    } else {
      if (x <= MIN_int32_T) {
        negThisX = MAX_int32_T;
      } else {
        negThisX = -x;
      }

      if (y <= MIN_int32_T) {
        negThisY = MAX_int32_T;
      } else {
        negThisY = -y;
      }

      if (negThisY <= negThisX) {
        i = div_repeat_s32_sat(negThisY, negThisX, 16U);
        if (i < 0) {
          i = 0;
        } else {
          if (i > 65535) {
            i = 65535;
          }
        }

        xfi = (uint16_T)i;
      } else {
        i = div_repeat_s32_sat(negThisX, negThisY, 16U);
        if (i < 0) {
          i = 0;
        } else {
          if (i > 65535) {
            i = 65535;
          }
        }

        xfi = (uint16_T)i;
      }

      guard1 = true;
    }
  } else if (x >= 0) {
    theta = 0;
  } else {
    theta = 25736;
  }

  if (guard2) {
    slice_temp = (uint8_T)(((uint32_T)xfi) >> 8U);
    theta = (int16_T)((uint32_T)(((uint32_T)((uint16_T)(((((uint32_T)
      ATAN_UFRAC_LUT[slice_temp]) << 14U) + (((uint32_T)((uint16_T)(mul_u32_loSR
      ((uint32_T)((int32_T)(((int32_T)xfi) & ((uint16_T)255))), (((uint32_T)
      ATAN_UFRAC_LUT[((int32_T)slice_temp) + 1]) << 14U) - (((uint32_T)
      ATAN_UFRAC_LUT[(int32_T)((uint32_T)(((uint32_T)xfi) >> 8U))]) << 14U), 8U)
      >> 14U))) << 14U)) >> 14U))) >> 3U));
    if (x >= 0) {
      if (x < y) {
        theta = (int16_T)(12867 - theta);
      }
    } else if (y > negThisX) {
      theta = (int16_T)(theta + 12867);
    } else {
      theta = (int16_T)(25735 - theta);
    }
  }

  if (guard1) {
    slice_temp = (uint8_T)(((uint32_T)xfi) >> 8U);
    theta = (int16_T)((uint32_T)(((uint32_T)((uint16_T)(((((uint32_T)
      ATAN_UFRAC_LUT[slice_temp]) << 14U) + (((uint32_T)((uint16_T)(mul_u32_loSR
      ((uint32_T)((int32_T)(((int32_T)xfi) & ((uint16_T)255))), (((uint32_T)
      ATAN_UFRAC_LUT[((int32_T)slice_temp) + 1]) << 14U) - (((uint32_T)
      ATAN_UFRAC_LUT[(int32_T)((uint32_T)(((uint32_T)xfi) >> 8U))]) << 14U), 8U)
      >> 14U))) << 14U)) >> 14U))) >> 3U));
    if (x >= 0) {
      if (negThisY <= x) {
        theta = (int16_T)(-theta);
      } else {
        theta = (int16_T)((((int32_T)theta) - 12867) | -65536);
      }
    } else if (negThisY > negThisX) {
      if (((-(((int32_T)theta) + 12867)) & 65536) != 0) {
        theta = (int16_T)((-(((int32_T)theta) + 12867)) | -65536);
      } else {
        theta = (int16_T)((-(((int32_T)theta) + 12867)) & 65535);
      }
    } else {
      theta = (int16_T)((((int32_T)theta) - 25735) | -65536);
    }
  }

  return theta;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void cos_init(void)
{
  static const int16_T iv[256] = { MAX_int16_T, 32758, 32729, 32679, 32610,
    32522, 32413, 32286, 32138, 31972, 31786, 31581, 31357, 31114, 30853, 30572,
    30274, 29957, 29622, 29269, 28899, 28511, 28106, 27684, 27246, 26791, 26320,
    25833, 25330, 24812, 24279, 23732, 23170, 22595, 22006, 21403, 20788, 20160,
    19520, 18868, 18205, 17531, 16846, 16151, 15447, 14733, 14010, 13279, 12540,
    11793, 11039, 10279, 9512, 8740, 7962, 7180, 6393, 5602, 4808, 4011, 3212,
    2411, 1608, 804, 0, -804, -1608, -2411, -3212, -4011, -4808, -5602, -6393,
    -7180, -7962, -8740, -9512, -10279, -11039, -11793, -12540, -13279, -14010,
    -14733, -15447, -16151, -16846, -17531, -18205, -18868, -19520, -20160,
    -20788, -21403, -22006, -22595, -23170, -23732, -24279, -24812, -25330,
    -25833, -26320, -26791, -27246, -27684, -28106, -28511, -28899, -29269,
    -29622, -29957, -30274, -30572, -30853, -31114, -31357, -31581, -31786,
    -31972, -32138, -32286, -32413, -32522, -32610, -32679, -32729, -32758,
    -32767, -32758, -32729, -32679, -32610, -32522, -32413, -32286, -32138,
    -31972, -31786, -31581, -31357, -31114, -30853, -30572, -30274, -29957,
    -29622, -29269, -28899, -28511, -28106, -27684, -27246, -26791, -26320,
    -25833, -25330, -24812, -24279, -23732, -23170, -22595, -22006, -21403,
    -20788, -20160, -19520, -18868, -18205, -17531, -16846, -16151, -15447,
    -14733, -14010, -13279, -12540, -11793, -11039, -10279, -9512, -8740, -7962,
    -7180, -6393, -5602, -4808, -4011, -3212, -2411, -1608, -804, 0, 804, 1608,
    2411, 3212, 4011, 4808, 5602, 6393, 7180, 7962, 8740, 9512, 10279, 11039,
    11793, 12540, 13279, 14010, 14733, 15447, 16151, 16846, 17531, 18205, 18868,
    19520, 20160, 20788, 21403, 22006, 22595, 23170, 23732, 24279, 24812, 25330,
    25833, 26320, 26791, 27246, 27684, 28106, 28511, 28899, 29269, 29622, 29957,
    30274, 30572, 30853, 31114, 31357, 31581, 31786, 31972, 32138, 32286, 32413,
    32522, 32610, 32679, 32729, 32758 };

  memcpy(&FI_SIN_COS_LUT[0], &iv[0], 256U * (sizeof(int16_T)));
}

/*
 * Arguments    : uint32_T numerator
 *                uint32_T denominator
 *                uint32_T nRepeatSub
 * Return Type  : uint32_T
 */
static uint32_T div_nzp_repeat_u32_sat(uint32_T numerator, uint32_T denominator,
  uint32_T nRepeatSub)
{
  uint32_T iRepeatSub;
  uint32_T quotient;
  boolean_T numeratorExtraBit;
  boolean_T overflow;
  overflow = false;
  quotient = numerator / denominator;
  numerator %= denominator;
  for (iRepeatSub = 0U; iRepeatSub < nRepeatSub; iRepeatSub++) {
    numeratorExtraBit = (numerator >= 2147483648U);
    numerator <<= 1U;
    overflow = (overflow || (quotient >= 2147483648U));
    quotient <<= 1U;
    if (numeratorExtraBit || (numerator >= denominator)) {
      quotient++;
      numerator -= denominator;
    }
  }

  if (overflow) {
    quotient = MAX_uint32_T;
  }

  return quotient;
}

/*
 * Arguments    : int32_T numerator
 *                int32_T denominator
 *                uint32_T nRepeatSub
 * Return Type  : int32_T
 */
static int32_T div_repeat_s32_sat(int32_T numerator, int32_T denominator,
  uint32_T nRepeatSub)
{
  int32_T quotient;
  uint32_T b_denominator;
  uint32_T b_numerator;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      b_numerator = (~((uint32_T)numerator)) + 1U;
    } else {
      b_numerator = (uint32_T)numerator;
    }

    if (denominator < 0) {
      b_denominator = (~((uint32_T)denominator)) + 1U;
    } else {
      b_denominator = (uint32_T)denominator;
    }

    b_numerator = div_nzp_repeat_u32_sat(b_numerator, b_denominator, nRepeatSub);
    if ((numerator < 0) != (denominator < 0)) {
      if (b_numerator <= 2147483647U) {
        quotient = -((int32_T)b_numerator);
      } else {
        quotient = MIN_int32_T;
      }
    } else if (b_numerator <= 2147483647U) {
      quotient = (int32_T)b_numerator;
    } else {
      quotient = MAX_int32_T;
    }
  }

  return quotient;
}

/*
 * Arguments    : int32_T a
 *                uint32_T b
 *                uint32_T aShift
 * Return Type  : int32_T
 */
static int32_T mul_ssu32_loSR(int32_T a, uint32_T b, uint32_T aShift)
{
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_su32(a, b, &u32_chi, &u32_clo);
  u32_clo = (u32_chi << (32U - aShift)) | (u32_clo >> aShift);
  return (int32_T)u32_clo;
}

/*
 * Arguments    : uint32_T a
 *                uint32_T b
 *                uint32_T aShift
 * Return Type  : uint32_T
 */
static uint32_T mul_u32_loSR(uint32_T a, uint32_T b, uint32_T aShift)
{
  uint32_T result;
  uint32_T u32_chi;
  mul_wide_u32(a, b, &u32_chi, &result);
  return (u32_chi << (32U - aShift)) | (result >> aShift);
}

/*
 * Arguments    : int32_T in0
 *                uint32_T in1
 *                uint32_T *ptrOutBitsHi
 *                uint32_T *ptrOutBitsLo
 * Return Type  : void
 */
static void mul_wide_su32(int32_T in0, uint32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo)
{
  int32_T in0Hi;
  int32_T in0Lo;
  int32_T in1Hi;
  int32_T in1Lo;
  uint32_T absIn0;
  uint32_T outBitsLo;
  uint32_T productLoHi;
  uint32_T productLoLo;
  if (in0 < 0) {
    absIn0 = (~((uint32_T)in0)) + 1U;
  } else {
    absIn0 = (uint32_T)in0;
  }

  in0Hi = (int32_T)((uint32_T)(absIn0 >> 16U));
  in0Lo = (int32_T)((uint32_T)(absIn0 & 65535U));
  in1Hi = (int32_T)((uint32_T)(in1 >> 16U));
  in1Lo = (int32_T)((uint32_T)(in1 & 65535U));
  absIn0 = ((uint32_T)in0Hi) * ((uint32_T)in1Lo);
  productLoHi = ((uint32_T)in0Lo) * ((uint32_T)in1Hi);
  productLoLo = ((uint32_T)in0Lo) * ((uint32_T)in1Lo);
  in0Lo = 0;
  outBitsLo = productLoLo + (productLoHi << 16U);
  if (outBitsLo < productLoLo) {
    in0Lo = 1;
  }

  productLoLo = outBitsLo;
  outBitsLo += (absIn0 << 16U);
  if (outBitsLo < productLoLo) {
    in0Lo = (int32_T)((uint32_T)(((uint32_T)in0Lo) + 1U));
  }

  absIn0 = ((((uint32_T)in0Lo) + (((uint32_T)in0Hi) * ((uint32_T)in1Hi))) +
            (productLoHi >> 16U)) + (absIn0 >> 16U);
  if ((in1 != 0U) && (in0 < 0)) {
    absIn0 = ~absIn0;
    outBitsLo = ~outBitsLo;
    outBitsLo++;
    if (outBitsLo == 0U) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = outBitsLo;
}

/*
 * Arguments    : uint32_T in0
 *                uint32_T in1
 *                uint32_T *ptrOutBitsHi
 *                uint32_T *ptrOutBitsLo
 * Return Type  : void
 */
static void mul_wide_u32(uint32_T in0, uint32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo)
{
  int32_T in0Hi;
  int32_T in0Lo;
  int32_T in1Hi;
  int32_T in1Lo;
  uint32_T outBitsLo;
  uint32_T productHiLo;
  uint32_T productLoHi;
  uint32_T productLoLo;
  in0Hi = (int32_T)((uint32_T)(in0 >> 16U));
  in0Lo = (int32_T)((uint32_T)(in0 & 65535U));
  in1Hi = (int32_T)((uint32_T)(in1 >> 16U));
  in1Lo = (int32_T)((uint32_T)(in1 & 65535U));
  productHiLo = ((uint32_T)in0Hi) * ((uint32_T)in1Lo);
  productLoHi = ((uint32_T)in0Lo) * ((uint32_T)in1Hi);
  productLoLo = ((uint32_T)in0Lo) * ((uint32_T)in1Lo);
  in0Lo = 0;
  outBitsLo = productLoLo + (productLoHi << 16U);
  if (outBitsLo < productLoLo) {
    in0Lo = 1;
  }

  productLoLo = outBitsLo;
  outBitsLo += (productHiLo << 16U);
  if (outBitsLo < productLoLo) {
    in0Lo = (int32_T)((uint32_T)(((uint32_T)in0Lo) + 1U));
  }

  *ptrOutBitsHi = ((((uint32_T)in0Lo) + (((uint32_T)in0Hi) * ((uint32_T)in1Hi)))
                   + (productLoHi >> 16U)) + (productHiLo >> 16U);
  *ptrOutBitsLo = outBitsLo;
}

/*
 * Arguments    : int32_T u
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void sLong2MultiWord(int32_T u, uint32_T y[], int32_T n)
{
  int32_T i;
  uint32_T yi;
  y[0] = (uint32_T)u;
  if (u < 0) {
    yi = MAX_uint32_T;
  } else {
    yi = 0U;
  }

  for (i = 1; i < n; i++) {
    y[i] = yi;
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void sMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n)
{
  int32_T i;
  int32_T nm;
  uint32_T u1i;
  if (n1 < n) {
    nm = n1;
  } else {
    nm = n;
  }

  if (0 <= (nm - 1)) {
    memcpy(&y[0], &u1[0], ((uint32_T)nm) * (sizeof(uint32_T)));
  }

  if (n > n1) {
    if ((u1[n1 - 1] & 2147483648U) != 0U) {
      u1i = MAX_uint32_T;
    } else {
      u1i = 0U;
    }

    for (i = nm; i < n; i++) {
      y[i] = u1i;
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                int32_T n
 * Return Type  : int32_T
 */
static int32_T sMultiWordCmp(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  int32_T i;
  int32_T y;
  uint32_T su1;
  uint32_T u2i;
  su1 = u1[n - 1] & 2147483648U;
  if (su1 != (u2[n - 1] & 2147483648U)) {
    if (su1 != 0U) {
      y = -1;
    } else {
      y = 1;
    }
  } else {
    y = 0;
    i = n;
    while ((y == 0) && (i > 0)) {
      i--;
      su1 = u1[i];
      u2i = u2[i];
      if (su1 != u2i) {
        if (su1 > u2i) {
          y = 1;
        } else {
          y = -1;
        }
      }
    }
  }

  return y;
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                const uint32_T u2[]
 *                int32_T n2
 *                uint32_T b_y1[]
 *                int32_T m1
 *                uint32_T y2[]
 *                int32_T m2
 *                uint32_T t1[]
 *                int32_T l1
 *                uint32_T t2[]
 *                int32_T l2
 * Return Type  : void
 */
static void sMultiWordDivFloor(const uint32_T u1[], int32_T n1, const uint32_T
  u2[], int32_T n2, uint32_T b_y1[], int32_T m1, uint32_T y2[], int32_T m2,
  uint32_T t1[], int32_T l1, uint32_T t2[], int32_T l2)
{
  boolean_T denNeg;
  boolean_T numNeg;
  numNeg = ((u1[n1 - 1] & 2147483648U) != 0U);
  denNeg = ((u2[n2 - 1] & 2147483648U) != 0U);
  if (numNeg) {
    MultiWordNeg(u1, t1, n1);
  } else {
    sMultiWord2MultiWord(u1, n1, t1, l1);
  }

  if (denNeg) {
    MultiWordNeg(u2, t2, n2);
  } else {
    sMultiWord2MultiWord(u2, n2, t2, l2);
  }

  if (uMultiWordDiv(t1, l1, t2, l2, b_y1, m1, y2, m2) < 0) {
    if (numNeg) {
      MultiWordSetSignedMin(b_y1, m1);
    } else {
      MultiWordSetSignedMax(b_y1, m1);
    }
  } else {
    if (numNeg != denNeg) {
      if (MultiWord2Bool(y2, m2)) {
        uMultiWordInc(b_y1, m1);
      }

      MultiWordNeg(b_y1, b_y1, m1);
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                int32_T n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordEq(const uint32_T u1[], const uint32_T u2[], int32_T
  n)
{
  return sMultiWordCmp(u1, u2, n) == 0;
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                int32_T n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordGt(const uint32_T u1[], const uint32_T u2[], int32_T
  n)
{
  return sMultiWordCmp(u1, u2, n) > 0;
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                int32_T n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordLe(const uint32_T u1[], const uint32_T u2[], int32_T
  n)
{
  return sMultiWordCmp(u1, u2, n) <= 0;
}

/*
 * Arguments    : const uint32_T u1[]
 *                const uint32_T u2[]
 *                int32_T n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordLt(const uint32_T u1[], const uint32_T u2[], int32_T
  n)
{
  return sMultiWordCmp(u1, u2, n) < 0;
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                const uint32_T u2[]
 *                int32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n)
{
  int32_T a0;
  int32_T a1;
  int32_T b0;
  int32_T b1;
  int32_T cb1;
  int32_T cb2;
  int32_T i;
  int32_T j;
  int32_T k;
  int32_T ni;
  uint32_T cb;
  uint32_T t;
  uint32_T u1i;
  uint32_T w01;
  uint32_T yk;
  boolean_T isNegative1;
  boolean_T isNegative2;
  isNegative1 = ((u1[n1 - 1] & 2147483648U) != 0U);
  isNegative2 = ((u2[n2 - 1] & 2147483648U) != 0U);
  cb1 = 1;

  /* Initialize output to zero */
  if (0 <= (n - 1)) {
    memset(&y[0], 0, ((uint32_T)n) * (sizeof(uint32_T)));
  }

  for (i = 0; i < n1; i++) {
    cb = 0U;
    u1i = u1[i];
    if (isNegative1) {
      u1i = (~u1i) + ((uint32_T)cb1);
      cb1 = (int32_T)((u1i < ((uint32_T)cb1)) ? 1 : 0);
    }

    a1 = (int32_T)((uint32_T)(u1i >> 16U));
    a0 = (int32_T)((uint32_T)(u1i & 65535U));
    cb2 = 1;
    ni = n - i;
    if (n2 <= ni) {
      ni = n2;
    }

    k = i;
    for (j = 0; j < ni; j++) {
      u1i = u2[j];
      if (isNegative2) {
        u1i = (~u1i) + ((uint32_T)cb2);
        cb2 = (int32_T)((u1i < ((uint32_T)cb2)) ? 1 : 0);
      }

      b1 = (int32_T)((uint32_T)(u1i >> 16U));
      b0 = (int32_T)((uint32_T)(u1i & 65535U));
      u1i = ((uint32_T)a1) * ((uint32_T)b0);
      w01 = ((uint32_T)a0) * ((uint32_T)b1);
      yk = y[k] + cb;
      cb = (uint32_T)((yk < cb) ? 1 : 0);
      t = ((uint32_T)a0) * ((uint32_T)b0);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (u1i << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (w01 << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      y[k] = yk;
      cb += (u1i >> 16U);
      cb += (w01 >> 16U);
      cb += ((uint32_T)a1) * ((uint32_T)b1);
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1 != isNegative2) {
    cb = 1U;
    for (k = 0; k < n; k++) {
      yk = (~y[k]) + cb;
      y[k] = yk;
      cb = (uint32_T)((yk < cb) ? 1 : 0);
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n)
{
  int32_T i;
  int32_T i1;
  int32_T nb;
  int32_T nc;
  uint32_T nr;
  uint32_T u1i;
  uint32_T yi;
  uint32_T ys;
  nb = ((int32_T)n2) / 32;
  i = 0;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    ys = MAX_uint32_T;
  } else {
    ys = 0U;
  }

  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - (((uint32_T)nb) * 32U);
    if (nr > 0U) {
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = (u1i >> nr);
        u1i = u1[i1];
        y[i] = yi | (u1i << (32U - nr));
        i++;
      }

      if (nc < n1) {
        yi = u1[nc];
      } else {
        yi = ys;
      }

      y[i] = (u1i >> nr) | (yi << (32U - nr));
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void sin_init(void)
{
  static const int16_T iv[256] = { 0, 804, 1608, 2411, 3212, 4011, 4808, 5602,
    6393, 7180, 7962, 8740, 9512, 10279, 11039, 11793, 12540, 13279, 14010,
    14733, 15447, 16151, 16846, 17531, 18205, 18868, 19520, 20160, 20788, 21403,
    22006, 22595, 23170, 23732, 24279, 24812, 25330, 25833, 26320, 26791, 27246,
    27684, 28106, 28511, 28899, 29269, 29622, 29957, 30274, 30572, 30853, 31114,
    31357, 31581, 31786, 31972, 32138, 32286, 32413, 32522, 32610, 32679, 32729,
    32758, MAX_int16_T, 32758, 32729, 32679, 32610, 32522, 32413, 32286, 32138,
    31972, 31786, 31581, 31357, 31114, 30853, 30572, 30274, 29957, 29622, 29269,
    28899, 28511, 28106, 27684, 27246, 26791, 26320, 25833, 25330, 24812, 24279,
    23732, 23170, 22595, 22006, 21403, 20788, 20160, 19520, 18868, 18205, 17531,
    16846, 16151, 15447, 14733, 14010, 13279, 12540, 11793, 11039, 10279, 9512,
    8740, 7962, 7180, 6393, 5602, 4808, 4011, 3212, 2411, 1608, 804, 0, -804,
    -1608, -2411, -3212, -4011, -4808, -5602, -6393, -7180, -7962, -8740, -9512,
    -10279, -11039, -11793, -12540, -13279, -14010, -14733, -15447, -16151,
    -16846, -17531, -18205, -18868, -19520, -20160, -20788, -21403, -22006,
    -22595, -23170, -23732, -24279, -24812, -25330, -25833, -26320, -26791,
    -27246, -27684, -28106, -28511, -28899, -29269, -29622, -29957, -30274,
    -30572, -30853, -31114, -31357, -31581, -31786, -31972, -32138, -32286,
    -32413, -32522, -32610, -32679, -32729, -32758, -32767, -32758, -32729,
    -32679, -32610, -32522, -32413, -32286, -32138, -31972, -31786, -31581,
    -31357, -31114, -30853, -30572, -30274, -29957, -29622, -29269, -28899,
    -28511, -28106, -27684, -27246, -26791, -26320, -25833, -25330, -24812,
    -24279, -23732, -23170, -22595, -22006, -21403, -20788, -20160, -19520,
    -18868, -18205, -17531, -16846, -16151, -15447, -14733, -14010, -13279,
    -12540, -11793, -11039, -10279, -9512, -8740, -7962, -7180, -6393, -5602,
    -4808, -4011, -3212, -2411, -1608, -804 };

  memcpy(&b_FI_SIN_COS_LUT[0], &iv[0], 256U * (sizeof(int16_T)));
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                const uint32_T u2[]
 *                int32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void ssuMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n)
{
  int32_T a0;
  int32_T a1;
  int32_T b0;
  int32_T b1;
  int32_T cb1;
  int32_T i;
  int32_T j;
  int32_T k;
  int32_T ni;
  uint32_T cb;
  uint32_T t;
  uint32_T u1i;
  uint32_T w01;
  uint32_T yk;
  boolean_T isNegative1;
  isNegative1 = ((u1[n1 - 1] & 2147483648U) != 0U);
  cb1 = 1;

  /* Initialize output to zero */
  if (0 <= (n - 1)) {
    memset(&y[0], 0, ((uint32_T)n) * (sizeof(uint32_T)));
  }

  for (i = 0; i < n1; i++) {
    cb = 0U;
    u1i = u1[i];
    if (isNegative1) {
      u1i = (~u1i) + ((uint32_T)cb1);
      cb1 = (int32_T)((u1i < ((uint32_T)cb1)) ? 1 : 0);
    }

    a1 = (int32_T)((uint32_T)(u1i >> 16U));
    a0 = (int32_T)((uint32_T)(u1i & 65535U));
    ni = n - i;
    if (n2 <= ni) {
      ni = n2;
    }

    k = i;
    for (j = 0; j < ni; j++) {
      u1i = u2[j];
      b1 = (int32_T)((uint32_T)(u1i >> 16U));
      b0 = (int32_T)((uint32_T)(u1i & 65535U));
      u1i = ((uint32_T)a1) * ((uint32_T)b0);
      w01 = ((uint32_T)a0) * ((uint32_T)b1);
      yk = y[k] + cb;
      cb = (uint32_T)((yk < cb) ? 1 : 0);
      t = ((uint32_T)a0) * ((uint32_T)b0);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (u1i << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (w01 << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      y[k] = yk;
      cb += (u1i >> 16U);
      cb += (w01 >> 16U);
      cb += ((uint32_T)a1) * ((uint32_T)b1);
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1) {
    cb = 1U;
    for (k = 0; k < n; k++) {
      yk = (~y[k]) + cb;
      y[k] = yk;
      cb = (uint32_T)((yk < cb) ? 1 : 0);
    }
  }
}

/*
 * Arguments    : uint32_T u
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uLong2MultiWord(uint32_T u, uint32_T y[], int32_T n)
{
  y[0] = u;
  if (1 <= (n - 1)) {
    memset(&y[1], 0, ((uint32_T)((int32_T)(n + -1))) * (sizeof(uint32_T)));
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n)
{
  int32_T nm;
  if (n1 < n) {
    nm = n1;
  } else {
    nm = n;
  }

  if (0 <= (nm - 1)) {
    memcpy(&y[0], &u1[0], ((uint32_T)nm) * (sizeof(uint32_T)));
  }

  if ((n > n1) && (nm <= (n - 1))) {
    memset(&y[nm], 0, ((uint32_T)((int32_T)(n - nm))) * (sizeof(uint32_T)));
  }
}

/*
 * Arguments    : uint32_T a[]
 *                int32_T na
 *                uint32_T b[]
 *                int32_T nb
 *                uint32_T q[]
 *                int32_T nq
 *                uint32_T r[]
 *                int32_T nr
 * Return Type  : int32_T
 */
static int32_T uMultiWordDiv(uint32_T a[], int32_T na, uint32_T b[], int32_T nb,
  uint32_T q[], int32_T nq, uint32_T r[], int32_T nr)
{
  int32_T ka;
  int32_T kb;
  int32_T na1;
  int32_T nb1;
  int32_T nza;
  int32_T nzb;
  int32_T tpi;
  int32_T y;
  uint32_T ak;
  uint32_T bk;
  uint32_T kba;
  uint32_T kbb;
  uint32_T mask;
  uint32_T nba;
  uint32_T nbb;
  uint32_T nbq;
  uint32_T t;
  uint32_T tnb;
  nzb = nb;
  tpi = nb - 1;
  while ((nzb > 0) && (b[tpi] == 0U)) {
    nzb--;
    tpi--;
  }

  if (nzb > 0) {
    nza = na;
    if (0 <= (nq - 1)) {
      memset(&q[0], 0, ((uint32_T)nq) * (sizeof(uint32_T)));
    }

    tpi = na - 1;
    while ((nza > 0) && (a[tpi] == 0U)) {
      nza--;
      tpi--;
    }

    if ((nza > 0) && (nza >= nzb)) {
      nb1 = nzb - 1;
      na1 = nza - 1;
      if (0 <= (nr - 1)) {
        memset(&r[0], 0, ((uint32_T)nr) * (sizeof(uint32_T)));
      }

      /* Quick return if dividend and divisor fit into single word. */
      if (nza == 1) {
        ak = a[0];
        bk = b[0];
        nbq = ak / bk;
        q[0] = nbq;
        r[0] = ak - (nbq * bk);
        y = 7;
      } else {
        /* Remove leading zeros from both, dividend and divisor. */
        kbb = 1U;
        t = (b[nzb - 1] >> 1U);
        while (t != 0U) {
          kbb++;
          t >>= 1U;
        }

        kba = 1U;
        t = (a[nza - 1] >> 1U);
        while (t != 0U) {
          kba++;
          t >>= 1U;
        }

        /* Quick return if quotient is zero. */
        if ((nza > nzb) || (kba >= kbb)) {
          nba = (((uint32_T)((int32_T)(nza - 1))) * 32U) + kba;
          nbb = (((uint32_T)((int32_T)(nzb - 1))) * 32U) + kbb;

          /* Normalize b. */
          if (kbb != 32U) {
            bk = b[nzb - 1];
            for (kb = nzb - 1; kb > 0; kb--) {
              t = (bk << (32U - kbb));
              bk = b[kb - 1];
              t |= (bk >> kbb);
              b[kb] = t;
            }

            b[0] = (bk << (32U - kbb));
            mask = ~((1U << (32U - kbb)) - 1U);
          } else {
            mask = MAX_uint32_T;
          }

          /* Initialize quotient to zero. */
          tnb = 0U;
          y = 0;

          /* Until exit conditions have been met, do */
          do {
            /* Normalize a */
            if (kba != 32U) {
              tnb = (tnb - kba) + 32U;
              ak = a[na1];
              for (ka = na1; ka > 0; ka--) {
                t = (ak << (32U - kba));
                ak = a[ka - 1];
                t |= (ak >> kba);
                a[ka] = t;
              }

              a[0] = (ak << (32U - kba));
            }

            /* Compare b against the a. */
            ak = a[na1];
            bk = b[nzb - 1];
            if ((nzb - 1) == 0) {
              t = mask;
            } else {
              t = MAX_uint32_T;
            }

            if ((ak & t) == bk) {
              tpi = 0;
              ka = na1;
              kb = nzb - 1;
              while ((tpi == 0) && (kb > 0)) {
                ka--;
                ak = a[ka];
                kb--;
                bk = b[kb];
                if (kb == 0) {
                  t = mask;
                } else {
                  t = MAX_uint32_T;
                }

                if ((ak & t) != bk) {
                  if (ak > bk) {
                    tpi = 1;
                  } else {
                    tpi = -1;
                  }
                }
              }
            } else if (ak > bk) {
              tpi = 1;
            } else {
              tpi = -1;
            }

            /* If the remainder in a is still greater or equal to b, subtract normalized divisor from a. */
            if ((tpi >= 0) || (nba > nbb)) {
              nbq = nba - nbb;

              /* If the remainder and the divisor are equal, set remainder to zero. */
              if (tpi == 0) {
                ka = na1;
                for (kb = nzb - 1; kb > 0; kb--) {
                  a[ka] = 0U;
                  ka--;
                }

                a[ka] -= b[0];
              } else {
                /* Otherwise, subtract the divisor from the remainder */
                if (tpi < 0) {
                  ak = a[na1];
                  kba = 31U;
                  for (ka = na1; ka > 0; ka--) {
                    t = (ak << 1U);
                    ak = a[ka - 1];
                    t |= (ak >> 31U);
                    a[ka] = t;
                  }

                  a[0] = (ak << 1U);
                  tnb++;
                  nbq--;
                }

                tpi = 0;
                ka = (na1 - nzb) + 1;
                for (kb = 0; kb < nzb; kb++) {
                  t = a[ka];
                  ak = (t - b[kb]) - ((uint32_T)tpi);
                  if (((uint32_T)tpi) != 0U) {
                    tpi = (int32_T)((ak >= t) ? 1 : 0);
                  } else {
                    tpi = (int32_T)((ak > t) ? 1 : 0);
                  }

                  a[ka] = ak;
                  ka++;
                }
              }

              /* Update the quotient. */
              tpi = ((int32_T)nbq) / 32;
              q[tpi] |= (1U << (nbq - (((uint32_T)tpi) * 32U)));

              /* Remove leading zeros from the remainder and check whether the exit conditions have been met. */
              tpi = na1;
              while ((nza > 0) && (a[tpi] == 0U)) {
                nza--;
                tpi--;
              }

              if (nza >= nzb) {
                na1 = nza - 1;
                kba = 1U;
                t = (a[nza - 1] >> 1U);
                while (t != 0U) {
                  kba++;
                  t >>= 1U;
                }

                nba = ((((uint32_T)((int32_T)(nza - 1))) * 32U) + kba) - tnb;
                if (nba < nbb) {
                  y = 2;
                }
              } else if (nza == 0) {
                y = 1;
              } else {
                na1 = nza - 1;
                y = 4;
              }
            } else {
              y = 3;
            }
          } while (y == 0);

          /* Return the remainder. */
          if (y == 1) {
            r[0] = a[0];
          } else {
            tpi = ((int32_T)tnb) / 32;
            nbq = tnb - (((uint32_T)tpi) * 32U);
            if (nbq == 0U) {
              ka = tpi;
              for (nzb = 0; nzb <= nb1; nzb++) {
                r[nzb] = a[ka];
                ka++;
              }
            } else {
              ak = a[tpi];
              nzb = 0;
              for (ka = tpi + 1; ka <= na1; ka++) {
                t = (ak >> nbq);
                ak = a[ka];
                t |= (ak << (32U - nbq));
                r[nzb] = t;
                nzb++;
              }

              r[nzb] = (ak >> nbq);
            }
          }

          /* Restore b. */
          if (kbb != 32U) {
            bk = b[0];
            for (kb = 0; kb < nb1; kb++) {
              t = (bk >> (32U - kbb));
              bk = b[kb + 1];
              t |= (bk << kbb);
              b[kb] = t;
            }

            b[kb] = (bk >> (32U - kbb));
          }
        } else {
          if (0 <= (nr - 1)) {
            memcpy(&r[0], &a[0], ((uint32_T)nr) * (sizeof(uint32_T)));
          }

          y = 6;
        }
      }
    } else {
      if (0 <= (nr - 1)) {
        memcpy(&r[0], &a[0], ((uint32_T)nr) * (sizeof(uint32_T)));
      }

      y = 5;
    }
  } else {
    y = -1;
  }

  return y;
}

/*
 * Arguments    : uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uMultiWordInc(uint32_T y[], int32_T n)
{
  int32_T carry = 1;
  int32_T i;
  uint32_T yi;
  for (i = 0; i < n; i++) {
    yi = y[i] + ((uint32_T)carry);
    y[i] = yi;
    carry = (int32_T)((yi < ((uint32_T)carry)) ? 1 : 0);
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                const uint32_T u2[]
 *                int32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n)
{
  int32_T a0;
  int32_T a1;
  int32_T b0;
  int32_T b1;
  int32_T i;
  int32_T j;
  int32_T k;
  int32_T ni;
  uint32_T cb;
  uint32_T t;
  uint32_T u1i;
  uint32_T w01;
  uint32_T yk;

  /* Initialize output to zero */
  if (0 <= (n - 1)) {
    memset(&y[0], 0, ((uint32_T)n) * (sizeof(uint32_T)));
  }

  for (i = 0; i < n1; i++) {
    cb = 0U;
    u1i = u1[i];
    a1 = (int32_T)((uint32_T)(u1i >> 16U));
    a0 = (int32_T)((uint32_T)(u1i & 65535U));
    ni = n - i;
    if (n2 <= ni) {
      ni = n2;
    }

    k = i;
    for (j = 0; j < ni; j++) {
      u1i = u2[j];
      b1 = (int32_T)((uint32_T)(u1i >> 16U));
      b0 = (int32_T)((uint32_T)(u1i & 65535U));
      u1i = ((uint32_T)a1) * ((uint32_T)b0);
      w01 = ((uint32_T)a0) * ((uint32_T)b1);
      yk = y[k] + cb;
      cb = (uint32_T)((yk < cb) ? 1 : 0);
      t = ((uint32_T)a0) * ((uint32_T)b0);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (u1i << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      t = (w01 << 16U);
      yk += t;
      cb += (uint32_T)((yk < t) ? 1 : 0);
      y[k] = yk;
      cb += (u1i >> 16U);
      cb += (w01 >> 16U);
      cb += ((uint32_T)a1) * ((uint32_T)b1);
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n)
{
  int32_T i;
  int32_T nb;
  int32_T nc;
  uint32_T nl;
  uint32_T u1i;
  uint32_T yi;
  uint32_T ys;
  nb = ((int32_T)n2) / 32;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    ys = MAX_uint32_T;
  } else {
    ys = 0U;
  }

  if (nb > n) {
    nc = n;
  } else {
    nc = nb;
  }

  u1i = 0U;
  if (0 <= (nc - 1)) {
    memset(&y[0], 0, ((uint32_T)nc) * (sizeof(uint32_T)));
  }

  for (i = 0; i < nc; i++) {
  }

  if (nb < n) {
    nl = n2 - (((uint32_T)nb) * 32U);
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      for (nc = 0; nc < nb; nc++) {
        yi = (u1i >> (32U - nl));
        u1i = u1[nc];
        y[i] = yi | (u1i << nl);
        i++;
      }

      if (i < n) {
        y[i] = (u1i >> (32U - nl)) | (ys << nl);
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

/*
 * Arguments    : const uint32_T u1[]
 *                int32_T n1
 *                uint32_T n2
 *                uint32_T y[]
 *                int32_T n
 * Return Type  : void
 */
static void uMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n)
{
  int32_T i;
  int32_T i1;
  int32_T nb;
  int32_T nc;
  uint32_T nr;
  uint32_T u1i;
  uint32_T yi;
  nb = ((int32_T)n2) / 32;
  i = 0;
  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - (((uint32_T)nb) * 32U);
    if (nr > 0U) {
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = (u1i >> nr);
        u1i = u1[i1];
        y[i] = yi | (u1i << (32U - nr));
        i++;
      }

      yi = (u1i >> nr);
      if (nc < n1) {
        yi |= (u1[nc] << (32U - nr));
      }

      y[i] = yi;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = 0U;
    i++;
  }
}

/*
 * x_base = 29;
 *      y_base = 17;
 *      z_base = 11;
 * Arguments    : uint32_T az_in
 *                int32_T el_in
 *                uint32_T r_in
 *                int32_T x_base
 *                int32_T y_base
 *                int32_T z_base
 *                uint32_T *az_out
 *                int32_T *el_out
 *                uint32_T *r_out
 * Return Type  : void
 */
void ParalaxCalc_fixpt(
    const uint32_T az_in,
    const int32_T  el_in,
    const uint32_T r_in,
    const int32_T  x_base,
    const int32_T  y_base,
    const int32_T   z_base,
    uint32_T *     az_out,
    int32_T *      el_out,
    uint32_T *     r_out)
{
  static const int64m_T r31 = { { 0U, 0U }/* chunks */
  };

  static const int64m_T r47 = { { 1073741824U, 421657428U }/* chunks */
  };

  static int64m_T b_c;
  static int64m_T c;
  static int64m_T r12;
  static int64m_T r13;
  static int64m_T r14;
  static int64m_T r15;
  static int64m_T r16;
  static int64m_T r17;
  static int64m_T r18;
  static int64m_T r19;
  static int64m_T r20;
  static int64m_T r21;
  static int64m_T r22;
  static int64m_T r23;
  static int64m_T r24;
  static int64m_T r25;
  static int64m_T r26;
  static int64m_T r27;
  static int64m_T r28;
  static int64m_T r29;
  static int64m_T r30;
  static int64m_T r32;
  static int64m_T r33;
  static int64m_T r34;
  static int64m_T r35;
  static int64m_T r36;
  static int64m_T r37;
  static int64m_T r38;
  static int64m_T r39;
  static int64m_T r40;
  static int64m_T r41;
  static int64m_T r42;
  static int64m_T r43;
  static int64m_T r6;
  static int96m_T r10;
  static int96m_T r11;
  static int96m_T r7;
  static int96m_T r8;
  static int96m_T r9;
  static uint64m_T r;
  static uint96m_T r1;
  static uint96m_T r2;
  static uint96m_T r3;
  static uint96m_T r4;
  static uint96m_T r5;
  int64m_T r44;
  int64m_T r45;
  int64m_T r46;
  int64m_T r48;
  int64m_T r49;
  int64m_T r50;
  int32_T el_in_rad;
  int32_T negThisX;
  int32_T x_out;
  int32_T y;
  int32_T y_out;
  int32_T ytemp;
  int32_T z_out;
  uint32_T az_in_rad;
  uint32_T u;
  uint32_T u1;
  int16_T thPreCorr;
  int16_T var1;
  uint16_T b_idxUFIX16;
  uint16_T c_idxUFIX16;
  uint16_T d_idxUFIX16;
  uint16_T e_idxUFIX16;
  uint16_T idxUFIX16;
  int8_T i;
  uint8_T b_slice_temp;
  uint8_T c_slice_temp;
  uint8_T d_slice_temp;
  uint8_T e_slice_temp;
  uint8_T slice_temp;
  boolean_T guard1 = false;
  boolean_T guard2 = false;

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*                                                                           % */
  /*            Generated by MATLAB 9.9 and Fixed-Point Designer 7.1           % */
  /*                                                                           % */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /* u32 Az ���� = 180 * 2 ^-14 * az_in */
  /* i32 At ���� = 180 * 2 ^-14 * el_in */
  /* u32 r �     = r_in */
  /*      x_base = 0; */
  /*      y_base = 0; */
  /*      z_base = 0; */
  u = 3373259426U;
  uMultiWordMul((uint32_T *)(&az_in), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r.chunks[0U]), 2);
  uMultiWord2MultiWord((uint32_T *)(&r.chunks[0U]), 2, (uint32_T *)(&r1.chunks
    [0U]), 3);
  uMultiWordShl((uint32_T *)(&r1.chunks[0U]), 3, 14U, (uint32_T *)(&r2.chunks[0U]),
                3);
  MultiWordUnsignedWrap((uint32_T *)(&r2.chunks[0U]), 3, 18U, (uint32_T *)
                        (&r3.chunks[0U]));
  uMultiWordShr((uint32_T *)(&r3.chunks[0U]), 3, 14U, (uint32_T *)(&r4.chunks[0U]),
                3);
  uMultiWordShr((uint32_T *)(&r4.chunks[0U]), 3, 32U, (uint32_T *)(&r5.chunks[0U]),
                3);
  az_in_rad = MultiWord2uLong((uint32_T *)(&r5.chunks[0U]));
  u1 = (uint32_T)el_in;
  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                  (&r6.chunks[0U]), 2);
  sMultiWord2MultiWord((uint32_T *)(&r6.chunks[0U]), 2, (uint32_T *)(&r7.chunks
    [0U]), 3);
  uMultiWordShl((uint32_T *)(&r7.chunks[0U]), 3, 14U, (uint32_T *)(&r8.chunks[0U]),
                3);
  MultiWordSignedWrap((uint32_T *)(&r8.chunks[0U]), 3, 18U, (uint32_T *)
                      (&r9.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r9.chunks[0U]), 3, 14U, (uint32_T *)(&r10.chunks
    [0U]), 3);
  sMultiWordShr((uint32_T *)(&r10.chunks[0U]), 3, 32U, (uint32_T *)(&r11.chunks
    [0U]), 3);
  el_in_rad = MultiWord2sLong((uint32_T *)(&r11.chunks[0U]));
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r12.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r12.chunks[0U]), 2, 29U, (uint32_T *)(&r13.chunks
    [0U]), 2);
  uLong2MultiWord(3373259426U, (uint32_T *)(&r12.chunks[0U]), 2);
  sMultiWordDivFloor((uint32_T *)(&r13.chunks[0U]), 2, (uint32_T *)(&r12.chunks
    [0U]), 2, (uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)(&r14.chunks[0U]), 2,
                     (uint32_T *)(&r15.chunks[0U]), 2, (uint32_T *)(&r16.chunks
    [0U]), 2);
  sMultiWord2MultiWord((uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)
                       (&r6.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r6.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r17.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r17.chunks[0U]), 2, 30U, (uint32_T *)(&r6.chunks
    [0U]), 2);
  i = (int8_T)MultiWord2sLong((uint32_T *)(&r6.chunks[0U]));
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r16.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r16.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r15.chunks[0U]));
  if ((i & 8) != 0) {
    u1 = (uint32_T)((int8_T)(i | -8));
  } else {
    u1 = (uint32_T)((int8_T)(i & 7));
  }

  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                  (&r18.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r18.chunks[0U]), 2, 1U, (uint32_T *)(&r19.chunks
    [0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r19.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r16.chunks[0U]));
  MultiWordSub((uint32_T *)(&r15.chunks[0U]), (uint32_T *)(&r16.chunks[0U]),
               (uint32_T *)(&r14.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r14.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r12.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r12.chunks[0U]), 2, 17U, (uint32_T *)(&r13.chunks
    [0U]), 2);
  idxUFIX16 = (uint16_T)(mul_u32_loSR(683563337U, (uint32_T)((uint16_T)
    MultiWord2uLong((uint32_T *)(&r13.chunks[0U]))), 13U) >> 16U);
  slice_temp = (uint8_T)(((uint32_T)idxUFIX16) >> 8U);
  b_idxUFIX16 = (uint16_T)(mul_u32_loSR(683563337U, (az_in_rad - ((az_in_rad /
    3373259426U) * 3373259426U)) >> 16U, 13U) >> 16U);
  b_slice_temp = (uint8_T)(((uint32_T)b_idxUFIX16) >> 8U);
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r15.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r15.chunks[0U]), 2, 29U, (uint32_T *)(&r14.chunks
    [0U]), 2);
  uLong2MultiWord(3373259426U, (uint32_T *)(&r15.chunks[0U]), 2);
  sMultiWordDivFloor((uint32_T *)(&r14.chunks[0U]), 2, (uint32_T *)(&r15.chunks
    [0U]), 2, (uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)(&r16.chunks[0U]), 2,
                     (uint32_T *)(&r19.chunks[0U]), 2, (uint32_T *)(&r18.chunks
    [0U]), 2);
  sMultiWord2MultiWord((uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)
                       (&r12.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r12.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r20.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r20.chunks[0U]), 2, 30U, (uint32_T *)(&r12.chunks
    [0U]), 2);
  i = (int8_T)MultiWord2sLong((uint32_T *)(&r12.chunks[0U]));
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r18.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r18.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r19.chunks[0U]));
  if ((i & 8) != 0) {
    u1 = (uint32_T)((int8_T)(i | -8));
  } else {
    u1 = (uint32_T)((int8_T)(i & 7));
  }

  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                  (&r21.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r21.chunks[0U]), 2, 1U, (uint32_T *)(&r22.chunks
    [0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r22.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r18.chunks[0U]));
  MultiWordSub((uint32_T *)(&r19.chunks[0U]), (uint32_T *)(&r18.chunks[0U]),
               (uint32_T *)(&r16.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r16.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r15.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r15.chunks[0U]), 2, 17U, (uint32_T *)(&r14.chunks
    [0U]), 2);
  c_idxUFIX16 = (uint16_T)(mul_u32_loSR(683563337U, (uint32_T)((uint16_T)
    MultiWord2uLong((uint32_T *)(&r14.chunks[0U]))), 13U) >> 16U);
  c_slice_temp = (uint8_T)(((uint32_T)c_idxUFIX16) >> 8U);
  d_idxUFIX16 = (uint16_T)(mul_u32_loSR(683563337U, (az_in_rad - ((az_in_rad /
    3373259426U) * 3373259426U)) >> 16U, 13U) >> 16U);
  d_slice_temp = (uint8_T)(((uint32_T)d_idxUFIX16) >> 8U);
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r19.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r19.chunks[0U]), 2, 29U, (uint32_T *)(&r16.chunks
    [0U]), 2);
  uLong2MultiWord(3373259426U, (uint32_T *)(&r19.chunks[0U]), 2);
  sMultiWordDivFloor((uint32_T *)(&r16.chunks[0U]), 2, (uint32_T *)(&r19.chunks
    [0U]), 2, (uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)(&r18.chunks[0U]), 2,
                     (uint32_T *)(&r22.chunks[0U]), 2, (uint32_T *)(&r21.chunks
    [0U]), 2);
  sMultiWord2MultiWord((uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)
                       (&r15.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r15.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r23.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r23.chunks[0U]), 2, 30U, (uint32_T *)(&r15.chunks
    [0U]), 2);
  i = (int8_T)MultiWord2sLong((uint32_T *)(&r15.chunks[0U]));
  sLong2MultiWord(el_in_rad, (uint32_T *)(&r21.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r21.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r22.chunks[0U]));
  if ((i & 8) != 0) {
    u1 = (uint32_T)((int8_T)(i | -8));
  } else {
    u1 = (uint32_T)((int8_T)(i & 7));
  }

  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                  (&r24.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r24.chunks[0U]), 2, 1U, (uint32_T *)(&r25.chunks
    [0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r25.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r21.chunks[0U]));
  MultiWordSub((uint32_T *)(&r22.chunks[0U]), (uint32_T *)(&r21.chunks[0U]),
               (uint32_T *)(&r18.chunks[0U]), 2);
  MultiWordSignedWrap((uint32_T *)(&r18.chunks[0U]), 2, 30U, (uint32_T *)
                      (&r19.chunks[0U]));
  sMultiWordShr((uint32_T *)(&r19.chunks[0U]), 2, 17U, (uint32_T *)(&r16.chunks
    [0U]), 2);
  e_idxUFIX16 = (uint16_T)(mul_u32_loSR(683563337U, (uint32_T)((uint16_T)
    MultiWord2uLong((uint32_T *)(&r16.chunks[0U]))), 13U) >> 16U);
  e_slice_temp = (uint8_T)(((uint32_T)e_idxUFIX16) >> 8U);
  ytemp = ((int32_T)FI_SIN_COS_LUT[slice_temp]) * 32768;
  u1 = (uint32_T)((int16_T)asr_s32(ytemp + (((int32_T)((int16_T)asr_s32
    (mul_ssu32_loSR((((int32_T)FI_SIN_COS_LUT[(uint8_T)(((uint32_T)slice_temp) +
    1U)]) * 32768) - ytemp, (uint32_T)((int32_T)(((int32_T)idxUFIX16) &
    ((uint16_T)255))), 8U), 15U))) * 32768), 15U));
  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&r_in), 1, (uint32_T *)
                  (&r26.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r26.chunks[0U]), 2, 16U, (uint32_T *)(&r27.chunks
    [0U]), 2);
  ytemp = ((int32_T)FI_SIN_COS_LUT[b_slice_temp]) * 32768;
  u1 = (uint32_T)((int16_T)asr_s32(ytemp + (((int32_T)((int16_T)asr_s32
    (mul_ssu32_loSR((((int32_T)FI_SIN_COS_LUT[(uint8_T)(((uint32_T)b_slice_temp)
    + 1U)]) * 32768) - ytemp, (uint32_T)((int32_T)(((int32_T)b_idxUFIX16) &
    ((uint16_T)255))), 8U), 15U))) * 32768), 15U));
  sMultiWordMul((uint32_T *)(&r27.chunks[0U]), 2, (uint32_T *)(&u1), 1,
                (uint32_T *)(&r9.chunks[0U]), 3);
  sMultiWordShr((uint32_T *)(&r9.chunks[0U]), 3, 16U, (uint32_T *)(&r10.chunks
    [0U]), 3);
  sMultiWord2MultiWord((uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)
                       (&r24.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r24.chunks[0U]), 2, 31U, (uint32_T *)(&r25.chunks
    [0U]), 2);
  sLong2MultiWord(MultiWord2sLong((uint32_T *)(&r25.chunks[0U])), (uint32_T *)
                  (&r21.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r21.chunks[0U]), 2, 31U, (uint32_T *)(&r22.chunks
    [0U]), 2);
  sLong2MultiWord(x_base, (uint32_T *)(&r24.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r24.chunks[0U]), 2, 21U, (uint32_T *)(&r21.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r22.chunks[0U]), (uint32_T *)(&r21.chunks[0U]),
               (uint32_T *)(&r18.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r18.chunks[0U]), 2, 31U, (uint32_T *)(&r19.chunks
    [0U]), 2);
  x_out = MultiWord2sLong((uint32_T *)(&r19.chunks[0U]));
  ytemp = ((int32_T)FI_SIN_COS_LUT[c_slice_temp]) * 32768;
  u1 = (uint32_T)((int16_T)asr_s32(ytemp + (((int32_T)((int16_T)asr_s32
    (mul_ssu32_loSR((((int32_T)FI_SIN_COS_LUT[(uint8_T)(((uint32_T)c_slice_temp)
    + 1U)]) * 32768) - ytemp, (uint32_T)((int32_T)(((int32_T)c_idxUFIX16) &
    ((uint16_T)255))), 8U), 15U))) * 32768), 15U));
  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&r_in), 1, (uint32_T *)
                  (&r28.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r28.chunks[0U]), 2, 16U, (uint32_T *)(&r29.chunks
    [0U]), 2);
  ytemp = ((int32_T)b_FI_SIN_COS_LUT[d_slice_temp]) * 32768;
  u1 = (uint32_T)((int16_T)asr_s32(ytemp + (((int32_T)((int16_T)asr_s32
    (mul_ssu32_loSR((((int32_T)b_FI_SIN_COS_LUT[(uint8_T)(((uint32_T)
    d_slice_temp) + 1U)]) * 32768) - ytemp, (uint32_T)((int32_T)(((int32_T)
    d_idxUFIX16) & ((uint16_T)255))), 8U), 15U))) * 32768), 15U));
  sMultiWordMul((uint32_T *)(&r29.chunks[0U]), 2, (uint32_T *)(&u1), 1,
                (uint32_T *)(&r9.chunks[0U]), 3);
  sMultiWordShr((uint32_T *)(&r9.chunks[0U]), 3, 16U, (uint32_T *)(&r10.chunks
    [0U]), 3);
  sMultiWord2MultiWord((uint32_T *)(&r10.chunks[0U]), 3, (uint32_T *)
                       (&r26.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r26.chunks[0U]), 2, 31U, (uint32_T *)(&r27.chunks
    [0U]), 2);
  sLong2MultiWord(MultiWord2sLong((uint32_T *)(&r27.chunks[0U])), (uint32_T *)
                  (&r24.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r24.chunks[0U]), 2, 31U, (uint32_T *)(&r21.chunks
    [0U]), 2);
  sLong2MultiWord(y_base, (uint32_T *)(&r26.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r26.chunks[0U]), 2, 21U, (uint32_T *)(&r24.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r21.chunks[0U]), (uint32_T *)(&r24.chunks[0U]),
               (uint32_T *)(&r22.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r22.chunks[0U]), 2, 31U, (uint32_T *)(&r18.chunks
    [0U]), 2);
  y_out = MultiWord2sLong((uint32_T *)(&r18.chunks[0U]));
  ytemp = ((int32_T)b_FI_SIN_COS_LUT[e_slice_temp]) * 32768;
  u1 = (uint32_T)((int16_T)asr_s32(ytemp + (((int32_T)((int16_T)asr_s32
    (mul_ssu32_loSR((((int32_T)b_FI_SIN_COS_LUT[(uint8_T)(((uint32_T)
    e_slice_temp) + 1U)]) * 32768) - ytemp, (uint32_T)((int32_T)(((int32_T)
    e_idxUFIX16) & ((uint16_T)255))), 8U), 15U))) * 32768), 15U));
  ssuMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&r_in), 1, (uint32_T *)
                  (&r30.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r30.chunks[0U]), 2, 16U, (uint32_T *)(&r28.chunks
    [0U]), 2);
  sMultiWordShr((uint32_T *)(&r28.chunks[0U]), 2, 32U, (uint32_T *)(&r29.chunks
    [0U]), 2);
  sLong2MultiWord(MultiWord2sLong((uint32_T *)(&r29.chunks[0U])), (uint32_T *)
                  (&r26.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r26.chunks[0U]), 2, 31U, (uint32_T *)(&r24.chunks
    [0U]), 2);
  sLong2MultiWord(z_base, (uint32_T *)(&r28.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r28.chunks[0U]), 2, 21U, (uint32_T *)(&r26.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r24.chunks[0U]), (uint32_T *)(&r26.chunks[0U]),
               (uint32_T *)(&r21.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r21.chunks[0U]), 2, 31U, (uint32_T *)(&r22.chunks
    [0U]), 2);
  z_out = MultiWord2sLong((uint32_T *)(&r22.chunks[0U]));
  u1 = (uint32_T)x_out;
  u = (uint32_T)x_out;
  sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r28.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r28.chunks[0U]), 2, 1U, (uint32_T *)(&r26.chunks
    [0U]), 2);
  u1 = (uint32_T)y_out;
  u = (uint32_T)y_out;
  sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r30.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r30.chunks[0U]), 2, 1U, (uint32_T *)(&r28.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r26.chunks[0U]), (uint32_T *)(&r28.chunks[0U]),
               (uint32_T *)(&r24.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r24.chunks[0U]), 2, 1U, (uint32_T *)(&r21.chunks
    [0U]), 2);
  u1 = (uint32_T)z_out;
  u = (uint32_T)z_out;
  sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r26.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r26.chunks[0U]), 2, 2U, (uint32_T *)(&r24.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r21.chunks[0U]), (uint32_T *)(&r24.chunks[0U]),
               (uint32_T *)(&c.chunks[0U]), 2);
  y = 0;
  if (!sMultiWordLe((uint32_T *)(&c.chunks[0U]), (uint32_T *)(&r31.chunks[0U]),
                    2)) {
    for (el_in_rad = 30; el_in_rad >= 0; el_in_rad--) {
      ytemp = y | (1 << ((uint32_T)el_in_rad));
      u1 = (uint32_T)ytemp;
      u = (uint32_T)ytemp;
      sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                    (&r24.chunks[0U]), 2);
      if (sMultiWordLe((uint32_T *)(&r24.chunks[0U]), (uint32_T *)(&c.chunks[0U]),
                       2)) {
        y = ytemp;
      }
    }
  }

  *r_out = (((uint32_T)y) << 1U);
  u1 = (uint32_T)x_out;
  u = (uint32_T)x_out;
  sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r28.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r28.chunks[0U]), 2, 1U, (uint32_T *)(&r26.chunks
    [0U]), 2);
  u1 = (uint32_T)y_out;
  u = (uint32_T)y_out;
  sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                (&r30.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r30.chunks[0U]), 2, 1U, (uint32_T *)(&r28.chunks
    [0U]), 2);
  MultiWordAdd((uint32_T *)(&r26.chunks[0U]), (uint32_T *)(&r28.chunks[0U]),
               (uint32_T *)(&b_c.chunks[0U]), 2);
  y = 0;
  if (!sMultiWordLe((uint32_T *)(&b_c.chunks[0U]), (uint32_T *)(&r31.chunks[0U]),
                    2)) {
    for (el_in_rad = 30; el_in_rad >= 0; el_in_rad--) {
      ytemp = y | (1 << ((uint32_T)el_in_rad));
      u1 = (uint32_T)ytemp;
      u = (uint32_T)ytemp;
      sMultiWordMul((uint32_T *)(&u1), 1, (uint32_T *)(&u), 1, (uint32_T *)
                    (&r28.chunks[0U]), 2);
      sMultiWord2MultiWord((uint32_T *)(&r28.chunks[0U]), 2, (uint32_T *)
                           (&r9.chunks[0U]), 3);
      uMultiWordShl((uint32_T *)(&r9.chunks[0U]), 3, 1U, (uint32_T *)
                    (&r10.chunks[0U]), 3);
      sMultiWord2MultiWord((uint32_T *)(&b_c.chunks[0U]), 2, (uint32_T *)
                           (&r9.chunks[0U]), 3);
      if (sMultiWordLe((uint32_T *)(&r10.chunks[0U]), (uint32_T *)(&r9.chunks[0U]),
                       3)) {
        y = ytemp;
      }
    }
  }

  guard1 = false;
  guard2 = false;
  if (z_out > 0) {
    sLong2MultiWord(y, (uint32_T *)(&r32.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r32.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r33.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r33.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r34.chunks[0U]));
    sLong2MultiWord(z_out, (uint32_T *)(&r32.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r32.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r33.chunks[0U]));
    if (sMultiWordEq((uint32_T *)(&r34.chunks[0U]), (uint32_T *)(&r33.chunks[0U]),
                     2)) {
      thPreCorr = 6434;
    } else if (y >= 0) {
      sLong2MultiWord(z_out, (uint32_T *)(&r35.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r35.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r36.chunks[0U]));
      sLong2MultiWord(y, (uint32_T *)(&r38.chunks[0U]), 2);
      uMultiWordShl((uint32_T *)(&r38.chunks[0U]), 2, 1U, (uint32_T *)
                    (&r40.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r40.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r35.chunks[0U]));
      if (sMultiWordLe((uint32_T *)(&r36.chunks[0U]), (uint32_T *)(&r35.chunks
            [0U]), 2)) {
        if (y == 0) {
          idxUFIX16 = MAX_uint16_T;
        } else {
          ytemp = div_repeat_s32_sat(z_out, y, 15U);
          if (ytemp < 0) {
            ytemp = 0;
          } else {
            if (ytemp > 65535) {
              ytemp = 65535;
            }
          }

          idxUFIX16 = (uint16_T)ytemp;
        }
      } else {
        ytemp = div_repeat_s32_sat(y, z_out, 17U);
        if (ytemp < 0) {
          ytemp = 0;
        } else {
          if (ytemp > 65535) {
            ytemp = 65535;
          }
        }

        idxUFIX16 = (uint16_T)ytemp;
      }

      guard2 = true;
    } else {
      if (y <= MIN_int32_T) {
        negThisX = MAX_int32_T;
      } else {
        negThisX = -y;
      }

      sLong2MultiWord(z_out, (uint32_T *)(&r39.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r39.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r32.chunks[0U]));
      sLong2MultiWord(negThisX, (uint32_T *)(&r41.chunks[0U]), 2);
      uMultiWordShl((uint32_T *)(&r41.chunks[0U]), 2, 1U, (uint32_T *)
                    (&r42.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r42.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r39.chunks[0U]));
      if (sMultiWordEq((uint32_T *)(&r32.chunks[0U]), (uint32_T *)(&r39.chunks
            [0U]), 2)) {
        thPreCorr = 19302;
      } else {
        sLong2MultiWord(z_out, (uint32_T *)(&r41.chunks[0U]), 2);
        MultiWordSignedWrap((uint32_T *)(&r41.chunks[0U]), 2, 31U, (uint32_T *)(
          &r42.chunks[0U]));
        sLong2MultiWord(negThisX, (uint32_T *)(&r35.chunks[0U]), 2);
        uMultiWordShl((uint32_T *)(&r35.chunks[0U]), 2, 1U, (uint32_T *)
                      (&r36.chunks[0U]), 2);
        MultiWordSignedWrap((uint32_T *)(&r36.chunks[0U]), 2, 31U, (uint32_T *)(
          &r41.chunks[0U]));
        if (sMultiWordLt((uint32_T *)(&r42.chunks[0U]), (uint32_T *)
                         (&r41.chunks[0U]), 2)) {
          ytemp = div_repeat_s32_sat(z_out, negThisX, 15U);
          if (ytemp < 0) {
            ytemp = 0;
          } else {
            if (ytemp > 65535) {
              ytemp = 65535;
            }
          }

          idxUFIX16 = (uint16_T)ytemp;
        } else {
          ytemp = div_repeat_s32_sat(negThisX, z_out, 17U);
          if (ytemp < 0) {
            ytemp = 0;
          } else {
            if (ytemp > 65535) {
              ytemp = 65535;
            }
          }

          idxUFIX16 = (uint16_T)ytemp;
        }

        guard2 = true;
      }
    }
  } else if (z_out < 0) {
    sLong2MultiWord(y, (uint32_T *)(&r32.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r32.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r30.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r30.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r28.chunks[0U]));
    sLong2MultiWord(z_out, (uint32_T *)(&r32.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r32.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r30.chunks[0U]));
    if (sMultiWordEq((uint32_T *)(&r28.chunks[0U]), (uint32_T *)(&r30.chunks[0U]),
                     2)) {
      thPreCorr = -19302;
    } else if (y >= 0) {
      if (z_out <= MIN_int32_T) {
        el_in_rad = MAX_int32_T;
      } else {
        el_in_rad = -z_out;
      }

      sLong2MultiWord(el_in_rad, (uint32_T *)(&r41.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r41.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r42.chunks[0U]));
      sLong2MultiWord(y, (uint32_T *)(&r39.chunks[0U]), 2);
      uMultiWordShl((uint32_T *)(&r39.chunks[0U]), 2, 1U, (uint32_T *)
                    (&r35.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r35.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r41.chunks[0U]));
      if (sMultiWordEq((uint32_T *)(&r42.chunks[0U]), (uint32_T *)(&r41.chunks
            [0U]), 2)) {
        thPreCorr = -6434;
      } else {
        sLong2MultiWord(el_in_rad, (uint32_T *)(&r39.chunks[0U]), 2);
        MultiWordSignedWrap((uint32_T *)(&r39.chunks[0U]), 2, 31U, (uint32_T *)(
          &r35.chunks[0U]));
        sLong2MultiWord(y, (uint32_T *)(&r36.chunks[0U]), 2);
        uMultiWordShl((uint32_T *)(&r36.chunks[0U]), 2, 1U, (uint32_T *)
                      (&r38.chunks[0U]), 2);
        MultiWordSignedWrap((uint32_T *)(&r38.chunks[0U]), 2, 31U, (uint32_T *)(
          &r39.chunks[0U]));
        if (sMultiWordLt((uint32_T *)(&r35.chunks[0U]), (uint32_T *)
                         (&r39.chunks[0U]), 2)) {
          if (y == 0) {
            idxUFIX16 = MAX_uint16_T;
          } else {
            ytemp = div_repeat_s32_sat(el_in_rad, y, 15U);
            if (ytemp < 0) {
              ytemp = 0;
            } else {
              if (ytemp > 65535) {
                ytemp = 65535;
              }
            }

            idxUFIX16 = (uint16_T)ytemp;
          }
        } else {
          ytemp = div_repeat_s32_sat(y, el_in_rad, 17U);
          if (ytemp < 0) {
            ytemp = 0;
          } else {
            if (ytemp > 65535) {
              ytemp = 65535;
            }
          }

          idxUFIX16 = (uint16_T)ytemp;
        }

        guard1 = true;
      }
    } else {
      if (y <= MIN_int32_T) {
        negThisX = MAX_int32_T;
      } else {
        negThisX = -y;
      }

      if (z_out <= MIN_int32_T) {
        el_in_rad = MAX_int32_T;
      } else {
        el_in_rad = -z_out;
      }

      sLong2MultiWord(el_in_rad, (uint32_T *)(&r39.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r39.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r32.chunks[0U]));
      sLong2MultiWord(negThisX, (uint32_T *)(&r41.chunks[0U]), 2);
      uMultiWordShl((uint32_T *)(&r41.chunks[0U]), 2, 1U, (uint32_T *)
                    (&r42.chunks[0U]), 2);
      MultiWordSignedWrap((uint32_T *)(&r42.chunks[0U]), 2, 31U, (uint32_T *)
                          (&r39.chunks[0U]));
      if (sMultiWordLe((uint32_T *)(&r32.chunks[0U]), (uint32_T *)(&r39.chunks
            [0U]), 2)) {
        ytemp = div_repeat_s32_sat(el_in_rad, negThisX, 15U);
        if (ytemp < 0) {
          ytemp = 0;
        } else {
          if (ytemp > 65535) {
            ytemp = 65535;
          }
        }

        idxUFIX16 = (uint16_T)ytemp;
      } else {
        ytemp = div_repeat_s32_sat(negThisX, el_in_rad, 17U);
        if (ytemp < 0) {
          ytemp = 0;
        } else {
          if (ytemp > 65535) {
            ytemp = 65535;
          }
        }

        idxUFIX16 = (uint16_T)ytemp;
      }

      guard1 = true;
    }
  } else if (y >= 0) {
    thPreCorr = 0;
  } else {
    thPreCorr = 25736;
  }

  if (guard2) {
    slice_temp = (uint8_T)(((uint32_T)idxUFIX16) >> 8U);
    thPreCorr = (int16_T)((uint32_T)(((uint32_T)((uint16_T)(((((uint32_T)
      ATAN_UFRAC_LUT[slice_temp]) << 14U) + (((uint32_T)((uint16_T)(mul_u32_loSR
      ((uint32_T)((int32_T)(((int32_T)idxUFIX16) & ((uint16_T)255))),
       (((uint32_T)ATAN_UFRAC_LUT[((int32_T)slice_temp) + 1]) << 14U) -
       (((uint32_T)ATAN_UFRAC_LUT[(int32_T)((uint32_T)(((uint32_T)idxUFIX16) >>
      8U))]) << 14U), 8U) >> 14U))) << 14U)) >> 14U))) >> 3U));
    sLong2MultiWord(y, (uint32_T *)(&r37.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r37.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r38.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r38.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r40.chunks[0U]));
    sLong2MultiWord(z_out, (uint32_T *)(&r37.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r37.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r38.chunks[0U]));
    sLong2MultiWord(z_out, (uint32_T *)(&r46.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r46.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r37.chunks[0U]));
    sLong2MultiWord(negThisX, (uint32_T *)(&r48.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r48.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r49.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r49.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r46.chunks[0U]));
    if (y >= 0) {
      if (sMultiWordLt((uint32_T *)(&r40.chunks[0U]), (uint32_T *)(&r38.chunks
            [0U]), 2)) {
        thPreCorr = (int16_T)(12867 - thPreCorr);
      }
    } else if (sMultiWordGt((uint32_T *)(&r37.chunks[0U]), (uint32_T *)
                            (&r46.chunks[0U]), 2)) {
      thPreCorr = (int16_T)(thPreCorr + 12867);
    } else {
      thPreCorr = (int16_T)(25735 - thPreCorr);
    }
  }

  if (guard1) {
    slice_temp = (uint8_T)(((uint32_T)idxUFIX16) >> 8U);
    thPreCorr = (int16_T)((uint32_T)(((uint32_T)((uint16_T)(((((uint32_T)
      ATAN_UFRAC_LUT[slice_temp]) << 14U) + (((uint32_T)((uint16_T)(mul_u32_loSR
      ((uint32_T)((int32_T)(((int32_T)idxUFIX16) & ((uint16_T)255))),
       (((uint32_T)ATAN_UFRAC_LUT[((int32_T)slice_temp) + 1]) << 14U) -
       (((uint32_T)ATAN_UFRAC_LUT[(int32_T)((uint32_T)(((uint32_T)idxUFIX16) >>
      8U))]) << 14U), 8U) >> 14U))) << 14U)) >> 14U))) >> 3U));
    sLong2MultiWord(el_in_rad, (uint32_T *)(&r36.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r36.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r38.chunks[0U]));
    sLong2MultiWord(y, (uint32_T *)(&r40.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r40.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r37.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r37.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r36.chunks[0U]));
    sLong2MultiWord(el_in_rad, (uint32_T *)(&r40.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r40.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r37.chunks[0U]));
    sLong2MultiWord(negThisX, (uint32_T *)(&r33.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r33.chunks[0U]), 2, 1U, (uint32_T *)
                  (&r34.chunks[0U]), 2);
    MultiWordSignedWrap((uint32_T *)(&r34.chunks[0U]), 2, 31U, (uint32_T *)
                        (&r40.chunks[0U]));
    if (y >= 0) {
      if (sMultiWordLe((uint32_T *)(&r38.chunks[0U]), (uint32_T *)(&r36.chunks
            [0U]), 2)) {
        thPreCorr = (int16_T)(-thPreCorr);
      } else {
        thPreCorr = (int16_T)((((int32_T)thPreCorr) - 12867) | -65536);
      }
    } else if (sMultiWordGt((uint32_T *)(&r37.chunks[0U]), (uint32_T *)
                            (&r40.chunks[0U]), 2)) {
      if (((-(((int32_T)thPreCorr) + 12867)) & 65536) != 0) {
        thPreCorr = (int16_T)((-(((int32_T)thPreCorr) + 12867)) | -65536);
      } else {
        thPreCorr = (int16_T)((-(((int32_T)thPreCorr) + 12867)) & 65535);
      }
    } else {
      thPreCorr = (int16_T)((((int32_T)thPreCorr) - 25735) | -65536);
    }
  }

  var1 = b_atan2(y_out, x_out);
  el_in_rad = ((int32_T)var1) * 32768;
  if (el_in_rad < 0) {
    sLong2MultiWord(el_in_rad, (uint32_T *)(&r43.chunks[0U]), 2);
    uMultiWordShl((uint32_T *)(&r43.chunks[0U]), 2, 30U, (uint32_T *)
                  (&r45.chunks[0U]), 2);
    r43 = r47;
    MultiWordAdd((uint32_T *)(&r45.chunks[0U]), (uint32_T *)(&r47.chunks[0U]),
                 (uint32_T *)(&r48.chunks[0U]), 2);
    sMultiWordShr((uint32_T *)(&r48.chunks[0U]), 2, 30U, (uint32_T *)
                  (&r49.chunks[0U]), 2);
    el_in_rad = MultiWord2sLong((uint32_T *)(&r49.chunks[0U]));
  }

  sLong2MultiWord(el_in_rad, (uint32_T *)(&r44.chunks[0U]), 2);
  uMultiWordShl((uint32_T *)(&r44.chunks[0U]), 2, 31U, (uint32_T *)(&r43.chunks
    [0U]), 2);
  u1 = 1367130551U;
  ssuMultiWordMul((uint32_T *)(&r43.chunks[0U]), 2, (uint32_T *)(&u1), 1,
                  (uint32_T *)(&r7.chunks[0U]), 3);
  sMultiWordShr((uint32_T *)(&r7.chunks[0U]), 3, 31U, (uint32_T *)(&r8.chunks[0U]),
                3);
  sMultiWord2MultiWord((uint32_T *)(&r8.chunks[0U]), 3, (uint32_T *)
                       (&r45.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r45.chunks[0U]), 2, 29U, (uint32_T *)(&r48.chunks
    [0U]), 2);
  *az_out = MultiWord2uLong((uint32_T *)(&r48.chunks[0U]));
  sLong2MultiWord(((int32_T)thPreCorr) * 131072, (uint32_T *)(&r50.chunks[0U]),
                  2);
  uMultiWordShl((uint32_T *)(&r50.chunks[0U]), 2, 31U, (uint32_T *)(&r44.chunks
    [0U]), 2);
  ssuMultiWordMul((uint32_T *)(&r44.chunks[0U]), 2, (uint32_T *)(&u1), 1,
                  (uint32_T *)(&r7.chunks[0U]), 3);
  sMultiWordShr((uint32_T *)(&r7.chunks[0U]), 3, 31U, (uint32_T *)(&r8.chunks[0U]),
                3);
  sMultiWord2MultiWord((uint32_T *)(&r8.chunks[0U]), 3, (uint32_T *)
                       (&r43.chunks[0U]), 2);
  sMultiWordShr((uint32_T *)(&r43.chunks[0U]), 2, 30U, (uint32_T *)(&r45.chunks
    [0U]), 2);
  *el_out = MultiWord2sLong((uint32_T *)(&r45.chunks[0U]));

  /*      az_out = uint32(az_out); */
  /*      el_out = int32(el_out); */
  /*      r_out = uint32(r_out); */
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void ParalaxCalc_fixpt_initialize(void)
{
  cos_init();
  sin_init();
  atan2_fi_lut_private_init();
}

/*
 * File trailer for ParalaxCalc_fixpt.c
 *
 * [EOF]
 */
