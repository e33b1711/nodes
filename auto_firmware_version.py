
import subprocess



ret = subprocess.run("git rev-parse HEAD", stdout=subprocess.PIPE, text=True) #Uses only annotated tags
#ret = subprocess.run(["git", "describe", "--tags"], stdout=subprocess.PIPE, text=True) #Uses any tags
build_version = ret.stdout.strip()
build_flag = "-D auto_version=\\\"" + build_version + "\\\""
print ("Firmware Revision: " + build_version)
with open("include/version.h", "w+") as f:
    f.write("const char* auto_version = \"" + build_version + "\";")
