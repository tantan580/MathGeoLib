#include "../src/Math/myassert.h"
#include "../src/MathGeoLib.h"
#include "../tests/TestRunner.h"
#include "SystemInfo.h"

TEST(MonotonousClock)
{
	tick_t maxDiff = 0;
	tick_t prev = Clock::Tick();
	for(int i = 0; i < 10000; ++i)
	{
		tick_t now = Clock::Tick();
		tick_t diff = Clock::TicksInBetween(now, prev);
		assert(diff >= 0);
		assert(diff < Clock::TicksPerSec());
		prev = now;
		maxDiff = Max(diff, maxDiff);
	}

	assert(maxDiff > 0); // The clock must proceed at least some amount.
}

UNIQUE_TEST(ClockPrecision)
{
	LOGI("Clock::Tick() runs at %llu ticks/second.", Clock::TicksPerSec());
}

UNIQUE_TEST(Clock_RdTsc)
{
	unsigned long long tsc = Clock::Rdtsc();
	unsigned long long tsc2 = Clock::Rdtsc();
	LOGI("Two subsequent calls to rdtsc report %llu and %llu. (delta: %llu)", tsc, tsc2, tsc2 - tsc);
}

UNIQUE_TEST(SubMillisecondPrecision)
{
#ifdef EMSCRIPTEN
	if (IsChromeBrowserOnWin32())
	{
		if (GetChromeVersion() <= BrowserVersion("28.0.1500.95") || GetChromeVersion() == BrowserVersion("30.0.1599.0"))
			WARN_AND_EXPECT_FAIL("Chrome on Win32 has bad timer resolution: https://code.google.com/p/chromium/issues/detail?id=158234");
	}
	if (IsOperaBrowser() && GetOperaVersion() <= BrowserVersion("12.16"))
		WARN_AND_EXPECT_FAIL("Opera has bad timer resolution and doesn't support window.performance.now().");
	if (IsSafariBrowser() && GetSafariVersion() <= BrowserVersion("6.0.5"))
		WARN_AND_EXPECT_FAIL("Safari has bad timer resolution and doesn't support window.performance.now().");
#endif

	tick_t ticksPerMillisecond = Clock::TicksPerMillisecond();
	assert(ticksPerMillisecond > 1);

	tick_t minDiff = Clock::TicksPerSec();
	tick_t prev = Clock::Tick();
	int numTimesZeroDiff = 0;
	int numIters = 50000;
	for(int i = 0; i < numIters; ++i)
	{
		tick_t now = Clock::Tick();
		tick_t diff = Clock::TicksInBetween(now, prev);
		if (diff > 0)
			minDiff = Min(diff, minDiff);
		else
			++numTimesZeroDiff;

		prev = now;
	}

	LOGI("Smallest observed non-zero delta in Clock::Tick() is %d ticks. A zero delta was observed %d times (out of %d tests)", 
		(int)minDiff, numTimesZeroDiff, numIters);
	assert(minDiff > 0);
	assert(minDiff < ticksPerMillisecond/2); // Smallest met quantity must be less than half a millisecond.
}
