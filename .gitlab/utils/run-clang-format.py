#!/usr/bin/env python3
"""A wrapper script around clang-format, suitable for linting multiple files
and to use for continuous integration.

This is an alternative API for the clang-format command line.
It runs over multiple files and directories in parallel.
A diff output is produced and a sensible exit code is returned.

SOURCE: https://github.com/Sarcasm/run-clang-format
@modified: matti.ukkola@tum.de

"""

from __future__ import print_function, unicode_literals

import argparse
import difflib
import fnmatch
import io
import multiprocessing
import os
import signal
import subprocess
import sys
import traceback

from functools import partial

DEFAULT_EXTENSIONS = 'c,h,C,H,cpp,hpp,cc,hh,c++,h++,cxx,hxx'


class ExitStatus:
    SUCCESS = 0
    DIFF = 1
    TROUBLE = 2


def list_files(files, recursive=False, extensions=None, exclude=None):
    if extensions is None:
        extensions = []
    if exclude is None:
        exclude = []

    out = []
    for file in files:
        if recursive and os.path.isdir(file):
            for dirpath, dnames, fnames in os.walk(file):
                fpaths = [os.path.join(dirpath, fname) for fname in fnames]
                for pattern in exclude:
                    # os.walk() supports trimming down the dnames list
                    # by modifying it in-place,
                    # to avoid unnecessary directory listings.
                    dnames[:] = [
                        x for x in dnames
                        if
                        not fnmatch.fnmatch(os.path.join(dirpath, x), pattern)
                    ]
                    fpaths = [
                        x for x in fpaths if not fnmatch.fnmatch(x, pattern)
                    ]
                for f in fpaths:
                    ext = os.path.splitext(f)[1][1:]
                    if ext in extensions and not os.path.islink(f):
                        out.append(f)
        else:
            out.append(file)
    return out


def make_diff(file, original, reformatted):
    return list(
        difflib.unified_diff(
            original,
            reformatted,
            fromfile='{}\t(original)'.format(file),
            tofile='{}\t(reformatted)'.format(file),
            n=3))


class DiffError(Exception):
    def __init__(self, message, errs=None):
        super(DiffError, self).__init__(message)
        self.errs = errs or []


class UnexpectedError(Exception):
    def __init__(self, message, exc=None):
        super(UnexpectedError, self).__init__(message)
        self.formatted_traceback = traceback.format_exc()
        self.exc = exc


def run_clang_format_diff_wrapper(clang_args, file):
    try:
        clang_args["file"] = file
        ret = run_clang_format_diff(clang_args)
        return ret
    except DiffError:
        raise
    except Exception as e:
        raise UnexpectedError('{}: {}: {}'.format(file, e.__class__.__name__,
                                                  e), e)


def run_clang_format_diff(clang_args):
    # Open file and save original copy
    try:
        with io.open(clang_args["file"], 'r') as f:
            original = f.readlines()
            #original = fileLines.readlines('\n')
    except IOError as exc:
        raise DiffError(str(exc))

    # Run clang-format
    try:
        result = subprocess.run(' '.join(clang_args.values()), stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                text=True, check=True, shell=True)
        outs = result.stdout.splitlines(keepends=True)
        # print(original)
        # print(outs)
    except OSError as exc:
        raise DiffError(str(exc))

    if result.stderr is not None:
        errs = result.stderr.split('\n')
    else:
        errs = []

    if result.check_returncode():
        raise DiffError("clang-format exited with status {}: '{}'".format(
            os.system.proc.returncode, io.file), errs)

    # Return differences
    return make_diff(clang_args["file"], original, outs), errs


class Colorize():

    def bold(s):
        return '\x1b[1m' + s + '\x1b[0m'

    def cyan(s):
        return '\x1b[36m' + s + '\x1b[0m'

    def green(s):
        return '\x1b[32m' + s + '\x1b[0m'

    def red(s):
        return '\x1b[31m' + s + '\x1b[0m'

    def bold_red(s):
        return '\x1b[1m\x1b[31m' + s + '\x1b[0m'


def colorize_diff(diff_lines):
    for line in diff_lines:
        if line[:4] in ['--- ', '+++ ']:
            yield Colorize.bold(line)
        elif line.startswith('@@ '):
            yield Colorize.cyan(line)
        elif line.startswith('+'):
            yield Colorize.green(line)
        elif line.startswith('-'):
            yield Colorize.red(line)
        else:
            yield line


def print_stdout(message):
    sys.stdout.writelines(message)


def print_diff(diff_lines, use_colors):
    if use_colors:
        diff_lines = colorize_diff(diff_lines)
    print_stdout('\n')
    print_stdout(diff_lines)
    print_stdout('\n')


