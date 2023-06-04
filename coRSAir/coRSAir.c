#include "coRSAir.h"

const BIGNUM* get_modulus_publicKey(const char* pathPublicKey, const BIGNUM** n, const BIGNUM** e)
{
    FILE* publicKeyFile = fopen(pathPublicKey, "r");
    if (!publicKeyFile) {
        printf("Error al abrir %s\n", pathPublicKey);
        return 0;
    }

    EVP_PKEY* pkey = PEM_read_PUBKEY(publicKeyFile, NULL, NULL, NULL);
    fclose(publicKeyFile);

    if (!pkey) {
        printf("Error al leer la clave pública de %s\n", pathPublicKey);
        return 0;
    }

    RSA* rsaClavePublica = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);

    if (!rsaClavePublica) {
        printf("Error al obtener la clave pública RSA\n");
        return 0;
    }

    RSA_get0_key(rsaClavePublica, n, e, NULL);

  //  RSA_free(rsaClavePublica);

}

char*   get_encrypted_text()
{
    int     fd_encrypted = open("challenge_corsair/7.bin", O_RDONLY);
    char    buffer[4096];
    char    *text = NULL;
    char    *tmp = NULL;

    ssize_t bytes_leidos = 0;

    while ((bytes_leidos = read(fd_encrypted, buffer, sizeof(buffer))) > 0)
    {
        if (!text)
        {
            text = malloc(bytes_leidos);
            memcpy(text, buffer, bytes_leidos);
        }
        else
        {
            tmp = malloc(sizeof(text) + bytes_leidos);
            memcpy(tmp, text, sizeof(text));
            memcpy(tmp + sizeof(text), buffer, bytes_leidos);
            memcpy(text, tmp, sizeof(tmp));
        }
    }
    close(fd_encrypted);
}

int main() 
{
    const char* rutaArchivoClavePublica = "challenge_corsair/1.pem";
    BN_CTX *ctx = BN_CTX_new();

    const BIGNUM *modulus1 = BN_new();
    const BIGNUM *e = BN_new();
    get_modulus_publicKey("challenge_corsair/7.pem", &modulus1, &e);
    const BIGNUM *modulus2 = BN_new();
    get_modulus_publicKey("challenge_corsair/93.pem", &modulus2, NULL);
    BIGNUM *mcd = euclidean_algorithm(modulus1, modulus2);
    BIGNUM *q = BN_new();
    BN_div(q, NULL, modulus1, mcd, ctx);

    RSA* rsa = make_RSA(modulus1, mcd, q, e);

    
    char    *text = get_encrypted_text();
    unsigned char* datos_descifrados = (unsigned char*)malloc(4096);

    int resultado = RSA_private_decrypt(strlen(text), text, datos_descifrados, rsa, RSA_PKCS1_PADDING);
    int archivo_descifrado = open("archivo_descifrado.txt", O_WRONLY | O_CREAT | O_TRUNC);
    ssize_t bytes_escritos = write(archivo_descifrado, datos_descifrados, resultado);
    close(archivo_descifrado);

    printf("Modulo 1:\n%s\n\n", BN_bn2dec(modulus1));
    //FILE* file = fopen("private_key.pem", "w");
    //PEM_write_RSAPrivateKey(file, rsa, NULL, NULL, 0, NULL, NULL)
   
    //  RSA_free(rsa);
    //    OPENSSL_free(modulo1);
    //    OPENSSL_free(modulo2);

    return 0;
}
