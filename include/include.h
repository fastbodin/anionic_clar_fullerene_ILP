#include "gurobi_c++.h"
#include <fstream>

using namespace std;

// Input the maximum size of a fullerene you plan to read in
constexpr int NMAX = 120;
// For debugging purposes
#define DEBUG 0
#define DEBUG_DUAL 0
#define DEBUG_CLAR 0
#define DEBUG_GUROBI 0


//------ DO NOT CHANGE BELOW ------
//-------- HERE BE DRAGONS --------

// Fullerenes are 3-regular graphs, therefore a fullerene on n vertices has 3n/2 edges
constexpr int EMAX = 3 * NMAX / 2;
// By Euler's formula, the number of faces in a fullerene is 3n/2 - n + 2
constexpr int DMAX = NMAX / 2 + 2;
// number of out files
constexpr int NFILE = 4;

// information on each vertex
struct vertex {
    // vertices adjacent to it
    int adj_v[3];
    // faces it lies on
    int faces[3];
    // edges it is an endpoint of
    int edges[3];
    // number of edges assigned to the vertex currently
    int num_edges;
};

// information on each face
struct face {
    // length of face (will be 5 or 6)
    int size;
    // faces it shares an edge with
    int adj_f[6];
    // vertices on face, note they will be recorded in counter clockwise order
    int vertices[6];
};

// information on each edge
struct edge {
    // vertices that form its endpoints
    // vertices[0] < vertices[1]
    int vertices[2];
};

// information on each fullerene isomer
struct fullerene {
    // primal graph information
    vertex primal[NMAX];
    // planar dual graph information
    face dual[DMAX];
    // edge information
    edge edges[EMAX];
    // size of primal, size of dual, number of edges, and id
    int n, dual_n, num_edges, id;
};

// From read_and_print.cpp
bool read_fullerene(int *n, vertex (&primal)[NMAX]);
void print_primal(const int n, const vertex (&primal)[NMAX]);
void print_dual(const int dual_n, const face (&dual)[DMAX]);
void print_sol(const fullerene (&F), const int num_res_faces,
               const GRBVar fvars[DMAX], const GRBVar evars[EMAX]);
void save_sol(const fullerene (&F), const int p, const int num_res_faces,
              const GRBVar fvars[DMAX], const GRBVar evars[EMAX],
              ofstream out_files_ptr [NFILE]);
void get_out_name(const int n, const int p, string &fname);
void open_out_file(const int n, const int p, string (&out_file_names)[NFILE], 
                   ofstream out_files_ptr[NFILE]);
void close_files(ofstream out_files_ptr[NFILE]);

// From dual.cpp
int find_position(const int v, const int u, int v_adj[3]);
int counter_clockwise_walk(const int face_id, int u, int v, const int n, 
                           vertex (&primal)[NMAX], face& cur_face);
void construct_planar_dual(fullerene (&F), const int p);

// From lp.cpp
int check_if_sol_valid(const fullerene (&F), const int p, 
                       const GRBVar fvars[DMAX], const GRBVar evars[EMAX]);
void p_anionic_clar_lp(const fullerene (&F), const int p, GRBEnv grb_env,
                       ofstream out_files_ptr [NFILE]);
