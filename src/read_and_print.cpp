#include "include.h"

// read in fullerene graph and populate some default values into the 
// data structure
bool read_fullerene(int *n, vertex (&primal)[NMAX]) {
    int degree;
    // read in number of vertices of isomer
    if (!(cin >> *n)) return false;
    // check that number of vertices is within range
    // smallest fullerene is on 20 vertices and there exists no fullerene on 22 vertices
    if (*n < 20 || *n == 22 || *n > NMAX) {
        cerr << "Error: Invalid fullerene size of " << *n << endl;
        exit(0);
    }
    // for each vertex in the graph
    for (int i = 0; i < *n; i++) {
        // read in degree of vertex i
        if (!(cin >> degree)) {
            cerr << "Error: Failed to read in vertex degree" << endl;
            exit(0);
        }
        // check vertex degree
        if (degree != 3) {
            cerr << "Error: Invalid degree size: " << degree << endl;
            exit(0);
        }
        // for each neighbour of i
        for (int j = 0; j < 3; j++) {
            // update adjacency list of vertex i
            if (!(cin >> primal[i].adj_v[j])) {
                cerr << "Error: Failed to read in neighbour: " << j
                          << " of vertex " << i << endl;
                exit(0);
            }
            // each vertex lies on 3 faces, at this time, we do not know what their
            // ids are, we will therefore assign then as -1 to represent 'unassigned'
            primal[i].faces[j] = -1;
        }
        // at this point, we have not assigned edge ids, so the number of edge ids
        // assigned to this vertex is 0
        primal[i].num_edges = 0;
    }
    return true;
}

void print_primal(const int n, const vertex (&primal)[NMAX]) {
    cout << "Primal graph" << endl;
    cout << "Vert:  Neighbours         Faces           Edges" << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(4) << i << ": ";
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << primal[i].adj_v[j];
            if (j == 2) cout << " || ";
        }
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << primal[i].faces[j];
            if (j == 2) cout << " || ";
        }
        for (int j = 0; j < 3; j++) {
            cout << setw(4) << primal[i].edges[j];
        }
        cout << endl;
    }
}

void print_dual(const int dual_n, const face (&dual)[DMAX]) {
    cout << "Dual graph" << endl << "Face:       Neighbours" << endl;
    for (int i = 0; i < dual_n; i++) {
        cout << setw(4) << i << ": ";
        for (int j = 0; j < dual[i].size; j++) {
            cout << setw(4) << dual[i].adj_f[j];
        }
        cout << endl;
    }
}

void print_sol(const fullerene (&F), const int num_res_faces,
               const GRBVar fvars[DMAX], const GRBVar evars[EMAX]) {
    cout << endl << "n = " << F.n << ", graph num = " << F.id << endl << 
        "Solution summary:" << endl;
    // if there is no solution
    if (num_res_faces == 0) {
        cout << "No solution" << endl;
        return;
    }
    cout << "Num resonant faces: " << num_res_faces << endl;
    cout << "Resonant face ids: ";
    for (int i = 0; i < F.dual_n; i++) {
        if (fvars[i].get(GRB_DoubleAttr_X) < 0.99) continue;
        cout << i << " ";
    }
    cout << endl << "Matching edges: ";
    for (int i = 0; i < F.num_edges; i++) {
        if (evars[i].get(GRB_DoubleAttr_X) < 0.99) continue;
        cout << "(" << F.edges[i].vertices[0] << "," << F.edges[i].vertices[1] << ") ";
    }
    cout << endl << endl;
}

void save_sol(const fullerene (&F), const int p, const int num_res_faces,
              const GRBVar fvars[DMAX], const GRBVar evars[EMAX],
              ofstream out_files_ptr [NFILE]) {
    // if there is no solution
    if (num_res_faces == 0) {
        out_files_ptr[0] << 0 << endl;
        out_files_ptr[1] << 0 << endl;
        out_files_ptr[2] << 0 << endl;
        out_files_ptr[3] << 0 << endl;
        return;
    }
    // number of resonant faces
    out_files_ptr[0] << num_res_faces << endl;
    // print resonant faces
    out_files_ptr[1] << p << " ";
    out_files_ptr[2] << num_res_faces - p << " ";
    for (int i = 0; i < F.dual_n; i++) {
        if (fvars[i].get(GRB_DoubleAttr_X) < 0.99) continue;
        if (F.dual[i].size == 5) {
            out_files_ptr[1] << i << " ";
        } else {
            out_files_ptr[2] << i << " ";
        } 
    }
    out_files_ptr[1] << endl;
    out_files_ptr[2] << endl;
    // matching edges
    out_files_ptr[3] << F.n - 6*(num_res_faces - p) - 5*p << " ";
    for (int i = 0; i < F.num_edges; i++) {
        if (evars[i].get(GRB_DoubleAttr_X) < 0.99) continue;
        out_files_ptr[3] << F.edges[i].vertices[0] << " " 
                         << F.edges[i].vertices[1] << " ";
    }
    out_files_ptr[3] << endl;
}

void get_out_name(const int n, const int p, string &fname){
    // determine first digit of n
    int n1 = n / 100;
    // determine second digit of n
    int n2 = (n % 100) / 10;
    // determine third digit of n
    int n3 = (n % 100) % 10;
    // determine first digit of p
    int p1 = p / 10;
    // determine last digit of p
    int p2 = p % 10;
    // update the file name characters
    fname[7] = (char) n1 + '0';
    fname[8] = (char) n2 + '0';
    fname[9] = (char) n3 + '0';
    fname[11] = (char) p1 + '0';
    fname[12] = (char) p2 + '0';
}

void open_out_file(const int n, const int p, string (&out_file_names)[NFILE], 
                   ofstream out_files_ptr[NFILE]){ 
    if (n > 999) {
        cerr << "Function get_out_name is limited to n <= 999" << endl;
        exit(0);
    }
    for (int i = 0; i < NFILE; i++){
         get_out_name(n, p, out_file_names[i]);
         out_files_ptr[i].open(out_file_names[i], ios::app);
         if (!out_files_ptr[i].is_open()) {
             cerr << "Error opening file " << out_file_names[i] << endl;
             exit(0);
         }
    }
}

void close_files(ofstream out_files_ptr[NFILE]){
     for (int i = 0; i < NFILE; i++){
         out_files_ptr[i].close();
     }
}
