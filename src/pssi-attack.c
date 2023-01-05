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
    rbc_83_elt_add(r, r, s);
    if(rbc_83_elt_is_zero(r)) {
        return -1;
    } else {
        rbc_83_elt_inv(r, r);
        return 0;   
    }
}

int build_spaces_and_intersect(const rbc_83_elt f[4], const rbc_83_elt f_prime[4], const rbc_83_vspace Z[4], uint32_t Z_size) {
    rbc_83_vspace A, A1, A2, B, tmp_space;
    rbc_83_vspace_init(&A, 2*Z_size);
    rbc_83_vspace_init(&A1, Z_size);
    rbc_83_vspace_init(&A2, Z_size);
    rbc_83_vspace_init(&B, 2*Z_size);
    rbc_83_vspace_init(&tmp_space, 2*Z_size);

    //printf("2*Z_size = %d", 2*Z_size);

    for(int i = 0 ; i < 3 ; i++) {
        for(int j = i+1 ; j < 4 ; j++) {
            rbc_83_elt inv_det, tmp;

            if(inv_determinant(inv_det, f[i], f_prime[i], f[j], f_prime[j]) == -1) {
                rbc_83_vspace_set_zero(A, 2*Z_size);
            } else {
                rbc_83_elt_mul(tmp, inv_det, f[j]);
                rbc_83_vec_scalar_mul(A1, Z[i], tmp, Z_size);

                rbc_83_elt_mul(tmp, inv_det, f[i]);
                rbc_83_vec_scalar_mul(A2, Z[j], tmp, Z_size);

                // A = inv_det*(f[i]*Z[j] + f[j]*Z[i])
                rbc_83_vspace_direct_sum(A, A1, A2, Z_size, Z_size);
            }

            //rbc_83_vspace_print(A, 2*Z_size);

            // If it is the first time in the loop, set B = A
            // else set B = A \inter B
            if(i == 0 && j == 1) {
                rbc_83_vspace_set(B, A, 2*Z_size);
            } else {
                rbc_83_vspace_intersection(tmp_space, B, A, 2*Z_size ,2*Z_size);
                rbc_83_vspace_set(B, tmp_space, 2*Z_size);
            }

            //rbc_83_vspace_print(B, 2*Z_size);
            //printf("\nDimension %d\n", (int)rbc_83_vec_get_rank(B, 2*Z_size));
        }
    }

    int res = rbc_83_vec_get_rank(B, 2*Z_size);

    //printf("OK %d\n", res);

    rbc_83_vspace_clear(A);
    rbc_83_vspace_clear(A1);
    rbc_83_vspace_clear(A2);
    rbc_83_vspace_clear(B);
    rbc_83_vspace_clear(tmp_space);

    return res;
}