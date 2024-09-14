import pandas as pd
import numpy as np
import sys

def compare_files(known_sol, bo_solved_sol, n, p):
    # Read files into pandas DataFrames
    known_sol_df = pd.read_csv(known_sol, header=None, names=['known_sol'])
    bo_solved_df = pd.read_csv(bo_solved_sol, header=None, names=['bo_solved'])

    conditions = [
        (known_sol_df['known_sol'] == bo_solved_df['bo_solved']),
        (known_sol_df['known_sol'] > bo_solved_df['bo_solved']) & (bo_solved_df['bo_solved'] > 0),
        (known_sol_df['known_sol'] > bo_solved_df['bo_solved']) & (bo_solved_df['bo_solved'] == 0),
    ]
    choices = ['equal', known_sol_df['known_sol'] - bo_solved_df['bo_solved'], 'fail']
    bo_solved_df['results'] = np.select(conditions, choices, default=np.nan)

    print("Summary for fullerenes on {} vertices with non-zero {}-anionic clar number".format(n, p))
    print("Num. sol. that are exactly the same:  {} = {}%".format( 
          len(bo_solved_df.loc[bo_solved_df['results'] == 'equal']),
          len(bo_solved_df.loc[bo_solved_df['results'] == 'equal'])/len(bo_solved_df['results'])*100))
    for value in sorted(bo_solved_df['results'].unique()):
        if value == 'equal':
            continue
        elif value == 'fail':
            continue
        print("Num. sol. of optimal by {:3d}: {} = {}%".format(int(value), 
              len(bo_solved_df.loc[bo_solved_df['results'] == value]),
              len(bo_solved_df.loc[bo_solved_df['results'] == value])/len(bo_solved_df['results'])*100))
    print("Num. sol. failed:            {} = {}%".format( 
          len(bo_solved_df.loc[bo_solved_df['results'] == 'fail']),
          len(bo_solved_df.loc[bo_solved_df['results'] == 'fail'])/len(bo_solved_df['results'])*100))
    print("")



# Example usage:
if __name__ == "__main__":
    n = int(sys.argv[1])
    p = int(sys.argv[2])
    known_sol = "../../gen_output/{:03d}_{:02d}_anc_#".format(n, p)
    #bo_solved_sol = "../../output/sol_score"
    bo_solved_sol = "output/{:03d}_{:02d}_anionic_clar_num".format(n, p)

    try:
        compare_files(known_sol, bo_solved_sol, n, p)
    except FileNotFoundError:
        print("Error: One or both directories or files not found")




