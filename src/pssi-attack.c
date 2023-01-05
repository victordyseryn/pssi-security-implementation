#include <rbc.h>

void inv_determinant(rbc_83_elt r, const rbc_83_elt a, const rbc_83_elt b, const rbc_83_elt c, const rbc_83_elt d) {
    rbc_83_elt s;
    rbc_83_elt_mul(r, a, b);
    rbc_83_elt_mul(s, c, d);
    rbc_83_elt_add(r, r, s);
    rbc_83_elt_inv(r, r);
}

int build_spaces_and_intersect(const rbc_83_elt f[4], const rbc_83_elt f_prime[4], const rbc_83_vspace Z[4], uint32_t Z_size) {
    rbc_83_vspace A, A1, A2, B, tmp_space;
    rbc_83_vspace_init(&A, 2*Z_size);
    rbc_83_vspace_init(&A1, Z_size);
    rbc_83_vspace_init(&A2, Z_size);
    rbc_83_vspace_init(&B, 2*Z_size);
    rbc_83_vspace_init(&tmp_space, 2*Z_size);
    //rbc_83_vspace_set_zero(B, 2*Z_size);
    //rbc_83_vspace_set_zero(tmp_space, 2*Z_size);

    //printf("2*Z_size = %d", 2*Z_size);
    //rbc_83_vspace_print(B, 2*Z_size);

    for(int i = 0 ; i < 3 ; i++) {
        for(int j = i+1 ; j < 4 ; j++) {
            rbc_83_elt det, tmp;

            inv_determinant(det, f[i], f[j], f_prime[i], f_prime[j]);

            rbc_83_elt_mul(tmp, det, f[j]);
            rbc_83_vec_scalar_mul(A1, Z[i], tmp, Z_size);

            rbc_83_elt_mul(tmp, det, f[i]);
            rbc_83_vec_scalar_mul(A2, Z[j], tmp, Z_size);

            rbc_83_vspace_direct_sum(A, A1, A2, Z_size, Z_size);

            //rbc_83_vspace_print(A, 2*Z_size);

            if(i == 0 && j == 1) {
                rbc_83_vspace_set(B, A, 2*Z_size);
            } else {
                printf("%d\n", rbc_83_vspace_intersection(tmp_space, B, A, 2*Z_size ,2*Z_size));
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