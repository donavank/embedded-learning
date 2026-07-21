# C++ / Quant Dev Transition: Month-by-Month Study Plan

**Goal:** Move from Rails/React full-stack dev → C++/performance-focused role → quant firm.
**Assumption:** ~10-15 hrs/week outside work. Adjust pace up or down accordingly — the *order* matters more than the exact timing.

---

## Month 1-2: C++ Fundamentals

**Focus:** Get fluent in modern C++, not just syntax — understand *why* it's fast.

- Work through **"A Tour of C++" (Stroustrup)** or an equivalent fast-paced modern C++ course. Don't linger on beginner tutorials aimed at people with no programming background — you already think like an engineer, you just need the C++ mental model.
- Core topics to nail: pointers vs. references, stack vs. heap, RAII, move semantics, smart pointers (`unique_ptr`, `shared_ptr`), templates, the STL (`vector`, `unordered_map`, `map`, iterators).
- **Project:** Port a small tool you've already built in Ruby/JS (a CLI utility, a parser, a small game) into C++. The goal isn't the tool — it's forcing yourself to hit real compiler errors and memory bugs.
- Set up your toolchain now: CMake, a debugger (gdb/lldb), a sanitizer (ASan/UBSan). You'll use these constantly.

**Milestone:** Comfortable writing a non-trivial C++ program without constantly reaching for Stack Overflow syntax help.

---

## A parallel track: ESP32 Embedded Development (Months 3-8)

Since you're specifically on ESP32 (dual-core, WiFi-enabled, running lwIP and typically FreeRTOS), this track is a closer match to the main quant-dev track than generic bare-metal embedded work would be — you get real concurrency (dual-core task scheduling) and real networking (TCP/IP over WiFi), not just GPIO and interrupts. Run it alongside the main track rather than sequentially.

Suggested pacing (a few hours a week, alongside the main track):

- **Months 3-4:** Get comfortable with ESP-IDF and FreeRTOS fundamentals on the ESP32 — tasks, queues, semaphores/mutexes, and how work is pinned to or split across the two cores. Also revisit GPIO/timers/interrupts if you haven't already, since you've done LED/microcontroller wiring — this is the natural next step from wiring to writing the firmware and task logic yourself.
- **Months 5-6:** Go deeper on the networking stack: socket programming over WiFi with lwIP, understanding where latency and jitter come from (WiFi driver buffering, lwIP stack overhead, task scheduling delays), and writing allocation-conscious code — ESP32 has more RAM than a typical MCU, but hot paths still benefit from avoiding heap churn, which is good practice for the low-latency material later.
- **Around Month 6-7 — bridge project:** Build something that ties both tracks together directly, e.g., an ESP32 sampling a sensor on one FreeRTOS task (pinned to one core) while another task (on the other core) streams timestamped readings over a socket using a small custom binary protocol you design. Then measure end-to-end latency and break down where it's lost — task switch overhead, queue hand-off between cores, WiFi/lwIP overhead. This is effectively a miniature, real version of the low-latency systems thinking from Months 5-8 of the main track, on hardware you already understand.

**Milestone:** Comfortable writing FreeRTOS-based, multi-core, network-connected C/C++ on ESP32, with a measured latency breakdown you can walk through in an interview — and able to draw an explicit connection between that experience and low-latency system design.

---

## Month 3-4: Data Structures, Algorithms & Effective C++

**Focus:** Build the CS fundamentals that quant interviews actually test, in C++ specifically.

- Read **"Effective Modern C++" (Scott Meyers)** — this is where the "why" clicks: move semantics, rvalue references, `auto`, lambda captures, rule of five.
- Start LeetCode (medium → hard), but **write every solution in C++**, not Python. Force yourself to think about memory allocation and complexity, not just correctness.
- Study complexity analysis until you can reason about it out loud, unprompted — quant interviews often push on "why is this O(n log n) and not O(n)?"
- **Project:** Build a small in-memory data structure from scratch that isn't in the STL — e.g., a fixed-size ring buffer, a simple LRU cache, an intrusive linked list. This is exactly the kind of thing quant infra interviews ask you to implement live.

**Milestone:** Can solve LeetCode mediums in C++ in under 25 minutes, and explain time/space tradeoffs clearly.

---

## Month 5-6: Concurrency & Systems Fundamentals

**Focus:** This is where quant infra interviews differentiate candidates — most web devs have never touched this.

