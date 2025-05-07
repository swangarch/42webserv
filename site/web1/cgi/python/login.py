#!/usr/bin/env python3

import os
import sys
import cgi
from datetime import datetime
import hashlib

duration = 300

form = cgi.FieldStorage()

DATA_DIR = os.environ.get("UPLOAD_PATH", "") + "/users"
SESSION_DIR = os.environ.get("UPLOAD_PATH", "") + "/sessions"

os.makedirs(DATA_DIR, exist_ok=True)
os.makedirs(SESSION_DIR, exist_ok=True)

def login_verify(form, DATA_DIR, SESSION_DIR):
    fileList = []
    dictList = []

    for filename in os.listdir(DATA_DIR):
        fileList.append(filename)
        fullpath = os.path.join(DATA_DIR, filename)
        if (os.path.isfile(fullpath)):
            with open(fullpath, "r", encoding="utf-8") as f:
                userinfo = []
                for line in f:
                    userinfo.append(line.strip())
                    if (len(userinfo) > 2):
                        return False, "", ""
                if (userinfo[0] == "" or userinfo[1] == ""):
                    continue
                if (form["user"].value == userinfo[0] and form["password"].value == userinfo[1]):
                    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
                    session_data = userinfo[0] + timestamp
                    session_id = hashlib.sha256(session_data.encode()).hexdigest()[:10]

                    sessionpath = os.path.join(SESSION_DIR, session_id + ".txt")
                    with open(sessionpath, "w", encoding="utf-8") as fs:
                        fs.write(userinfo[0] + "\n")
                        fs.write(timestamp + "\n")
                    return True, userinfo[0], session_id
    
    return False, "", ""

        
login_status = login_verify(form, DATA_DIR, SESSION_DIR)

print("Content-Type: text/html; charset=UTF-8\r")
if (login_status[0] == True):
    print("Set-Cookie: username=" + login_status[1] + "; Max-Age=" + str(duration) + "; Path=/\r")
    print("Set-Cookie: sessionid=" + login_status[2] + "; Max-Age=" + str(duration) + "; Path=/\r")
else:
    print("Set-Cookie: username=; Max-Age=0; Path=/\r")
    print("Set-Cookie: sessionid=; Max-Age=0; Path=/\r")


resp = "<html><body>" 
if (login_status[0]):
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Login success<br><br>" + login_status[1] + "<br><br>Your session will expire in " + str(duration / 60) + " mins</h2>"
else:
    resp += "<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Login failed</h2>"
resp += '<p style=\"text-align: center; font-family: Arial, sans-serif;  font-weight: bold; font-size: 25px; color: black;\"><a href="/">Return to Home</a></p>'
resp += "</body></html>"

print("\r")
print(resp, end="")