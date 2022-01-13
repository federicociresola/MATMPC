/* This file was automatically generated by CasADi.
   The CasADi copyright holders make no ownership claim of its contents. */
#ifdef __cplusplus
extern "C" {
#endif

/* How to prefix internal symbols */
#ifdef CODEGEN_PREFIX
  #define NAMESPACE_CONCAT(NS, ID) _NAMESPACE_CONCAT(NS, ID)
  #define _NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) gp_jac_fun_ ## ID
#endif

#include <math.h>
#include <string.h>
#ifdef MATLAB_MEX_FILE
#include <mex.h>
#endif

#ifndef casadi_real
#define casadi_real double
#endif

#define to_double(x) (double) x
#define to_int(x) (int) x
#define CASADI_CAST(x,y) (x) y

/* Pre-c99 compatibility */
#if __STDC_VERSION__ < 199901L
  #define fmin CASADI_PREFIX(fmin)
  casadi_real fmin(casadi_real x, casadi_real y) { return x<y ? x : y;}
  #define fmax CASADI_PREFIX(fmax)
  casadi_real fmax(casadi_real x, casadi_real y) { return x>y ? x : y;}
#endif

/* CasADi extensions */
#define sq CASADI_PREFIX(sq)
casadi_real sq(casadi_real x) { return x*x;}
#define sign CASADI_PREFIX(sign)
casadi_real CASADI_PREFIX(sign)(casadi_real x) { return x<0 ? -1 : x>0 ? 1 : x;}
#define twice CASADI_PREFIX(twice)
casadi_real twice(casadi_real x) { return x+x;}
#define if_else CASADI_PREFIX(if_else)
casadi_real if_else(casadi_real c, casadi_real x, casadi_real y) { return c!=0 ? x : y;}

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_fill CASADI_PREFIX(fill)
#define casadi_from_mex CASADI_PREFIX(from_mex)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_s2 CASADI_PREFIX(s2)
#define casadi_s3 CASADI_PREFIX(s3)
#define casadi_s4 CASADI_PREFIX(s4)
#define casadi_to_mex CASADI_PREFIX(to_mex)

/* Printing routine */
#ifdef MATLAB_MEX_FILE
  #define PRINTF mexPrintf
#else
  #define PRINTF printf
#endif

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

static const int casadi_s0[8] = {4, 1, 0, 4, 0, 1, 2, 3};
static const int casadi_s1[5] = {1, 1, 0, 1, 0};
static const int casadi_s2[9] = {5, 1, 0, 5, 0, 1, 2, 3, 4};
static const int casadi_s3[4] = {0, 1, 0, 0};
static const int casadi_s4[23] = {4, 4, 0, 4, 8, 12, 16, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};

void casadi_fill(casadi_real* x, int n, casadi_real alpha) {
  int i;
  if (x) {
    for (i=0; i<n; ++i) *x++ = alpha;
  }
}

#ifdef MATLAB_MEX_FILE
casadi_real* casadi_from_mex(const mxArray* p, casadi_real* y, const int* sp, casadi_real* w) {
  if (!mxIsDouble(p) || mxGetNumberOfDimensions(p)!=2)
    mexErrMsgIdAndTxt("Casadi:RuntimeError","\"from_mex\" failed: Not a two-dimensional matrix of double precision.");
  int nrow = *sp++, ncol = *sp++, nnz = sp[ncol];
  const int *colind=sp, *row=sp+ncol+1;
  size_t p_nrow = mxGetM(p), p_ncol = mxGetN(p);
  int is_sparse = mxIsSparse(p);
  mwIndex *Jc, *Ir;
  if (is_sparse) {
#ifndef CASADI_MEX_NO_SPARSE
    Jc = mxGetJc(p);
    Ir = mxGetIr(p);
#else /* CASADI_MEX_NO_SPARSE */
    mexErrMsgIdAndTxt("Casadi:RuntimeError","\"from_mex\" failed: Sparse inputs disabled.");
#endif /* CASADI_MEX_NO_SPARSE */
  }
  const double* p_data = (const double*)mxGetData(p);
  if (p_nrow==1 && p_ncol==1) {
    double v = is_sparse && Jc[1]==0 ? 0 : *p_data;
    casadi_fill(y, nnz, v);
  } else {
    int tr = 0;
    if (nrow!=p_nrow || ncol!=p_ncol) {
      tr = nrow==p_ncol && ncol==p_nrow && (nrow==1 || ncol==1);
      if (!tr) mexErrMsgIdAndTxt("Casadi:RuntimeError","\"from_mex\" failed: Dimension mismatch.");
    }
    int r,c,k;
    if (is_sparse) {
      if (tr) {
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]+c*nrow]=0;
        for (c=0; c<p_ncol; ++c)
          for (k=Jc[c]; k<Jc[c+1]; ++k) w[c+Ir[k]*p_ncol] = p_data[k];
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) y[k] = w[row[k]+c*nrow];
      } else {
        for (c=0; c<ncol; ++c) {
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]]=0;
          for (k=Jc[c]; k<Jc[c+1]; ++k) w[Ir[k]]=p_data[k];
          for (k=colind[c]; k<colind[c+1]; ++k) y[k]=w[row[k]];
        }
      }
    } else {
      for (c=0; c<ncol; ++c) {
        for (k=colind[c]; k<colind[c+1]; ++k) {
          y[k] = p_data[row[k]+c*nrow];
        }
      }
    }
  }
  return y;
}

#endif

