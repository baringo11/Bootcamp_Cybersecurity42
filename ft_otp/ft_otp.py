import argparse
import os
from cryptography.fernet import Fernet
import base64

# Configuración del argumento de línea de comandos
parser = argparse.ArgumentParser(description='Guarda una clave cifrada en un archivo.')
#parser.add_argument('-g', '--generate', type=str, help='clave hexadecimal para cifrar')
parser.add_argument('-g', '--generate-key', metavar='FILE', help='Genera una nueva clave y la guarda en FILE')
args = parser.parse_args()

# Verificar que se proporcionó la opción '-g' y que la clave tiene 64 caracteres hexadecimales
if not args.generate_key:
    print("Debe proporcionar una clave hexadecimal con la opción '-g'.")
    exit()

try:
    with open(args.generate_key, 'r') as file:
        key_hex = file.read().strip()
        print(key_hex)
except FileNotFoundError:
    print(f'El archivo {args.generate_key} no existe')
if len(key_hex) < 64 or not all(c in '0123456789abcdefABCDEF' for c in key_hex):
    print("La clave debe tener mínimo 64 caracteres hexadecimales.")
    exit()

# Cifrar la clave con Fernet
key_bytes = bytes.fromhex(key_hex)
cipher_suite = Fernet(base64.urlsafe_b64encode(key_bytes))
ciphered_text = cipher_suite.encrypt(b'')

# Guardar la clave cifrada en un archivo llamado 'ft_otp.key'
if not os.path.exists('ft_otp.key'):
    with open('ft_otp.key', 'wb') as f:
        f.write(ciphered_text)
        print("Clave guardada con éxito en 'ft_otp.key'.")
else:
    print("El archivo 'ft_otp.key' ya existe, no se sobrescribirá.")
