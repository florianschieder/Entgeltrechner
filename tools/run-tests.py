from os.path import dirname, join
from subprocess import run, PIPE
from sys import exit


def _print_heading():
    print(" ___        _    __ _       _  _                  _                    ")
    print("| __| _ _  | |_ / _` | ___ | || |_  _ _  ___  __ | |_   _ _   ___  _ _ ")
    print("| _| | ' \ |  _|\__. |/ -_)| ||  _|| '_|/ -_)/ _||   \ | ' \ / -_)| '_|")
    print("|___||_||_| \__||___/ \___||_| \__||_|  \___|\__||_||_||_||_|\___||_|  ")



def _run(description: str, args: list) -> bool:
    print(f"running: {description}", end="")
    result = run(
        args,
        stdout=PIPE,
        stderr=PIPE,
    )

    success = result.returncode == 0
    if not success:
        print("... fail")
        print(result.stdout.decode("ansi"))
    else:
        print("... ok")
    return success


def _tool(name: str) -> str:
    return join(dirname(__file__), name)


def main():
    _print_heading()

    failure = False

    success = _run("unit tests",
                   [_tool("run-unittests.bat")])
    if not success:
        failure = True

    success = _run("clang format check",
                   ["python", _tool("clang-format.py"), "--dry-run"])
    if not success:
        failure = True

    if failure:
        exit(1)


if __name__ == "__main__":
    main()