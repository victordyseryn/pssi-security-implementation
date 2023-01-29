#include <rbc.h>

int inv_determinant(rbc_elt r, rbc_elt a, rbc_elt b, rbc_elt c, rbc_elt d);
void sample_full_rank_pair_from_support(random_source *rand_ctx, rbc_elt o1, rbc_elt o2, const rbc_vspace F, uint32_t size);
int build_spaces_and_intersect(rbc_elt o, rbc_elt f[4], rbc_elt f_prime[4], rbc_vspace Z[4], uint32_t Z_size);