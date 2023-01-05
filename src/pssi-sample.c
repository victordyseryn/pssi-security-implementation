#include <rbc.h>

//Samples U from EF.
void sampleU(random_source *rand_ctx, rbc_83_vspace U, rbc_83_vspace EF, rbc_83_vspace E, rbc_83_vspace F,
             uint32_t r, uint32_t d, uint32_t lambda) {
  int filtering;
  rbc_83_vspace eiF, inter;
  rbc_83_vspace_init(&eiF, d);
  rbc_83_vspace_init(&inter, r*d-lambda);

  do {
    do {
      rbc_83_vec_set_random_from_support(rand_ctx, U, r*d-lambda, EF, r*d, 0);
    } while(rbc_83_vec_get_rank(U, r*d-lambda) != r*d-lambda);

    filtering = 1;

    //For every element e_i of E, check if U \cap e_iF = 0
    for(int i=0 ; i<(1 << r) ; i++) {
      rbc_83_elt tmp;
      rbc_83_elt_set_zero(tmp);

      for(int j=0 ; j<r ; j++) {
        if(i & (1 << j)) rbc_83_elt_add(tmp, tmp, E[j]);
      }

      //Compute e_i.F
      rbc_83_vec_scalar_mul(eiF, F, tmp, d);

      //Check intersection
      rbc_83_vspace_intersection(inter, U, eiF, r*d-lambda, d);
      int dim = rbc_83_vec_get_rank(inter, r*d-lambda);

      if(dim != 0) {
        filtering = 0;
        break;
      }
    }
  } while(filtering == 0);

  rbc_83_vspace_clear(eiF);
  rbc_83_vspace_clear(inter);
}

void generate_pssi_sample(random_source *rand_ctx, rbc_83_vspace F, rbc_83_vspace Z, rbc_83_vspace E,
                          uint32_t w, uint32_t r, uint32_t d, uint32_t lambda) {
    rbc_83_vspace W, EF, U;
    rbc_83_vspace_init(&W, w);
    rbc_83_vspace_init(&EF, r*d);
    rbc_83_vspace_init(&U, r*d-lambda);

    do {
        rbc_83_vspace_set_random_full_rank(rand_ctx, F, d);
        rbc_83_vspace_set_random_full_rank(rand_ctx, W, w);
        rbc_83_vspace_product(EF, E, F, r, d);
        sampleU(rand_ctx, U, EF, E, F, r, d, lambda);
        rbc_83_vspace_direct_sum(Z, W, U, w, r*d-lambda);
    } while(rbc_83_vec_get_rank(Z, w+r*d-lambda) != w+r*d-lambda);

    rbc_83_vspace_clear(W);
    rbc_83_vspace_clear(EF);
    rbc_83_vspace_clear(U);
}