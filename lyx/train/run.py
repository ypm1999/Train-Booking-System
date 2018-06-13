#!/usr/bin/env python
from flask import Flask, url_for
import main

with main.app.test_request_context():
    print(url_for('index'))
    print(url_for('register'))

if __name__ == '__main__':
    main.app.port = 5000
    main.app.debug = True
    main.app.run()
