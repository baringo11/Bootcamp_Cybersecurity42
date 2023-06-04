from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa


# Calcula el inverso multiplicativo de e módulo phi
def extended_gcd(a, b):
    if b == 0:
        return (a, 1, 0)
    else:
        d, x, y = extended_gcd(b, a % b)
        return (d, y, x - (a // b) * y)

def mod_inverse(a, m):
    d, x, y = extended_gcd(a, m)
    if d != 1:
        raise ValueError("El número no tiene inverso multiplicativo módulo m")
    return x % m

def gen_private_key(p, q, e):

    n = p * q
    # Calcula la función phi de Euler
    phi = (p - 1) * (q - 1)

    # Elige un número e que sea coprimo con phi y menor que phi
    # e = 65537

    d = mod_inverse(e, phi)

    # Calcula los valores de dmp1, dmq1 e iqmp
    dmp1 = d % (p - 1)
    dmq1 = d % (q - 1)
    iqmp = mod_inverse(q, p)


    # Construir el objeto de clave privada RSA
    private_key = rsa.RSAPrivateNumbers(
        p=p,
        q=q,
        d=d,
        dmp1=dmp1,
        dmq1=dmq1,
        iqmp=iqmp,
        public_numbers=rsa.RSAPublicNumbers(e=e, n=n)
    ).private_key()

    # Exportar la clave privada en formato .pem
    pem = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )

    # Guardar el contenido del archivo .pem en un archivo
    with open("private_key.pem", "wb") as f:
        f.write(pem)
