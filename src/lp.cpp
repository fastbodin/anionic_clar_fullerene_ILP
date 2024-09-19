#include "include.h"

int check_if_sol_valid(const Fullerene(&F), const int p,
                       const vector<GRBVar> fvars, const vector<GRBVar> evars) {
  int num_res_faces = 0, res_pents = 0;
  // for each vertex in the graph
  for (int i = 0; i < F.n; i++) {
    // they should be covered by the p-anionic Clar structure exactly once
    int covered = 0;
    for (int j = 0; j < 3; j++) {
      // note the tolerance given to the variable assignment, i.e. > 0.99
      // covered by matching edge
      if (evars[F.primal[i].edges[j]].get(GRB_DoubleAttr_X) > 0.99)
        covered++;
      // covered by resonant face
      if (fvars[F.primal[i].faces[j]].get(GRB_DoubleAttr_X) > 0.99)
        covered++;
    }
    if (covered != 1) {
      const string msg = "\nVertex " + to_string(i) + " is covered " +
                         to_string(covered) + " times by structure.";
      throw_error(F.n, p, F.id, msg);
    }
  }
  // for each face in graph
  for (int i = 0; i < F.dual_n; i++) {
    if (fvars[i].get(GRB_DoubleAttr_X) > 0.99) {
      num_res_faces++;
      if (F.dual[i].size == 5)
        res_pents += 1;
    }
  }
  if (res_pents != p) {
    const string msg = "\nIncorrect # of res. pents: " + to_string(res_pents);
    throw_error(F.n, p, F.id, msg);
  }
  return num_res_faces;
}

void p_anionic_clar_lp(const Fullerene(&F), const int p, GRBEnv grb_env,
                       ofstream out_files_ptr[NFILE]) {
#if DEBUG_CLAR
  cout << "n = " << F.n << ", p = " << p << ", graph num = " << F.id << endl;
  cout << "Solving LP" << endl;
#endif
  try {
    // create an empty model
    GRBModel model = GRBModel(grb_env);
    // get the solver to focus on finding integer solutions
    // https://www.gurobi.com/documentation/11.0/refman/integralityfocus.html
    model.set(GRB_IntParam_IntegralityFocus, 1);

    // define a 1-dim array for face variables
    // fvars[f] = 1 if f is resonant and 0 otherwise
    vector<GRBVar> fvars(F.dual_n);
    // make face variables
    for (int f = 0; f < F.dual_n; f++) {
      // lower bound, upper bound, objective coeff, type
      fvars[f] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY);
    }

    // define a 1-dim array for edge variables
    // evars[i] = 1 if the edge i is matching edge and 0 otherwise
    vector<GRBVar> evars(F.num_edges);
    // make edge variables
    for (int i = 0; i < F.num_edges; i++) {
      evars[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
    }

    // each vertex is either in a resonant face or is the endpoint of
    // a matching edge
    for (int i = 0; i < F.n; i++) {
      GRBLinExpr cons1 = 0;
      for (int j = 0; j < 3; j++) {
#if DEBUG_CLAR
        cout << i << " is endpoint of edge " << F.primal[i].edges[j] << endl;
        cout << i << " lies on face " << F.primal[i].faces[j] << endl;
#endif
        cons1 += evars[F.primal[i].edges[j]] + fvars[F.primal[i].faces[j]];
      }
      // add the constraint to the model
      model.addConstr(cons1 == 1);
    }

    // need p resonant pentagons
    GRBLinExpr cons2 = 0;
    for (int f = 0; f < F.dual_n; f++) {
      if (F.dual[f].size == 5)
        cons2 += fvars[f];
    }
    // add constraint to model
    model.addConstr(cons2 == p);

    // The objective is to maximize number of resonant faces
    model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);

    // The objective coefficients are set during the creation of
    // the decision variables above. Run model
    model.optimize();
    int optimstatus = model.get(GRB_IntAttr_Status);

    // if optimum is attained
    if (optimstatus == GRB_OPTIMAL) {
      // check solution and grab number of resonant faces
      int num_res_faces = check_if_sol_valid(F, p, fvars, evars);
      save_sol(F, p, num_res_faces, fvars, evars, out_files_ptr);
#if DEBUG_CLAR
      print_sol(F, num_res_faces, fvars, evars);
#endif
      return;
      // if there is no solution
    } else if (optimstatus == GRB_INFEASIBLE) {
      // there are 0 resonant faces since no valid solution
      save_sol(F, p, 0, fvars, evars, out_files_ptr);
#if DEBUG_CLAR
      print_sol(F, 0, fvars, evars);
#endif
      return;
    } else {
      const string msg =
          "\nStatus of solve is: " + to_string(optimstatus) +
          "\nTo see what went wrong, checkout Gurobi Optimization Status Codes";
      throw_error(F.n, p, F.id, msg);
    }
  } catch (GRBException e) {
    const string msg = "\nCode: " + to_string(e.getErrorCode()) +
                       "\nMessage: " + e.getMessage();
    throw_error(F.n, p, F.id, msg);
  } catch (runtime_error e) {
    throw runtime_error(e);
  } catch (...) {
    throw_error(F.n, p, F.id, "\nUnknown error during optimization");
  }
}
