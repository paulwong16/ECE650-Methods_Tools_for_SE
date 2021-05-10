#!/usr/bin/env python3

#
# Skeleton for ECE650 Test 1.5
# DO NOT DISTRIBUTE
#
# Complete the two provided functions. Implement as much functionality as you
# can. Make sure to not change the interface.
#
# More details are in the question on LEARN.
#
# You are allowed to use a Python IDE and Python Documentation available at
# https://docs.python.org/3/
#
# You are very likely to need to add extra functions and/or classes to organize
# your code. We will mark your code for functionality only. Using extra
# functions is not required, but is highly advised
#


def print_syaml(data):
    """Print data in SYAML format. Raise an exception if this is not possible"""

    if isinstance(data, dict) or isinstance(data, list):
        print("---")
        # YOUR CODE TO PRINT DATA GOES HERE
        print("...")
    else:
        raise Exception("Wrong type of data: " + str(data.__class__))


def parse_syaml(lines):
    if not isinstance(lines, list):
        raise Exception("Expecting a list of strings")
    if not (len(lines) >= 2 and lines[0] == "---\n" and lines[-1] == "...\n"):
        raise Exception("Begin or end document is missing")

    # YOUR CODE GOES HERE
    return None


# This is a simple test function provided from your convenience
if __name__ == "__main__":
    d1 = ["one", "two", "three"]
    print_syaml(d1)

    t1 = ["---\n",
          "one: un\n",
          "two: deux\n",
          "three: trois\n",
          "...\n"]
    d2 = parse_syaml(t1)
    print(d2)
    print_syaml(d2)
