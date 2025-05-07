#!/usr/bin/env python3

import os
import sys
import cgi
from datetime import datetime

FILE_DIR = os.environ.get("UPLOAD_PATH", "") + "/files"
os.makedirs(FILE_DIR, exist_ok=True)
form = cgi.FieldStorage()

for key in form.keys():
    if key == "file":
        file = form["file"]
        if file.filename:
            file_data_path = os.path.join(FILE_DIR, file.filename)
            with open (file_data_path, "wb") as outfile:
                outfile.write(file.file.read())

print("Content-Type: text/html; charset=UTF-8\r")
print("\r")


print("<html><body>")
print("<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Form Data Received and Saved Successfully</h2>")
print('<p style=\"text-align: center; font-family: Arial, sans-serif;  font-weight: bold; font-size: 25px; color: black;\"><a href="/">Return to Home</a></p>')
print("</body></html>")
