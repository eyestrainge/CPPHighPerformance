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
    > Project properties, C/C++ Code Generation, Enable Enhanced Instruction Set
    > Enable C++ Exceptions: /EHa


### Compiler Vectorization and Libraries
- For demo purposes disable optimization:
    > C/C++ Optimization, Optimization Disabled (/Od)
	> Output Files, Assembler Output: Assembly With Source Code (/FAs)
Digging through generated asm file for result[i], movss instruction on xmm register
says scalar operation which is not effective at all.
    > Now set optimization to Maximum Speed and compare results
	> Code Generation, Basic Runtime Checks should be default for above to work
