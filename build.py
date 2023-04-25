#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import logging
import shutil
import subprocess
import os
import re
import sys

CONSOLE_LOG_LEVEL = logging.INFO
FILE_LOG_LEVEL = logging.DEBUG
VERSION = "0.3.1"


class MyParser(argparse.ArgumentParser):
    """
    Wrapper on ArgumentParser to log errors and exit

    ...

    Methods:
    --------
    error(message: str):
        Overrides the parent error method
        Prints the error, prints the parser help, and exits
    """
    def error(self, message: str):
        logger.error("error: %s", message)
        self.print_help()
        sys.exit(2)


def parse():
    """Creates a new ArgumentParser and parses sys.argv

    Returns:
        tuple[Namespace, list[str]]: A namespace of matched flags and a list of
        unmatched flags
    """

    parser = MyParser(description="Build and run GrailGUI")
    parser.add_argument("-v", "--version", action="version", version=VERSION)
    subparsers = parser.add_subparsers(
        help="subcommands", required=True, dest="subcommand"
    )

    generate = subparsers.add_parser(
        "generate",
        help="generates configurations for CMake",
        description="When called with no additional arguments, generates a\
        valid configuration for Grail using the Ninja build system.",
    )
    generate.add_argument(
        "-G", help="Use a different generator for CMake", default="",
        dest="generator"
    )
    generate.add_argument(
        "-f", "--force", action="store_true", dest="force", default=False
    )
    generate.add_argument("-b", help="Change the build mode", default="",
                          dest="mode")
    generate.add_argument(
        "--c-compiler",
        default="",
        dest="c_compiler",
        type=str,
        help="Set C compiler to use",
    )
    generate.add_argument(
        "--cxx-compiler",
        default="",
        dest="cxx_compiler",
        type=str,
        help="Set C++ compiler to use",
    )
    generate.add_argument(
        "--linker", default="", dest="linker", type=str,
        help="Set linker to use"
    )
    generate.add_argument(
        "--list-options", action="store_true", dest="list_options",
        default=False, help="List options configurable by CMake from this tool"
    )
    generate.add_argument(
        "--dump-options", action="store_true", dest="dump_options",
        default=False, help="List options configurable by CMake"
        " from this tool (with their current values)"
    )
    generate.add_argument(
        "--dump-option", default="", dest="dump_option",
        help="List the value of one option"
    )
    generate.add_argument(
        "-c", "--config", action="append", dest="config", default=None
    )

    generate.add_argument(
        "cmake_args",
        nargs="*",
        default=[],
        type=str,
        help="Additional CMake arguments",
    )

    build = subparsers.add_parser(
        "build",
        help="compiles library and tests",
        description="""When called with no additional arguments, builds all
            valid targets (libraries and executables). If no configuration
            exists, then a new one will be generated using the Ninja build
            system.""",
    )
    build.add_argument(
        "-d", "--dir", action="store_true", help="Compile test subdirectory"
    )
    build.add_argument(
        "-l", "--list", action="store_true", help="List available targets"
    )
    # --force doesn't actually do anything at compile-time,
    # but forces a regeneration.
    # Small but important distinction
    build.add_argument(
        "-f", "--force", action="store_true", dest="force", default=False
    )
    build.add_argument(
        "target",
        nargs="?",
        default="",
        type=str,
        help="Target to execute",
        metavar="target",
    )

    run = subparsers.add_parser(
        "run",
        help="run compiled tests",
        description="When called with one argument, runs an executable with\
            the target's name in Grail's bin folder. Additional arguments can\
            be passed in for the target.",
    )
    run.add_argument("target", nargs=1, type=str, help="Target to execute")
    run.add_argument(
        "target_args",
        nargs="*",
        default=[],
        type=str,
        help="Arguments for running target",
    )

    subparsers.add_parser(
        "clean",
        help="clean all targets (rarely needed with CMake)",
        description="""Cleans all targets (libraries and executables) in one
        command. This is rarely needed, as most issues that are believed to
        require a clean build are usually due to a malformed config file (ex.
        forgetting to add a .cc file toa CMakeLists.txt file. To modify the
        base CMake configuration (ie. changing generators or compilers), see
        the nuke subcommand.""",
    )
    subparsers.add_parser(
        "nuke",
        help="deletes all ephemeral build files (to the best of ourknowledge)",
        description="""Deep cleans or "nukes" the generated and compiled files.
        Any artifacts in common directories are likely to be destroyed. This
        currently includes: bin/, build/, buildbuild/, external/, and libs/.
        logs/ is not included in this list because logs are sometimes necessary
        to keep and deleting them automatically seems counterintuitive.""",
    )
    args = parser.parse_known_args()
    return args


