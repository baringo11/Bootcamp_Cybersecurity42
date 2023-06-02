# Stockholm

Simple ransomware that only affects to `infection` directoy in HOME for Linux o MacOS.<br>
Encrypts/decrypts files with the given password.

# Run
You need python3 and pyAesCrypt library installed. Then run:

```bash
./stockholm.py [-h] [-r] [-s] [-v] <password>
```
where the `<password>` is a minimum 16 characters long string. 

And the optional flags are:<br>
`[-h]` is for help.<br>
`[-r]` is for reverse, decrypts files encrypted.<br>
`[-s]` is for silent the output.<br>
`[-v]` is for printin the version.<br>
