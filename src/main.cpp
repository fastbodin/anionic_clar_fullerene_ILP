#include "include.h"

int main(int argc, char *argv[]) {
    // want to solve for the p-anionic Clar number
    int p = atoi(argv[1]);
    // isomer
    fullerene F;
    // out files
    string out_file_names[NFILE] = {"output/nnn_pp_anionic_clar_num",
                                    "output/nnn_pp_r_pent",
                                    "output/nnn_pp_r_hex",
                                    "output/nnn_pp_match_e"};
    ofstream out_files_ptr[NFILE];

    // define gurobi solve environments 
    GRBEnv grb_env = GRBEnv(true);
    grb_env.set(GRB_IntParam_OutputFlag, DEBUG_GUROBI);
    grb_env.start();

    int graph_num = 0;
    // while there are isomers to read in
    while (read_fullerene(&F.n, F.primal)) {
        F.id = graph_num;
        // construct planar dual graph
        construct_planar_dual(F, p);
        //construct_planar_dual(F.n ,F.primal, &F.dual_n, F.dual, &F.num_edges, F.edges);
        open_out_file(F.n, p, out_file_names, out_files_ptr);
        // attempt to find p-anionic Clar structure
        p_anionic_clar_lp(F, p, grb_env, out_files_ptr);
        //print_sol(F, p, anionic_clar_num, out_files_ptr);
        close_files(out_files_ptr);

#if DEBUG
        cout << "Graph number " << graph_num << endl;
        print_primal(F.n, F.primal);
        print_dual(F.dual_n, F.dual);
#endif
        graph_num++;
    }
}

