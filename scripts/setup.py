import os
import shutil
from elevate import elevate

def is_root():
    return os.getuid() == 0

if not is_root():
    elevate()

# A list of common package managers for linux as well as the commands to install clang
packageManagers = [
    ["apt", "apt install clang"],
    ["dnf", "dnf install clang"],
    ["yum", "yum install clang"],
    ["pacman", "pacman -S clang"],
    ["paru", "paru -S clang"],
    ["emerge", "emerge --ask --update --deep --changed-use sys-devel/clang"],
    ["zypper", "zypper install llvm-clang"]
]
validManagers = []

def is_tool(name):
    return shutil.which(name) is not None

for i in packageManagers:
    if is_tool(i[0]):
        validManagers.append(i)
        print(f"Found package manager: \033[1;32m{i[0]}\033[0m")
    else:
        print(f"Could not find package manager: \033[1;31m{i[0]}\033[0m")
        
print(f"\nFound package managers:\n - {(chr(10)+' - ').join(map(lambda x:x[0], validManagers))}\n")

for i in validManagers:
    print(f"\033[1;33mAttempting install with {i[0]}\033[0m")
    try:
        os.system(i[1])
        print(f"\033[1;32mInstall with {i[0]} succeeded\033[0m")
        print(f"\033[1;32mAll packages installed...\033[0m")
        break
    except:
        print(f"\033[1;31mInstall with {i[0]} Failed\033[0m")
        
print(f"\033[1;32mDone\033[0m")
