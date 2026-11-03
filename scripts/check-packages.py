#!/usr/bin/env python3

"""Check that every package build.sh would install exists in a FreeBSD package repository.

FreeBSD removes and renames packages every quarter; this catches that
before a build fails halfway through on a FreeBSD machine.

usage: check-packages.py <packagesite.yaml> <settings/packages.* file>...

packagesite.yaml is inside https://pkg.freebsd.org/FreeBSD:<major>:amd64/quarterly/packagesite.pkg
(a zstd-compressed tar archive)
"""

import json
import re
import sys

site, lists = sys.argv[1], sys.argv[2:]
names, origins = set(), set()
with open(site) as f:
    for line in f:
        p = json.loads(line)
        names.add(p["name"])
        origins.add(p["origin"])

total = 0
for path in lists:
    with open(path) as f:
        for n, line in enumerate(f, 1):
            # Same filter as packages() in build.sh: sed '/^#/d;/\!i386/d;/^cirrus:/d;/^https:/d' | xargs
            if re.match(r"#|cirrus:|https:", line) or "!i386" in line:
                continue
            for pkg in line.split():
                if pkg not in names and pkg not in origins:
                    print(f"{path}:{n}: '{pkg}' is not in the repository")
                    total += 1

if total:
    print(f"{total} package(s) missing")
    sys.exit(1)
print("All packages found")
