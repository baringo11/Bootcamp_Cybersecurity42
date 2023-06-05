#include "coRSAir.h"

int main()
{
    t_keys keys;

    RSA_mcd_attack(&keys);
    int i = 0;
    while(i < keys.n_keys)
    {
        int resultado;
        char *encrypted_file = strdup(keys.key[i].path);
        strncpy(strstr(encrypted_file, ".pem"), ".bin", 5);

        char *datos_descifrados = decrypt_file(open(encrypted_file, O_RDONLY), keys.key[i].rsa, &resultado);
        char *decrypted_file = strdup(keys.key[i].path);
        strncpy(strstr(decrypted_file, ".pem"), ".txt", 5);
        char *decrypted_file2 = strdup(strrchr(decrypted_file, '/') + 1);

        int archivo_descifrado = open(decrypted_file2, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        ssize_t bytes_escritos = write(archivo_descifrado, datos_descifrados, resultado);
        close(archivo_descifrado);

        write(1, decrypted_file2, strlen(decrypted_file2));
        write(1, "\n", 2);

        free(encrypted_file);
        free(datos_descifrados);
        free(decrypted_file);
        free(decrypted_file2);
        i++;
    }
    printf("i:%d\n", keys.n_keys);

    for (int i = 0; i < keys.n_keys; i++){
        free(keys.key[i].path);
        RSA_free(keys.key[i].rsa);
    }

   // CRYPTO_cleanup_all_ex_data(); //no hace na

    return 0;
}
