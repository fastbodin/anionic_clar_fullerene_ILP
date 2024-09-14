This repo contains code to solve the $p$-anionic Clar number on fullerenes.

# Definitions

A *fullerene* is a 3-regular graph such that every face is a pentagon or a hexagon.
For a fixed integer $p$, a *$p$-anionic resonance structure* 
$(\mathcal{F}, \mathcal{M})$ of a fullerene $F_n$ is a set of independent 
faces $\mathcal{F} \subseteq F(F_n)$ (containing exactly $p$ pentagons) and a perfect 
matching $\mathcal{M} \subseteq E(F_n)$ on the graph obtained from $F_n$ by removing the 
vertices of the faces in $\mathcal{F}$. The *$p$-anionic Clar number* $C_p(F_n)$ 
of a fullerene $F_n$ is defined to be zero if $F_n$ has no $p$-anionic resonance 
structures and, otherwise, to be equal to the maximum value of 
$\lvert \mathcal{F} \rvert $ over all $p$-anionic resonance structures 
$(\mathcal{F}, \mathcal{M})$ of $F_n$. A $p$-anionic resonance structure that has 
$C_p(F_n)$ faces in $\mathcal{F}$ is called a *$p$-anionic Clar structure* on 
$F_n$. 

# Solving via ILP

This code uses the following ILP (Integer Linear Program) to solve the $p$-anionic Clar 
number of a fullerene $F_n$.

Let $H(F_n)$ and $P(F_n)$ denote the set of hexagonal and pentagonal faces of $F_n$ and, 
for each $i \in V(F_n)$, let $HP(i)$ denote the set of faces containing the vertex $i$.  
Given a perfect matching $\mathcal{M}$ that achieves $C_p(F_n)$ independent resonant 
faces, recall that $p$ faces are pentagons. For each face $f\in H(F_n)\cup P(F_n)$, 
let $y_f=1$ if $f$ is a resonant face and 0 otherwise. For each unordered edge 
$(i,j) \in E(F_n)$, let $x_{i,j}=1$ if $(i,j) \in \mathcal{M}$ but $(i,j)$ is not in a 
resonant face and 0 otherwise. The $p$-anionic Clar number of $F_n$ is the cost of an 
optimal solution to the following ILP:

**Maximize**: $p +\sum_{f \in H(G)} y_{f}$

**Subject to:** 
1. $\sum_{j \in N(i)} x_{i,j} + \sum_{f \in HP(i)} y_{f} = 1$, for each vertex $i \in V(G)$,
2. $\sum_{f \in P(G)} y_f = p$, and
3. $x_{i,j}, y_f \in \{0,1\}$, for each $(i,j)\in E(G)$ and $f \in H(G)\cup P(G)$.  

# Code

**Required**: 

1. A CPP+14 compiler.

2. This implementation requires a local copy of a Gurobi solver (https://www.gurobi.com/).
*Update* the Makefile to point to your copy of gurobi, I included an example
that I used on my Macbook when running Gurobi 11.

3. A file containing fullerenes and their adjacency lists in a *particular* format.
For each isomer in the file, please use the following format such that 
**there exists a planar embedding of the vertices where each neighbor
is listed in clockwise order.** See bottom of Readme for an example of such a 
file for all fullerenes on 30 vertices.

*{number of vertices in graph 0 (call it n_0)}

{degree of vertex 0} {neighbor 0} {neighbor 1} {neighbor 2}

{degree of vertex 1} {neighbor 0} {neighbor 1} {neighbor 2}

...

{degree of vertex n_0-1} {neighbor 0} {neighbor 1} {neighbor 2}


{number of vertices in graph 1 (call it n_1)}

{degree of vertex 0} {neighbor 0} {neighbor 1} {neighbor 2}

{degree of vertex 1} {neighbor 0} {neighbor 1} {neighbor 2}

...

{degree of vertex n_1-1} {neighbor 0} {neighbor 1} {neighbor 2}


...


{number of vertices in last graph in file (call it n_f)}

{degree of vertex 0} {neighbor 0} {neighbor 1} {neighbor 2}

{degree of vertex 1} {neighbor 0} {neighbor 1} {neighbor 2}

...

{degree of vertex n_f-1} {neighbor 0} {neighbor 1} {neighbor 2}$


**To run**:
./comp_p_anionic_clar_num {value of p to solve for} < {file of fullerenes}

**Output**: 
Given a file of your input fullerenes, files will be written to *output/*. There
will be four files where row $i$ corresponds with graph $i$ in your input file.

n_p_anionic_clar_num <- $p$-anionic Clar number of fullerenes on n vertices
n_p_r_pent <- File of resonant pentagons of fullerenes on n vertices. Format per row: 
{# of res. pent} {face ids of res. pent.}
n_p_r_hex <- File of resonant hexagon of fullerenes on n vertices. Format per row: 
{# of res. hex} {face ids of res. hex.}
n_p_match_e <- File of matching edges of fullerenes on n vertices. Format per row: 
{2*(# of matching edges)} {endpoint 0 and endpoint 1 of each matching edge}

See *output/* for an example output for the 2-anionic Clar number of
all fullerenes on 30 vertices.

# Example adj. list format for all fullerenes on 30 vertices, sorted by minimum face spiral

30

 3  4  7  1

 3  0  9  2

 3  1 11  3

 3  2 13  4

 3  3  5  0

 3  4 14  6

 3  5 17  7

 3  6  8  0

 3  7 19  9

 3  8 10  1

 3  9 21 11

 3 10 12  2

 3 11 23 13

 3 12 14  3

 3 13 15  5

 3 14 24 16

 3 15 26 17

 3 16 18  6

 3 17 27 19

 3 18 20  8

 3 19 28 21

 3 20 22 10

 3 21 29 23

 3 22 24 12

 3 23 25 15

 3 24 29 26

 3 25 27 16

 3 26 28 18

 3 27 29 20

 3 28 25 22


30

 3  4  7  1

 3  0  9  2

 3  1 11  3

 3  2 13  4

 3  3  5  0

 3  4 15  6

 3  5 17  7

 3  6  8  0

 3  7 19  9

 3  8 10  1

 3  9 21 11

 3 10 12  2

 3 11 23 13

 3 12 14  3

 3 13 24 15

 3 14 16  5

 3 15 26 17

 3 16 18  6

 3 17 27 19

 3 18 20  8

 3 19 28 21

 3 20 22 10

 3 21 29 23

 3 22 24 12

 3 23 25 14

 3 24 29 26

 3 25 27 16

 3 26 28 18

 3 27 29 20

 3 28 25 22



30
 
 3  4  7  1

 3  0  9  2

 3  1 11  3

 3  2 14  4

 3  3  5  0

 3  4 16  6

 3  5 18  7

 3  6  8  0

 3  7 20  9

 3  8 10  1

 3  9 22 11

 3 10 12  2

 3 11 23 13

 3 12 25 14

 3 13 15  3

 3 14 26 16

 3 15 17  5

 3 16 27 18

 3 17 19  6

 3 18 28 20

 3 19 21  8

 3 20 29 22

 3 21 23 10

 3 22 24 12

 3 23 29 25

 3 24 26 13

 3 25 27 15

 3 26 28 17 

 3 27 29 19 

 3 28 24 21 
