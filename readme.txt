B+Tree at a Glance
Andrew Pham anp6338@g.harvard.edu

This is not the full documentation, this is an abstract of form and function.
 
 Form:
- This program is an implementation of a B+ Tree.
- The tree has a fanout of exactly 3 at all times.
Function:
- An insert on a full leaf node will use the in order pointers to shift numbers over.
- A data shift will activate a rebuild of the tree's internal nodes.
- Get will find a key and return its corresponding value
- Put will insert a new key & value into the corrent location if not found
- Put will update a value if its key is found
- TODO range

Node members:
a: left key
b: right key
P1 points to less than a, P2 points to between a and b, P3 points to larger than b
isleaf: this boolean tells us if the node is a data leaf at the bottom
vala: value a corresponds to key a in a leaf node, null in internal nodes
valb: value b corresponds to key b in a leaf node, null in internal nodes

Tree diagram:

                             | P1 | a | P2 | b | P3 |
                               |         |        |
              ------------------         |        -------------------
              |                          |                          |
              v                          v                          v
[ P1 | a | P2 | b | P3 ] --> [ P1 | a | P2 | b | P3 ] --> [ P1 | a | P2 | b | P3 ]

The following tests have been created:
1. Very little data: one piece of data is inserted
2. Insert left: a value is inserted taking spot a in a leaf
3. Insert right: a value is inserted taking spot b in a leaf
4. Full: A value is inserted to a full tree.
5. End Full: A value is inserted at the very end of a full data set.
6. Update
7. Range
8. 5 million: Insert the minimum 5 million data points

TODO: running the tests
