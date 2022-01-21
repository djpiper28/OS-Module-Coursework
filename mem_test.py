import subprocess
import os
import sys

NO_LEAKS = "All heap blocks were freed -- no leaks are possible"
TEST_EXEC_NAME = "dlist-test"
EXEC_NAME = "dlist"

VALGRIND_OPTS = "--leak-check=full --show-leak-kinds=all --track-fds=all"

SYSTEM_TEST_INPUT = "+2048 - 2048adsadasda\n-9+99 -8test\n"

def tests():
    print(f"Running memcheck for {TEST_EXEC_NAME}", )

    p = subprocess.Popen(f"valgrind {VALGRIND_OPTS} ./{TEST_EXEC_NAME}",
                         shell=True,
                         bufsize=64,
                         stdin=subprocess.PIPE,
                         stderr=subprocess.PIPE,
                         stdout=subprocess.DEVNULL)
    p.wait()

    output = ""
    for line in p.stderr:
        output += line.decode("UTF-8")
    print(f">>\n{output}<<")

    if NO_LEAKS in output and p.returncode == 0:
        return 0
    else:
        return 1

def sys_test():
    print(f"Running system test and, memcheck for {EXEC_NAME}", )

    p = subprocess.Popen(f"valgrind {VALGRIND_OPTS} ./{EXEC_NAME}",
                         shell=True,
                         bufsize=64,
                         stdin=subprocess.PIPE,
                         stderr=subprocess.PIPE,
                         stdout=subprocess.DEVNULL)

    p.stdin.write(SYSTEM_TEST_INPUT.encode("UTF-8"))
    p.stdin.close()
    p.wait()

    output = ""
    for line in p.stderr:
        output += line.decode("UTF-8")
    print(f">>\n{output}<<")

    if NO_LEAKS in output and p.returncode == 0:
        return 0
    else:
        return 1

def main():
    a = tests()
    if (a == 1):
        print("Tests failed")
        sys.exit(1)

    a = sys_test()
    if (a == 1):
        print("System test failed")
        sys.exit(1)

    sys.exit(0)

if __name__ == "__main__":
    main()