- Concurrency: `std::thread`, mutexes, condition variables, atomics, the C++ memory model (acquire/release semantics). Understand what a data race actually is at the hardware level.
- Systems basics: how the OS scheduler works, virtual memory, how CPU caches work (cache lines, false sharing), branch prediction, why cache locality matters more than Big-O in practice.
- Networking basics: TCP vs UDP, what actually causes latency in a network call, socket programming in C++.
- **Project:** Build a simple multithreaded producer-consumer queue, then a lock-free version using atomics. Benchmark both under contention and write up the difference — this comparison is a great interview talking point.

**Milestone:** Can explain, from memory, why a lock-free queue is faster under contention and what its failure modes are.

---

## Month 7-8: Low-Latency Systems & Market Microstructure

**Focus:** Start speaking the specific language of quant/HFT infra.

- Low-latency techniques: memory pools / custom allocators, avoiding syscalls and heap allocation in hot paths, lock-free data structures, understanding kernel-bypass networking at a conceptual level (DPDK, `io_uring`, kernel bypass NICs — you don't need to implement these, just know what they solve).
- Market microstructure basics: what an order book is, how a matching engine works, market vs. limit orders, what "latency" means in a trading context (tick-to-trade, wire-to-wire). No finance background needed — just enough vocabulary to not be lost in an interview.
- **Major project starts here: a limit order book matching engine in C++.** This is the single highest-leverage project you can build — it combines data structures, performance optimization, and domain vocabulary in one artifact.
  - v1: correctness — orders in, matches out, basic price-time priority.
  - v2 (Month 8): optimize for speed — benchmark throughput, reduce allocations, profile with `perf`.

**Milestone:** Working order book engine with benchmarks you can discuss in an interview.

---

## Month 9: Profiling & Performance Engineering

**Focus:** Learn to *measure*, not guess — this is core quant-dev culture.

- Learn `perf`, flame graphs, and at least one memory profiler (Valgrind/Massif or similar).
- Take your order book engine (or another project) and do a proper optimization pass: profile, find the bottleneck, fix it, re-measure. Do this 2-3 times on different bottlenecks (allocation, cache misses, branch mispredictions).
- Write up the before/after numbers. This kind of "I measured X, changed Y, got Z speedup" story is exactly what interviewers want to hear.

**Milestone:** A documented optimization case study you can walk through in an interview.

---

## Month 10: Resume, Projects Polish & Networking

**Focus:** Package everything and start engaging with the industry.

- Clean up GitHub: order book engine, lock-free queue, any other projects — good READMEs with benchmarks, not just code dumps.
- Rewrite resume to foreground: the order book project, any performance-sensitive work you picked up at your day job, C++ + systems fundamentals.
- Start networking: follow quant/HFT engineers on Twitter/LinkedIn, join relevant Discord/Slack communities, look at Glassdoor/Blind for interview experience threads at target firms.
- Start identifying specific companies (see the stepping-stone list from earlier: market data vendors, exchange connectivity teams, mid-tier prop shops, fintech infra roles).

**Milestone:** Resume ready, 5-10 target companies identified.

---

## Month 11-12: Interview Prep & Applications

**Focus:** Convert preparation into interviews.

- Interview-specific prep: C++ language trivia (rule of five, virtual dispatch cost, object layout, `const` correctness), systems design questions scoped to low-latency systems, and continued LeetCode (timed, mixed difficulty).
- Do mock interviews if possible (peers, Pramp, or similar).
- Start applying — don't wait until you feel "fully ready." Apply to stepping-stone roles (systems/infra, market data, exchange connectivity) in parallel with continued study.
- Keep the order book project alive — add features (cancel/replace, different order types) as a way to keep learning while interviewing.

**Milestone:** Actively interviewing, with a live project and clear "why I'm pivoting" narrative.

---

## Ongoing habits (all 12 months)

- **Weekly:** at least 2-3 LeetCode problems in C++, timed.
- **Bi-weekly:** read one C++ or systems blog post/paper (e.g., from engineering blogs of firms like Jane Street, or C++ conference talks — CppCon talks on YouTube are excellent and free).
- **At work:** keep grabbing the performance-sensitive tickets, concurrency bugs, and infra-adjacent work discussed earlier — it's slower-moving but free real-world credibility.

---

## If you want to accelerate

If you can free up more time, the fastest compression is: front-load Months 1-4 (C++ fundamentals + DS&A) in 6-8 weeks instead of 4 months, then spend the saved time on a second, more ambitious project (e.g., a simple market data feed handler with a custom binary protocol parser) rather than rushing the low-latency/systems material — that material rewards depth over speed.
