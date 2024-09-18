#include "include.h"

// read in fullerene graph and populate some default values into the 
// data structure
bool read_fullerene(Fullerene (&F)) {
    int degree, n;
    // read in number of vertices of isomer
    if (!(cin >> n)) return false;
    // check that number of vertices is valid
    // n should be an even number at least 20 and not equal to 22
    if (n < 20 || n == 22) {
        cerr << "Error: Invalid fullerene size of " << n << endl;
        exit(1);
    }
    // resize the fullerene
    F.Resize(n);
    // for each vertex in the graph
    for (int i = 0; i < n; i++) {
        // read in degree of vertex i
        if (!(cin >> degree)) {
            cerr << "Error: Failed to read in vertex degree" << endl;
            exit(1);
        }
        // check vertex degree
        if (degree != 3) {
            cerr << "Error: Invalid degree size: " << degree << endl;
            exit(1);
        }
        // for each neighbour of i
        for (int j = 0; j < 3; j++) {
            // update adjacency list of vertex i
            if (!(cin >> F.primal[i].adj_v[j])) {
                cerr << "Error: Failed to read in neighbour: " << j
                          << " of vertex " << i << endl;
                exit(1);
            }
            // each vertex lies on 3 faces, at this time, we do not know what their
            // ids are, we will therefore assign then as -1 to represent 'unassigned'
            F.primal[i].faces[j] = -1;
        }
        // at this point, we have not assigned edge ids, so the number of edge ids
        // assigned to this vertex is 0
        F.primal[i].num_edges = 0;
    }
    return true;
}

void print_primal(const int n, const vector<vertex> primal) {
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

void print_dual(const int dual_n, const vector<face> dual) {
    cout << "Dual graph" << endl << "Face:       Neighbours" << endl;
    for (int i = 0; i < dual_n; i++) {
        cout << setw(4) << i << ": ";
        for (int j = 0; j < dual[i].size; j++) {
            cout << setw(4) << dual[i].adj_f[j];
        }
        cout << endl;
    }
}

void print_sol(const Fullerene (&F), const int num_res_faces,
               const vector<GRBVar> fvars, const vector<GRBVar> evars) {
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

void save_sol(const Fullerene (&F), const int p, const int num_res_faces,
              const vector<GRBVar> fvars, const vector<GRBVar> evars,
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

void get_out_name(const int p, string &fname){
    // determine first digit of p
    int p1 = p / 10;
    // determine last digit of p
    int p2 = p % 10;
    // update the file name characters
    fname[7] = (char) p1 + '0';
    fname[8] = (char) p2 + '0';
}

void open_out_file(const int p, string (&out_file_names)[NFILE], 
                   ofstream out_files_ptr[NFILE]){ 
    for (int i = 0; i < NFILE; i++){
         get_out_name(p, out_file_names[i]);
         out_files_ptr[i].open(out_file_names[i], ios::app);
         if (!out_files_ptr[i].is_open()) {
             cerr << "Error opening file " << out_file_names[i] << endl;
             exit(1);
         }
    }
}

void close_files(ofstream out_files_ptr[NFILE]){
     for (int i = 0; i < NFILE; i++){
         out_files_ptr[i].close();
     }
}
