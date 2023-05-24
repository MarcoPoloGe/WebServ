import unittest
import requests

get_endpoint = "/index.html"
get_cgi = "/cgi/time.py"
# post_endpoint = "/post.py"

class TestWebserver(unittest.TestCase):
    BASE_URL = 'http://localhost:8080'

    def test_get(self):
        response = requests.get(f'{self.BASE_URL}')
        self.assertEqual(response.status_code, 200)

    def test_404(self):
        response = requests.get(f'{self.BASE_URL}/notfoubddd')
        self.assertEqual(response.status_code, 404)

    def test_get2(self):
        response = requests.get(f'{self.BASE_URL}{get_endpoint}')
        self.assertEqual(response.status_code, 200)

    def test_get3(self):
        response = requests.get(f'{self.BASE_URL}{get_cgi}')
        self.assertEqual(response.status_code, 200)
    #
    # def test_post(self):
    #     data = {"name": "eduardo"}
    #     response = requests.post(f'{self.BASE_URL}{post_endpoint}', json=data)
    #     self.assertEqual(response.status_code, 200)
    #
    # def test_delete(self):
    #     response = requests.delete(f'{self.BASE_URL}/upload/test.txt')
    #     self.assertEqual(response.status_code, 202)
    #
    # def test_bad_delete(self):
    #     response = requests.delete(f'{self.BASE_URL}/upload/dontexist.txt')
    #     self.assertEqual(response.status_code, 404)


if __name__ == '__main__':
    unittest.main()