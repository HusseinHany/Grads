# Grads

Working versions of a console student grade management system in C++, written for a Design and Analysis of Algorithms course.

**The finished version lives in [Student-Managment-System](https://github.com/HusseinHany/Student-Managment-System).** This repository keeps the earlier drafts.

## Files

| File | What it is |
| --- | --- |
| `StudentGradeSystem_final.cpp` | A complete, compilable version (962 lines) |
| `New Text Document.txt` | An earlier draft of the same program |
| `The Finaaaaaaaaaaaaaaaaaal` | The later version, almost identical to the one in Student-Managment-System |
| `FINAL` | Empty placeholder |

## What the program does

- **Records:** add, delete, update and list students (ID, name, GPA, age, major)
- **Sorting:** bubble sort, insertion sort, selection sort, merge sort and quick sort, each on a different field
- **Searching:** linear search by name (full or partial) and binary search by ID
- **Analytics:** top-K ranking, statistics and distribution, filter by major, duplicate GPA detection
- **AVL tree:** insert, search and in-order traversal
- **Benchmark:** times the sorting algorithms on random, nearly sorted and reverse-sorted data, counting comparisons and swaps

## Build and run

```bash
g++ -std=c++11 StudentGradeSystem_final.cpp -o grades
./grades
```
