# GPGPU-Sim UVMSmart - CUDA 11.8 Compatibility

This README describes the changes required to build and run GPGPU-Sim UVMSmart with CUDA 11.8 on modern Linux systems (tested with GCC 11.4.0).

## Prerequisites

- CUDA 11.8 installed (e.g., at `/apps/cuda-11.8`)
- GCC 11.x
- flex, bison
- Standard build tools (make, etc.)

## Quick Start

```bash
cd gpgpu-sim_UVMSmart
export CUDA_INSTALL_PATH=/apps/cuda-11.8
source setup_environment
make -j$(nproc)
cd benchmarks/Managed/bfs #run a benchmark (BFS for example)
./bfs ../../data/bfs/graph256k.txt
```

## Changes Made for CUDA 11.8 Compatibility

| File | Issue | Fix |
|------|-------|-----|
| `setup_environment`, `version_detection.mk` | GCC 11.x version not detected | Changed regex `[0-9]` to `[0-9]+` |
| `src/gpgpu-sim/shader.h` | Heap corruption causing segfault | Fixed `reinterpret_cast` on uninitialized member to `static_cast<shader_core_stats_pod*>(this)` |
| `linux-so-version.txt` | Missing CUDA 11.x symbols | Added `libcudart.so.10.x{}` and `libcudart.so.11.x{}` entries |
| `libcuda/cuobjdump.l` | Flex/M4 build failure | Removed problematic `\./` pattern in comment |
| `libcuda/cuobjdump.y` | Parse error on CUDA 11.8 cuobjdump output | Added grammar rule for missing "producer" field |
| `src/intersim2/globals.cpp` | Duplicate symbol linker error | Removed `cuobjdump_parse` stub |
| `src/cuda-sim/cuda-sim.cc` | PTX 7.x parameter alignment assertion | Relaxed assertion, log warning instead |
| `libcuda/cuda_runtime_api.cc` | Missing CUDA 11.x APIs | Added `cudaLaunchKernel`, `__cudaPushCallConfiguration`, `__cudaPopCallConfiguration` |

## Benchmark Configuration

For benchmarks compiled with sm_52 or higher, update `gpgpusim.config`:

```
# Change from:
-gpgpu_ptx_force_max_capability 20

# To:
-gpgpu_ptx_force_max_capability 52
```

## Verifying GPGPU-Sim is Being Used

After sourcing `setup_environment`, verify the benchmark uses GPGPU-Sim's library:

```bash
source setup_environment
ldd ./your_benchmark | grep libcudart
```

Should show:
```
libcudart.so.11.0 => /path/to/gpgpu-sim_UVMSmart/lib/gcc-11.4.0/cuda-11080/release/libcudart.so.11.0
```

**NOT:**
```
libcudart.so.11.0 => /apps/cuda-11.8/lib64/libcudart.so.11.0
```

## Troubleshooting

### "version `libcudart.so.11.0' not found"
- Ensure `linux-so-version.txt` has the CUDA 11.x entries
- Rebuild the library: `make clean && make`

### Segmentation fault at startup
- Check the shader.h fix was applied correctly
- Rebuild: `rm build/*/release/gpgpu-sim/shader.o && make`

### "syntax error near 'host ='" during cuobjdump parsing
- Apply the cuobjdump.y grammar fix
- Regenerate parser: `rm build/*/release/libcuda/cuobjdump_parser.* && make`

### M4 "end of file in string" error during build
- Apply the cuobjdump.l fix
- Regenerate lexer: `rm build/*/release/libcuda/cuobjdump_lexer.* && make`

### "No PTX sections found with sm capability..."
- Update `-gpgpu_ptx_force_max_capability` in gpgpusim.config to match your binary's SM version

## Files Modified

| File | Change Type |
|------|-------------|
| setup_environment | GCC version regex |
| version_detection.mk | GCC version regex |
| src/gpgpu-sim/shader.h | Heap corruption fix |
| linux-so-version.txt | CUDA 11.x symbols |
| libcuda/cuobjdump.l | M4 compatibility |
| libcuda/cuobjdump.y | CUDA 11.8 format |
| src/intersim2/globals.cpp | Duplicate symbol |
| src/cuda-sim/cuda-sim.cc | PTX alignment |
| libcuda/cuda_runtime_api.cc | CUDA 11.x API |