def execute_generate(
    args: argparse.Namespace,
    rest: "list[str]",
    config: "tuple[dict[str,str],list[str]]" = ({}, []),
):
    """Generates a new compile environment via CMake

    Args:
        args (argparse.Namespace): A parsed namespace to work from
        rest (list[str]): A list of extra arguments (to pass to CMake)
    """
    check_dir()

    # Generate a new configuration if no build files are generated
    if "build" not in os.listdir() or args.force:
        if not args.force:
            logger.debug("Build folder does not exist, generating " +
                         "from scratch")
        else:
            logger.debug(
                "Build folder exists, but '--force' is set. " +
                "Generating regardless."
            )
        env, post_args = config
        gen = args.generator if args.generator else "Ninja"
        _args = ["cmake", "-B", "build", "-G", gen]
        _args = _args + args.cmake_args + rest + post_args
        logger.debug(f"Generating with {_args}")
        env = {**os.environ.copy(), **env}
        status = subprocess.run(_args, env=env)
        logger.debug(f"Generation status: ${status}")
        if status.returncode != 0:
            exit(status)
    # Modify an existing configuration
    elif any([args.cmake_args, args.mode, rest]):
        logger.info("build/ exists but cmake_args were specified")
        _args = ["cmake", "-B", "build"]
        if args.mode:
            if args.mode.lower() not in [
                "debug",
                "release",
                "relwithdebinfo",
                "minsizerel",
            ]:
                raise ValueError(
                    """Unknown build mode, please specify one of the following
                    [Debug, Release, RelWithDebInfo, MinSizeRel]"""
                )
            _args.append("-DCMAKE_BUILD_TYPE=" + args.mode)
        _args = _args + args.cmake_args + rest
        logger.debug("Generating with %s", (_args))
        status = subprocess.run(_args)
        logger.debug("Generation status: %d", status)
        if status.returncode != 0:
            sys.exit(status)
    # No notable changes being made, check to see that someone isn't changing
    # the generator on us
    else:
        assert not args.generator, "Generator option cannot be set when"\
                                   " a build exists"
        logger.info("No action necessary, no flags specified")


def process_target(args: argparse.Namespace):
    """Optionally format folder targets
    ex. test/xdl/all instead of all or simpleXDLServer

    Args:
        args (argparse.Namespace): A parsed namespace to work from

    Returns:
        str: The target to build
    """
    if not args.target:
        logger.info("No target specified, using 'all' as default target")
        args.target = "all"
    logger.debug(f"Target is {args.target}")
    target = args.target
    if args.dir:
        logger.debug("args.dir subdirectory argument is set, fixing"
                     f" target: {target}")
        target = f"test/{target}/all"
        logger.debug(f"New target is {target}")
    return target


def check_dir(has_logging: bool = True):
    """Changes to the directory containing the script

    Args:
        has_logging (bool, optional): Enables logging. Defaults to True.
    """
    dir = os.path.abspath(os.path.dirname(__file__))
    if os.path.abspath(".") == dir:
        return
    print(os.path.dirname("."))
    if has_logging:
        logger.info(f"going to {dir}")
    os.chdir(os.path.dirname(__file__))


