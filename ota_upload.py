# firmware upload over the network, see README "Flashen über das Netzwerk (OTA)"
#   pio run -e og_ost -t ota                         ip from custom_ota_host in platformio.ini
#   pio run -t ota                                   all nodes in default_envs
#   OTA_HOST=192.168.178.50 pio run -e test -t ota   only for environments without custom_ota_host
import base64
import os
import re
import sys
import urllib.error
import urllib.request

Import("env")

OTA_PORT = 65280

# ArduinoOTA takes the raw image, not the hex file
bin_file = env.Command(
    "$BUILD_DIR/${PROGNAME}.bin",
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(
        "$OBJCOPY -O binary -R .eeprom -R .fuse -R .lock -R .signature $SOURCE $TARGET",
        "Building $TARGET",
    ),
)


def read_password():
    path = os.path.join(env.subst("$PROJECT_INCLUDE_DIR"), "secret_ota.h")
    try:
        with open(path) as f:
            match = re.search(r'#define\s+OTA_PASSWORD\s+"([^"]*)"', f.read())
    except OSError:
        match = None
    if not match:
        sys.stderr.write("Error: OTA_PASSWORD not found in %s\n" % path)
        env.Exit(1)
    return match.group(1)


def ota_upload(target, source, env):
    # the ini wins, otherwise a run over all nodes would send every image to OTA_HOST
    host = env.GetProjectOption("custom_ota_host", "") or os.environ.get("OTA_HOST")
    if not host:
        sys.stderr.write(
            "Error: no ip for %s, set custom_ota_host in platformio.ini or OTA_HOST\n"
            % env.subst("$PIOENV")
        )
        env.Exit(1)
    image = source[0].get_abspath()
    with open(image, "rb") as f:
        data = f.read()
    auth = base64.b64encode(("arduino:" + read_password()).encode()).decode()
    url = "http://%s:%d/sketch" % (host, OTA_PORT)
    print("Uploading %s (%d bytes) to %s" % (image, len(data), url))
    request = urllib.request.Request(
        url,
        data=data,
        method="POST",
        headers={"Authorization": "Basic " + auth, "Content-Type": "application/octet-stream"},
    )
    try:
        with urllib.request.urlopen(request, timeout=60) as response:
            print("Node answered: %d %s" % (response.status, response.read().decode(errors="replace")))
    except urllib.error.HTTPError as e:
        sys.stderr.write("Error: node answered %d %s\n" % (e.code, e.reason))
        env.Exit(1)
    except (urllib.error.URLError, OSError) as e:
        sys.stderr.write("Error: upload failed: %s\n" % e)
        env.Exit(1)
    print("The node copies the firmware and restarts, the revision shows up in Home Assistant.")


env.AddCustomTarget(
    name="ota",
    dependencies=bin_file,
    actions=ota_upload,
    title="Upload OTA",
    description="Upload the firmware over the network",
)
