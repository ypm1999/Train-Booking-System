# KitJudge

**KitJudge** is a functional On-line Judge(KitOJ for abbreviation) under 64-bit Ubuntu 14.04+ OS. 

## Features

KitOJ has the following features:

1. Fast: Codes are specifically optimized and plug-ins are carefully picked.
2. Beautiful: Bootstrap is used to build the whole front-end system.
3. Easy to distribute judge task: Work queue is used to distribute task, which can make the judgment persistent. If there are more than one computer in the network, we can make the use of them by running judges on every computer. Tasks are distribute automatically and smartly.
4. Contest Module: Contest module has been added to this project to support On-line Contest.
5. Sandbox System: A sandbox is used to run the judge task, which prevent the computer from being attack by spiteful code, even in compilation phase.
6. WebSocket-based Dynamic Refreshing: A WebSocket-based method is used to dynamically refresh the status.

## Installation

In this project, some of the plug-ins have not been installed by default. However, you can manually install them easily by using the command ```apt-get``` or ```apt``` (>=16.04) in Ubuntu. And here a brief guide is provided.

## License

This project is under MIT License.