#include "coRSAir.h"

void    exit_program(t_keys *keys, int exit_status)
{
    for (int i = 0; i < keys->n_keys; i++){
        free(keys->key[i].path);
        RSA_free(keys->key[i].rsa);
    }
    free(keys->key);
    exit(exit_status);
}

int main()
{
    t_keys  keys;
    int     total_files_decrypted = 0;

    RSA_mcd_attack(&keys);
    int i = 0;
    while(i < keys.n_keys)
    {
        int resultado;
        char *encrypted_file = strdup(keys.key[i].path);
        strncpy(strstr(encrypted_file, ".pem"), ".bin", 5);

        int fd = open(encrypted_file, O_RDONLY);
        if (fd >= 0)
        {   
            char *datos_descifrados = decrypt_file(fd, keys.key[i].rsa, &resultado);
            char *decrypted_file = strdup(keys.key[i].path);
            strncpy(strstr(decrypted_file, ".pem"), ".txt", 5);
            char *decrypted_file2 = strdup(strrchr(decrypted_file, '/') + 1);

            int archivo_descifrado = open(decrypted_file2, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if (archivo_descifrado >= 0)
            {
                ssize_t bytes_escritos = write(archivo_descifrado, datos_descifrados, resultado);
                close(archivo_descifrado);

                write(1, decrypted_file2, strlen(decrypted_file2));
                write(1, "\n", 2);
                total_files_decrypted++;
            }
            else
                printf("Can't open/create decrypted file %s\n", archivo_descifrado);

            free(datos_descifrados);
            free(decrypted_file);
            free(decrypted_file2);
        }
        else
            printf("Can't open %s for being decrypted\n", encrypted_file);
        free(encrypted_file);
        i++;
    }
    printf("Number of files decrypted: %d\n", total_files_decrypted);
    exit_program(&keys, EXIT_SUCCESS);
}
