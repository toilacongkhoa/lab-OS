# Operating Systems Laboratory

A comprehensive collection of simulations and implementations for core Operating Systems concepts, focusing on CPU Scheduling, Process Synchronization, and Deadlock management.

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
