//生成跨 centroid 并行 LUT 构建的汇编代码的独立源文件
#include <arm_neon.h>
#include <cstddef>

// 内联一个简化的点积函数（实际上你会用 inner_product_neon_optB，但这里为了汇编清晰，直接写个基本 NEON 版本）
static inline float dot_product(const float* a, const float* b, int d) {
    float32x4_t sum = vdupq_n_f32(0.0f);
    for (int i = 0; i < d; i += 4) {
        float32x4_t va = vld1q_f32(a + i);
        float32x4_t vb = vld1q_f32(b + i);
        sum = vmlaq_f32(sum, va, vb);
    }
    return vaddvq_f32(sum);
}

// 跨 centroid 并行：一次计算 4 个中心的点积，并存入 LUT
void build_lut_centroid_parallel(const float* sub_q,
                                 const float* codebook,  // 形状 [256][sub_dim]
                                 float* lut,
                                 int sub_dim) {
    for (int c = 0; c < 256; c += 4) {
        float32x4_t sum0 = vdupq_n_f32(0.0f);
        float32x4_t sum1 = vdupq_n_f32(0.0f);
        float32x4_t sum2 = vdupq_n_f32(0.0f);
        float32x4_t sum3 = vdupq_n_f32(0.0f);

        for (int d = 0; d < sub_dim; ++d) {
            float32x4_t qd = vdupq_n_f32(sub_q[d]);

            float32x4_t c0 = vld1q_f32(codebook + (c+0)*sub_dim + d);
            float32x4_t c1 = vld1q_f32(codebook + (c+1)*sub_dim + d);
            float32x4_t c2 = vld1q_f32(codebook + (c+2)*sub_dim + d);
            float32x4_t c3 = vld1q_f32(codebook + (c+3)*sub_dim + d);

            sum0 = vmlaq_f32(sum0, c0, qd);
            sum1 = vmlaq_f32(sum1, c1, qd);
            sum2 = vmlaq_f32(sum2, c2, qd);
            sum3 = vmlaq_f32(sum3, c3, qd);
        }

        float tmp[4];
        vst1q_f32(tmp, sum0); lut[c+0] = 1.0f - tmp[0];
        vst1q_f32(tmp, sum1); lut[c+1] = 1.0f - tmp[0];
        vst1q_f32(tmp, sum2); lut[c+2] = 1.0f - tmp[0];
        vst1q_f32(tmp, sum3); lut[c+3] = 1.0f - tmp[0];
    }
}