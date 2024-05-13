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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecdh.h"
#include <time.h>
#include <sys/time.h>

/* pseudo random number generator with 128 bit internal state... probably not suited for cryptographical usage */


static void ecdh_demo(void)
{
  static uint8_t puba[ECC_PUB_KEY_SIZE];
  static uint8_t prva[ECC_PRV_KEY_SIZE];
  static uint8_t seca[ECC_PUB_KEY_SIZE];
  static uint8_t pubb[ECC_PUB_KEY_SIZE];
  static uint8_t prvb[ECC_PRV_KEY_SIZE];
  static uint8_t secb[ECC_PUB_KEY_SIZE];
  uint32_t i;

  for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
  {
    prva[i] = rand();
    prvb[i] = rand();
  }
  
  /* 1. Alice picks a (secret) random natural number 'a', calculates P = a * g and sends P to Bob. */

  ecdh_generate_keys(puba, prva);

  /* 2. Bob picks a (secret) random natural number 'b', calculates Q = b * g and sends Q to Alice. */

  ecdh_generate_keys(pubb, prvb);

  /* 3. Alice calculates S = a * Q = a * (b * g). */
  ecdh_shared_secret(prva, pubb, seca);

  /* 4. Bob calculates T = b * P = b * (a * g). */
  ecdh_shared_secret(prvb, puba, secb);

}

struct timeval start, end, total, total_new = {0};

int main(int argc, char* argv[])
{
  int i;
  int ncycles = 1;
  double total_sum = 0.0;
  if (argc > 1)
  {
    ncycles = atoi(argv[1]);
  }

  for (i = 0; i < ncycles; ++i)
  {
    gettimeofday(&start, NULL);
    ecdh_demo();
    gettimeofday(&end, NULL);
    timersub(&end, &start, &total);
        if (i == 0) {
            total_new = total;
        } else {
            timeradd(&total, &total_new, &total_new);
        }
    }
    total_sum = ((double)total_new.tv_sec * 1000.0) + ((double)total_new.tv_usec / 1000.0);
    total_sum /= ncycles;

    printf("Key Pair Generation Time: %fms\n", total_sum);

  return 0;
}



