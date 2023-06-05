#include "coRSAir.h"

const BIGNUM *get_modulus_publicKey(const char *pathPublicKey, const BIGNUM **n, const BIGNUM **e)
{
    FILE *publicKeyFile = fopen(pathPublicKey, "r");
    if (!publicKeyFile)
    {
        printf("Error al abrir %s\n", pathPublicKey);
        return 0;
    }

    EVP_PKEY *pkey = PEM_read_PUBKEY(publicKeyFile, NULL, NULL, NULL);
    fclose(publicKeyFile);

    if (!pkey)
    {
        printf("Error al leer la clave pública de %s\n", pathPublicKey);
        return 0;
    }

    RSA *rsaClavePublica = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);

    if (!rsaClavePublica)
    {
        printf("Error al obtener la clave pública RSA\n");
        return 0;
    }

    RSA_get0_key(rsaClavePublica, n, e, NULL);
}

void    fill_key_struct(t_keys *keys, char *path, BIGNUM *n, BIGNUM *p, BIGNUM *e)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *q = BN_new();
    BN_div(q, NULL, n, p, ctx);

    int n_keys = keys->n_keys;
    t_key *tmp = (t_key *)malloc(sizeof(t_key) * (n_keys+1));

    for (int i = 0; i < n_keys; i++)
    {
        tmp[i].path = keys->key[i].path;
        tmp[i].rsa = keys->key[i].rsa;
    }
    tmp[n_keys].path = strdup(path);
    tmp[n_keys].rsa = make_RSA(n, p, q, e);
    keys->n_keys++;

    free(keys->key);
    keys->key = tmp;
}

void get_key_matches(t_keys *keys)
{
    const char *path = "challenge_corsair/";

    int matches[100];
    for (int i = 0; i < 100; i++)
        matches[i] = 0;

    int i = 1;
    while (i < 100)
    {
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

                if (!BN_is_one(mcd))
                {
                    if (!matches[i]){
                        fill_key_struct(keys, pem_1, modulus1, mcd, e1);
                        matches[i] = 1;
                    }
                    if (!matches[j]){
                        fill_key_struct(keys, tmp_pem, tmp_modulus, mcd, e2);
                        matches[j] = 1;
                    }
                }
            }
            j++;
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
