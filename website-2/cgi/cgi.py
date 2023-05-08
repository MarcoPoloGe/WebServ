#!/usr/bin/env python3
import os
from datetime import datetime

t = datetime.now().strftime("%H:%M:%S")

# Generate the CGI response
print('HTTP/1.1 200 OK\n\n')
print('<html><head><title>CGI Response</title></head>')
print('<body><h1>CGI Response</h1>')
print('<p>CGI script has been executed!</p>')
print(f'<p>The time is now : {t}')
print('</body></html>')
