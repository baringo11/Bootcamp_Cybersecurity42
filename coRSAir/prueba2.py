import subprocess
from prueba import gen_private_key 

def euclidean_algorithm(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def get_rsa_modulus(public_key_path):
    command = ['openssl', 'rsa', '-in', public_key_path, '-pubin', '-text', '-noout']
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()

    if process.returncode == 0:
        output = output.decode('utf-8')
        start_index = output.find('Modulus:')
        end_index = output.find('Exponent:')
        
        if start_index != -1 and end_index != -1:
            modulus = output[start_index:end_index].replace('Modulus:', '').strip().replace(':', '').replace(' ', '')
            modulus = modulus.replace('\n', '')
            return int(modulus, 16)
    else:
        print(f"Error: {error.decode('utf-8')}")

    return None


if __name__ == "__main__":

    public_key_path = 'challenge_corsair/' #sample/public.pem'

    first_modulus = get_rsa_modulus(public_key_path + '7.pem')
    second_modulus = get_rsa_modulus(public_key_path + '93.pem')

    mcd = euclidean_algorithm(first_modulus, second_modulus)
    print("mcd: " + str(mcd))

    q1 = first_modulus / mcd
    q1= 11632285988230946246714551486716113190291520068494163099500564698778303325737351498239617583337540511764131867572374744310703041201014109767591825596334999
    
    q2 = int(second_modulus / mcd)
    q2= 13074358256206665105956844254872464791455783011218926315574506536899756094202061832400538633584249598415024636338845868462911949255730651995021077864973483

    print(f"\nm1: {first_modulus}    \nq1: {q1}")
    print(f"\nm2: {second_modulus}   \nq2: {q2}\n")

    n = q1 * mcd
    print(f"n: {n}")

    gen_private_key(mcd, q2, 65537)

    exit()
    for i in range(2, 101):
        print(i)
        tmp_modulus = get_rsa_modulus(public_key_path + str(i) + '.pem')
        mcd = euclidean_algorithm(first_modulus, tmp_modulus)
        if mcd != 1:
            print("mcd: " + str(mcd))
