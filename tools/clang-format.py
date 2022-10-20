from glob import glob
from sys import argv
from xml.etree import ElementTree

from subprocess import run, PIPE


def _reformat_file(path):
    stdout = run(
        ["clang-format", source_file],
        stdout=PIPE,
        stderr=PIPE,
    ).stdout

    with open(source_file, "w", newline="\n") as fh:
        fh.write(stdout.decode())


if __name__ == "__main__":
    source_files = [
        *glob("src/**/*.cpp", recursive=True),
        *glob("src/**/*.hpp", recursive=True),
        *glob("src/**/*.c", recursive=True),
        *glob("src/**/*.h", recursive=True),
        *glob("tests/**/*.cpp", recursive=True),
        *glob("tests/**/*.hpp", recursive=True),
        *glob("tests/**/*.c", recursive=True),
        *glob("tests/**/*.h", recursive=True),
    ]

    dry_run = "--dry-run" in argv

    for source_file in source_files:
        stdout = run(
            ["clang-format", "--output-replacements-xml", source_file],
            stdout=PIPE,
            stderr=PIPE,
        ).stdout
        replacements = ElementTree.fromstring(stdout)
        children = list(replacements)

        if len(children) > 0:
            if dry_run:
                print(f"would reformat {source_file}")
            else:
                _reformat_file(source_file)
