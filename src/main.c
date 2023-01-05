#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "pssi-attack.h"
#include "pssi-sample.h"

#define M 83
#define R 3
#define D 3
#define LAMBDA 6
#define W 12

int main(int argc, char const *argv[])
{
    random_source rand_ctx;
    random_init(&rand_ctx, RANDOM_SOURCE_PRNG);

    rbc_83_field_init();
    
    rbc_83_vspace E;
    rbc_83_vspace F[4];
    rbc_83_vspace Z[4];
    rbc_83_elt f[4];
    rbc_83_elt f_prime[4];

    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    rbc_83_vspace_init(&E, R);
    rbc_83_vspace_set_random_full_rank(&rand_ctx, E, R);

    for(int i = 0; i < 4 ; i++) {
        rbc_83_vspace_init(&F[i], D);
        rbc_83_vspace_init(&Z[i], W+R*D-LAMBDA);
        printf("Generating PSSI sample...\n");
        generate_pssi_sample(&rand_ctx, F[i], Z[i], E, W, R, D, LAMBDA);
        printf("Generated PSSI sample...\n");
        rbc_83_elt_set_random(&rand_ctx, f[i]);
        rbc_83_elt_set_random(&rand_ctx, f_prime[i]);
    }

    rbc_83_vec_set_random_from_support(&rand_ctx, &f[1], 1, F[1], D, 0);
    rbc_83_vec_set_random_from_support(&rand_ctx, &f_prime[1], 1, F[1], D, 0);

    for(int j = 0 ; j < 10 ; j++) {
        for(int i = 1 ; i < 4 ; i++) {
            rbc_83_vec_set_random_from_support(&rand_ctx, &f[i], 1, F[1], D, 0);
            rbc_83_vec_set_random_from_support(&rand_ctx, &f_prime[i], 1, F[1], D, 0);
        }

        printf("Trying attack...\n");

        int dim = build_spaces_and_intersect(f, f_prime, Z, W+R*D-LAMBDA);
        if(dim > 0) {
            printf("Found dimension: %d\n", dim);
        }
    }

    for(int i = 0 ; i < 4 ; i++) {
        rbc_83_vspace_clear(F[i]);
        rbc_83_vspace_clear(Z[i]);
    }

    rbc_83_vspace_clear(E);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("CPU time: %f sec\n", cpu_time_used);

    random_clear(&rand_ctx);

    return 0;
}