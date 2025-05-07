#!/usr/bin/env python3

import os
import sys
import cgi
from datetime import datetime

form = cgi.FieldStorage()
cookie_str = os.environ.get("HTTP_COOKIE", "")
SESSION_DIR = os.environ.get("UPLOAD_PATH", "") + "/sessions"

cookies = {}
for pair in cookie_str.split(";"):
    if "=" in pair:
        key, value = pair.strip().split("=", 1)
        cookies[key] = value

time = ""
formatted = ""
userinfoStr = ""
if ("sessionid" in cookies and "username" in cookies):
    session_file = cookies["sessionid"] + ".txt"
    full_path = os.path.join(SESSION_DIR, session_file)
    if os.path.isfile(full_path):
        with open(full_path, "r", encoding="utf-8") as f:
            userinfo = [line.strip() for line in f]

        if cookies["username"] != userinfo[0]:
            login_status = False
        else:
            login_status = True
            time = datetime.strptime(userinfo[1], "%Y%m%d_%H%M%S")
            formatted = time.strftime("%Y-%m-%d %H:%M:%S")

            new_info = form.getfirst("userinfo", "").strip()
            if new_info:
                with open(full_path, "a", encoding="utf-8") as f_append:
                    f_append.write(new_info + "\n")

            for i in range(2, len(userinfo)):
                userinfoStr += userinfo[i] + "<br>"
            if new_info:
                userinfoStr += new_info + "<br>"

    else:
        login_status = False
else:
    login_status = False

print("Content-Type: text/html; charset=UTF-8\r")
resp = "<html><body>"
if login_status:
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Your info is saved.<br><br></h2>"
else:
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Welcome! <br>Please login first.</h2>"
resp += '<p style="text-align: center; font-family: Arial, sans-serif; font-weight: bold; font-size: 25px; color: black;"><a href="/">Return to Home</a></p>'
resp += "</body></html>"

print("\r")
print(resp)