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
        decrypted_file = strrchr(decrypted_file, '/') + 1;

        int archivo_descifrado = open(decrypted_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        ssize_t bytes_escritos = write(archivo_descifrado, datos_descifrados, resultado);
        close(archivo_descifrado);

        write(1, decrypted_file, strlen(decrypted_file));
        write(1, "\n", 2);
        i++;
    }
    return 0;
}
