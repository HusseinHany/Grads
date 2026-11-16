# Grads

Working versions of a console student grade management system in C++, written for a Design and Analysis of Algorithms course.

**The finished version lives in [Student-Management-System](https://github.com/HusseinHany/Student-Management-System).** This repository keeps the earlier drafts.

These drafts are kept as they were written. Fixes made later (clean exit when input ends, the fairer benchmark, the aligned statistics box) are only in the finished version.

## Files

| File | What it is |
| --- | --- |
| `StudentGradeSystem_draft1.cpp` | The first draft |
| `StudentGradeSystem_final.cpp` | A complete, compilable version (962 lines) |
| `StudentGradeSystem_v3.cpp` | The later version, almost identical to the one in Student-Management-System |

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
