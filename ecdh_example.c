/*
  Diffie-Hellman key exchange (without HMAC) aka ECDH_anon in RFC4492

  1. Alice picks a (secret) random natural number 'a', calculates P = a * G and sends P to Bob.
     'a' is Alice's private key. 
     'P' is Alice's public key.

  2. Bob picks a (secret) random natural number 'b', calculates Q = b * G and sends Q to Alice.
     'b' is Bob's private key.
     'Q' is Bob's public key.

  3. Alice calculates S = a * Q = a * (b * G).

  4. Bob calculates T = b * P = b * (a * G).

  .. which are the same two values since multiplication in the field is commutative and associative.

  T = S = the new shared secret.

  Pseudo-random number generator inspired / stolen from: http://burtleburtle.net/bob/rand/smallprng.html

*/
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/time.h>
// #include "ecdh.h"
// #include "cpucycles.h"
// #include "speed_print.h"

// #define NTESTS 100

// static void ecdh_demo(void)
// {
//     static uint8_t puba[ECC_PUB_KEY_SIZE];
//     static uint8_t prva[ECC_PRV_KEY_SIZE];
//     static uint8_t seca[ECC_PUB_KEY_SIZE];
//     static uint8_t pubb[ECC_PUB_KEY_SIZE];
//     static uint8_t prvb[ECC_PRV_KEY_SIZE];
//     static uint8_t secb[ECC_PUB_KEY_SIZE];
//     uint32_t i;

//     // Generate random private keys
//     for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
//     {
//         prva[i] = rand();
//         prvb[i] = rand();
//     }

//     // Alice generates keys and sends public key to Bob
//     ecdh_generate_keys(puba, prva);

//     // Bob generates keys and sends public key to Alice
//     ecdh_generate_keys(pubb, prvb);

//     // Alice calculates shared secret
//     ecdh_shared_secret(prva, pubb, seca);

//     // Bob calculates shared secret
//     ecdh_shared_secret(prvb, puba, secb);
// }



// int main(int argc, char* argv[])
// {
//     unsigned int i;
//     uint64_t t[NTESTS];

//     for(i = 0; i < NTESTS; i++) {
//         t[i] = cpucycles();
//         ecdh_demo();
//     }
    
//     print_result("CPU: ", t, NTESTS );

//     return 0;
// }

/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "ecdh.h"

static void ecdh_demo(void)
{
    static uint8_t puba[ECC_PUB_KEY_SIZE];
    static uint8_t prva[ECC_PRV_KEY_SIZE];
    static uint8_t seca[ECC_PUB_KEY_SIZE];
    static uint8_t pubb[ECC_PUB_KEY_SIZE];
    static uint8_t prvb[ECC_PRV_KEY_SIZE];
    static uint8_t secb[ECC_PUB_KEY_SIZE];
    uint32_t i;

    // Generate random private keys
    for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
    {
        prva[i] = rand();
        prvb[i] = rand();
    }

    // Alice generates keys and sends public key to Bob
    ecdh_generate_keys(puba, prva);

    // Bob generates keys and sends public key to Alice
    ecdh_generate_keys(pubb, prvb);

    // Alice calculates shared secret
    ecdh_shared_secret(prva, pubb, seca);

    // Bob calculates shared secret
    ecdh_shared_secret(prvb, puba, secb);
}

int main(int argc, char* argv[])
{
    int i, ncycles = 1;
    struct timeval start, end, total;
    double total_time = 0.0;

    if (argc > 1)
    {
        ncycles = atoi(argv[1]);
    }

    // Measure execution time
    for (i = 0; i < ncycles; ++i)
    {
        gettimeofday(&start, NULL);
        ecdh_demo();
        gettimeofday(&end, NULL);
        timersub(&end, &start, &total);
        total_time += total.tv_sec + total.tv_usec * 1e-6;
    }

    // Calculate average time
    total_time /= ncycles;

    printf("Average Execution Time: %f seconds\n", total_time);

    return 0;
}