def print_stderr(prog, message, use_colors):
    error_text = 'ERROR:'
    if use_colors:
        error_text = Colorize.bold_red(error_text)
    print("{}: {} {}".format(prog, error_text, message), file=sys.stderr)


def get_version(command):
    try:
        versionCmd = command + ' --version'
        result = subprocess.run(
            versionCmd, stdout=subprocess.PIPE, text=True, check=True, shell=True)
        major_version = result.stdout.strip().split(' ')[-1].split('.')[0]
        return major_version
    except os.system.CalledProcessError as _:
        print_stderr(versionCmd, result.stderr, use_colors=True)
        return


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        '--clang-format-executable',
        metavar='EXECUTABLE',
        help='path to the clang-format executable',
        default='clang-format')
    parser.add_argument(
        '--extensions',
        help='comma separated list of file extensions (default: {})'.format(
            DEFAULT_EXTENSIONS),
        default=DEFAULT_EXTENSIONS)
    parser.add_argument(
        '-r',
        '--recursive',
        action='store_true',
        help='run recursively over directories')
    parser.add_argument('files', metavar='file', nargs='+')
    parser.add_argument(
        '-q',
        '--quiet',
        action='store_true')
    parser.add_argument(
        '-j',
        metavar='N',
        type=int,
        default=0,
        help='run N clang-format jobs in parallel'
        ' (default number of cpus + 1)')
    parser.add_argument(
        '--color',
        default='auto',
        choices=['auto', 'always', 'never'],
        help='show colored diff (default: auto)')
    parser.add_argument(
        '-e',
        '--exclude',
        metavar='PATTERN',
        action='append',
        default=[],
        help='exclude paths matching the given glob-like pattern(s)'
        ' from recursive search')
    parser.add_argument(
        '--style',
        metavar='PATTERN',
        default='-style=file',
        help='clang-format style')

    args = parser.parse_args()

    # Get clang format version
    major_version = get_version(args.clang_format_executable)
    if major_version is None:
        retcode = ExitStatus.TROUBLE
        return retcode

    clang_args = {
        "cmd": args.clang_format_executable,
        "path": '',
        "style": args.style
    }

    print_stdout("run-clang-format.py: Running " + clang_args["cmd"] + " with \'" + clang_args["style"] + "\' " +
                 "and files: \'" + ' '.join(args.files) + "\'\n")

    # use default signal handling, like diff return SIGINT value on ^C
    # https://bugs.python.org/issue14229#msg156446
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    try:
        signal.SIGPIPE
    except AttributeError:
        # compatibility, SIGPIPE does not exist on Windows
        pass
    else:
        signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    colored_stdout = False
    colored_stderr = False
    if args.color == 'always':
        colored_stdout = True
        colored_stderr = True
    elif args.color == 'auto':
        colored_stdout = sys.stdout.isatty()
        colored_stderr = sys.stderr.isatty()

    retcode = ExitStatus.SUCCESS
    files = list_files(
        args.files,
        recursive=args.recursive,
        exclude=args.exclude,
        extensions=args.extensions.split(','))

    if not files:
        return

    njobs = args.j
    if njobs == 0:
        njobs = multiprocessing.cpu_count() + 1
    njobs = min(len(files), njobs)

    njobs = 1
    if njobs == 1:
        # execute directly instead of in a pool,
        # less overhead, simpler stacktraces
        it = (run_clang_format_diff_wrapper(clang_args, file)
              for file in files)
        pool = None
    else:
        pool = multiprocessing.Pool(njobs)
        it = pool.imap_unordered(
            partial(run_clang_format_diff_wrapper, clang_args), files)

    while True:
        try:
            outs, errs = next(it)
        except StopIteration:
            break
        except DiffError as e:
            print_stderr(parser.prog, str(e), use_colors=colored_stderr)
            retcode = ExitStatus.TROUBLE
            sys.stderr.writelines(e.errs)
        except UnexpectedError as e:
            print_stderr(parser.prog, str(e), use_colors=colored_stderr)
            sys.stderr.write(e.formatted_traceback)
            retcode = ExitStatus.TROUBLE
            # stop at the first unexpected error,
            # something could be very wrong,
            # don't process all files unnecessarily
            if pool:
                pool.terminate()
            break
        else:
            sys.stderr.writelines(errs)
            if outs == []:
                continue
            if not args.quiet:
                print_diff(outs, use_colors=colored_stdout)
            if retcode == ExitStatus.SUCCESS:
                retcode = ExitStatus.DIFF
    return retcode


if __name__ == '__main__':
    sys.exit(main())
