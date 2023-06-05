#include "coRSAir.h"

BIGNUM * euclidean_algorithm(BIGNUM *a, BIGNUM *b)
{
    BIGNUM *tmp;
    BIGNUM *tmpA = BN_new();
    BIGNUM *tmpB = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    BN_copy(tmpA, a);
    BN_copy(tmpB, b);
    while (BN_is_zero(tmpB) == 0) 
    {
        tmp = BN_new();
        BN_copy(tmp, tmpB);
        BN_mod(tmpB, tmpA, tmpB, ctx);
        
        BN_free(tmpA);
        tmpA = BN_new();

        BN_copy(tmpA, tmp);
        BN_free(tmp);
    }
    BN_free(tmpB);
    BN_CTX_free(ctx);
    return tmpA;
}

RSA* make_RSA(BIGNUM *n, BIGNUM *p, BIGNUM *q, BIGNUM *e)
{
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM* p_minus_1 = BN_new();
    BN_sub(p_minus_1, p, BN_value_one());
    BIGNUM* q_minus_1 = BN_new();
    BN_sub(q_minus_1, q, BN_value_one());
    
    BIGNUM* phi = BN_new();
    BN_mul(phi, p_minus_1, q_minus_1, ctx);

    BIGNUM *d = BN_new();
    BN_mod_inverse(d, e, phi, ctx);

    BIGNUM *dmp1 = BN_new();
    BN_mod(dmp1, d, p_minus_1, ctx);

    BIGNUM *dmq1 = BN_new();
    BN_mod(dmq1, d, q_minus_1, ctx);

    BIGNUM* iqmp = BN_new();
    BN_mod_inverse(iqmp, q, p, NULL);

    RSA* rsa = RSA_new();
    RSA_set0_key(rsa, n, e, d);
    RSA_set0_factors(rsa, p, q);
    RSA_set0_crt_params(rsa, dmp1, dmq1, iqmp);

    BN_CTX_free(ctx);
    BN_free(p_minus_1);
    BN_free(q_minus_1);
    BN_free(phi);
    return rsa;
}
