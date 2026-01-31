# Installation Guide

## Prerequisites

- C++ compiler with C++17 support (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher
- Make (on Unix/Linux) or Visual Studio (on Windows)

## Building the Library

### Linux / macOS

```bash
# Navigate to the library directory
cd ct/code/sf

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the library
make

# Optional: Install system-wide (requires sudo)
sudo make install
```

### Windows (Visual Studio)

```cmd
:: Navigate to the library directory
cd ct\code\sf

:: Create build directory
mkdir build
cd build

:: Configure with CMake
cmake ..

:: Open in Visual Studio and build, or build from command line
cmake --build . --config Release
```

## Testing the Installation

After building, run the test suite:

```bash
cd build
./path_planning_test
```

You should see output showing tests for all algorithms.

## Integration into Your Project

### Method 1: CMake (Recommended)

Add to your project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the path planning library
add_subdirectory(path/to/sf)

# Your executable
add_executable(your_program main.cpp)
target_link_libraries(your_program path_planning)
```

### Method 2: Manual Compilation

```bash
# Compile all source files
g++ -std=c++17 -I/path/to/sf/include -c src/*.cpp

# Create static library
ar rcs libpath_planning.a *.o

# Compile your program
g++ -std=c++17 -I/path/to/sf/include your_program.cpp -L. -lpath_planning -o your_program
```

### Method 3: Using as Header-Only (Simplified)

For quick prototyping, you can include the main header:

```cpp
#include "sf/path_planning.h"

// All algorithms are available in the sf namespace
```

## Directory Structure

```
sf/
├── CMakeLists.txt          # Build configuration
├── README.md               # Overview and features
├── USAGE.md                # Usage examples and API reference
├── INSTALL.md              # This file
├── include/
│   └── sf/
│       ├── path_planning.h    # Main header (include this)
│       ├── graph.h            # Graph data structures
│       ├── uninformed_search.h
│       ├── informed_search.h
│       ├── dynamic_search.h
│       └── sampling_based.h
├── src/
│   ├── graph.cpp
│   ├── uninformed_search.cpp
│   ├── informed_search.cpp
│   ├── dynamic_search.cpp
│   └── sampling_based.cpp
├── tests/
│   └── test.cpp            # Test suite
└── examples/
    └── simple_example.cpp  # Usage examples
```

## Quick Start Example

Create a file `main.cpp`:

```cpp
#include "sf/path_planning.h"
#include <iostream>

int main() {
    // Create a simple graph
    sf::Graph graph;
    
    // Add nodes
    graph.addNode(sf::Node(0, 0.0, 0.0));
    graph.addNode(sf::Node(1, 1.0, 0.0));
    graph.addNode(sf::Node(2, 2.0, 0.0));
    
    // Add edges
    graph.addEdgeUndirected(0, 1, 1.0);
    graph.addEdgeUndirected(1, 2, 1.0);
    
    // Find path using A*
    auto result = sf::AStar::search(graph, 0, 2);
    
    if (result.found) {
        std::cout << "Path found!\n";
        std::cout << "Cost: " << result.cost << "\n";
    }
    
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -I/path/to/sf/include main.cpp -L/path/to/sf/build -lpath_planning -o main
./main
```

## Build Options

### CMake Options

```bash
# Build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Specify install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

### Compiler Flags

Add to your CMakeLists.txt before adding the library:

```cmake
# Enable all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Enable optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Enable sanitizers (for debugging)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
```

## Platform-Specific Notes

### Linux

- Requires g++ 7+ or clang++ 5+
- Tested on Ubuntu 18.04+, Fedora 28+, CentOS 8+

### macOS

- Requires Xcode Command Line Tools
- Tested on macOS 10.14+

### Windows

- Requires Visual Studio 2017 or newer
- Or MinGW-w64 with g++ 7+
- Tested on Windows 10/11

## Troubleshooting

### CMake not finding compiler

```bash
# Specify compiler explicitly
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
```

### Linker errors

- Ensure C++17 is enabled: `set(CMAKE_CXX_STANDARD 17)`
- Check include paths: `-I/path/to/sf/include`
- Check library path: `-L/path/to/sf/build`

### Missing headers

- Verify the include directory structure
- Use `#include "sf/path_planning.h"` not `#include "path_planning.h"`

## Contributing

To contribute to the library:

1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Ensure all tests pass
5. Submit a pull request

## License

MIT License - See LICENSE file for details.

## Support

For issues and questions:
- Check USAGE.md for detailed examples
- Review the test files in tests/
- Look at examples/simple_example.cpp
