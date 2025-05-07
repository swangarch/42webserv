#!/usr/bin/env python3

import os
import sys
import cgi
from datetime import datetime

DATA_DIR = os.environ.get("UPLOAD_PATH", "") + "/comments"
FILE_DIR = os.environ.get("UPLOAD_PATH", "") + "/files"

os.makedirs(DATA_DIR, exist_ok=True)
os.makedirs(FILE_DIR, exist_ok=True)

form = cgi.FieldStorage()

timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

form_data_path = os.path.join(DATA_DIR, f"form_{timestamp}.txt")

with open(form_data_path, "w", encoding="utf-8") as f:
    f.write(f"Timestamp: {timestamp}\n")
    
    for key in form.keys():
        if key == "file":
            file = form["file"]
            if file.filename:
                file_data_path = os.path.join(FILE_DIR, f"file_{timestamp}_{file.filename}")
                with open (file_data_path, "wb") as outfile:
                    outfile.write(file.file.read())
            continue

        else:
            value = form.getvalue(key)
            if value:
                f.write(f"{key}: {value}\n")
            else:
                f.write(f"{key}: (No data)\n")

def show_files_in_dir(path):
    fileList = []
    dictList = []

    for filename in os.listdir(path):
        fileList.append(filename)
    
    fileList.sort(reverse=True)

    for filename in fileList:
        fullpath = os.path.join(path, filename)
        linedict = dict()
        if (os.path.isfile(fullpath)):
            with open(fullpath, "r", encoding="utf-8") as f:
                for line in f:
                    lineword = line.split(":", 1)
                    linedict[lineword[0]] = lineword[1].strip()
            dictList.append(linedict)

    for dictionary in dictList:

        if "comment" in dictionary:
            print("<div style=\"width: 60%; background-color: #e7e7e7; border-radius: 15px; box-shadow: 0 2px 4px gray; padding: 5px; margin: 20 auto;\">", end="")
            print("<div style=\"width: 100%; text-align: center; font-family: Arial, sans-serif; font-size: 15px; color: black;\">", end="")
            strtime = dictionary["Timestamp"]
            time = datetime.strptime(strtime, "%Y%m%d_%H%M%S")
            formatted = time.strftime("%Y-%m-%d %H:%M:%S")
            print(formatted, "&nbsp;&nbsp;&nbsp;")
            print("Username: ", dictionary["name"], "<br>")
            print("</div>", end="")

            print("<div style=\"width: 95%; background-color: white; text-align: center; font-family: Arial, sans-serif; font-size: 20px; margin: 5 auto;  color: black;\">", end="")
            print(dictionary["comment"], "<br><br>")
            print("</div>", end="")
            print("</div>", end="")
        
    print("<br>", end="")


print("Content-Type: text/html; charset=UTF-8\r")
print("\r")
print("<html><body>")
print("<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Form Data Received and Saved Successfully</h2>")

print(f"<p style=\"text-align: center; font-family: Arial, sans-serif; font-weight: bold; font-size: 25px; color: black;\">All comments</p>")
show_files_in_dir(DATA_DIR)
print('<p style=\"text-align: center; font-family: Arial, sans-serif;  font-weight: bold; font-size: 25px; color: black;\"><a href="/">Return to Home</a></p>')
print("</body></html>")