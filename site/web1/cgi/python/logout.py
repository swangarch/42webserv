#!/usr/bin/env python3

print("Content-Type: text/html; charset=UTF-8\r")
print("Set-Cookie: username=; sessionid=; Max-Age=0; Path=/\r")

resp = "<html><body>" 
resp += '<h2 style=\"text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;\">Cookie unset.<br><br><a href="/">Return to Home</a></p></h2>'
resp += "</body></html>"
print("\r")

print(resp, end="")