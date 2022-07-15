#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import logging
import shutil
import subprocess
import os

console_log_level = logging.INFO
file_log_level = logging.DEBUG

class MyParser(argparse.ArgumentParser):
    def error(self, message:str):
        logger.error('error: %s' % message)
        self.print_help()
        exit(2)
        
def parse():
    """Creates a new ArgumentParser and parses sys.argv

    Returns:
        tuple[Namespace, list[str]]: A namespace of matched flags and a list of unmatched flags
    """

    parser = MyParser(description="Build and run GrailGUI")
    subparsers = parser.add_subparsers(
        help="subcommands", required=True, dest="subcommand"
    )

    generate = subparsers.add_parser(
        "generate",
        help="generates configurations for CMake",
        description="When called with no additional arguments, generates a valid configuration for Grail using the Ninja build system.",
    )
    generate.add_argument(
        "-G", help="Use a different generator for CMake", default="", dest="generator"
    )
    generate.add_argument(
        "cmake_args", nargs="*", default=[], type=str, help="Additional CMake arguments"
    )

    build = subparsers.add_parser(
        "build",
        help="compiles library and tests",
        description="When called with no additional arguments, builds all valid targets (libraries and executables). If no configuration exists, then a new one will be generated using the NInja build system.",
    )
    # group.add_argument('-l','--list', action='store_true', help='List available targets')
    build.add_argument(
        "-d", "--dir", action="store_true", help="Compile test subdirectory"
    )
    build.add_argument(
        "target",
        nargs="?",
        default="all",
        type=str,
        help="Target to execute",
        metavar="target",
    )

    run = subparsers.add_parser(
        "run",
        help="run compiled tests",
        description="When called with one argument, runs an executable with the target's name in Grail's bin folder. Additional arguments can be passed in for the target.",
    )
    run.add_argument("target", nargs=1, type=str, help="Target to execute")
    run.add_argument(
        "target_args",
        nargs="*",
        default=[],
        type=str,
        help="Arguments for running target",
    )

    _clean = subparsers.add_parser(
        "clean",
        help="clean all targets (rarely needed with CMake)",
        description="Cleans all targets (libraries and executables) in one command. This is rarely needed, as most issues that are believed to require a clean build are usually due to a malformed config file (ex. forgetting to add a .cc file toa CMakeLists.txt file. To modify the base CMake configuration (ie. changing generators or compilers), see the nuke subcommand.",
    )
    _nuke = subparsers.add_parser(
        "nuke",
        help="deletes all ephemeral build files (to the best of our knowledge)",
        description='Deep cleans or "nukes" the generated and compiled files. Any artifacts in common directories are likely to be destroyed. This currently includes: bin/, build/, buildbuild/, external/, and libs/. logs/ is not included in this list because logs are sometimes necessary to keep and deleting them automatically seems counterintuitive.',
    )
    args = parser.parse_known_args()
    return args


def execute_generate(args: argparse.Namespace, rest: "list[str]"):
    """Generates a new compile environment via CMake

    Args:
        args (argparse.Namespace): A parsed namespace to work from
        rest (list[str]): A list of extra arguments (to pass to CMake)
    """
    check_grail_dir()

    # Generate a new configuration if no build files are generated
    if "build" not in os.listdir():
        gen = args.generator if args.generator else "Ninja"
        _args = ["cmake", "-B", "build", "-G", gen]
        _args = _args + args.cmake_args + rest
        logger.debug(f"Generating with {_args}")
        status = subprocess.run(_args)
        logger.debug(f"Generation status: ${status}")
        if status.returncode != 0:
            exit(status)
    # Modify an existing configuration
    elif args.cmake_args or rest:
        logger.info("build/ exists but cmake_args were specified")
        _args = ["cmake", "-B", "build"] + args.cmake_args + rest
        logger.debug(f"Generating with {_args}")
        status = subprocess.run(_args)
        logger.debug(f"Generation status: ${status}")
        if status.returncode != 0:
            exit(status)
    # No notable changes being made, check to see that someone isn't changing the generator on us
    else:
        assert not args.generator, "Generator option cannot be set when a build exists"
        logger.info("No action necessary, no flags specified")


def process_target(args: argparse.Namespace):
    """Optionally format folder targets (ie. test/xdl/all instead of all or simpleXDLServer)

    Args:
        args (argparse.Namespace): A parsed namespace to work from

    Returns:
        str: The target to build
    """
    logger.debug(f"Target is {args.target}")
    target = args.target
    if args.dir:
        logger.debug(f"args.dir subdirectory argument is set, fixing target")
        target = f"test/{target}/all"
        logger.debug(f"New target is {target}")
    return target


def check_grail_dir(dir: str = ".", has_logging: bool = True):
    """Recursively checks to see if the root of Grail is in any parent folder

    Args:
        dir (str, optional): The starting directory. Defaults to ".".
        has_logging (bool, optional): Enables logging. Defaults to True.
    """
    dir = os.path.abspath(dir)
    if has_logging:
        logger.debug(f"Checking directory: {dir}")
    assert (
        dir != "/"
    ), "Grail Workspace is not in any parent directory from here to root"
    if "Grail_Workspace.code-workspace" not in os.listdir(dir):
        check_grail_dir(os.path.dirname(dir))
    else:
        if has_logging:
            logger.info(f"Grail_Wordspace.code-workspace found in {dir}")
        os.chdir(dir)


def execute_build(target: str, args: argparse.Namespace, rest:"list[str]"):
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
        check_grail_dir()

    _args = ["cmake", "--build", "build", "-t", target]
    logger.debug(f"Generating with {_args}")
    status = subprocess.run(_args)
    logger.debug(f"Build status: {status}")
    if status.returncode != 0:
        exit(status.returncode)

    shutil.copy("./build/compile_commands.json", ".")


def execute_run(target: str, args: "list[str]"):
    """Runs a test or demo

    Args:
        target (str): The target to run
        args (list[str]): A parsed namespace to work from
    """
    check_grail_dir()
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
    execute_build("clean", args,[])


def execute_nuke():
    """Deep cleans (or 'nukes') all folder generated by the build process (except for ./logs)"""
    check_grail_dir()
    dirs = ["bin", "build", "buildbuild", "external", "libs"]
    d2s = ", ".join(dirs)
    status = input(
        f"This will wipe out the following directories: {d2s}\nAre you sure you want to do this? (y/N)\t"
    )
    if status.lower() != "y":
        logger.info("Canceling nuke")
    logger.debug(f'Deleting the following directories: {dirs}')
    status = subprocess.run(["rm", "-rf"] + dirs)
    if status.returncode != 0:
        exit(status)


def main():
    args, rest = parse()
    logger.debug(f"Command args: {args}")
    logger.debug(f"Leftover args: {rest}")
    if args.subcommand == "generate":
        logger.debug("Entering generation state")
        execute_generate(args, rest)
    elif args.subcommand == "build":
        logger.debug("Entering build state")
        target = process_target(args)
        execute_build(target, args,rest)
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
        raise NotImplementedError(f"command '{args.subcommand}' is not yet supported")


if __name__ == "__main__":
    logger = logging.getLogger(__name__)
    fh = None

    try:
        check_grail_dir(has_logging=False)

        # Create the log folder
        if not os.path.exists("logs"):
            os.makedirs("logs")

        # Create console handler and set message level
        ch = logging.StreamHandler()
        ch.setLevel(console_log_level)

        # Create file handler and set message level
        fh = logging.FileHandler("logs/build.log")
        fh.setLevel(file_log_level)
        logger.setLevel(file_log_level)

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
