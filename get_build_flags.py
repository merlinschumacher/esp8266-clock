import subprocess
import sys
from datetime import datetime

try:
    if sys.argv[1] == "debug":
        build_type = "debug"
    else:
        build_type = "release"
except:
    build_type = "release"


if build_type  == "debug":
    revision = (
        subprocess.check_output(["git", "rev-parse", "--short", "HEAD"])
        .strip()
        .decode("utf-8")
    )
    dt_string = datetime.now().strftime("_%d-%m-%Y_%H-%M-%S")
    version = revision + dt_string; 
    flags = "-fexceptions -DDEBUG_BUILD -O0 -ggdb3 -g3 "
else:
    version = "v" + (
        subprocess.check_output(["git", "describe", "--abbrev=0", "--tags"])
        .strip()
        .decode("utf-8")
    )
    flags = "-O3 "

if sys.argv[2] == "esp8266":
    common_flags = "-DCONT_STACKSIZE=20480 "
    print(common_flags + flags + "-DVERSION='\"%s\"'" % version)
elif sys.argv[2] == "esp32":
    print("-DVERSION='\"%s\"'" % version)