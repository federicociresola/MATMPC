/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High Performance Interior Point Method.                                                *
* Copyright (C) 2017 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* HPIPM is mxFree software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPIPM is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPIPM; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                                                                                                 *
**************************************************************************************************/

//#include "mex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef_utils.h"
#include "hpipm_d_ocp_qp_ipm.h"
#include "hpipm_d_ocp_qp_dim.h"
#include "hpipm_d_ocp_qp.h"
#include "hpipm_d_ocp_qp_sol.h"

 
int hpipm_sparse(TypeMem *mem, TypeSettings *settings)
	{
		
	// get data 

    double *Q = mem[0].Q; //&(mem->
    double *S = mem[0].S;
    double *R = mem[0].R;
    double *A = mem[0].A;
    double *B = mem[0].B;
    double *Cx = mem[0].Cx;
    double *C = mem[0].Cgx;
    double *D = mem[0].Cgu;
    double *CN = mem[0].CgN;
    double *q = mem[0].gx;
    double *r = mem[0].gu;
    double *b = mem[0].a;
    double *ds0 = mem[0].ds0;
    double *lg = mem[0].lc;
    double *ug = mem[0].uc;
    double *lbu = mem[0].lb_du;
    double *ubu = mem[0].ub_du;
    double *lbx = mem[0].lb_dx;
    double *ubx = mem[0].ub_dx;

    double *x = mem[0].dx;
    double *u = mem[0].du;
    double *pi = mem[0].lambda_new;
    double *mu_u = mem[0].mu_u_new;
    double *mu_x = mem[0].mu_x_new;
    double *mu = mem[0].mu_new;

    int nx = settings->nx;
    int nu = settings->nu;
    int ng = settings->nc;
    int ngN = settings->ncN;
    int N = settings->N;
    int nbx = settings->nbx;
    int *nbx_idx = settings[0].nbx_idx;

    double mu0 = mem->mu0;
    int max_qp_it = mem->max_qp_it;
    int pred_corr = mem->pred_corr;
    int cond_pred_corr = mem->cond_pred_corr;
    int solver_mode = mem->solver_mode;
               	
	int ii, jj;
    int idx;
    
    // number of states for each stage
	int nx_v[N+1];
	for(ii=0; ii<=N; ii++)
		nx_v[ii] = nx;

    // number of controls for each stage
	int nu_v[N+1];
	for(ii=0; ii<N; ii++)
		nu_v[ii] = nu;
	nu_v[N] = 0;

    // number of bounds for each stage
	int nb_v[N+1];
    nb_v[0] = nu+nx;
	for(ii=1; ii<N; ii++)
		nb_v[ii] = nu+nbx;
    nb_v[N] = nbx;
    
    // number of control bounds for each stage
	int nbu_v[N+1];
	for(ii=0; ii<N; ii++)
        nbu_v[ii] = nu;
	nbu_v[N] = 0;

    // number of state bounds for each stage
	int nbx_v[N+1];
    nbx_v[0] = nx;
	for(ii=1; ii<=N; ii++)
        nbx_v[ii] = nbx;
		
    // number of general bounds for each stage
	int ng_v[N+1];
	for(ii=0; ii<N; ii++)
		ng_v[ii] = ng;
	ng_v[N] = ngN;

    
    int nsbx_v[N+1]; // number of softed constraints on state box constraints
    int nsbu_v[N+1]; // number of softed constraints on input box constraints
    int nsbg_v[N+1]; // number of softed constraints on general constraints
	for(ii=0; ii<=N; ii++){
		nsbx_v[ii] = 0;
        nsbu_v[ii] = 0;
        nsbg_v[ii] = 0;
    }
    
    // index of state and control bounds for each stage
	int *hidxbx[N+1];
    int *hidxbu[N+1];
    for(ii=0; ii<=N; ii++){
        hidxbx[ii] = (int *)malloc(nbx_v[ii]*sizeof(int));
        hidxbu[ii] = (int *)malloc(nbu_v[ii]*sizeof(int));
    }
    
    for(jj=0; jj<nbu_v[0]; jj++)
		hidxbu[0][jj] = jj;
    for(jj=0; jj<nbx_v[0]; jj++)
		hidxbx[0][jj] = jj;
	for(ii=1; ii<=N; ii++){		
		for(jj=0; jj<nbu_v[ii]; jj++)
			hidxbu[ii][jj] = jj;
        
        for(jj=0; jj<nbx_v[ii]; jj++){
            idx = (int)nbx_idx[jj]-1;
			hidxbx[ii][jj] = idx;
        }
	}	

    // create arrays for all-stage variables
	double *hA[N];
	double *hB[N];
	double *hb[N];
	double *hQ[N+1];
	double *hS[N+1];
	double *hR[N+1];
	double *hq[N+1];
	double *hr[N+1];
	double *hlbx[N+1];
	double *hubx[N+1];
    double *hlbu[N+1];
	double *hubu[N+1];
	double *hC[N+1];
	double *hD[N+1];
	double *hlg[N+1];
	double *hug[N+1];
	double *hx[N+1];
	double *hu[N+1];
	double *hpi[N];
	double *hlam_lb[N+1];
	double *hlam_ub[N+1];
	double *hlam_lg[N+1];
	double *hlam_ug[N+1];

    // initialize all QP data
    for(ii=0; ii<N; ii++)
		hA[ii] = A+ii*nx*nx;

	for(ii=0; ii<N; ii++)
		hB[ii] = B+ii*nx*nu;
    
	for(ii=0; ii<N; ii++)
		hb[ii] = b+ii*nx;

	for(ii=0; ii<=N; ii++)
		hQ[ii] = Q+ii*nx*nx;
        
	for(ii=0; ii<N; ii++)
		hS[ii] = S+ii*nx*nu;

    for(ii=0; ii<N; ii++)
		hR[ii] = R+ii*nu*nu;

	for(ii=0; ii<=N; ii++)
		hq[ii] = q+ii*nx;
	
	for(ii=0; ii<N; ii++)
		hr[ii] = r+ii*nu;

    hlbx[0] = (double *)malloc(nbx_v[0]*sizeof(double));
    hubx[0] = (double *)malloc(nbx_v[0]*sizeof(double));
    hlbu[0] = (double *)malloc(nbu_v[0]*sizeof(double));
    hubu[0] = (double *)malloc(nbu_v[0]*sizeof(double));
    for(ii=0; ii<nu; ii++){
		hlbu[0][ii] = lbu[ii];
        hubu[0][ii] = ubu[ii];
    }
    for(ii=0; ii<nx; ii++){
		hlbx[0][ii] = ds0[ii];
        hubx[0][ii] = ds0[ii];
    }
	for(ii=1; ii<=N; ii++){
        hlbx[ii] = (double *)malloc(nbx_v[ii]*sizeof(double));
        hubx[ii] = (double *)malloc(nbx_v[ii]*sizeof(double));
        hlbu[ii] = (double *)malloc(nbu_v[ii]*sizeof(double));
        hubu[ii] = (double *)malloc(nbu_v[ii]*sizeof(double));
        for(jj=0;jj<nbu_v[ii];jj++){
            hlbu[ii][jj] = lbu[ii*nu+jj];
            hubu[ii][jj] = ubu[ii*nu+jj];
        }
        for(jj=0;jj<nbx_v[ii];jj++){
            hlbx[ii][jj] = lbx[(ii-1)*nbx+jj];
            hubx[ii][jj] = ubx[(ii-1)*nbx+jj];
        }
    }

	for(ii=0; ii<N; ii++)
		hC[ii] = C+ii*ng*nx;
	hC[N] = CN;

	for(ii=0; ii<N; ii++)
		hD[ii] = D+ii*ng*nu;

	for(ii=0; ii<=N; ii++)
		hlg[ii] = lg+ii*ng;

	for(ii=0; ii<=N; ii++)
		hug[ii] = ug+ii*ng;
	
	for(ii=0; ii<=N; ii++)
		hx[ii] = x+ii*nx;

	for(ii=0; ii<N; ii++)
		hu[ii] = u+ii*nu;
	
	for(ii=0; ii<N; ii++)
		hpi[ii] = pi+(ii+1)*nx;
    
    for(ii=0; ii<=N; ii++){
        hlam_lb[ii] = (double *)malloc(nb_v[ii]*sizeof(double));
        hlam_ub[ii] = (double *)malloc(nb_v[ii]*sizeof(double));
        hlam_lg[ii] = (double *)malloc(ng_v[ii]*sizeof(double));
        hlam_ug[ii] = (double *)malloc(ng_v[ii]*sizeof(double));
    }
    
	// ocp qp dim
    int dim_size = d_ocp_qp_dim_memsize(N);
    void *dim_mem = malloc(dim_size);

	struct d_ocp_qp_dim dim;
    d_ocp_qp_dim_create(N, &dim, dim_mem);
    d_ocp_qp_dim_set_all(nx_v, nu_v, nbx_v, nbu_v, ng_v, nsbx_v, nsbu_v, nsbg_v, &dim);
    
	// ocp qp
    int qp_size = d_ocp_qp_memsize(&dim);
    void *qp_mem = malloc(qp_size);
	struct d_ocp_qp qp;
    d_ocp_qp_create(&dim, &qp, qp_mem);
    d_ocp_qp_set_all(hA, hB, hb, hQ, hS, hR, hq, hr, hidxbx, hlbx, hubx, hidxbu, hlbu, hubu, hC, hD, hlg, hug, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &qp);

	// ocp qp sol
    int qp_sol_size = d_ocp_qp_sol_memsize(&dim);
    void *qp_sol_mem = malloc(qp_sol_size);
	struct d_ocp_qp_sol qp_sol;
    d_ocp_qp_sol_create(&dim, &qp_sol, qp_sol_mem);

	// ipm arg
    int ipm_arg_size = d_ocp_qp_ipm_arg_memsize(&dim);
    void *ipm_arg_mem = malloc(ipm_arg_size);

	struct d_ocp_qp_ipm_arg arg;
    d_ocp_qp_ipm_arg_create(&dim, &arg, ipm_arg_mem);
    
    d_ocp_qp_ipm_arg_set_default(solver_mode, &arg);

    double alpha_min = 1e-8;
    double tol_stat = 1e-4;
    double tol_eq = 1e-5;
    double tol_ineq = 1e-5;
    double tol_comp = 1e-5;
    double reg_prim = 1e-12;
    int warm_start = 0;
    int ric_alg = 0;
    
	d_ocp_qp_ipm_arg_set_iter_max(&max_qp_it, &arg);
	d_ocp_qp_ipm_arg_set_alpha_min(&alpha_min, &arg);
	d_ocp_qp_ipm_arg_set_mu0(&mu0, &arg);
	d_ocp_qp_ipm_arg_set_tol_stat(&tol_stat, &arg);
	d_ocp_qp_ipm_arg_set_tol_eq(&tol_eq, &arg);
	d_ocp_qp_ipm_arg_set_tol_ineq(&tol_ineq, &arg);
	d_ocp_qp_ipm_arg_set_tol_comp(&tol_comp, &arg);
	d_ocp_qp_ipm_arg_set_reg_prim(&reg_prim, &arg);
	d_ocp_qp_ipm_arg_set_warm_start(&warm_start, &arg);
	d_ocp_qp_ipm_arg_set_pred_corr(&pred_corr, &arg);
    d_ocp_qp_ipm_arg_set_cond_pred_corr(&cond_pred_corr, &arg);
	d_ocp_qp_ipm_arg_set_ric_alg(&ric_alg, &arg);

	// ipm workspace
    int ipm_size = d_ocp_qp_ipm_ws_memsize(&dim, &arg);
	void *ipm_mem = malloc(ipm_size);

	struct d_ocp_qp_ipm_ws workspace;
	d_ocp_qp_ipm_ws_create(&dim, &arg, &workspace, ipm_mem);

	// call solver
    int hpipm_status;
    d_ocp_qp_ipm_solve(&qp, &qp_sol, &arg, &workspace);
	d_ocp_qp_ipm_get_status(&workspace, &hpipm_status);
        
    // print stats
    int err = 0;
    if(hpipm_status == 0)
		{
        // printf("\n -> QP solved!\n");
		}
	else if(hpipm_status==1)
		{
        printf("\n -> Solver failed! Maximum number of iterations reached\n");
        err++;
		}
	else if(hpipm_status==2)
		{
        printf("\n -> Solver failed! Minimum step lenght reached\n");
        err++;
		}
	else if(hpipm_status==3)
		{
        printf("\n -> Solver failed! NaN in computations\n");
        err++;
		}
	else
		{
        printf("\n -> Solver failed! Unknown return flag\n");
        err++;
		}   
    if (err > 0){
        printf("Stop: HPIPM solver reports error!\n");
        return -1;
    }
    

	// convert back solution
    d_ocp_qp_sol_get_all(&qp_sol, hu, hx, NULL, NULL, hpi, hlam_lb, hlam_ub, hlam_lg, hlam_ug, NULL, NULL);
    
    // extrac multipliers
    for(jj=0;jj<nx;jj++)
        pi[jj] = hlam_ub[0][nu+jj] - hlam_lb[0][nu+jj];
    
    for(ii=0;ii<N;ii++){       
        for(jj=0;jj<nu;jj++)
            mu_u[ii*nu+jj] = hlam_ub[ii][jj] - hlam_lb[ii][jj];
                
        for(jj=0;jj<ng;jj++)
            mu[ii*ng+jj] = hlam_ug[ii][jj] - hlam_lg[ii][jj];
    }
    for(jj=0;jj<ngN;jj++)
        mu[N*ng+jj] = hlam_ug[N][jj] - hlam_lg[N][jj];
    
    for(ii=0;ii<N;ii++){  
        for(jj=0;jj<nbx;jj++)
            mu_x[ii*nbx+jj] = hlam_ub[ii+1][nbu_v[ii+1]+jj] - hlam_lb[ii+1][nbu_v[ii+1]+jj];
    }       
                     
    // Free memory
	for(ii=0;ii<=N;ii++){
        free(hidxbx[ii]);
        free(hidxbu[ii]);
        free(hlbx[ii]);
        free(hubx[ii]);
        free(hlbu[ii]);
        free(hubu[ii]);
        free(hlam_lb[ii]);
        free(hlam_ub[ii]);
        free(hlam_lg[ii]);
        free(hlam_ug[ii]);
    }
    
	free(dim_mem);
	free(qp_mem);
	free(qp_sol_mem);
	free(ipm_arg_mem);
	free(ipm_mem);

    return 1;

}