def get_grail_params() -> dict:
    cache_dump = []
    with open("build/CMakeCache.txt", "r") as f:
        cache_dump = f.readlines()
    target_regex = re.compile(r"(\S+):.+=(.+)\n")

    caches = filter(
        None,
        map(
            lambda x: y.groups() if (y := target_regex.match(x)) else "",
            filter(lambda x: not any([x.startswith("//"), x == "\n"]),
                   cache_dump),
        ),
    )

    extra_keys = [
        "CMAKE_CXX_COMPILER",
        "CMAKE_C_COMPILER",
        "CMAKE_LINKER",
        "CMAKE_BUILD_TYPE",
    ]
    cache_dict = {
        key: value
        for (key, value) in caches
        if any([key.startswith("GRAIL"), key in extra_keys])
    }

    return cache_dict


def get_target_list() -> "tuple[list[str], list[str]]":
    cache_dict = get_grail_params()
    targets = cache_dict["GRAIL_TEST_TARGETS"].split(";")
    dirs = cache_dict["GRAIL_TEST_DIRS_REL"].split(";")
    return targets, dirs


def config(args: argparse.Namespace) -> "tuple[dict[str,str], list[str]]":
    # config_regex = re.compile(r"^([\w\-]+)=([\w\-]+)$")
    check_dir()
    has_generated = os.path.exists("./build/CMakeCache.txt")

    if has_generated and any(
        [args.c_compiler, args.linker, args.cxx_compiler, args.mode]
    ):
        raise RuntimeError(
            "Build has already been generated, but pre-build options have"
            " been specified. Please either run the 'nuke' subcommand before"
            " specifying these options or do not modify them.\nThe following"
            " options cannot be specified after CMake generation has taken"
            " place (-b, --c-compiler, --c-linker, --cxx-compiler)"
        )

    if not has_generated and any(
        [args.list_options, args.dump_options, args.dump_option]
    ):
        raise RuntimeError(
            "Build directory does not exist.\nPlease generate before"
            " interacting with project options."
        )

    # list options that can be dumped to stdout
    if args.list_options:
        logger.info("Available options: %s", ", ".join(get_grail_params().keys()))
        return
    # dump all options
    if args.dump_options:
        logger.debug("Dumping entire config and exiting")
        for (c, v) in get_grail_params().items():
            logger.info(f"{c}: {v}")
        return
    # dump one option
    if args.dump_option:
        logger.debug("Dump option flag set. Dumping config and exiting")
        logger.info(f"{args.dump_option}: "
                    f"{get_grail_params()[args.dump_option]}")
        return
    # configure options

    env = {}
    post_args: list[str] = []
    if args.c_compiler:
        env["CC"] = args.c_compiler
    if args.cxx_compiler:
        env["CXX"] = args.cxx_compiler
    if args.linker:
        post_args += [f'-DCMAKE_CXX_FLAGS="-fuse-ld={args.linker}"']
    if args.mode:
        post_args += [f"-DCMAKE_BUILD_TYPE={args.mode}"]
    return (env, post_args)


def execute_build(target: str, args: argparse.Namespace, rest: list[str]):
    """Builds a target

    Args:
        target (str): The target to build
        args (argparse.Namespace): A parsed namespace to work from
    """
    if "build" not in os.listdir():
        args.generator = "Ninja"
        args.cmake_args = []
        execute_generate(args, rest)
    else:
        check_dir()

    if args.list:
        logger.debug("Listing targets that can be compiled")
        _line_size: int = 72
        # _char_lim = re.compile(rf"(.{_line_size})")
        _targets, _dirs = get_target_list()
        _max_target_len = max(len(x) for x in _targets) + 2
        _num_per_target = _line_size // _max_target_len
        _max_dir_len = max(len(x) for x in _dirs) + 2
        _num_per_dir = _line_size // _max_dir_len

        _dir_str = ""
        for i, _dir in enumerate(_dirs):
            if i % _num_per_dir == 0:
                _dir_str += "\n"
            _dir_str += f"{_dir:<{_max_dir_len}}"

        logger.critical(
            "\nTo compile all targets in a directory, "
            f"specify one of the following with -d:{_dir_str}"
        )

        _target_str = ""
        for i, target in enumerate(_targets):
            if i % _num_per_target == 0:
                _target_str += "\n"
            _target_str += f"{target:<{_max_target_len}}"

        logger.critical(
            "\nTo compile a specific target, "
            f"specify one of the following:{_target_str}"
        )
        return

    _args = ["cmake", "--build", "build", "-t", target]
    logger.debug(f"Generating with {_args}")
    status = subprocess.run(_args)
    logger.debug(f"Build status: {status}")
    if status.returncode != 0:
        exit(status.returncode)

    shutil.copy("./build/compile_commands.json", ".")


