#!/usr/bin/env python3

import ctypes

# this script will lead to a segfault
ctypes.string_at(0)