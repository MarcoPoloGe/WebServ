import time

def print_numbers():
    print("<html>")
    print("<head>")
    print("<title>Impression des chiffres</title>")
    print("</head>")
    print("<body>")
    for i in range(1, 6):
        print("<h1>{}</h1>".format(i))
        time.sleep(2)
    print("</body>")
    print("</html>")

print_numbers()

