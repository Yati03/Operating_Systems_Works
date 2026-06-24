## Contents

### `producteur.c` / `consommateur.c` — Shared memory IPC

A classic producer/consumer pair communicating through a System V shared-memory segment (`shmget`/`shmat`, key `0x1111`).

- **`producteur.c`** takes a number `n` on the command line (default 5), computes the first `n` **Catalan numbers** (`C(n) = (2n)! / ((n+1)! * n!)`), and writes each one into the shared segment one at a time (with a 3-second delay between writes to simulate work), then sets an `end` flag.
- **`consommateur.c`** attaches to the same segment, busy-waits on the `end` flag, then reads and prints all the Catalan numbers the producer wrote, and finally cleans up the shared memory (`shmctl(..., IPC_RMID, ...)`).

Run the producer first, then the consumer, in separate terminals.

### `cpr.c` — Recursive process chain with pipes 

Takes a process count `N` and recursively `fork()`s/`execvp()`s a chain of `N` child processes, each one connected to its parent by a pipe. The last process (`N == 1`) sleeps 5 seconds then exits; each parent blocks reading its pipe until its child terminates, then reports completion. The file includes a written explanation of zombie processes as required by the assignment.

### `devoir3.c` — TA / students synchronization problem

A multithreaded simulation (pthreads + POSIX semaphores + a mutex) modeling a teaching assistant (TA) with a limited number of office-hours chairs (`N_CHAISES = 4`). Student threads arrive, try to take one of the chairs, wait for the TA, get helped, and return to work; the TA sleeps until a student arrives, helps students one at a time, and the simulation stops after `N_RUNS = 12` total help sessions. Takes the number of student threads as an optional command-line argument (default 5).

### `D2_Q1.java` — Threaded prime finder

Prompts the user for a number `n`, then spawns a `PrimeNum` thread that prints every prime number from 0 to `n` (trial division up to `sqrt(n)`).

### `D2_Q2.java` — Threaded Fibonacci generator

Prompts the user for a number `n`, spawns a `Fibo` thread that computes the first `n` Fibonacci numbers into an array, joins on it, then prints the resulting sequence.

### `Library.py` — Page replacement simulator

Implements and compares two page-replacement algorithms over a randomly generated reference string:

- `fifo(ref, numb)` — First-In-First-Out replacement using a queue.
- `lru(ref, numb)` — Least-Recently-Used replacement using a usage-order list.

Both track and report the number of page faults. Run interactively — it prompts for the total number of page references and the number of frames (must be between 2 and 6), then prints a frame-by-frame trace and the page-fault counts for each algorithm.

## Building & Running

```bash
# C files
gcc producteur.c -o producteur && ./producteur 5
gcc consommateur.c -o consommateur && ./consommateur
gcc cpr.c -o cpr && ./cpr 3
gcc devoir3.c -o devoir3 -lpthread && ./devoir3 5

# Java files
javac D2_Q1.java && java D2_Q1
javac D2_Q2.java && java D2_Q2

# Python
python3 Library.py
```