#ifdef MATLAB_MEX_FILE
mxArray* casadi_to_mex(const int* sp, const casadi_real* x) {
  int nrow = *sp++, ncol = *sp++, nnz = sp[ncol];
  const int *colind = sp, *row = sp+ncol+1;
#ifndef CASADI_MEX_NO_SPARSE
  if (nnz!=nrow*ncol) {
    mxArray*p = mxCreateSparse(nrow, ncol, nnz, mxREAL);
    int i;
    mwIndex* j;
    for (i=0, j=mxGetJc(p); i<=ncol; ++i) *j++ = *colind++;
    for (i=0, j=mxGetIr(p); i<nnz; ++i) *j++ = *row++;
    if (x) {
      double* d = (double*)mxGetData(p);
      for (i=0; i<nnz; ++i) *d++ = to_double(*x++);
    }
    return p;
  }
#endif /* CASADI_MEX_NO_SPARSE */
  mxArray* p = mxCreateDoubleMatrix(nrow, ncol, mxREAL);
  if (x) {
    double* d = (double*)mxGetData(p);
    int c, k;
    for (c=0; c<ncol; ++c) {
      for (k=colind[c]; k<colind[c+1]; ++k) {
        d[row[k]+c*nrow] = to_double(*x++);
      }
    }
  }
  return p;
}

#endif

/* gp_jac_fun:(i0[4],i1,i2[5],i3[0])->(o0[4x4],o1[4]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem) {
  casadi_real a0=0.;
  if (res[0]!=0) res[0][0]=a0;
  if (res[0]!=0) res[0][1]=a0;
  if (res[0]!=0) res[0][2]=a0;
  if (res[0]!=0) res[0][3]=a0;
  if (res[0]!=0) res[0][4]=a0;
  if (res[0]!=0) res[0][5]=a0;
  if (res[0]!=0) res[0][6]=a0;
  if (res[0]!=0) res[0][7]=a0;
  if (res[0]!=0) res[0][8]=a0;
  if (res[0]!=0) res[0][9]=a0;
  if (res[0]!=0) res[0][10]=a0;
  if (res[0]!=0) res[0][11]=a0;
  if (res[0]!=0) res[0][12]=a0;
  if (res[0]!=0) res[0][13]=a0;
  if (res[0]!=0) res[0][14]=a0;
  if (res[0]!=0) res[0][15]=a0;
  if (res[1]!=0) res[1][0]=a0;
  if (res[1]!=0) res[1][1]=a0;
  if (res[1]!=0) res[1][2]=a0;
  if (res[1]!=0) res[1][3]=a0;
  return 0;
}

CASADI_SYMBOL_EXPORT int gp_jac_fun(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT void gp_jac_fun_incref(void) {
}

CASADI_SYMBOL_EXPORT void gp_jac_fun_decref(void) {
}

CASADI_SYMBOL_EXPORT int gp_jac_fun_n_in(void) { return 4;}

CASADI_SYMBOL_EXPORT int gp_jac_fun_n_out(void) { return 2;}

CASADI_SYMBOL_EXPORT const char* gp_jac_fun_name_in(int i){
  switch (i) {
    case 0: return "i0";
    case 1: return "i1";
    case 2: return "i2";
    case 3: return "i3";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* gp_jac_fun_name_out(int i){
  switch (i) {
    case 0: return "o0";
    case 1: return "o1";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* gp_jac_fun_sparsity_in(int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s1;
    case 2: return casadi_s2;
    case 3: return casadi_s3;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* gp_jac_fun_sparsity_out(int i) {
  switch (i) {
    case 0: return casadi_s4;
    case 1: return casadi_s0;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int gp_jac_fun_work(int *sz_arg, int* sz_res, int *sz_iw, int *sz_w) {
  if (sz_arg) *sz_arg = 4;
  if (sz_res) *sz_res = 2;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 1;
  return 0;
}

#ifdef MATLAB_MEX_FILE
void mex_gp_jac_fun(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  int i, j;
  if (argc>4) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"gp_jac_fun\" failed. Too many input arguments (%d, max 4)", argc);
  if (resc>2) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"gp_jac_fun\" failed. Too many output arguments (%d, max 2)", resc);
  int *iw = 0;
  casadi_real w[35];
  const casadi_real* arg[4] = {0};
  if (--argc>=0) arg[0] = casadi_from_mex(argv[0], w, casadi_s0, w+30);
  if (--argc>=0) arg[1] = casadi_from_mex(argv[1], w+4, casadi_s1, w+30);
  if (--argc>=0) arg[2] = casadi_from_mex(argv[2], w+5, casadi_s2, w+30);
  if (--argc>=0) arg[3] = casadi_from_mex(argv[3], w+10, casadi_s3, w+30);
  casadi_real* res[2] = {0};
  --resc;
  res[0] = w+10;
  if (--resc>=0) res[1] = w+26;
  i = gp_jac_fun(arg, res, iw, w+30, 0);
  if (i) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"gp_jac_fun\" failed.");
  if (res[0]) resv[0] = casadi_to_mex(casadi_s4, res[0]);
  if (res[1]) resv[1] = casadi_to_mex(casadi_s0, res[1]);
}
#endif


#ifdef MATLAB_MEX_FILE
void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  char buf[11];
  int buf_ok = --argc >= 0 && !mxGetString(*argv++, buf, sizeof(buf));
  if (!buf_ok) {
    /* name error */
  } else if (strcmp(buf, "gp_jac_fun")==0) {
    return mex_gp_jac_fun(resc, resv, argc, argv);
  }
  mexErrMsgTxt("First input should be a command string. Possible values: 'gp_jac_fun'");
}
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif
