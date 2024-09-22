# Algorithms

Many algorithms are provided with the tools. The tools and prng's are not officially part of the package and are provided for your convenience only. The only alterations I make to the third party libraries consist to remove the <stdint.h> and move all declarations to the beginning of code blocks when old compilers complains. 

## Noise Map (NM64)

Creates a virtual one-time-pad directly accessible as a file, 16 exbibytes. For the seedyfs implementation. Holds an individual file. Based on QX.

## Noise Map (NM80)

Creates 65536 virtual one-time-pads, each directly accessible as a file, 1 yobibyte long. For the seedyfs implementation A folder will hold 65536 files, one for each individual bank of the static map.

## Quad XOR (QX)

- **NOT** cryptographically safe.
- 64 bit (2M pool -> 128EiB period) 70368744177664x
- 32 bit (768K pool -> 16GiB period) 22357x
- 16 bit (16K pool -> 128K period) 8x
- Fast algorithm (1.9GiB/s M1)
- Fill function drop unused bytes.[^1] 

"Accelerator" algorithm. It uses some data to generate more data. Useful to pair with slow generators. I created it to accelerate the testigng of ICM, now libseedy. It uses a "noise map" to combine the value of 4 different samples at each iteration to generate a large sequence from to the noise map provided. I think of this "generator" as an noise amplifier as the quality of the input will bi directly reflected in the output.

The noise map (pool) allow for direct indexing of the entire sequence (128EiB 64 bit, 16GiB 32 bit). This allow unlimited parallel processing. I have not experimented with this aspect of this otherwise extremely simple algorithm. Unless you need to "amplify" a small sample or encode at TB/s speed with DSPs, use [shishua](shishua.md)

### 32 bit variation - A good addition

I created a 32 bit version to offer the fastest algorithm on 32 bit platforms. The latest incarnation uses less operations and passes practrand, at first glance anyway and fed with arc4random_buf in macos 14.6.1. This combination allows for platforms with slow timers (anything but a recent modern desktops OS) to get numbers immediatly. This is done specifically to avoid the perception of stalling when ICM'S bandwidth is limited by coarse timers, such as Windows 95.

### 16 bit variation - A useless curiosity

The 16 bit variation also passes practrand but the performance is not great and requires constant reseed. Also the 768kb state makes it too large for DOS. I do forsee see any practical application, but if anyone needs to "amplify data" in 16 bits, this 8x expander  pass practrand.

## Mersenne Twister (MT) - Historical algorithm

I attempted a simple 16 port of mersenne twister to see if it was possible and went nowhere after an initial port. I removed it from the codebase as it was failing every tests. This is a reference implementation.

Original package here : [mtwister/](mtwister/)

- **NOT** cryptographically safe.
- 32 bit
- Excellent bit distribution quality
- Website [github.io](https://github.com/ESultanik/mtwister).
- Fill function drop unused bytes.[^1] 

## ChaCha-8 (CC8)

ChaCha-8 reference implementation.

- **NOT** cryptographically safe.
- 32 bit
- Website [https://cr.yp.to/](https://cr.yp.to/).
- Fill function maintains the sequence. 

## ChaCha-20 (CC20)

ChaCha-20 reference implementation.

- Cryptographically safe.
- 32 bit
- Website [https://cr.yp.to/](https://cr.yp.to/).
- Fill function maintains the sequence. 

## Shishua (SS)

I am not intimate with the internals of this algorithm.

Original package here : [shishua/](shishua/)

- **NOT** cryptographically safe.
- 64 bit
- Fastest.
- Website [github.io](https://espadrine.github.io/blog/posts/shishua-the-fastest-prng-in-the-world.html).
- Fill function is native.

## Prime

Lightweight library that returns the next prime number equal or greater than the one provided. It is not especially fast, but I do not need more than one every few years, in whoch case the libraries providing vast swath of them at a time to maximize throughput are not ideal for my planned usage. The algorithm was inspired from an example in python I saw on the Internet.

[^1]: Example: you ask for 3 bytes: 4 are generated, 3 are transmitted, 1 is dropped and will never be used.