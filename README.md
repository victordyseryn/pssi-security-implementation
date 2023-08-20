# Context

This repository is an implementation of the attack presented in [[1]](https://eprint.iacr.org/2023/926) against the post-quantum signature scheme Durandal [[2]](https://eprint.iacr.org/2018/1192).

# Build

For building the attack, run one out of the three

    make a2
    make a3
    make a4

depending on which experiment you want to run. Experiment A2 has the smallest parameters, A4 the largest. See [[1, Section 7]](https://eprint.iacr.org/2023/926) for details about the parameters for the experiments.

For building the Lemma 5 checking, run one out of the two

    make b1
    make b2

# Run

    ./build/main



