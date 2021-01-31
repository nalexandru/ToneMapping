#include <math.h>

#include <Windows.h>
#include <xmmintrin.h>

#include "Tonemapping.h"

struct ThreadArgs
{
	float *hdr;
	uint8_t *ldr;
	uint32_t count;
	float exposure, invGamma;
	HANDLE evt;
};

static uint32_t _cpuCount = 0;
static HANDLE *_threads = NULL;
static struct ThreadArgs *_args = NULL;

static inline float
clamp(float v, float min, float max)
{
	_mm_store_ss(&v, _mm_min_ss(_mm_max_ss(_mm_set_ss(v), _mm_set_ss(min)), _mm_set_ss(max)));
	return v;
}

static DWORD WINAPI
_acesToneMapProc(void *a)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	const struct ThreadArgs args = *(struct ThreadArgs *)a;
	
	uint8_t *ldr = args.ldr;
	for (uint32_t i = 0; i < args.count; i += 4) {
		const float hdr[4] = { args.hdr[i], args.hdr[i + 1], args.hdr[i + 2], args.hdr[i + 3] };

		float f, m;
		f =  1.f - expf(-hdr[0] * args.exposure);
		m = (f * (A * f + B)) / (f * (C * f + D) + E);
		ldr[0] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[1] * args.exposure);
		m = (f * (A * f + B)) / (f * (C * f + D) + E);
		ldr[1] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[2] * args.exposure);
		m = (f * (A * f + B)) / (f * (C * f + D) + E);
		ldr[2] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		ldr += 3;
	}

	SetEvent(args.evt);

	return 0;
}

static DWORD WINAPI
_uc2ToneMapProc(void *a)
{
	const float A = 0.15f;
	const float B = 0.50f;
	const float C = 0.10f;
	const float D = 0.20f;
	const float E = 0.02f;
	const float F = 0.30f;
	const float whiteScale = 1.f / (((11.2f * (A * 11.2f + C * B) + D * E) / (11.2f * (A * 11.2f + B) + D * F)) - E / F);
	const struct ThreadArgs args = *(struct ThreadArgs *)a;

	uint8_t *ldr = args.ldr;
	for (uint32_t i = 0; i < args.count; i += 4) {
		const float hdr[4] = { args.hdr[i], args.hdr[i + 1], args.hdr[i + 2], args.hdr[i + 3] };

		float f, m;
		f =  (1.f - expf(-hdr[0] * args.exposure)) * 2.f;
		m = ((f * (A * f + C * B) + D * E) / (f * (A * f + B) + D * F)) - E / F;
		m *= whiteScale;
		ldr[0] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  (1.f - expf(-hdr[1] * args.exposure)) * 2.f;
		m = ((f * (A * f + C * B) + D * E) / (f * (A * f + B) + D * F)) - E / F;
		m *= whiteScale;
		ldr[1] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  (1.f - expf(-hdr[2] * args.exposure)) * 2.f;
		m = ((f * (A * f + C * B) + D * E) / (f * (A * f + B) + D * F)) - E / F;
		m *= whiteScale;
		ldr[2] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		ldr += 3;
	}

	SetEvent(args.evt);

	return 0;
}

