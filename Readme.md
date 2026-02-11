# False Sharing Demonstration in C (Using pthreads)

## 📌 What is this project?

This project demonstrates **false sharing**, a performance problem in multi‑threaded programs, using **C and POSIX threads (pthreads)**.

You have written **two programs**:

- `falseSharing.c` → demonstrates **false sharing**
- `falseSharingSolution.c` → demonstrates the **solution using padding**

Both programs are *logically correct* and produce the same result, but their **execution time differs significantly**.

---

## 🧠 What is False Sharing?

**False sharing** occurs when:
- Two or more threads
- Modify **different variables**
- That happen to reside on the **same CPU cache line**

Even though the variables are independent, the CPU cache coherence protocol treats them as *shared*, causing **unnecessary cache invalidations** and **performance degradation**.

👉 The variables are *not logically shared*, but they are *physically close in memory*.

---

## 🏗 How CPUs Access Memory (Quick Background)

Modern CPUs do **not** access RAM directly every time.

Instead, they use **cache memory**, divided into **cache lines**.

- Typical cache line size: **64 bytes**
- When one variable is accessed, the **entire cache line** is loaded

---

## 🧩 Memory Layout in `falseSharing.c`

### Structure used:
```c
typedef struct {
    long a;
    long b;
} Counter;
```

### Memory layout (typical):

```
Cache Line (64 bytes)
┌──────────────────────────────────────────────┐
│  a (8 bytes)  │  b (8 bytes)  │ unused ...   │
└──────────────────────────────────────────────┘
```

### What happens?

- **Thread 1** increments `a`
- **Thread 2** increments `b`
- Both variables are on the **same cache line**
- Each write causes the cache line to bounce between CPU cores

📉 Result: **Huge performance penalty**

---

## 🧨 Cache Coherence Effect (False Sharing)

```
Thread 1 (Core 0)        Thread 2 (Core 1)
----------------        ----------------
writes a                writes b
  ↓                        ↓
invalidates cache      invalidates cache
  ↓                        ↓
reload cache line      reload cache line
```

This repeats **500 million times**.

---

## 🚀 Memory Layout in `falseSharingSolution.c`

### Structure used:
```c
#define CACHE_LINE 64

typedef struct {
    long a;
    char padding[CACHE_LINE - sizeof(long)];
    long b;
} Counter;
```

### Memory layout:

```
Cache Line 0 (64 bytes)
┌───────────────────────────────┐
│ a (8 bytes) │ padding (56 B)  │
└───────────────────────────────┘

Cache Line 1 (64 bytes)
┌───────────────────────────────┐
│ b (8 bytes) │ unused ...      │
└───────────────────────────────┘
```

### What changes?

- `a` and `b` are now on **different cache lines**
- Each thread modifies its **own cache line**
- No cache invalidation between threads

📈 Result: **Much faster execution**

---

## ⏱ Observed Output (Your System)

### False Sharing version:
```
Time taken : ~3.7 s
```

### Padded (solution) version:
```
Time taken : ~1.1 s
```

✔ Same logic  
✔ Same result  
❌ Very different performance

---

## 🧪 Why the Logic is Still Correct

- Each variable is accessed by **only one thread**
- No data race
- No synchronization needed
- The issue is **purely architectural**

False sharing is a **hardware-level performance bug**, not a logical bug.

---

## 🛠 How to Compile and Run

### Compile:
```bash
gcc falseSharing.c -o a.out 
gcc falseSharingSolution.c -o b.out 
```

### Run:
```bash
./a.out
./b.out
```

Run multiple times to observe consistent timing differences.

---

## 🎓 Key Takeaways

- False sharing can **destroy performance** in multithreaded programs
- It happens due to **cache line granularity**
- Padding or alignment fixes the issue
- This concept is critical in:
  - OS kernels
  - High‑performance computing
  - Game engines
  - Lock‑free data structures

---
