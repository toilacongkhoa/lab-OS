# Operating Systems Laboratory

A comprehensive collection of simulations and implementations for core Operating Systems concepts, focusing on CPU Scheduling, Process Synchronization, Deadlock management, and Memory Management.

## Project Information
- **Author:** Nguyễn Công Khoa
- **Technology:** C Programming Language
- **Purpose:** Educational simulation of Operating System algorithms

---

## 🚀 Key Modules

### 1. CPU Scheduling Simulations
Located in: `Labfor-CPU-Scheduling/`
This module simulates various CPU scheduling algorithms to calculate turnaround time, waiting time, and CPU utilization.

- **FCFS (First-Come, First-Served):** Simplest scheduling algorithm.
- **SJF (Shortest Job First):**
  - **Non-preemptive:** Executes the process with the smallest burst time.
  - **Preemptive (SRTF):** Preempts the current process if a new one arrives with a shorter remaining time.
- **Round Robin (RR):** Time-sharing algorithm using a fixed time quantum.
- **Priority Scheduling:**
  - **Non-preemptive:** Based on static priorities.
  - **Preemptive:** High-priority processes can preempt the CPU.
- **Priority with Round Robin:** A hybrid approach using priority for different levels and RR within the same priority.
- **Multilevel Feedback Queue:** Dynamically adjusts process priorities based on their CPU burst behavior.

### 2. Process Synchronization
Located in: `Lab-for-Synchronization/`
Simulations of classic synchronization problems using synchronization primitives (Semaphores, Mutex locks, etc.).

- **Bounded-Buffer Problem (Producer – Consumer):** Handling shared buffer access between producers and consumers.
- **Readers and Writers Problem:** Managing concurrent access to shared data with priority constraints.
- **Dining-Philosophers Problem:** A classic illustration of deadlock and starvation avoidance in concurrent programming.

### 3. Deadlock Management
Located in: `Lab-for-Deadlock/`
Algorithms for managing system resources and avoiding circular waits.

- **Banker's Algorithm:** Used for deadlock avoidance by simulating resource allocation safety.
- **Deadlock Detection Algorithm:** Identifies the presence of a deadlock cycle in the system.

### 4. Memory Management
Located in: `Lab-for-Memory-Management/`
Simulate Memory Management technologies in operating systems.

#### Contiguous Memory Allocation in Main Memory
Algorithms for allocating contiguous blocks of memory to processes:

- **First Fit:** Allocates the first memory hole that is large enough for the process.
- **Best Fit:** Allocates the smallest hole that is large enough, minimizing leftover space.
- **Worst Fit:** Allocates the largest available hole, leaving the most leftover space.

#### Page Replacement in Virtual Memory
Algorithms for managing page frames when a page fault occurs:

- **FIFO (First-In, First-Out):** Replaces the oldest page in memory.
- **Optimal:** Replaces the page that will not be used for the longest time in the future.
- **LRU (Least Recently Used):** Replaces the page that has not been used for the longest period.
- **Counting LFU (Least Frequently Used):** Replaces the page with the lowest usage count.
- **Counting MFU (Most Frequently Used):** Replaces the page with the highest usage count.

---

## 🛠️ How to Compile and Run

Ensure you have a C compiler (like `gcc`) installed on your system.

### Compilation
Navigate to the specific directory and compile the desired file:
```bash
gcc filename.c -o outputname
```

### Execution
Run the compiled binary:
```bash
# On Windows
./outputname.exe

# On Linux/Unix
./outputname
```

## 📝 License
This project is for educational purposes. All rights reserved by the author.
