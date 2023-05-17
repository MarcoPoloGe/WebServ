#!/usr/bin/env python3

import sys
import os
import cgi
import cgitb
from urllib.parse import parse_qs

# Enable detailed error messages
cgitb.enable()

# Get the query parameters from the environment
query_string = os.environ.get("QUERY_STRING", "")
query_params = parse_qs(query_string)

# Get the values of specific query parameters
name = query_params.get("name", [""])[0]
age = query_params.get("age", [""])[0]
city = query_params.get("city", [""])[0]

# Print necessary headers
# print("Content-Type: text/html\n")

# Print the HTML content
print("<html>")
print("<head>")
print("<title>CGI Test</title>")
print("<style>")
print("body { font-family: Arial, sans-serif; background: linear-gradient(to bottom right, #ffafbd, #ffc3a0); display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }")
print("#container { max-width: 600px; padding: 20px; background-color: #fff; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }")
print("h1 { color: #333; text-align: center; }")
print("p { color: #666; text-align: center; }")
print("form { text-align: center; margin-top: 20px; }")
print("input { margin: 5px; }")
print(".kitty-image { text-align: center; }")
print("</style>")
print("</head>")
print("<body>")
print('<div id="container">')

# Kitty cat image
print('<div class="kitty-image">')
print('<img src="https://placekitten.com/200/200" alt="Kitty Cat">')
print('</div>')

print("<h1>CGI Test</h1>")

# Display the query parameter values
if name:
    print('<p>Hello, <strong>{}</strong>!</p>'.format(name))
else:
    print("<p>No name specified in the query parameter.</p>")

if age:
    print('<p>Your age is <strong>{}</strong>.</p>'.format(age))

if city:
    print('<p>You live in <strong>{}</strong>.</p>'.format(city))

# Form to add additional query parameters
print("<form method='get' action='{}'>".format(os.environ.get("SCRIPT_NAME", "")))
print("<label for='name_input'>Name:</label>")
print("<input type='text' id='name_input' name='name'>")
print("<br>")
print("<label for='age_input'>Age:</label>")
print("<input type='text' id='age_input' name='age'>")
print("<br>")
print("<label for='city_input'>City:</label>")
print("<input type='text' id='city_input' name='city'>")
print("<br>")
print("<input type='submit' value='Add to Query'>")
print("</form>")

print("</div>")
print("</body>")
print("</html>")