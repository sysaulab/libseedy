# Commands

## Generator commands usage

Generator commands implement a single algorithm seeded internally with seedy or from an outside source. I created them as debugging, quality control and testing tools. All commands uses the seedy internal seed generator by default. You can override the default by passing the name of the source as an argument like so:

- genqxo64 _stdin_ < /dev/urandom
- genqxo64 _arc4_
- genqxo64 _seedy_

_stdin_ reads from the standard input.

_arc4_ uses the arc4ramdom_buf() native implementation (when available).

_seedy_ uses the built-in seedless generator provided by libseedy.

### List of generator commands

**symtqx32** seedy32 > MT32 > QX32 - Ideal combination for old 32 bit systems like Windows 95. 

**syss64** seedy32 > SS64 - Fastest generator self-initialized with seedy. 

**genmt16:** Mersenne Twister - 16 bit - removed (it was no good)

**genmt32:** Mersenne Twister - 32 bit - reference

**genqxo16** Quad XOR - 16 bit - experimental

**genqxo32** Quad XOR - 32 bit - experimental

**genqxo64** Quad XOR - 64 bit - texted extensively

**genss64** Shishua - 64 bit - reference

**gensy16** seedy16() raw output - 16 bit - tested

**gensy32** seedy32() raw output - 32 bit - tested

**gensy64** seedy64() raw output - 64 bit - tested extensively

**nprime__** Returns the next prime number equal or higher than: nprime64 10 -> 11. Included to test a basic function used to get the next prime number. In case I want to create a generator that iterates through different a different prime instead of reseeding data. Maybe using the iterator gap as a nonce when using QXO to encrypt communications.