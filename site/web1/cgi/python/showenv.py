#!/usr/bin/env python3

import os

print("Content-Type: text/plain; charset=UTF-8\r\n\r")

for key, value in sorted(os.environ.items()):
    print(f"{key}={value}")
