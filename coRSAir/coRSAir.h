#ifndef PHILO_H
# define PHILO_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/evp.h>

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

const BIGNUM* get_modulus_publicKey(const char* pathPublicKey, const BIGNUM** n, const BIGNUM** e);
RSA* make_RSA(BIGNUM *n, BIGNUM *p, BIGNUM *q, BIGNUM *e);
BIGNUM * euclidean_algorithm(BIGNUM *a, BIGNUM *b);

#endif