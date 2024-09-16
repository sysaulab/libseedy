# Algorithms

Some descriptions of the PRNG algorithm distributed with libseedy.

## Seedy (SY)
- 64 bit (2M pool -> 128EiB period) 70,368,744,177,664x ( 2^46 )
- 32 bit (768K pool -> 16GiB period) 21,845x
- 16 bit (16K pool -> 128K period) 8x
- Slow algorithm ( speed dependends on timer's resolution )
- Chaotic generator, not a sequence

The chaotic generator is capable of providing original entropy reliably on any platform capable of concurrent multithreading. It creates 3 threads, each running a hash function. To better understand it's inner working I will explain the algorithm step by step as if it was performed by people in an office. The hashing functions will be represented by three accountants. The shared memory that connect the ring can be represented by three filing cabinets. 

- Accountant A (AA) uses read from cabinet 3 (C3) and write results in cabinet 1 (C1).
- Accountant B (AB) uses read from cabinet 1 (C1) and write results in cabinet 2 (C2).
- Accountant C (AC) uses read from cabinet 2 (C2) and write results in cabinet 3 (C3).

The job of AA is:

1. The accountant write the number 1 on a sheet on its desk.
2. Do the following 64 times:
    1. Look if the number in C3 is odd or even.
    2. According to step 1, pick the first number from the next pair on the list. If even use the first number, if odd use the second number.
    3. Multiply the number picked with the number on your sheet of paper.
    4. Go look again at the number in C3 and do a binary xor to combine it with the number currently of your sheet of paper.
    5. Take the number obtained at the last step and add it to the number in C1, replacing the number in C1.
3. Use the number on your sheet of paper and combining it with C1 using binary xor.

AB and AC are also doing the same thing with their own respective cabinets. The supervisor use an alarm to periodically go read the number in cabinets C1, C2 and C3, combine them using binary xor and add them to the list...

Each accountant's work in itself is very predictable. When three accountants do this continously, the number they generate will vary wildly according to the order each of them arrive to each cabinet first. When all accountants are not working at the same time, the numbers being calculated are not as chaotic. This is why I describe the behaviour in single core environments as pseudo chaotic. When that happens, the work of each accountant is predictable and only the moment at which they stop and start again contributes to the deviation from the expected norm.

While each process is deterministic in itself, the overall configuration makes it an unstable, divergent, complex system. This is what I call "synthetic chaos".

## Quad XOR (QX)

- **Not cryptographically safe.**
- 64 bit (2M pool -> 128EiB period) 70368744177664x
- 32 bit (768K pool -> 16GiB period) 22357x
- 16 bit (16K pool -> 128K period) 8x
- Fast algorithm (1.9GiB/s M1)
- Fill function drop unused bytes.[^1] 

"Accelerator" algorithm. It uses some data to generate more data. Useful to pair with slow generators. I created it to accelerate the testigng of ICM, now libseedy. It uses a "noise map" to combine the value of 4 different samples at each iteration to generate a large sequence from to the noise map provided. I think of this "generator" as an noise amplifier as the quality of the input will bi directly reflected in the output.

The noise map (pool) allow for direct indexing of the entire sequence (128EiB 64 bit, 16GiB 32 bit). This allow unlimited parallel processing. Unless you need to "amplify" a small sample or encode at TB/s speed with DSPs, use [shishua](shishua.md)

Two variations

I created a 32 bit version to offer the fastest algorithm on 32 bit platforms. The latest incarnation uses less operations and passes practrand, at first glance anyway and fed with arc4random_buf in macos 14.6.1. This combination allows for platforms with slow timers (anything but a recent modern desktops OS) to get numbers immediatly. This is done specifically to avoid the perception of stalling when ICM'S bandwidth is limited by coarse timers, such as Windows 95.

The 16 bit variation also passes practrand but the performance is not great and requires constant reseed. Also the 768kb state makes it too large for DOS. I do forsee see any practical application, but if anyone needs to "amplify data" in 16 bits, this can a 8x expander that passes practrand.

## Mersenne Twister (MT)

- **Not cryptographically safe.**
- 32 bit
- Excellent bit distribution quality
- Fast (1.0GiB/s M1).
- Website [github.io](https://github.com/ESultanik/mtwister).
- Fill function drop unused bytes.[^1] 

## Shishua (SS)

I am not intimate with the internals of this algorithm. It claims to be the fastest and I believe it. 

- **Not cryptographically safe.**
- 64 bit
- Fastest (2.7GiB/s on M1).
- Website [github.io](https://espadrine.github.io/blog/posts/shishua-the-fastest-prng-in-the-world.html).
- Fill function is native.

[^1]: Example: you ask for 3 bytes: 4 are generated, 3 are transmitted, 1 is dropped and will never be used.