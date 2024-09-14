### Definitions

A **fullerene** $F_n$ is a 3-regular graph such that every face is a pentagon or a hexagon.
Let $F(F_n)$ and $E(F_n)$ denote the set of faces and edges in a fullerene $F_n$, respectively.
For a fixed integer p, a **p-anionic resonance structure**
$(\mathcal{F}, \mathcal{M})$ of a fullerene $F_n$ is a set of independent 
faces $\mathcal{F} \subseteq F(F_n)$ (containing exactly p pentagons) and a perfect 
matching $\mathcal{M} \subseteq E(F_n)$ on the graph obtained from $F_n$ by removing the 
vertices of the faces in $\mathcal{F}$. The **p-anionic Clar number** $C_p(F_n)$ 
of a fullerene $F_n$ is defined to be zero if $F_n$ has no p-anionic resonance 
structures and, otherwise, to be equal to the maximum value of 
$|\mathcal{F}|$ over all p-anionic resonance structures 
$(\mathcal{F}, \mathcal{M})$ of $F_n$. A p-anionic resonance structure that has 
$C_p(F_n)$ faces in $\mathcal{F}$ is called a **p-anionic Clar structure** on $F_n$. 

### Solving via ILP

This code uses the following ILP (Integer Linear Program) to solve the p-anionic Clar 
number of a fullerene $F_n$.

Let $H(F_n)$ and $P(F_n)$ denote the set of hexagonal and pentagonal faces of $F_n$ and, 
for each $i \in V(F_n)$, let $HP(i)$ denote the set of faces containing the vertex $i$. 
For each face $f\in H(F_n)\cup P(F_n)$, let $y_f=1$ if $f$ is a resonant face and 0 
otherwise. For each unordered edge $(i,j) \in E(F_n)$, let $x_{i,j}=1$ if 
$(i,j)$ is a matching edge and 0 otherwise. The 
p-anionic Clar number of $F_n$ is the cost of an optimal solution to the following ILP:

**Maximize**: $\sum_{f \in F(F_n)} y_{f}$

**Subject to:** 
1. $\sum_{j \in N(i)} x_{i,j} + \sum_{f \in HP(i)} y_{f} = 1$, for each vertex $i \in V(F_n)$,
2. $\sum_{f \in P(F_n)} y_f = p$, and
3. $x_{i,j}, y_f \in \{0,1\}$, for each $(i,j)\in E(F_n)$ and $f \in H(F_n)\cup P(F_n)$.  

## Code

### Requirements:

1. A CPP+14 compiler.

2. This implementation requires a local copy of a Gurobi solver (https://www.gurobi.com/).

3. A file containing fullerenes and their adjacency lists in a *particular* format.
For each isomer in the file, please use the following format such that 
**there exists a planar embedding of the vertices where each neighbor
is listed in clockwise order.** See bottom output/030_adj for an example
for all fullerenes on 30 vertices. Note that Buckygen 
(https://github.com/evanberkowitz/buckygen) can be used to generate fullerenes in this
format. **Note**, vertices should be labelled starting at 0!

```
{number of vertices in graph (call it n)}
{degree of vertex 0} {neighbor 0} {neighbor 1} {neighbor 2}
{degree of vertex 1} {neighbor 0} {neighbor 1} {neighbor 2}
...
{degree of vertex n-1} {neighbor 0} {neighbor 1} {neighbor 2}
```

### Compile:

**Update** the Makefile to point to your copy of Gurobi. I included an example
that I used on my Macbook when running Gurobi 11.

There are a couple compiling flags you can change in `include.h`. 

```
// Input the maximum size of a fullerene you plan to read in
constexpr int NMAX = 120;
// For debugging purposes
#define DEBUG 0
#define DEBUG_DUAL 0
#define DEBUG_CLAR 0
#define DEBUG_GUROBI 0

```

This code will work up to fullerenes on 998 vertices, but the default is set 
to 120. Change `NMAX` to the maximum number of vertices you want to use.
The other flags can be changed from 0 to 1 depending on what you want to debug (
you should not need to).

### To run:
./comp_p_anionic_clar_num {value of p to solve for} < {file of fullerenes}

### Output:
Given a file of your input fullerenes, files will be written to *output/*. 

n_p_anionic_clar_num <- p-anionic Clar number of fullerenes on n vertices

n_p_r_pent <- File of resonant pentagons of fullerenes on n vertices. Format per row: 
{# of res. pent} {face ids of res. pent.}

n_p_r_hex <- File of resonant hexagon of fullerenes on n vertices. Format per row: 
{# of res. hex} {face ids of res. hex.}

n_p_match_e <- File of matching edges of fullerenes on n vertices. Format per row: 
{2*(# of matching edges)} {endpoint 0 and endpoint 1 of each matching edge}

See *output/* for an example output for the 2-anionic Clar number of
all fullerenes on 30 vertices.

