#include "coRSAir.h"

void *ft_realloc(void **ptr, int ptr_size, int total_size)
{
    void *tmp = malloc(total_size);
    if (!tmp)
    {
        free(*ptr);
        return NULL;
    }
    if (ptr_size != 0)
    {
        memcpy(tmp, *ptr, ptr_size);
        free(*ptr);
    }
    return tmp;
}

char *get_encrypted_text(int fd, int *len)
{
    char buffer[5];
    char *text = NULL;
    ssize_t bytes_leidos = 0;

    while ((bytes_leidos = read(fd, buffer, sizeof(buffer))) > 0)
    {
        text = ft_realloc(&text, *len, *len + bytes_leidos);
        if (!text)
            return NULL;
        memcpy(text + *len, buffer, bytes_leidos);
        *len += bytes_leidos;
    }
    close(fd);
    return text;
}

char * decrypt_file(int fd, RSA* rsa, int *output_bytes)
{
    int encrypted_text_bytes = 0;
    char *encrypted_text = get_encrypted_text(fd, &encrypted_text_bytes);
    char *decrypted_text = malloc(encrypted_text_bytes);
    if (!encrypted_text || !decrypted_text)
        return NULL;

    *output_bytes = RSA_private_decrypt(encrypted_text_bytes, encrypted_text, decrypted_text, rsa, RSA_PKCS1_PADDING);

    char * output_text = malloc(*output_bytes + 1);
    if (!output_text)
    {
        free(encrypted_text);
        free(decrypted_text);
        return NULL;
    }
    memcpy(output_text, decrypted_text, *output_bytes);
    output_text[*output_bytes] = '\0';
    free(encrypted_text);
    free(decrypted_text);

    return output_text;
}
