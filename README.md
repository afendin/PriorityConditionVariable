### Exercise 5.15

Implement a priority condition variable. A priority condition variable (PCV) has three public methods:

```c++
void PCV::wait(Lock *enclosingLock, int priority);
void PCV::signal(Lock *enclosingLock);
void PCV::broadcast(Lock *enclosingLock, int priority);
```

These methods are similar to those of a standard condition variable. The one difference is that a PCV enforces both priority and ordering. In particular, signal(Lock *lock) causes the currently waiting thread with the highest priority to return from wait; if multiple threads with the same priority are waiting, then the one that is waiting the longest should return before any that have been waiting a shorter amount of time. Similarly, broadcast(Lock *lock, int priority) causes all currently waiting threads whose priority equals or exceeds priority to return from wait.