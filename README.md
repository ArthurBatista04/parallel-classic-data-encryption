# A simple parallel implementation of the Rail Fence Cipher algorithm.

This implementation was develop for the discipline of Parallel Computing (Computer Science - UEM) in September / 2020 by @ArthurBatista04 and @deelefrati .

# The Rail Fence Cipher algorithm

The rail fence cipher (also called a zigzag cipher) is a form of transposition cipher. In cryptography, a transposition cipher is a method of encryption by which the positions held by units of plaintext (which are commonly characters or groups of characters) are shifted according to a regular system, so that the ciphertext constitutes a permutation of the plaintext. That is, the order of the units is changed (the plaintext is reordered).

Specifically, in the rail fence cipher, the plaintext is written downwards and diagonally on successive "rails" of an imaginary fence, then moving up when we get to the bottom. The message is then read off in rows.

## Encryption

In a transposition cipher, the order of the alphabets is re-arranged to obtain the cipher-text. Firstly the plain-text is written downwards and diagonally on successive rails of an imaginary fence.

When we reach the bottom rail, we traverse upwards moving diagonally, after reaching the top rail, the direction is changed again. Thus the alphabets of the message are written in a zig-zag manner.

After each alphabet has been written, the individual rows are combined to obtain the cipher-text.

For example, if the message is “ThreadsAreAwesome” and the number of rails = 3 then cipher is prepared as:

    T . . . A . . . R . . . E . . . E
    . H . E . D . A . E . W . S . M .
    . . R . . . S . . . A . . . O . .

Then reads off to get the ciphertext:

    TAREEHEDAEWSMRSAO

## Decryption

As we’ve seen earlier, the number of columns in rail fence cipher remains equal to the length of plain-text message. And the key corresponds to the number of rails.

Hence, rail matrix can be constructed accordingly. Once we’ve got the matrix we can figure-out the spots where texts should be placed (using the same way of moving diagonally up and down alternatively ). Then, we fill the cipher-text row wise. After filling it, we traverse the matrix in zig-zag manner to obtain the original text.

# Usage

## Complie

To compile the program, just type the command `make` in the root directory.

## Run

There are two ways to execute the algorithm, sequentially and in parallel. Both accept the followging flags.

- `-a` input file name. (required)
- `-k` add this flag to specify the key. By default the key is 3.
- `-p` add this flag if you want me to generate the files with the output of the algorithms of encrypt and decrypt.

### Sequentially

To run the sequential algorithm, enter the following command:

- `./original_version -a example.txt`

### Parallel

In order to run the parallel version, it is necessary to add the following flag:

- `-t` number of threads. (required)

Lastly, just type the following command:

- `./parallel_version -a example.txt -t 4`
