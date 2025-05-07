#!/usr/bin/env python3

import os
import sys
import cgi
from   datetime import datetime

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
if ("sessionid" in cookies.keys() and "username" in cookies.keys()):
    session_file = cookies["sessionid"] + ".txt"
    for filename in os.listdir(SESSION_DIR):
        full_path = os.path.join(SESSION_DIR, session_file)
        if (os.path.isfile(full_path)):
            with open(full_path, "r", encoding="utf-8") as f:
                userinfo = []
                for line in f:
                    userinfo.append(line.strip())
                if (cookies["username"] != userinfo[0]):
                    login_status = False
                else:
                    time = datetime.strptime(userinfo[1], "%Y%m%d_%H%M%S")
                    formatted = time.strftime("%Y-%m-%d %H:%M:%S")

                    for i in range(len(userinfo)):
                        if (i > 1):
                            userinfoStr += userinfo[i]
                            userinfoStr += "<br>"

                    login_status = True
                    break
else:
    login_status = False

print("Content-Type: text/html; charset=UTF-8\r")
resp = "<html><body>" 
if (login_status):
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Welcome back! <br> Your cookie is recognized.<br><br>" + cookies["username"] + "<br><br>Your last login is " + formatted + "<br><br>" + "Your info:<br>" + userinfoStr + "</h2>"
else:
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Welcome! <br> No cookie is recognized.</h2>"
resp += '<p style=\"text-align: center; font-family: Arial, sans-serif;  font-weight: bold; font-size: 25px; color: black;\"><a href="/">Return to Home</a></p>'
resp += "</body></html>"

print("\r")
print(resp)