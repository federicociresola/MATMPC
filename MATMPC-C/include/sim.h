#ifndef SIM_H_
#define SIM_H_

//#include "mex.h"
#include <stdbool.h>
#include <stdio.h>
#include "typedef_utils.h"

typedef struct{
    double h;
    int nx;
    int nu;
    int nz;
    int num_stages;
    int num_steps;
    bool forw_sens_flag;
    bool adj_sens_flag;
//    bool gp_flag;
//    bool gp_status_flag;
//    bool only_gp_flag;
}sim_opts;

typedef struct{
    double *x;
    double *u;
    double *p;
    double *z;
    double *lambda;
}sim_in;

typedef struct{
    double *xn;
    double *zn;
    double *Sx;
    double *Su;
    double *adj_sens;
    // GP
//    double *xn_gp;
//    double *xn_ode;
//    double *A_gp;
//    double *B_gp;
//    double *A_ode;
//    double *B_ode;
}sim_out;

sim_opts* sim_opts_create(const TypeMem *mem);
sim_in* sim_in_create(sim_opts *opts);
sim_out* sim_out_create(sim_opts *opts);

void sim_opts_free(sim_opts *opts);
void sim_in_free(sim_in *in);
void sim_out_free(sim_out *out);

#endif