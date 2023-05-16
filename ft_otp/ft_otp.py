import argparse
import subprocess
import time
from cryptography.fernet import Fernet
import base64
import hmac
import struct

password = base64.urlsafe_b64encode(bytes.fromhex("cb3b0eefc4b970b8a0a2b9263f851997eeeaba6ef213571fe7f3d9a4f1bd34d0"))

def gen_otp(key):
    key = bytes.fromhex(key)
    counter = int(time.time()) // 30
    # Convierte el contador en una secuencia de 8 bytes ('Q' = "unsigned long long") en orden big-endian ('>')
    counter_bytes = struct.pack('>Q', counter)
    # Calcula el HMAC-SHA1 de la clave y el contador
    hmac_result = hmac.new(key, counter_bytes, 'sha1').digest()
    # Obtiene el último byte de la digestión para usar como offset
    offset = hmac_result[-1] & 0x0f
    # selecciona una subsecuencia de 4 bytes a partir del offset
    subsecuencia = hmac_result[offset:offset+4] 
    # combina los 4 bytes en un número entero de 32 bits
    numero_entero = struct.unpack('>I', subsecuencia)[0]
    # aplica una máscara para obtener un valor de 31 bits
    valor_mascara = numero_entero & 0x7FFFFFFF
    # divide por 10 a la potencia de la cantidad de dígitos de la clave OTP
    clave_otp = valor_mascara % (10 ** 6)
    # rellena con ceros a la izquierda para alcanzar la cantidad de dígitos requeridos
    clave_otp = str(clave_otp).zfill(6)
    return clave_otp

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-g', metavar='FILE', help='Cifra la clave y la guarda en ft_otp.key')
    parser.add_argument('-k', metavar='FILE', help='Obtiene una contraseña temporal utilizando la clave guardada en FILE')
    args = parser.parse_args()

    if args.k:
        try:
            with open(args.k, 'r') as file:
                key = file.read()
                fernet = Fernet(password)
                try:
                    decrypted_key = fernet.decrypt(key.encode()).decode()
                except:
                    print(f"Error al descifar '{args.k}': Contraseña incorrecta")
                    exit()
                print(f"key: {key}")
                print(f"decrypted_key: {decrypted_key}")
                print(f"decrypted_key texto: {bytes.fromhex(decrypted_key)}")

                print(gen_otp(decrypted_key))
                print(subprocess.check_output(["oathtool", "--totp", decrypted_key]).decode())
               
                otp_oathtool = subprocess.check_output(["oathtool", "--totp", "-v", decrypted_key]).decode()
                print('OTP generado por Oathtool:\n', otp_oathtool)
        except FileNotFoundError:
            print(f'El archivo {args.k} no existe')
        except:
            print("Error oathtool :(")

        exit()
    if not args.g:
        print("Debe proporcionar una clave hexadecimal con la opción '-g'.")
        exit()

    try:
        with open(args.g, 'r') as file:
            key_hex = file.read()
    except FileNotFoundError:
        print(f'El archivo "{args.g}" no existe')
        exit()
    if len(key_hex) < 64 or not all(c in '0123456789abcdefABCDEF' for c in key_hex):
        print("La clave debe tener mínimo 64 caracteres hexadecimales.")
        exit()

    # Cifrar la clave hexadecimal con Fernet
    cipher_suite = Fernet(password)
    ciphered_text = cipher_suite.encrypt(key_hex.encode())

    try:
        with open('ft_otp.key', 'wb') as f:
            f.write(ciphered_text)
            print("Clave guardada con éxito en 'ft_otp.key'")
    except:
        print("Error al crear el archivo 'ft_otp.key'")
    