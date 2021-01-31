#ifndef _TONEMAPPING_H_
#define _TONEMAPPING_H_

#include <stdint.h>

enum ToneMapMode
{
	TM_ACES,
	TM_UC2,
	TM_HEJL_RICHARD,
	TM_REINHARD
};

void initToneMap(void);
void toneMap(enum ToneMapMode tm, float *exr, uint8_t *ldr, uint32_t width, uint32_t height, float exposure, float gamma);
void toneMapMT(enum ToneMapMode tm, float *exr, uint8_t *ldr, uint32_t width, uint32_t height, float exposure, float gamma);
void termToneMap(void);

#endif /* _TONEMAPPING_H_ */
