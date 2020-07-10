# ShiftBits
Shifts the bits at a position a certain amount

An example input set is:
* 127;2;2
* 127;2;3
* 127;2;4
* 127;3;4

An example result set is:
* getbits(x=127, p=2, n=2) = 3
* getbits(x=127, p=2, n=3) = 7
* Error: too many bits requested from position
* getbits(x=127, p=3, n=4) = 15
