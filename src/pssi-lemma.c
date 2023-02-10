#include "common.h"
#include "pssi-lemma.h"

int belongs_to(const rbc_elt x, const rbc_vspace A, const uint32_t A_size) {
    rbc_vspace tmp;
    rbc_vspace_init(&tmp, A_size+1);

    rbc_elt_set(tmp[0], x);
    rbc_vspace_set(&tmp[1], A, A_size);

    int res = rbc_vec_get_rank_vartime(tmp, A_size+1);

    rbc_vspace_clear(tmp);

    return (res == A_size);
}

/*

int check_lemma_5(const rbc_elt f[3], const rbc_elt f_prime[3], 
                  const rbc_vspace E, uint32_t E_size, const rbc_vspace Z[3], uint32_t Z_size) {

    uint8_t lambda = 1;
    uint8_t lambda_prime = 1;

    rbc_elt e, e_prime, x[3], y, z;

    while(lambda < (1 << E_size)) {

        rbc_elt_set_zero(e);
        for(int i = 0 ; i < E_size ; i++) {
            if((lambda >> i) & 1) rbc_elt_add(e, e, E[i]);
        }
        for(int j = 0 ; j < 3 ; j++) {
            rbc_elt_mul(x[j], e, f[j]);
        }

        lambda_prime = 1;
        while(lambda_prime < (1 << E_size)) {
            
            rbc_elt_set_zero(e_prime);
            for(int i = 0 ; i < E_size ; i++) {
                if((lambda_prime >> i) & 1) rbc_elt_add(e_prime, e_prime, E[i]);
            }


            if(!rbc_elt_is_equal_to(e, e_prime)) {
                int j = 0;
                for(j = 0 ; j < 3 ; j++) {
                    rbc_elt_mul(y, e_prime, f_prime[j]);
                    rbc_elt_add(z, x[j], y);

                    if(!belongs_to(z, Z[j], Z_size)) break;
                }
                if(j == 3) {
                    return 1;
                }
            }

            // printf("%d,%d -- ", lambda, lambda_prime);
            // for(int i = 0 ; i < E_size ; i++) {
            //     printf("%d,", (lambda >> i) & 1);
            //     printf("%d", (lambda_prime >> i) & 1);
            //     printf(" ");
            // }
            // printf("\n");
            lambda_prime++;
        }
  
        lambda++;
    }

    return 0;

}

/*///*


int check_lemma_5(const rbc_elt f[4], const rbc_elt f_prime[4], 
                  const rbc_vspace E, uint32_t E_size, const rbc_vspace Z[4], uint32_t Z_size) {

    uint8_t lambda = 1;
    uint8_t lambda_prime = 1;

    rbc_elt e, e_prime, x[4], y, z;

    while(lambda < (1 << E_size)) {

        rbc_elt_set_zero(e);
        for(int i = 0 ; i < E_size ; i++) {
            if((lambda >> i) & 1) rbc_elt_add(e, e, E[i]);
        }
        for(int j = 0 ; j < 4 ; j++) {
            rbc_elt_mul(x[j], e, f[j]);
        }

        lambda_prime = 1;
        while(lambda_prime < (1 << E_size)) {
            
            rbc_elt_set_zero(e_prime);
            for(int i = 0 ; i < E_size ; i++) {
                if((lambda_prime >> i) & 1) rbc_elt_add(e_prime, e_prime, E[i]);
            }


            if(!rbc_elt_is_equal_to(e, e_prime)) {
                int j = 0;
                for(j = 0 ; j < 4 ; j++) {
                    rbc_elt_mul(y, e_prime, f_prime[j]);
                    rbc_elt_add(z, x[j], y);

                    if(!belongs_to(z, Z[j], Z_size)) break;
                }
                if(j == 4) {
                    return 1;
                }
            }

            // printf("%d,%d -- ", lambda, lambda_prime);
            // for(int i = 0 ; i < E_size ; i++) {
            //     printf("%d,", (lambda >> i) & 1);
            //     printf("%d", (lambda_prime >> i) & 1);
            //     printf(" ");
            // }
            // printf("\n");
            lambda_prime++;
        }
  
        lambda++;
    }

    return 0;

}

//*/