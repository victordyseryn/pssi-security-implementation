#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "common.h"
#include "pssi-attack.h"
#include "pssi-sample.h"
#include "pssi-debug.h"
#include "pssi-lemma.h"

#define M 83
#define R 3
#define D 3
#define LAMBDA 6
#define W 10

int main(int argc, char const *argv[])
{
    random_source rand_ctx;
    random_init(&rand_ctx, RANDOM_SOURCE_PRNG);

    rbc_field_init();
    
    rbc_elt e, e_prime;
    rbc_vspace E;
    rbc_vspace E_guess;
    rbc_vspace inter;
    rbc_vspace F[4];
    rbc_vspace Z[4];
    rbc_elt f[4];
    rbc_elt f_prime[4];

    uint32_t rank_E_guess = 0;
    uint32_t index_E_guess = 0;
    uint32_t dim_inter_E_and_E_guess = 0;

    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    rbc_vspace_init(&E, R);
    rbc_vspace_init(&E_guess, 2*R);
    rbc_vspace_init(&inter, 2*R);
    rbc_vspace_set_random_full_rank(&rand_ctx, E, R);
    rbc_vspace_set_zero(E_guess, 2*R);

    for(int i = 0; i < 4 ; i++) {
        rbc_vspace_init(&F[i], D);
        rbc_vspace_init(&Z[i], W+R*D-LAMBDA);
    }

    //sample_full_rank_pair_from_support(&rand_ctx, e, e_prime, E, R);

    /******************************************************

    for(int n = 0 ; n < 5000 ; n++) {
        for(int i = 0 ; i < 4 ; i++) {
            generate_pssi_sample(&rand_ctx, F[i], Z[i], E, W, R, D, LAMBDA);
            sample_full_rank_pair_from_support(&rand_ctx, f[i], f_prime[i], F[i], D);
        }

        check_lemma_5(f, f_prime, E, R, Z, W+R*D-LAMBDA);
    }
    
    // printf("%d\n", check_lemma_5(f, f_prime, E, R, Z, W+R*D-LAMBDA));

    goto cleanup;

    /*****************************************************/

    while(rank_E_guess < R && index_E_guess < 2*R) {

        printf("Generating new PSSI samples...\n");

        for(int i = 0; i < 4 ; i++) {
            generate_pssi_sample(&rand_ctx, F[i], Z[i], E, W, R, D, LAMBDA);
            //generate_pssi_sample_debug(&rand_ctx, e, e_prime, f[i], f_prime[i], F[i], Z[i], E, W, R, D, LAMBDA);
        }

        sample_full_rank_pair_from_support(&rand_ctx, f[0], f_prime[0], F[0], D);

        for(int j = 0 ; j < (1 << (6*D-3)) ; j++) {
            if(j % 10000 == 0) {
                //printf("Attempt %d...\n", j);
            }
            for(int i = 1 ; i < 4 ; i++) {
                sample_full_rank_pair_from_support(&rand_ctx, f[i], f_prime[i], F[i], D);
            }

            //printf("Trying attack...\n");

            int dim = build_spaces_and_intersect(E_guess[index_E_guess], f, f_prime, Z, W+R*D-LAMBDA);
            if(dim > 0) {
                index_E_guess++;
                rank_E_guess = rbc_vec_get_rank_vartime(E_guess, 2*R);
                rbc_vspace_intersection_vartime(inter, E, E_guess, R, 2*R);
                dim_inter_E_and_E_guess = rbc_vec_get_rank_vartime(inter, 2*R);
                printf("dim(E_guess) is now %d ; dim(E inter E_guess) is now %d\n", rank_E_guess, dim_inter_E_and_E_guess);
                j = (1 << (6*D-3));
            }
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\n==========  Results  ===========\n");
    printf("CPU time: %f sec\n", cpu_time_used);
    printf("dim(E) = %d\n", R);
    printf("dim(E_guess) = %d\n", rank_E_guess);
    printf("dim(E inter E_guess) = %d\n", dim_inter_E_and_E_guess);
    if(dim_inter_E_and_E_guess == R) {
        printf("The attack is a SUCCESS\n");
    } else {
        printf("The attack is a FAILURE\n");
    }

cleanup:

    for(int i = 0 ; i < 4 ; i++) {
        rbc_vspace_clear(F[i]);
        rbc_vspace_clear(Z[i]);
    }

    rbc_vspace_clear(E);
    rbc_vspace_clear(E_guess);
    rbc_vspace_clear(inter);

    random_clear(&rand_ctx);

    return 0;
}