# The-SmartRoute-Logistics-Optimizer


> **C++17 path-planning engine that packs and routes city-wide deliveries for up to three vans on a 25 × 25 grid—balancing capacity and distance with an A*-driven solver.**



## Table of Contents
1. [Key Features](#key-features)  
2. [Tech Stack](#tech-stack)  
3. [Quick Start](#quick-start)  


---

## Key Features
- **A* shortest-path search** on a configurable grid (default 25×25)  
- **Load-balancing heuristic**—packages assigned by combined weight / volume to minimise van diversions  
- **Real-world constraints**: 1 200 kg / 50 m³ van limits; 0.5 / 1 / 5 m³ box sizes  
- **Graceful overflow**: shipments that exceed capacity are auto-queued for “Ship Tomorrow”  
- **100 % unit + integration test coverage** with _Microsoft VS Test_ (CI-gated via a Git pre-push hook)  
- **Cross-platform build** with CMake ≥ 3.20 (Windows / Linux / macOS)  
- **Clean console UI** that colour-codes van assignments and prints live route costs

---

## Tech Stack
| Layer          | Technology                    | Notes                              |
| -------------- | ----------------------------- | ---------------------------------- |
| Language       | **C++17**                     | STL algorithms, `<chrono>`, `<filesystem>` |
| Build System   | **CMake 3.20+**               | Single‐command out-of-source build |
| Tests          | **VS Test** _(Windows)_ / **Catch2** _(x-plat)_ | Integrated in CI |
| CI / VCS       | **GitHub Actions**, Git hooks | Lint, build, and test per push     |
| PM / Agile     | Jira Kanban                   | Sprint backlog & traceability      |

---

## Quick Start

```bash
# clone & build
git clone https://github.com/YourUser/smart-route.git
cd smart-route
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# run demo with sample dataset
./build/bin/smart_route --input samples/shipments.csv
