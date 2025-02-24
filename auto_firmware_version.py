
import subprocess



ret = subprocess.run("git rev-parse HEAD", stdout=subprocess.PIPE, text=True) #Uses only annotated tags
#ret = subprocess.run(["git", "describe", "--tags"], stdout=subprocess.PIPE, text=True) #Uses any tags
build_version = ret.stdout.strip()
print ("Firmware Revision: " + build_version)
ret = subprocess.run("git status --porcelain", stdout=subprocess.PIPE, text=True)
dirty = not ret.stdout.strip()==""
with open("include/version.h", "w+") as f:
    f.write("const String auto_version = \"" + build_version + "\";\n")
    if dirty:
        f.write("const bool dirty = true;\n")
    else:
        f.write("const bool dirty = false;\n")
