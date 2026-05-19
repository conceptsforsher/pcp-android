# C++ Latency Tracker (`latency_stats.cpp`)

## Overview

A high-precision network latency measurement and analysis engine. Records every HTTP request made by the Matrix SDK, computes real-time statistics including percentiles (P50/P95/P99), standard deviation, jitter, and packet loss rate. Designed as a drop-in observability tool that integrates into the OkHttp interceptor chain.

This module is the foundation for Progressive Chat's network debugging capabilities. It enables features like the "Network Statistics" dashboard, per-server latency comparison, and automatic server selection based on historical performance.

## Terminology

**Latency** is the round-trip time from request dispatch to response completion.

**Jitter** is the average absolute difference between consecutive latency samples. It measures connection stability — low jitter means consistent latency, high jitter means fluctuating network conditions. Jitter is critical for real-time features like voice/video calls.

**Packet loss rate** is the fraction of requests that failed (non-2xx HTTP status or network error). Even on TCP connections with retransmission, application-level failures indicate network health problems.

**Percentiles (P50/P95/P99)** tell a more accurate story than simple averages. A server might have 100ms average latency but P99 of 5000ms — meaning 1% of requests are extremely slow. This is invisible to a simple average calculation.

## Architecture

### Measurement Collection

Latency samples are collected at the OkHttp interceptor level in the Kotlin layer. Each HTTP request goes through an interceptor that:

1. Records `startTimeMs` using `nanoTime()`
2. Waits for the response (or failure)
3. Records `endTimeMs` and computes `latencyMs = endTimeMs - startTimeMs`
4. Calls `ProgressiveNative.nativeLatencyRecord(latencyMs, serverName, endpoint, success)`

This interceptor approach captures ALL Matrix traffic — sync requests, media uploads, `/send` calls, `/members` queries — without modifying the SDK.

### Statistical Computation

When the user opens the Network Statistics dashboard, `computeStats()` aggregates all samples into a `LatencyStats` structure:

```
Raw Samples: [15ms, 22ms, 18ms, 500ms, 17ms, 20ms, 19ms]
                    │
                    ▼
Sort:          [15, 17, 18, 19, 20, 22, 500]
                    │
    ├── Minimum:    15ms
    ├── Maximum:    500ms
    ├── Average:    (15+17+18+19+20+22+500) / 7 = 87ms
    │
    ├── P50 (Median): 19ms  (middle value)
    ├── P95:          500ms (value at 95th percentile)
    ├── P99:          500ms (value at 99th percentile)
    │
    ├── StdDev:  sqrt(sum((x - avg)²) / n) ≈ 160ms
    └── Jitter:  avg(|17-15|, |18-17|, |19-18|, |20-19|, |22-20|, |500-22|) ≈ 80ms
```

Notice how the average of 87ms is misleading — most requests complete in 15-22ms, but one slow 500ms request inflates the average. The P50 of 19ms tells the real story.

### Percentile Calculation

The percentile function uses linear interpolation between adjacent sorted values:

```cpp
double computePercentile(vector<double> sorted, double p) {
    double idx = p * (sorted.size() - 1);
    int lo = floor(idx);
    int hi = ceil(idx);
    double frac = idx - lo;
    return sorted[lo] * (1.0 - frac) + sorted[hi] * frac;
}
```

For example, with 100 samples, P95 = index 94.05. The value is interpolated between sorted[94] and sorted[95].

## Data Retention

The `prune(maxAgeSeconds)` method removes samples older than the specified threshold. This is important because:

1. **Memory control.** Without pruning, a long-running app would accumulate millions of samples.
2. **Window relevance.** Month-old latency data is not meaningful for current network conditions.
3. **Adapter changes.** When the user switches from WiFi to cellular, old samples on the previous network become irrelevant.

The default retention window is 3600 seconds (1 hour), configurable per use case.

## Per-Server Analysis

`computeServerStats(serverName)` filters samples by server and computes independent statistics. This enables:

- **Home server comparison.** Is matrix.org faster than server.com?
- **Media server performance.** Which CDN endpoint has lower latency?
- **Identity server health.** Is the identity server responsive?

The Kotlin UI can display per-server stats side by side for easy comparison.

## JNI API

```kotlin
// Called from OkHttp interceptor
ProgressiveNative.nativeLatencyRecord(
    latencyMs = 23.5,
    server = "matrix.org",
    endpoint = "/_matrix/client/v3/sync",
    success = true
)

// Display stats
val json = ProgressiveNative.nativeLatencyStats()
// {"avgMs": 87.3, "p95Ms": 500.0, "jitterMs": 80.2, "packetLossRate": 0.0}

val text = ProgressiveNative.nativeLatencyStatsText()
// Latency Stats (142 samples)
//   Avg:   87ms
//   P50:   19ms
//   P95:   500ms
//   Jitter: 80ms
//   Loss:  0.0%
```

## Performance

Stats computation for 10,000 samples takes ~2ms. The heavy operation is sorting — `O(n log n)` — which only runs when the dashboard is opened, not on every sample recording.

Sample recording is `O(1)` — a simple `push_back` to a `vector`.