static DWORD WINAPI
_hejlRichardToneMapProc(void *a)
{
	const struct ThreadArgs args = *(struct ThreadArgs *)a;
	
	uint8_t *ldr = args.ldr;
	for (uint32_t i = 0; i < args.count; i += 4) {
		const float hdr[4] = { args.hdr[i], args.hdr[i + 1], args.hdr[i + 2], args.hdr[i + 3] };

		float f;
		f =  1.f - expf(-hdr[0] * args.exposure);
		f = max(0.f, f - 0.004f);
		f = (f * (6.2f * f + 0.5f)) / (f * (6.2f * f + 1.7f) + 0.06f);
		ldr[0] = (uint8_t)(clamp(f, 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[1] * args.exposure);
		f = max(0.f, f - 0.004f);
		f = (f * (6.2f * f + 0.5f)) / (f * (6.2f * f + 1.7f) + 0.06f);
		ldr[1] = (uint8_t)(clamp(f, 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[2] * args.exposure);
		f = max(0.f, f - 0.004f);
		f = (f * (6.2f * f + 0.5f)) / (f * (6.2f * f + 1.7f) + 0.06f);
		ldr[2] = (uint8_t)(clamp(f, 0.f, 1.f) * 255);

		ldr += 3;
	}

	SetEvent(args.evt);

	return 0;
}

static DWORD WINAPI
_reinhardToneMapProc(void *a)
{
	const struct ThreadArgs args = *(struct ThreadArgs *)a;
	
	uint8_t *ldr = args.ldr;
	for (uint32_t i = 0; i < args.count; i += 4) {
		const float hdr[4] = { args.hdr[i], args.hdr[i + 1], args.hdr[i + 2], args.hdr[i + 3] };

		float f, m;
		f =  1.f - expf(-hdr[0] * args.exposure);
		m = f / (1.f + f);
		ldr[0] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[1] * args.exposure);
		m = f / (1.f + f);
		ldr[1] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		f =  1.f - expf(-hdr[2] * args.exposure);
		m = f / (1.f + f);
		ldr[2] = (uint8_t)(clamp(powf(m, args.invGamma), 0.f, 1.f) * 255);

		ldr += 3;
	}

	SetEvent(args.evt);

	return 0;
}

void
initToneMap(void)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	_cpuCount = sysInfo.dwNumberOfProcessors;

	_args = (struct ThreadArgs *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _cpuCount * sizeof(*_args));
	_threads = (HANDLE *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _cpuCount * sizeof(*_threads));

	for (uint32_t i = 0; i < _cpuCount; ++i)
		_args[i].evt = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void
toneMap(enum ToneMapMode tm, float *exr, uint8_t *ldr, uint32_t width, uint32_t height, float exposure, float gamma)
{
	_args[0].hdr = exr;
	_args[0].ldr = ldr;
	_args[0].exposure = exposure;
	_args[0].invGamma = 1.f / gamma;
	_args[0].count = width * height * 4;

	switch (tm) {
	case TM_UC2: _uc2ToneMapProc(&_args[0]); break;
	case TM_HEJL_RICHARD: _hejlRichardToneMapProc(&_args[0]); break;
	case TM_REINHARD: _reinhardToneMapProc(&_args[0]); break;
	case TM_ACES:
	default: _acesToneMapProc(&_args[0]); break;
	}

	ResetEvent(_args[0].evt);
}

void
toneMapMT(enum ToneMapMode tm, float *hdr, uint8_t *ldr, uint32_t width, uint32_t height, float exposure, float gamma)
{
	const uint32_t hdrShareSize = (width * height * 4) / _cpuCount;
	const uint32_t ldrShareSize = (width * height * 3) / _cpuCount;

	const float invGamma = 1.f / gamma;

	LPTHREAD_START_ROUTINE proc = NULL;
	switch (tm) {
	case TM_UC2: proc = _uc2ToneMapProc; break;
	case TM_HEJL_RICHARD: proc = _hejlRichardToneMapProc; break;
	case TM_REINHARD: proc = _reinhardToneMapProc; break;
	case TM_ACES:
	default: proc = _acesToneMapProc; break;
	}

	for (uint32_t i = 0; i < _cpuCount; ++i) {
		_args[i].hdr = hdr; hdr += hdrShareSize;
		_args[i].ldr = ldr; ldr += ldrShareSize;
		_args[i].exposure = exposure;
		_args[i].invGamma = invGamma;
		_args[i].count = hdrShareSize;

		CreateThread(NULL, 0, proc, &_args[i], 0, NULL);
	}

	for (uint32_t i = 0; i < _cpuCount; ++i) {
		WaitForSingleObject(_args[i].evt, INFINITE);
		CloseHandle(_threads[i]);
	}
}

void
termToneMap(void)
{
	HeapFree(GetProcessHeap(), 0, _threads);
	HeapFree(GetProcessHeap(), 0, _args);
}