#include <rbc.h>

int inv_determinant(rbc_83_elt r, rbc_83_elt a, rbc_83_elt b, rbc_83_elt c, rbc_83_elt d);
void sample_full_rank_pair_from_support(random_source *rand_ctx, rbc_83_elt o1, rbc_83_elt o2, const rbc_83_vspace F, uint32_t size);
int build_spaces_and_intersect(rbc_83_elt o, rbc_83_elt f[4], rbc_83_elt f_prime[4], rbc_83_vspace Z[4], uint32_t Z_size);