#include <rbc.h>
#include "pssi-attack.h"

void generate_pssi_sample_debug(random_source *rand_ctx, rbc_83_elt e, rbc_83_elt e_prime, rbc_83_elt f, rbc_83_elt f_prime, 
                                rbc_83_vspace F, rbc_83_vspace Z, rbc_83_vspace E,
                                uint32_t w, uint32_t r, uint32_t d, uint32_t lambda) {
    rbc_83_elt x, y;
    rbc_83_vspace W, EF, preU, U;
    rbc_83_vspace_init(&W, w);
    rbc_83_vspace_init(&EF, r*d);
    rbc_83_vspace_init(&preU, r*d-lambda-1);
    rbc_83_vspace_init(&U, r*d-lambda);

    do {
        rbc_83_vspace_set_random_full_rank(rand_ctx, F, d);
        rbc_83_vspace_set_random_full_rank(rand_ctx, W, w);
        rbc_83_vspace_product(EF, E, F, r, d);
        do {
          rbc_83_vec_set_random_from_support(rand_ctx, preU, r*d-lambda-1, EF, r*d, 0);
        } while(rbc_83_vec_get_rank(preU, r*d-lambda-1) != r*d-lambda-1);

        sample_full_rank_pair_from_support(rand_ctx, f, f_prime, F, d);
        rbc_83_elt_mul(x, e, f);
        rbc_83_elt_mul(y, e_prime, f_prime);
        rbc_83_elt_add(x, x, y);

        rbc_83_vspace_direct_sum(U, preU, &x, r*d-lambda-1, 1);
        rbc_83_vspace_direct_sum(Z, W, U, w, r*d-lambda);
    } while(rbc_83_vec_get_rank(Z, w+r*d-lambda) != w+r*d-lambda);

    rbc_83_vspace_clear(W);
    rbc_83_vspace_clear(EF);
    rbc_83_vspace_clear(U);
}