#include <rbc.h>
#include "common.h"

//Samples U from EF.
void sampleU(random_source *rand_ctx, rbc_vspace U, rbc_vspace EF, rbc_vspace E, rbc_vspace F,
             uint32_t r, uint32_t d, uint32_t lambda) {
  int filtering;
  rbc_vspace eiF, inter;
  rbc_vspace_init(&eiF, d);
  rbc_vspace_init(&inter, r*d);

  do {
    do {
      rbc_vec_set_random_from_support(rand_ctx, U, r*d-lambda, EF, r*d, 0);
    } while(rbc_vec_get_rank_vartime(U, r*d-lambda) != r*d-lambda);

    filtering = 1;

    //For every element e_i of E, check if U \cap e_iF = 0
    for(int i=0 ; i<(1 << r) ; i++) {
      rbc_elt tmp;
      rbc_elt_set_zero(tmp);

      for(int j=0 ; j<r ; j++) {
        if(i & (1 << j)) rbc_elt_add(tmp, tmp, E[j]);
      }

      //Compute e_i.F
      rbc_vec_scalar_mul(eiF, F, tmp, d);

      //Check intersection
      rbc_vspace_intersection(inter, U, eiF, r*d-lambda, d);
      int dim = rbc_vec_get_rank_vartime(inter, r*d-lambda);

      if(dim != 0) {
        filtering = 0;
        break;
      }
    }
  } while(filtering == 0);

  rbc_vspace_clear(eiF);
  rbc_vspace_clear(inter);
}

void generate_pssi_sample(random_source *rand_ctx, rbc_vspace F, rbc_vspace Z, rbc_vspace E,
                          uint32_t w, uint32_t r, uint32_t d, uint32_t lambda) {
    rbc_vspace W, EF, U;
    rbc_vspace_init(&W, w);
    rbc_vspace_init(&EF, r*d);
    rbc_vspace_init(&U, r*d-lambda);

    do {
        rbc_vspace_set_random_full_rank(rand_ctx, F, d);
        rbc_vspace_set_random_full_rank(rand_ctx, W, w);
        rbc_vspace_product(EF, E, F, r, d);
        sampleU(rand_ctx, U, EF, E, F, r, d, lambda);
        rbc_vspace_direct_sum(Z, W, U, w, r*d-lambda);
    } while(rbc_vec_get_rank_vartime(Z, w+r*d-lambda) != w+r*d-lambda);

    rbc_vspace_clear(W);
    rbc_vspace_clear(EF);
    rbc_vspace_clear(U);
}