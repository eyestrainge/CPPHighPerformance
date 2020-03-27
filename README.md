# CPPHighPerformance
High performance computing with C++



## SIMD
- Not all processor instructions are created equal
- SIMD related instructions are not portable even for x86 architectures
- Streaming SIMD Extensions (SSE)
    - 128-bit registers (xmm0...xmm7)
- Advanced Vector Extensions (AVX) supported by AMD/Intel
    - Extends xmm to ymm (bits 128...256) and zmm (bits 256...511)


### Inline Assembly
- Unsupported in 64-bit MSVC


### Intrinsics
- There is no SSE3 support for MSVC (Intel C++ compiler supports it)
    - Project properties, C/C++ Code Generation, Enable Enhanced Instruction Set
    - Enable C++ Exceptions: /EHa


### Compiler Vectorization and Libraries
- For demo purposes disable optimization:
    - C/C++ Optimization, Optimization Disabled (/Od)
	- Output Files, Assembler Output: Assembly With Source Code (/FAs)
Digging through generated asm file for result[i], movss instruction on xmm register
says scalar operation which is not effective at all.
    - Now set optimization to Maximum Speed and compare results
	- Code Generation, Basic Runtime Checks should be default for above to work


### Summary
- SIMD allows vectorized operations using large registers (128/256/512)
- Scale the large register to data size
- Check level of support using cpuid
- Three modes of interaction: Inline Assembly, Intrinsics, Compiler Vectorization



## OpenMP (Declarative Parallelism)
- A standard for decorating code for 'multiprocessing' (a compiler+library solution)
- Many times OpenMP is implicitly used by compilers for optimizing parallelism


### Hello Open MP
- Explicitly turn on OpenMP support through Project settings
    - C/C++, Language, Open MP Support


### Synchronization
- critical: block executed by one thread at a time
- atomic: nex memory update is atomic
- ordered: block executed in same order as if it were sequential
- barrier: all threads wait until each one has reached this point
- nowait: threads can proceed without waiting on other threads


### Data Sharing
- shared: item is accessible by all threads simulteniously
    - All variables except loop counter shared by default
- private: item is shared-local, not inited or available outside parallel region
- firstprivate: like private but inited to original value
- lastprivate: like private except original value updated on exit
- default: defines whether, by default, variables are shared or not (none)


### Summary
- Work sharing (for/do/sections)
- Control synchronization (critical, atomic, barrier)
- Data sharing
