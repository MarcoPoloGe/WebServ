#!/usr/bin/env python3

import sys
import os

# Print necessary headers
print("Content-Type: text/html\n")

# Print the HTML content
print("<html>")
print("<head>")
print("<title>CGI Test</title>")
print("</head>")
print("<body>")
print("<h1>CGI Test</h1>")
print("<p>This is a test of your CGI implementation.</p>")

# Print server information
print("<h2>Server Information</h2>")
print("<ul>")
print("<li>Server Name: %s</li>" % os.environ.get("SERVER_NAME"))
print("<li>Server Port: %s</li>" % os.environ.get("SERVER_PORT"))
print("<li>Server Software: %s</li>" % os.environ.get("SERVER_SOFTWARE"))
print("</ul>")

# Print client information
print("<h2>Client Information</h2>")
print("<ul>")
print("<li>Client Address: %s</li>" % os.environ.get("REMOTE_ADDR"))
print("<li>Client Host: %s</li>" % os.environ.get("REMOTE_HOST"))
print("<li>Client User-Agent: %s</li>" % os.environ.get("HTTP_USER_AGENT"))
print("</ul>")

print("</body>")
print("</html>")
