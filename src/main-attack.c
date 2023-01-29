#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "common.h"
#include "pssi-attack.h"
#include "pssi-sample.h"
#include "pssi-lemma.h"

int main(int argc, char const *argv[])
{
    uint32_t index_E_guess = 0;
    uint32_t dim_inter_E_and_E_guess = 0;
    uint32_t curr_sample = 0;
    uint32_t nb_success = 0;
    uint32_t nb_samples_used = 0;
    random_source rand_ctx;
    rbc_elt e, e_prime;
    rbc_vspace E;
    rbc_vspace E_guess;
    rbc_vspace inter;
    rbc_vspace F[NB_SAMPLES];
    rbc_vspace Z[NB_SAMPLES];
    rbc_elt f[NB_SAMPLES];
    rbc_elt f_prime[NB_SAMPLES];
    clock_t start, end;
    double cpu_time_used = 0;

    random_init(&rand_ctx, RANDOM_SOURCE_PRNG);
    rbc_field_init();

    rbc_vspace_init(&E, PARAM_R);
    rbc_vspace_init(&E_guess, PARAM_R);
    rbc_vspace_init(&inter, PARAM_R);
    for(int i = 0; i < NB_SAMPLES ; i++) {
        rbc_vspace_init(&F[i], PARAM_D);
        rbc_vspace_init(&Z[i], PARAM_W+PARAM_R*PARAM_D-PARAM_LAMBDA);
    }

    for(int n = 0 ; n < NB_TEST ; n++) {

        index_E_guess = 0;
        dim_inter_E_and_E_guess = 0;
        curr_sample = 0;

        rbc_vspace_set_random_full_rank(&rand_ctx, E, PARAM_R);
        rbc_vspace_set_zero(E_guess, PARAM_R);
        for(int i = 0 ; i < NB_SAMPLES ; i++) {
            generate_pssi_sample(&rand_ctx, F[i], Z[i], E, PARAM_W, PARAM_R, PARAM_D, PARAM_LAMBDA);
        }

        start = clock();

        while(index_E_guess < PARAM_R && curr_sample < NB_SAMPLES) {

            sample_full_rank_pair_from_support(&rand_ctx, f[curr_sample], f_prime[curr_sample], F[curr_sample], PARAM_D);

            for(int j = 0 ; j < (1 << (6*PARAM_D-3)) ; j++) {
                for(int i = 1 ; i < 4 ; i++) {
                    sample_full_rank_pair_from_support(&rand_ctx, f[curr_sample+i], f_prime[curr_sample+i], F[curr_sample+i], PARAM_D);
                }

                int dim = build_spaces_and_intersect(E_guess[index_E_guess], &f[curr_sample], &f_prime[curr_sample], &Z[curr_sample], PARAM_W+PARAM_R*PARAM_D-PARAM_LAMBDA);
                if(dim > 0 && rbc_vec_get_rank_vartime(E_guess, PARAM_R) > index_E_guess) {
                    index_E_guess++;
                    rbc_vspace_intersection(inter, E, E_guess, PARAM_R, PARAM_R);
                    dim_inter_E_and_E_guess = rbc_vec_get_rank_vartime(inter, PARAM_R);
                    //printf("dim(E_guess) is now %d ; dim(E inter E_guess) is now %d\n", rank_E_guess, dim_inter_E_and_E_guess);
                    j = (1 << (6*PARAM_D-3));
                }
            }

            curr_sample += 4;
        }

        end = clock();
        cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;

        if(dim_inter_E_and_E_guess == PARAM_R) {
            nb_success++;
        } else if(index_E_guess == PARAM_R) {
            printf("E_guess is of rank %d but has intersection %d with E\n", index_E_guess, dim_inter_E_and_E_guess);
        } else if(curr_sample == NB_SAMPLES) {
            printf("All samples used\n");
        } else {
            printf("Unknown error\n");
        }
        nb_samples_used += curr_sample;
    }


    printf("\n==========  Results  ===========\n");
    printf("Average CPU time: %f sec\n", cpu_time_used / NB_TEST);
    printf("Average number of PSSI samples: %d\n", nb_samples_used / NB_TEST);
    printf("Number of successes:  %d/%d\n", nb_success, NB_TEST);

cleanup:

    for(int i = 0 ; i < NB_SAMPLES ; i++) {
        rbc_vspace_clear(F[i]);
        rbc_vspace_clear(Z[i]);
    }

    rbc_vspace_clear(E);
    rbc_vspace_clear(E_guess);
    rbc_vspace_clear(inter);

    random_clear(&rand_ctx);

    return 0;
}