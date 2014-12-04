parallel_final_fall14
=====================

Parallel Computing Final Project (Fall 2014)

MAKE commands:
make       - compiles all file
make hash  - compiles just src/hashtable.cpp
make clean - removes unneeded files ( *.o,  *~, executables)
make edit  - opens all files in gedit (sorry if u don't like gedit)
make dox   - generates doxygen files, puts them in doc/ directory (latex and html)
make zip   - runs a tar -czf on the directory, drops the zipped file as prog4.tgz on directory up
make read  - opens the doxygen html file in chrome

Project Proposal:

We will be designing a protocol for paralellizing hash table operations. That includes look-ups, insertions, and deletions.

We will have two solutions. One in a shared memory architecture where each worker thread has access to a shared hash table. And another on a distributed architecture that divides the hash table into segments, making each node responsible for operations of their own segment of the table. As we work on the design and implementation, we will have more details. But we believe this project to be within the scope of the course, and extremely useful.

‌     ∧,_,∧
    (`･ω･´)　 ｎ_
  η  ＞　 ⌒＼/､_∃
 (∃)/   ∧　　＼_/
 ＼_/　＼　　丶
　  　　／ y　 ﾉ
　  　／ ／　／
    　(　(　〈
　   　＼ ＼  ＼
　   　
=====================
