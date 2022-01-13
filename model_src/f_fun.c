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
  #define CASADI_PREFIX(ID) f_fun_ ## ID
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

/* f_fun:(states[4],controls,params[5],alg[0])->(xdot[4]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem) {
  casadi_real a0=arg[0] ? arg[0][2] : 0;
  if (res[0]!=0) res[0][0]=a0;
  a0=arg[0] ? arg[0][3] : 0;
  if (res[0]!=0) res[0][1]=a0;
  casadi_real a1=9.8100000000000005e+000;
  casadi_real a2=arg[2] ? arg[2][1] : 0;
  casadi_real a3=(a1*a2);
  casadi_real a4=arg[0] ? arg[0][1] : 0;
  casadi_real a5=cos(a4);
  a3=(a3*a5);
  a5=sin(a4);
  a3=(a3*a5);
  a5=arg[2] ? arg[2][2] : 0;
  casadi_real a6=(a2*a5);
  casadi_real a7=sin(a4);
  a6=(a6*a7);
  a7=sq(a0);
  a6=(a6*a7);
  a3=(a3-a6);
  a6=arg[1] ? arg[1][0] : 0;
  a3=(a3+a6);
  a7=arg[2] ? arg[2][0] : 0;
  casadi_real a8=(a7+a2);
  casadi_real a9=cos(a4);
  a9=sq(a9);
  a9=(a2*a9);
  a8=(a8-a9);
  a3=(a3/a8);
  a9=arg[2] ? arg[2][3] : 0;
  a3=(a3*a9);
  if (res[0]!=0) res[0][2]=a3;
  a3=cos(a4);
  a6=(a6*a3);
  a3=(a2*a5);
  casadi_real a10=cos(a4);
  a3=(a3*a10);
  a10=sin(a4);
  a3=(a3*a10);
  a0=sq(a0);
  a3=(a3*a0);
  a6=(a6-a3);
  a7=(a7+a2);
  a1=(a1*a7);
  a4=sin(a4);
  a1=(a1*a4);
  a6=(a6+a1);
  a5=(a5*a8);
  a6=(a6/a5);
  a6=(a6*a9);
  if (res[0]!=0) res[0][3]=a6;
  return 0;
}

CASADI_SYMBOL_EXPORT int f_fun(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT void f_fun_incref(void) {
}

CASADI_SYMBOL_EXPORT void f_fun_decref(void) {
}

CASADI_SYMBOL_EXPORT int f_fun_n_in(void) { return 4;}

CASADI_SYMBOL_EXPORT int f_fun_n_out(void) { return 1;}

CASADI_SYMBOL_EXPORT const char* f_fun_name_in(int i){
  switch (i) {
    case 0: return "states";
    case 1: return "controls";
    case 2: return "params";
    case 3: return "alg";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* f_fun_name_out(int i){
  switch (i) {
    case 0: return "xdot";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* f_fun_sparsity_in(int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s1;
    case 2: return casadi_s2;
    case 3: return casadi_s3;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* f_fun_sparsity_out(int i) {
  switch (i) {
    case 0: return casadi_s0;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int f_fun_work(int *sz_arg, int* sz_res, int *sz_iw, int *sz_w) {
  if (sz_arg) *sz_arg = 4;
  if (sz_res) *sz_res = 1;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 11;
  return 0;
}

#ifdef MATLAB_MEX_FILE
void mex_f_fun(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  int i, j;
  if (argc>4) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_fun\" failed. Too many input arguments (%d, max 4)", argc);
  if (resc>1) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_fun\" failed. Too many output arguments (%d, max 1)", resc);
  int *iw = 0;
  casadi_real w[25];
  const casadi_real* arg[4] = {0};
  if (--argc>=0) arg[0] = casadi_from_mex(argv[0], w, casadi_s0, w+14);
  if (--argc>=0) arg[1] = casadi_from_mex(argv[1], w+4, casadi_s1, w+14);
  if (--argc>=0) arg[2] = casadi_from_mex(argv[2], w+5, casadi_s2, w+14);
  if (--argc>=0) arg[3] = casadi_from_mex(argv[3], w+10, casadi_s3, w+14);
  casadi_real* res[1] = {0};
  --resc;
  res[0] = w+10;
  i = f_fun(arg, res, iw, w+14, 0);
  if (i) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_fun\" failed.");
  if (res[0]) resv[0] = casadi_to_mex(casadi_s0, res[0]);
}
#endif


#ifdef MATLAB_MEX_FILE
void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  char buf[6];
  int buf_ok = --argc >= 0 && !mxGetString(*argv++, buf, sizeof(buf));
  if (!buf_ok) {
    /* name error */
  } else if (strcmp(buf, "f_fun")==0) {
    return mex_f_fun(resc, resv, argc, argv);
  }
  mexErrMsgTxt("First input should be a command string. Possible values: 'f_fun'");
}
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif
