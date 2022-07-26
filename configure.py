import subprocess
import sys
import argparse
import platform
from enum import Enum

class EmeraldBuildConfig(Enum):
    debug = 0,
    release = 1

class EmeraldBuildType:
    def __init__(self, build_config, generator):
        self.build_config = build_config
        self.generator = generator

def parse_build_type():
    parser = argparse.ArgumentParser(description = 'Generate a CMake ouptut using the given settings.')
    parser.add_argument('build_config', help='build configuration you want, e.g debug')
    parser.add_argument('generator', help='generator for CMake to use e.g \'MinGW Makefiles\'')
    args = parser.parse_args()

    return EmeraldBuildType(EmeraldBuildConfig[args.build_config], args.generator)

def configure():
    build_type = parse_build_type()
    build_config_str = build_type.build_config.name

    os_name_dict = {
        'Linux': 'linux',
        'Darwin': 'macos',
        'Java': 'java',
        'Windows': 'win'
    }

    os_name = os_name_dict[platform.system()]
    if os_name != 'linux' and os_name != 'win':
        sys.exit('Failed to configure Emerald CMake project -- Operating system %s is not recognised as a valid build platform.' % os_name)

    build_dir = os_name + "_" + build_config_str
    cmd = "cmake -B \"build/%s\" -DEMERALD_BUILDCONFIG=%s -G \"%s\" -S ." % (build_dir, build_config_str, build_type.generator)
    print('Configuring Emerald build system generation with default settings...')
    subprocess.call(cmd)

if __name__ == "__main__":
    configure()

