#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "common.h"
#include "pssi-attack.h"
#include "pssi-sample.h"
#include "pssi-debug.h"
#include "pssi-lemma.h"

int main(int argc, char const *argv[])
{
    int nb_true = 0;

    random_source rand_ctx;
    random_init(&rand_ctx, RANDOM_SOURCE_PRNG);

    rbc_field_init();
    
    rbc_vspace E;
    rbc_vspace F[4];
    rbc_vspace Z[4];
    rbc_elt f[4];
    rbc_elt f_prime[4];

    rbc_vspace_init(&E, PARAM_R);
    rbc_vspace_set_random_full_rank(&rand_ctx, E, PARAM_R);

    for(int i = 0; i < 4 ; i++) {
        rbc_vspace_init(&F[i], PARAM_D);
        rbc_vspace_init(&Z[i], PARAM_W+PARAM_R*PARAM_D-PARAM_LAMBDA);
    }

    //sample_full_rank_pair_from_support(&rand_ctx, e, e_prime, E, PARAM_R);

    /******************************************************/

    for(int n = 0 ; n < NB_TEST ; n++) {
        for(int i = 0 ; i < 4 ; i++) {
            generate_pssi_sample(&rand_ctx, F[i], Z[i], E, PARAM_W, PARAM_R, PARAM_D, PARAM_LAMBDA);
            sample_full_rank_pair_from_support(&rand_ctx, f[i], f_prime[i], F[i], PARAM_D);
            //generate_pssi_sample_debug(&rand_ctx, e, e_prime, f[i], f_prime[i], F[i], Z[i], E, PARAM_W, PARAM_R, PARAM_D, PARAM_LAMBDA);
        }

        nb_true += check_lemma_5(f, f_prime, E, PARAM_R, Z, PARAM_W+PARAM_R*PARAM_D-PARAM_LAMBDA);
    }

    printf("\n==========  Results  ===========\n");
    printf("Number of tests      : %d\n", NB_TEST);
    printf("Empirical probability: %lf %%\n", (double)(nb_true*100)/(double)NB_TEST);
    printf("Theorical probability: %lf %%\n", (double)100/(double)(1 << (6*PARAM_R)));

cleanup:

    for(int i = 0 ; i < 4 ; i++) {
        rbc_vspace_clear(F[i]);
        rbc_vspace_clear(Z[i]);
    }

    rbc_vspace_clear(E);

    random_clear(&rand_ctx);

    return 0;
}