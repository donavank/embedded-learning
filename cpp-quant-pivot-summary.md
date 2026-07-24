# Context Summary: C++/Quant Dev Career Pivot

## Background & Goal
Currently a full-stack web developer using Ruby on Rails and React. Goal is to pivot to C++/performance-focused software engineering, with the ultimate aim of joining a quant trading firm.

## Current-Job Leverage
Plan to pick up performance-sensitive tickets, concurrency/race-condition bugs, and infra-adjacent work (deployment, caching, message queues) at the current job to build adjacent skills, even though the day-to-day stack (Rails/React) doesn't map directly.

## Target Positions & Strategy
Rather than jumping straight to a top-tier HFT firm, the plan is to use a stepping-stone role first:
- Systems/infrastructure engineer roles (any C++ shop, not necessarily finance)
- Market data / exchange connectivity engineer roles
- Backend/infra roles at fintech or market-data vendors
- Mid-tier prop shops / trading firms (more open to non-traditional backgrounds than top-tier firms like Jane Street, HRT, Citadel Securities, Jump)
- Embedded roles (see below) are also being considered as a stepping stone, since they're often more attainable and build real-time/resource-constrained instincts
- Freelance C++/embedded work is being treated as opportunistic supplementary experience, not a primary strategy (entry-level C++ freelance is thin and often requires credibility not yet built)

## Study Plan (12-Month Roadmap)
A month-by-month self-study plan was created and saved as an artifact (**cpp-quant-study-plan.md**). Structure:

- **Months 1-2:** C++ fundamentals (modern C++, RAII, smart pointers, templates, STL); toolchain setup
- **Months 3-4:** Data structures & algorithms in C++, "Effective Modern C++" material, custom data structure project
- **Months 5-6:** Concurrency (threads, atomics, memory model) and systems fundamentals (OS scheduling, cache locality, networking basics); lock-free queue project
- **Months 7-8:** Low-latency systems techniques (memory pools, lock-free structures, kernel bypass concepts) and market microstructure basics; **major project: a limit order book matching engine in C++** (the centerpiece resume artifact)
- **Month 9:** Profiling and performance engineering (perf, flame graphs, optimization case studies)
- **Month 10:** Resume/project polish, networking, identifying target companies
- **Months 11-12:** Interview-specific prep (C++ trivia, systems design, timed coding) and active applications

**Parallel track (Months 3-8): ESP32 embedded development.** Since the ESP32 work is dual-core and network-enabled (FreeRTOS, WiFi, lwIP), this track reinforces the main roadmap rather than being a separate detour — it covers real multi-core task scheduling and socket programming, not just bare-metal GPIO work. It culminates in a bridge project: an ESP32 sampling a sensor on one core while streaming timestamped data over WiFi on the other, with a measured latency breakdown (task switching, queue hand-off, WiFi/lwIP overhead) as a concrete interview talking point.

## Key Decisions Made
- Targeting **C++17** as the core standard to master deeply, with working knowledge of C++20 concepts (concepts, ranges, coroutines, modules); C++23 treated as supplementary, not a priority, since most production codebases (including many quant firms) lag the latest standard.
- Using **CMake with Ninja** as the build system generator (faster builds, standard in modern C++ tooling, no lock-in vs. Make since it's just a generator flag).
- Using learncpp.com as a targeted reference/lookup tool for concepts that need deeper explanation, rather than a primary linear course — books remain the primary structured material since they assume prior programming experience.

## Not Included Here
Specific book titles/recommendations and detailed toolchain setup steps were discussed but are intentionally left out of this summary per request — refer to the original conversation or the study plan artifact for those specifics.