def execute_run(target: str, args: list[str]):
    """Runs a test or demo

    Args:
        target (str): The target to run
        args (list[str]): A parsed namespace to work from
    """
    check_dir()
    logger.debug("chdir'ing to test directory")
    os.chdir("test")
    logger.debug(f"running ../bin/{target} with args {args}")
    status = subprocess.run([f"../bin/{target}"] + args)
    logger.debug(f"Run status: {status}")
    if status.returncode != 0:
        exit(status.returncode)


def execute_clean(args: argparse.Namespace):
    """Cleans all targets

    Args:
        args (argparse.Namespace): A parsed namespace to work from
    """
    logger.debug("Cleaning all targets")
    args.list = None
    execute_build("clean", args, [])


def execute_nuke():
    """Deep cleans (or 'nukes') all folders created by Grail (except for ./logs)"""
    check_dir()
    dirs = ["bin", "build", "buildbuild", "external", "libs"]
    d2s = ", ".join(dirs)
    status = input(
        f"This will wipe out the following directories: {d2s}"
        "\nAre you sure you want to do this? (y/N)\t"
    )
    if status.lower() != "y":
        logger.info("Canceling nuke")
    logger.debug(f"Deleting the following directories: {dirs}")
    status = subprocess.run(["rm", "-rf"] + dirs)
    if status.returncode != 0:
        exit(status)


def main():
    args, rest = parse()
    logger.debug(f"Command args: {args}")
    logger.debug(f"Leftover args: {rest}")
    if args.subcommand == "generate":
        logger.debug("Entering generation state")
        config_params = config(args)
        execute_generate(args, rest, config_params)
    elif args.subcommand == "build":
        logger.debug("Entering build state")
        target = process_target(args)
        execute_build(target, args, rest)
    elif args.subcommand == "run":
        logger.debug("Entering run state")
        execute_run(args.target[0], args.target_args)
    elif args.subcommand == "clean":
        logger.debug("Entering clean state")
        execute_clean(args)
    elif args.subcommand == "nuke":
        logger.debug("Entering nuke state")
        execute_nuke()
    else:
        raise NotImplementedError(f"command '{args.subcommand}'"
                                  " is not yet supported")


if __name__ == "__main__":
    logger = logging.getLogger(__name__)
    fh = None

    try:
        check_dir(has_logging=False)

        # Create the log folder
        if not os.path.exists("logs"):
            os.makedirs("logs")

        # Create console handler and set message level
        ch = logging.StreamHandler()
        ch.setLevel(CONSOLE_LOG_LEVEL)

        # Create file handler and set message level
        fh = logging.FileHandler("logs/build.log")
        fh.setLevel(FILE_LOG_LEVEL)
        logger.setLevel(FILE_LOG_LEVEL)

        # Create formatter
        formatter = logging.Formatter(
            "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
        )

        # Add formatter to fh
        fh.setFormatter(formatter)

        # add handlers to logger
        logger.addHandler(ch)
        logger.addHandler(fh)
        main()
    except KeyboardInterrupt:
        # Try to ignore KeyboardInterrupts. They're mostly okay for this
        logger.debug("KeyboardException raised")
        pass
    except Exception:
        # Dump all other exceptions to all loggers
        logger.exception("")
    finally:
        # Close file handler at the end
        if fh:
            fh.close()
