# 10-1
Assume a system where the value returned by the call `sysconf(_SC_CLK_TCK)` is 100. Assuming that the clock_t value returned by times() is an unsigned 32-bit integer, how long will it take before this value cycles so that it restarts at 0? Perform the same calculation for the `CLOCKS_PER_SEC` value returned by `clock()`.

## answer
Caculate cycle time with assuming `syscong(_SC_CLK_TCK)` is 100.

$T_{cycle\_time} = \frac{2^{32}-1}{100} = 42949672.95 seconds \approx 497.1 days$

Caculate cycle time using `CLOCKS_PER_SEC` returned by `clock()`, which is 1,000,000.

$T_{cycle\_time} = \frac{2^{32}-1}{1000000} = 4294.96 seconds \approx 1.19 hours$

