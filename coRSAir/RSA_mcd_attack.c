#include "coRSAir.h"

void    get_modulus_publicKey(const char *pathPublicKey, BIGNUM **n, BIGNUM **e)
{
    BIO *publicKeyBio = BIO_new_file(pathPublicKey, "r");
    if (!publicKeyBio){
        printf("Error al abrir %s\n", pathPublicKey);
        return ;
    }

    EVP_PKEY *pkey = PEM_read_bio_PUBKEY(publicKeyBio, NULL, NULL, NULL);
    if (!pkey){
        printf("Error al leer la clave pública de %s\n", pathPublicKey);
        return ;
    }

    RSA *rsaClavePublica = EVP_PKEY_get1_RSA(pkey);
    if (!rsaClavePublica){
        printf("Error al obtener la clave pública RSA\n");
        return ;
    }

    const BIGNUM *tmp_n;
    const BIGNUM *tmp_e;
    RSA_get0_key(rsaClavePublica, &tmp_n, &tmp_e, NULL);

    BN_copy(*n, tmp_n);
    BN_copy(*e, tmp_e);

    BIO_free(publicKeyBio);
    EVP_PKEY_free(pkey);
    RSA_free(rsaClavePublica);
}

void    fill_key_struct(t_keys *keys, char *path, BIGNUM *n, BIGNUM *p, BIGNUM *e)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *q = BN_new();
    BN_div(q, NULL, n, p, ctx);
    BN_CTX_free(ctx);

    int n_keys = keys->n_keys;
    t_key *tmp = (t_key *)malloc(sizeof(t_key) * (n_keys+1)); //leaks

    for (int i = 0; i < n_keys; i++)
    {
        tmp[i].path = keys->key[i].path;
        tmp[i].rsa = keys->key[i].rsa;
    }
    tmp[n_keys].path = strdup(path);
    tmp[n_keys].rsa = make_RSA(n, p, q, e);
    keys->n_keys++;

    if (keys->key)
        free(keys->key);
    keys->key = tmp;
}

void get_key_matches(t_keys *keys)
{
    const char *path = "challenge_corsair/";

    int flag_free;
    int matches[100];
    for (int i = 0; i < 100; i++)
        matches[i] = 0;

    int i = 1;
    while (i < 100)
    {
        flag_free = 1;
        const BIGNUM *modulus1 = BN_new();
        const BIGNUM *e1 = BN_new();
        char pem_1[100];
        int t_1 = snprintf(pem_1, 100, "challenge_corsair/%d.pem", i);
            //write(1, pem_1, t_1);
        get_modulus_publicKey(pem_1, &modulus1, &e1);
        int j = 1;
        while (j < 100)
        {
            if (j != i)
            {
                const BIGNUM *tmp_modulus = BN_new();
                const BIGNUM *e2 = BN_new();
                char tmp_pem[100]; 
                int t = snprintf(tmp_pem, 100, "challenge_corsair/%d.pem", j);
                get_modulus_publicKey(tmp_pem, &tmp_modulus, &e2);

                BIGNUM *mcd = euclidean_algorithm(modulus1, tmp_modulus);
                BIGNUM *mcd2 = BN_new();
                BN_copy(mcd2, mcd);
                if (!BN_is_one(mcd)){
                    if (!matches[i]){
                        flag_free = 0;
                        fill_key_struct(keys, pem_1, modulus1, mcd, e1);
                        matches[i] = 1;
                    }
                    if (!matches[j]){
                        fill_key_struct(keys, tmp_pem, tmp_modulus, mcd2, e2);
                        matches[j] = 1;
                    }
                    else{
                        BN_free(tmp_modulus);
                        BN_free(e2);
                        BN_free(mcd);
                        BN_free(mcd2);
                    }
                }
                else {
                    BN_free(tmp_modulus);
                    BN_free(e2);
                    BN_free(mcd);
                    BN_free(mcd2);
                }
            }
            j++;
        }
        if (flag_free){
            BN_free(modulus1);
            BN_free(e1);
        }
        i++;
    }
}

int RSA_mcd_attack(t_keys *keys)
{
    keys->key = NULL;
    keys->n_keys = 0;

    get_key_matches(keys);
}
