# Algorithms

Some descriptions of the PRNG algorithms I distribute with libseedy.

## Quad XOR (QX)

- **Not cryptographically safe.**
- 64 bit
- 32 bit
- 16 bit
- Fast algorithm (1.9GiB/s M1)

Algorithm I created to accelerate the testigng of ICM, now libseedy. It uses a "noise map" to combine the value of 4 different samples at each iteration to generate a large sequence from to the noise map provided. I think of this "generator" as an noise amplifier as the quality of the input will bi directly reflected in the output.

The noise map allow for direct indexing of the entire sequence (128EiB 64 bit, 16GiB 32 bit). This allow unlimited parallel processing.

Unless you need to "amplify" a small sample or encode at TB/s speed with DSPs, use [shishua](shishua.md)

## Mersenne Twister (MT)

- **Not cryptographically safe.**
- 32 bit
- Excellent bit distribution quality
- Fast(1.0GiB/s M1).
- Website [github.io](https://github.com/ESultanik/mtwister).

## Shishua (SS)

- **Not cryptographically safe.**
- 64 bit
- Fastest (2.7GiB/s M1).
- Website [github.io](https://espadrine.github.io/blog/posts/shishua-the-fastest-prng-in-the-world.html).

