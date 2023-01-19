#include <rbc.h>

/* Computes the inverse of the determinant of the 2x2 matrix (a b)
                                                             (c d)
   r = (a*d+b*c)^(-1)
   Returns -1 if the determinant is 0 and the inverse could not be performed
            0 otherwise
*/
int inv_determinant(rbc_83_elt r, const rbc_83_elt a, const rbc_83_elt b, const rbc_83_elt c, const rbc_83_elt d) {
    rbc_83_elt s;
    rbc_83_elt_mul(r, a, d);
    rbc_83_elt_mul(s, b, c);
    rbc_83_elt_add(s, r, s);
    if(rbc_83_elt_is_zero(s)) {
        return -1;
    } else {
        rbc_83_elt_inv(r, s);
        return 0;   
    }
}

void sample_full_rank_pair_from_support(random_source *rand_ctx, rbc_83_elt o1, rbc_83_elt o2, const rbc_83_vspace F, uint32_t size) {
    rbc_83_vspace tmp;
    rbc_83_vspace_init(&tmp, 2);

    int rank = 0;
    while(rank < 2) {
        rbc_83_vec_set_random_from_support(rand_ctx, tmp, 2, F, size, 0);
        rank = rbc_83_vec_get_rank(tmp, 2);
    }
    rbc_83_elt_set(o1, tmp[0]);
    rbc_83_elt_set(o2, tmp[1]);

    rbc_83_vspace_clear(tmp);
}

int build_spaces_and_intersect(rbc_83_elt o, const rbc_83_elt f[4], const rbc_83_elt f_prime[4], const rbc_83_vspace Z[4], uint32_t Z_size) {
    int res = 0;

    rbc_83_vspace A, A1, A2, B, tmp_space;
    rbc_83_vspace_init(&A, 2*Z_size);
    rbc_83_vspace_init(&A1, Z_size);
    rbc_83_vspace_init(&A2, Z_size);
    rbc_83_vspace_init(&B, 2*Z_size);
    rbc_83_vspace_init(&tmp_space, 2*Z_size);

    rbc_83_elt_set_zero(o);
    //printf("2*Z_size = %d", 2*Z_size);

    for(int i = 0 ; i < 3 ; i++) {
        for(int j = i+1 ; j < 4 ; j++) {
            rbc_83_elt inv_det, tmp;

            // rbc_83_elt x, y;
            // rbc_83_elt_mul(x, e, f[i]);
            // rbc_83_elt_mul(y, e_prime, f_prime[i]);
            // rbc_83_elt_add(x, x, y);

            //int dim2 = rbc_83_vspace_intersection(tmp_space, Z[i], &x, Z_size, 1);
            //printf("ef[i]+e'f'[i] belongs to Z[i]: %d\n", dim2);

            if(inv_determinant(inv_det, f[i], f_prime[i], f[j], f_prime[j]) == -1) {
                res = 0;
                goto cleanup;
            } else {
                rbc_83_elt_mul(tmp, inv_det, f[j]);
                rbc_83_vec_scalar_mul(A1, Z[i], tmp, Z_size);

                rbc_83_elt_mul(tmp, inv_det, f[i]);
                rbc_83_vec_scalar_mul(A2, Z[j], tmp, Z_size);

                // A = inv_det*(f[i]*Z[j] + f[j]*Z[i])
                rbc_83_vspace_direct_sum(A, A1, A2, Z_size, Z_size);

                //int dim = rbc_83_vspace_intersection(tmp_space, A, (rbc_83_vspace)&e_prime, 2*Z_size, 1);
                //printf("e_prime belongs to A: %d\n", dim);
            }

            //rbc_83_vspace_print(A, 2*Z_size);

            // If it is the first time in the loop, set B = A
            // else set B = A \inter B
            if(i == 0 && j == 1) {
                rbc_83_vspace_set(B, A, 2*Z_size);
            } else {
                rbc_83_vspace_intersection(tmp_space, B, A, 2*Z_size ,2*Z_size);
                rbc_83_vspace_set(B, tmp_space, 2*Z_size);
                res = (int)rbc_83_vec_get_rank(B, 2*Z_size);
                if(res == 0) {
                    goto cleanup;
                }
            }

            // rbc_83_vspace_print(B, 2*Z_size);
            // printf("\nDimension %d\n", (int)rbc_83_vec_get_rank(B, 2*Z_size));
            // printf("res = %d\n", res);
        }
    }

    if(res == 1) {
        rbc_83_vec_echelonize(B, 2*Z_size);
        rbc_83_elt_set(o, B[0]);
    } else {
        res = 0;
    }

    //printf("OK %d\n", res);
cleanup:
    rbc_83_vspace_clear(A);
    rbc_83_vspace_clear(A1);
    rbc_83_vspace_clear(A2);
    rbc_83_vspace_clear(B);
    rbc_83_vspace_clear(tmp_space);

    return res;
}