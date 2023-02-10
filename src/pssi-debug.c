#include <rbc.h>
#include "common.h"
#include "pssi-attack.h"

void generate_pssi_sample_debug(random_source *rand_ctx, rbc_elt e, rbc_elt e_prime, rbc_elt f, rbc_elt f_prime, 
                                rbc_vspace F, rbc_vspace Z, rbc_vspace E,
                                uint32_t w, uint32_t r, uint32_t d, uint32_t lambda) {
    rbc_elt x, y;
    rbc_vspace W, EF, preU, U;
    rbc_vspace_init(&W, w);
    rbc_vspace_init(&EF, r*d);
    rbc_vspace_init(&preU, r*d-lambda-1);
    rbc_vspace_init(&U, r*d-lambda);

    do {
        rbc_vspace_set_random_full_rank(rand_ctx, F, d);
        rbc_vspace_set_random_full_rank(rand_ctx, W, w);
        rbc_vspace_product(EF, E, F, r, d);
        do {
          rbc_vec_set_random_from_support(rand_ctx, preU, r*d-lambda-1, EF, r*d, 0);
        } while(rbc_vec_get_rank_vartime(preU, r*d-lambda-1) != r*d-lambda-1);

        sample_full_rank_pair_from_support(rand_ctx, f, f_prime, F, d);
        rbc_elt_mul(x, e, f);
        rbc_elt_mul(y, e_prime, f_prime);
        rbc_elt_add(x, x, y);

        rbc_vspace_direct_sum(U, preU, &x, r*d-lambda-1, 1);
        rbc_vspace_direct_sum(Z, W, U, w, r*d-lambda);
    } while(rbc_vec_get_rank_vartime(Z, w+r*d-lambda) != w+r*d-lambda);

    rbc_vspace_clear(W);
    rbc_vspace_clear(EF);
    rbc_vspace_clear(U);
}