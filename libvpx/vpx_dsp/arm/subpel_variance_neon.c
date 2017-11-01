/*
 *  Copyright (c) 2014 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <arm_neon.h>
#include "./vpx_dsp_rtcd.h"
#include "./vpx_config.h"

#include "vpx_ports/mem.h"
#include "vpx/vpx_integer.h"

#include "vpx_dsp/variance.h"

static const uint8_t bilinear_filters[8][2] = {
  { 128, 0 }, { 112, 16 }, { 96, 32 }, { 80, 48 },
  { 64, 64 }, { 48, 80 },  { 32, 96 }, { 16, 112 },
};

// Process a block exactly 8 wide and any height.
static void var_filter_block2d_bil_w8(const uint8_t *src_ptr,
                                      uint8_t *output_ptr,
                                      unsigned int src_pixels_per_line,
                                      int pixel_step,
                                      unsigned int output_height,
                                      const uint8_t *filter) {
  const uint8x8_t f0 = vmov_n_u8(filter[0]);
  const uint8x8_t f1 = vmov_n_u8(filter[1]);
  unsigned int i;
  for (i = 0; i < output_height; ++i) {
    const uint8x8_t src_0 = vld1_u8(&src_ptr[0]);
    const uint8x8_t src_1 = vld1_u8(&src_ptr[pixel_step]);
    const uint16x8_t a = vmull_u8(src_0, f0);
    const uint16x8_t b = vmlal_u8(a, src_1, f1);
    const uint8x8_t out = vrshrn_n_u16(b, FILTER_BITS);
    vst1_u8(&output_ptr[0], out);
    // Next row...
    src_ptr += src_pixels_per_line;
    output_ptr += 8;
  }
}

// Process a block which is a mutiple of 16 wide and any height.
static void var_filter_block2d_bil_w16(const uint8_t *src_ptr,
                                       uint8_t *output_ptr,
                                       unsigned int src_pixels_per_line,
                                       int pixel_step,
                                       unsigned int output_height,
                                       unsigned int output_width,
                                       const uint8_t *filter) {
  const uint8x8_t f0 = vmov_n_u8(filter[0]);
  const uint8x8_t f1 = vmov_n_u8(filter[1]);
  unsigned int i, j;
  for (i = 0; i < output_height; ++i) {
    for (j = 0; j < output_width; j += 16) {
      const uint8x16_t src_0 = vld1q_u8(&src_ptr[j]);
      const uint8x16_t src_1 = vld1q_u8(&src_ptr[j + pixel_step]);
      const uint16x8_t a = vmull_u8(vget_low_u8(src_0), f0);
      const uint16x8_t b = vmlal_u8(a, vget_low_u8(src_1), f1);
      const uint8x8_t out_lo = vrshrn_n_u16(b, FILTER_BITS);
      const uint16x8_t c = vmull_u8(vget_high_u8(src_0), f0);
      const uint16x8_t d = vmlal_u8(c, vget_high_u8(src_1), f1);
      const uint8x8_t out_hi = vrshrn_n_u16(d, FILTER_BITS);
      vst1q_u8(&output_ptr[j], vcombine_u8(out_lo, out_hi));
    }
    // Next row...
    src_ptr += src_pixels_per_line;
    output_ptr += output_width;
  }
}

// TODO(johannkoenig): support 4xM block sizes.
#define sub_pixel_varianceNxM(n, m)                                      \
  unsigned int vpx_sub_pixel_variance##n##x##m##_neon(                   \
      const uint8_t *src, int src_stride, int xoffset, int yoffset,      \
      const uint8_t *dst, int dst_stride, unsigned int *sse) {           \
    DECLARE_ALIGNED(16, uint8_t, fdata3[n * (m + 1)]);                   \
    DECLARE_ALIGNED(16, uint8_t, temp2[n * m]);                          \
                                                                         \
    if (n == 8) {                                                        \
      var_filter_block2d_bil_w8(src, fdata3, src_stride, 1, (m + 1),     \
                                bilinear_filters[xoffset]);              \
      var_filter_block2d_bil_w8(fdata3, temp2, n, n, m,                  \
                                bilinear_filters[yoffset]);              \
    } else {                                                             \
      var_filter_block2d_bil_w16(src, fdata3, src_stride, 1, (m + 1), n, \
                                 bilinear_filters[xoffset]);             \
      var_filter_block2d_bil_w16(fdata3, temp2, n, n, m, n,              \
                                 bilinear_filters[yoffset]);             \
    }                                                                    \
    return vpx_variance##n##x##m(temp2, n, dst, dst_stride, sse);        \
  }

sub_pixel_varianceNxM(8, 4);
sub_pixel_varianceNxM(8, 8);
sub_pixel_varianceNxM(8, 16);
sub_pixel_varianceNxM(16, 8);
sub_pixel_varianceNxM(16, 16);
sub_pixel_varianceNxM(16, 32);
sub_pixel_varianceNxM(32, 16);
sub_pixel_varianceNxM(32, 32);
sub_pixel_varianceNxM(32, 64);
sub_pixel_varianceNxM(64, 32);
sub_pixel_varianceNxM(64, 64);
