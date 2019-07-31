B+Tree
Andrew Pham anp6338@g.harvard.edu

Instructions:
1. "make"
2. "./main -1" (replace 1 with the number of the test number you want to run)
   -"./main -f workload.txt" (replace workload.txt with location of custom workload)

 Form:
- This program is an implementation of a B+ Tree.
- The tree has a fanout 3.
Function:
- Get will find a key and return its corresponding value
- Put will insert a new key & value into the correct location if not found
- Put will update a value if its key is found
- TODO range
- An insert on a full leaf node will use the in order pointers to shift numbers over.
- A data shift will activate a rebuild of the tree's internal nodes.
- The tree will automatically rebuild with the minimum required depth to hold the data.

Node members:
a: left key
b: right key
P1 points to less than a, P2 points to between a and b, P3 points to larger than b
isleaf: this boolean tells us if the node is a data leaf at the bottom
vala: value a corresponds to key a in a leaf node, null in internal nodes
valb: value b corresponds to key b in a leaf node, null in internal nodes

Tree diagram:

                                 [ P1 | a | P2 | b | P3 ]
                                   |         |        |
                  ------------------         |        -------------------
                  |                          |                          |
                  v                          v                          v
0 <- [ P1 | a | P2 | b | P3 ] <-> [ P1 | a | P2 | b | P3 ] <-> [ P1 | a | P2 | b | P3 ] -> 0

Tests:
Tests will print out detailed descriptions, inputs, and outputs before execution. Briefly the tests are listed here...
1. One (passing): one insert
2. Very Small (passing): few inserts, not enough to need more nodes
3. Average (passing): insert enough to need a deeper tree, tree not full
4. Update (passing): update a value
5. Larger Tree (passing): insert enough to require a deeper tree, twice
6. Range: TODO
7. Negatives (failing): insert negative numbers
8. Back to back (passing): insert sequential numbers
9. Edges (passing): insert to the front and back of data set
10. Large (failing): insert a very large dataset

Notes:
This program may overflow the default maximum stack size in your system. Use command "ulimit -s unlimited" to bypass.
