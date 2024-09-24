#include "include.h"

int main(int argc, char *argv[]) {
  // isomer
  Fullerene F;
  // out files
  string out_file_names[NFILE] = {"output/pp_anionic_clar_num",
                                  "output/pp_r_pent", "output/pp_r_hex",
                                  "output/pp_match_e"};
  ofstream out_files_ptr[NFILE];

  // define gurobi solve environments
  GRBEnv grb_env = GRBEnv(true);
  grb_env.set(GRB_IntParam_OutputFlag, DEBUG_GUROBI);
  grb_env.start();

  int graph_num = 0;

  // the first fullerene we test is C20:1
  // C_0(C20:1) = 0
  int p = 0;
  read_fullerene(F, 0);
  open_out_file(p, out_file_names, out_files_ptr);
  F.id = graph_num;
  // construct planar dual graph
  construct_planar_dual(F, p);
  if (p_anionic_clar_lp(F, p, grb_env, out_files_ptr) != 0) {
    throw runtime_error("Failed solving C_0(C20:1)\n");
  }
  close_files(out_files_ptr);

  // the second fullerene we test is C60:1812
  // it has p-anionic Clar numbers
  // p = 0, 8
  // p = 2, 6
  // p = 4, 6
  // p = 6, 8
  // p = 8, 8
  // p = 10, 10
  // p = 12, 12
  const int clar_nums[7] = {8, 6, 6, 8, 8, 10, 12};
  read_fullerene(F, 0);
  construct_planar_dual(F, 0);
  F.id = graph_num;
  for (int p = 0; p < 13; p += 2) {
    open_out_file(p, out_file_names, out_files_ptr);
    // construct planar dual graph
    if (p_anionic_clar_lp(F, p, grb_env, out_files_ptr) != clar_nums[p / 2]) {
      throw runtime_error("Failed solving C_" + to_string(p) + "(C60:1812)\n");
    }
    close_files(out_files_ptr);
  }
  cout << "Successfully solved ILPs\n";
}
