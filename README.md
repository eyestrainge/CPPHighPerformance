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



## Intel MPI (Message Passing Interface)
- Multithreading typically confined to a single device/process
- Run many processes on many machines/devices
    - Several machines on a network, each having >1 GPU or CPU
- Some sort of IPC required
- All forms of parallelism can be used at the same time
- Library + process manager solution
    - Requires some structure undarstanding


### Hello, MPI!
- Communicator: group of processes. Default: MPI_COMM_WORLD all the processes.
- Rank: the processes' position within the communicator
- API commands prefixed by MPI_
- MPI ises its own typedefs

#### Config
- Additional Include Directories: MPI's include
- Additional Library Directories: MPI's lib
- Linker Input Additional Dependencies: impi.lib;impicxx.lib
- Configuration Properties, Debugging, Command to mpiexec.exe
    - Command Arguments "$(TargetPath)" -n 4
- dll files still needs to be linked for runtime
- Boost MPI needs to be built explicitly


### Point-to-Point Communication Modes
- Standard: Returns as soon as message is sent
- Synchronous: Returns only when the exchange is complete (handshake)
- Buffered: Returns immediately, buffers the message
- Ready: Initiated only when receiving is set up. Finishes immediately


### Collective Communication
- Broadcast: Used both for sending and receiving determined by root parameter
- Barrier: Ensures every process reaches this point


### Summary
- Use Boost if you can :)



## C++ AMP
- GPU computing: Highly parallel architecture not just for games/graphics!
- Different programming model
    - SIMT (Single Instruction, Multiple Threads)
- CUDA (NVIDIA), OpenCL (Cross platform) as examples
- C++ AMP as Accelerated Massive Parallelism
    - A Compiler_Library solution for heterogeneous computing
        - Leverage data-parallel hardware (GPU)
    - Uses mostly ordinary C++ with some language enhancements


### Hello C++ AMP
- Debugger Type: GPU Only (C++ AMP)


### Tiling
- GPU Memory
    - Global memory
        - Slow access, available for all threads
    - Thread-local memory
        - Fast but per-thread
    - Shared (tile-static) memory
        - Shared between a group of threads
        - Fast, but limited in scope
        - Beneficial when several reads/writes are taking place
- What is Tiling?
    - Splits the overall extent into several tiles
    - Take a 4x4 extent and split it into four 2x2 tiles
    - Now each element has: A global index, a local index



## SUMMARY
- Start with a non-parallel solution and work up
- SIMD --> Threads --> Processes
- Custom hardware (GPU, FPGA, etc.) does parallelism better at the cost of development time
