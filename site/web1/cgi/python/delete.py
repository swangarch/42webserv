#!/usr/bin/env python3

import os
import sys
import cgi
import datetime


def delete_files_in_dir(path):
    for filename in os.listdir(path):
        fullpath = os.path.join(path, filename)

        if (os.path.isfile(fullpath)):
            os.remove(fullpath)


DIR = os.environ.get("UPLOAD_PATH", "")
DATA_DIR = os.environ.get("UPLOAD_PATH", "") + "/comments"
FILE_DIR = os.environ.get("UPLOAD_PATH", "") + "/files"

try:
    if os.environ.get("QUERY_STRING", "") == "files=comments":
        delete_files_in_dir(DATA_DIR)

    elif os.environ.get("QUERY_STRING", "") == "files=files":
        delete_files_in_dir(FILE_DIR)

    elif os.environ.get("QUERY_STRING", "") == "files=all":
        delete_files_in_dir(DATA_DIR)
        delete_files_in_dir(FILE_DIR)
    else:
        print("Status: 502 Bad Gateway\r")
        print("Content-Type: text/html; charset=UTF-8\r")
        print("\r")
        print("<html><body>")
        print("<h2>502 Bad Gateway</h2>")
        print('<p><a href="/">Return to Home</a></p>')
        print("</body></html>")
        exit()

    print("Status: 204 No Content\r")
    print("Content-Type: text/html; charset=UTF-8\r")
    print("\r")
    print("<html><body>")
    print("<h2>Data deleted Successfully</h2>")
    print('<p><a href="/">Return to Home</a></p>')
    print("</body></html>")

except:
    print("Status: 502 Bad Gateway\r")
    print("Content-Type: text/html; charset=UTF-8\r")
    print("\r")
    print("<html><body>")
    print("<h2>502 Bad Gateway</h2>")
    print('<p><a href="/">Return to Home</a></p>')
    print("</body></html>")