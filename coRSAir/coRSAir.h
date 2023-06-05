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

typedef struct s_key
{
    char *  path;
    RSA *   rsa;
} t_key;

typedef struct s_keys
{
    t_key   *key;
    int     n_keys;
} t_keys;

const BIGNUM* get_modulus_publicKey(const char* pathPublicKey, const BIGNUM** n, const BIGNUM** e);
RSA* make_RSA(BIGNUM *n, BIGNUM *p, BIGNUM *q, BIGNUM *e);
BIGNUM * euclidean_algorithm(BIGNUM *a, BIGNUM *b);
char *get_encrypted_text(int fd, int *len);
char * decrypt_file(int fd, RSA* rsa, int *output_bytes);
int RSA_mcd_attack(t_keys *keys);

#endif