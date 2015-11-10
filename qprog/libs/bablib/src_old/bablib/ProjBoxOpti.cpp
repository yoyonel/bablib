#include "ProjBoxOpti.h"
#include <Message.h>
#ifdef BABLIB_GSL_SUPPORT
    #include <gsl/gsl_linalg.h>
    #include <gsl/gsl_multifit.h>
#endif
BABLIB_NAMESPACE_USE

/**************************************************************************************************/

void ProjBoxOpti::addMapping(Vec4 p, Vec4 q, float w) {
    map << Mapping(p,q,w);
    }

void ProjBoxOpti::addMapping(Vec3 p, Vec3 q, float w) { addMapping(p.toVec4(), q.toVec4(), w); }

/**************************************************************************************************/

/*
static gsl_vector* solveSystemSVD(gsl_matrix *A, gsl_vector *b) {
    const int N = b->size;
    gsl_vector *x = gsl_vector_alloc(N),        // solution vector
               *S = gsl_vector_alloc(N),        // singular values vector
               *temp = gsl_vector_alloc(N);     // temporary matrix
    gsl_matrix *V = gsl_matrix_alloc(N, N);     // matrix V of decomposition A = U.S.tV  (U is placed in A after decomposition)
    
    gsl_linalg_SV_decomp(A, V, S, temp);
    gsl_linalg_SV_solve(A, V, S, b, x);
    
    gsl_vector_free(temp);
    gsl_vector_free(S);
    gsl_matrix_free(V);

    return x;
    }
*/

#ifdef BABLIB_GSL_SUPPORT
namespace {
    gsl_vector* leastSquares(gsl_matrix *A, gsl_vector *b, gsl_vector *w = NULL) {
        const int n = A->size1,
                  p = A->size2;
        gsl_vector *x = gsl_vector_alloc(p);
        gsl_matrix *cov = gsl_matrix_alloc(p, p);
        gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc(n, p);
        double chisq;
        
        if (w == NULL)
            gsl_multifit_linear(A, b, x, cov, &chisq, work);        // non-weighted least-squares
        else
            gsl_multifit_wlinear(A, w, b, x, cov, &chisq, work);    // weighted least-squares

        gsl_matrix_free(cov);
        gsl_multifit_linear_free(work);
        return x;
        }
    }
#endif

/**************************************************************************************************/

ProjTransfo ProjBoxOpti::calcTransfo() const {
    #ifdef BABLIB_GSL_SUPPORT
        bool weighted = false;
        
        const int
            N = map.size(),
            n = 4 * N,
            p = 16 + (N-1);
        gsl_matrix *A = gsl_matrix_alloc(n, p);
        gsl_vector *b = gsl_vector_alloc(n);
        gsl_vector *w = gsl_vector_alloc(n);
        
        gsl_matrix_set_zero(A);
        gsl_vector_set_zero(b);
        
        for (int a=0; a<map.size(); a++) {
            for (int i=0; i<4; i++) {
                const int c = 4*a + i;
                for (int j=0; j<4; j++) {
                    const int k = 4*i + j;
                    *gsl_matrix_ptr(A, c, k) = map[a].p[j];
                    }
                if (a < 7)
                    *gsl_matrix_ptr(A, c, 16+a) = map[a].q[i];
                else
                    *gsl_vector_ptr(b, c) = map[a].q[i];
                *gsl_vector_ptr(w, c) = map[a].w;
                weighted = weighted || (map[a].w != 1.0f);
                }
            }
        
        gsl_vector *x = leastSquares(A, b, weighted ? w : NULL);
        
        ProjTransfo T;
        for (int i=0; i<4; i++) for (int j=0; j<4; j++)
            T(i, j) = gsl_vector_get(x, 4*i + j);
        
        gsl_matrix_free(A);
        gsl_vector_free(b);
        gsl_vector_free(x);
        gsl_vector_free(w);
        
        /*
        foreach (const Mapping &m, map) {
            Vec4 q = T * m.p;
            MSG_INFO("q = %1, T * q = %2, error = %3", (m.q.toVec3().toQString())(q.toVec3().toQString())(dist(m.q.toVec3(), q.toVec3())));
            }
        */
        
        return T;
    #else
        Message::error("ProjBoxOpti::calcTransfo : requires GSL support");
        return ProjTransfo();
    #endif
    }

