# Tools

## Generic generators

Generator commands implement a single algorithm seeded internally with seedy or from an outside source. I created them as debugging, quality control and testing tools. All commands uses the seedy internal seed generator by default. You can override the default by passing the name of the source as an argument like so:

- genqxo64 stdin < /dev/urandom
- genqxo64 

stdin: reads from the standard input.

default: self-seeded using libseedy.

**genmt16:** Mersenne Twister - 16 bit - removed (it was no good)

**genmt32:** Mersenne Twister - 32 bit - reference

**genqxo16** Quad XOR - 16 bit - experimental

**genqxo32** Quad XOR - 32 bit - experimental

**genqxo64** Quad XOR - 64 bit - texted extensively

**genss64** Shishua - 64 bit - reference

**gensy16** seedy16() raw output - 16 bit - tested

**gensy32** seedy32() raw output - 32 bit - tested

**gensy64** seedy64() raw output - 64 bit - tested extensively

## Prime finder

Returns the next prime number equal or higher than: nprime64 10 -> 11. Included to test a basic function used to get the next prime number. In case I want to create a generator that iterates through different a different prime instead of reseeding data. Maybe using the iterator gap as a nonce when using QXO to encrypt communications.

**nprime16:** 16 bit variant 

**nprime32:** 32 bit variant 

**nprime64:** 64 bit variant 

## Fixed solutions

Seeding > PRNGs > User

**sycc2032** seedy > CC2032

**syccqx32** seedy > CC2032 > QX32

**syccqx64** seedy > CC2032 > QX64

**symt32** seedy > MT32

**symtqx32** seedy > MT32 > QX32

**symtqx64** seedy > MT32 > QX64

**syss64** seedy > SS64

**wincrypt.c** Example of using Windows Crypto API to obtain a seed.

