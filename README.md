# Philosophers

## Overview

This project is a multithreaded simulation of the classic Dining Philosophers problem, designed to illustrate the concepts of threading and mutex-based synchronization in C. It demonstrates how multiple threads can safely access and manage shared resources without causing deadlocks or data inconsistencies.

Ideal for those learning about concurrency, resource sharing, and thread synchronization in systems programming.

---

## Features

🧵 Multithreading: Each philosopher is implemented as a separate thread   
🔒 Mutex Locks: Ensures safe access to shared forks (resources)   
🌀 Deadlock Prevention: Strategies to avoid circular wait conditions   
🧠 Educational Value: Helps understand concurrency control in real-time systems   
📉 Performance Optimization: Efficient use of thread management and sleep functions   

---

## Tech Stack

| Category      | Technology        |
| ------------- | ----------------- |
| Language      | C                 |
| Libraries     | pthread.h         |
| Tools         | Make, GCC         |
| Platform      | Linux/Unix        |

---

## File Structure

```bash
Philosophers/
│
├── src/                    # Source files for simulation logic
├── includes/               # Header files
├── Makefile                # For building the project
└── README.md               # Project documentation
```
---

## How It Works

Each philosopher alternates between thinking and eating. To eat, a philosopher must acquire two forks (shared resources).  
Mutexes are used to ensure that no two philosophers pick up the same fork simultaneously, preventing race conditions and deadlocks.  

---

## Lessons Learned 

This project helped me:

•	Gain hands-on experience with `POSIX` threads and `mutex`es  
•	Understand `synchronization`, critical sections, and `deadlock` scenarios   
•	Develop debugging strategies for concurrent systems   
•	Practice building efficient and safe multithreaded applications in `C`   

---

## Try It Out

### Pre-requisites

•	Linux-based OS  
•	GCC compiler  
•	make utility  

### Build and Run

```bash
# Clone the repo
git clone https://github.com/svvoii/Philosophers.git

# Go into the project directory
cd Philosophers
cd philo

# Build the project
make

# Run the program
./philo 5 800 200 200 7

# Arguments:
# 1. Number of philosophers (5)
# 2. Time to die (800 ms)
# 3. Time to eat (200 ms)
# 4. Time to sleep (200 ms)
# 5. Number of times each philosopher must eat (7) "Optional"
```

### Example Output

```bash
Philosopher 1 is thinking...
Philosopher 1 picked up left fork.
Philosopher 1 picked up right fork.
Philosopher 1 is eating...
...
...
```

### Notes

•	The number of philosophers and other simulation parameters may be configurable in the code.   
•	Forks are represented as mutexes to prevent simultaneous use.   
•	Different strategies (e.g. odd/even picking order) can be implemented to reduce the chance of deadlock.   
   
---

## Author

[My GitHub](https://github.com/svvoii)  
[My LinkedIn](https://www.linkedin.com/in/bocancia/)  
[My Portfolio](https://sbocanci.me/)  

---
