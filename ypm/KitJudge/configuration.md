# Configuration

This is a step-by-step introduction to the installation of KitOJ.

## Requirements

There are a few limits to the OS:

* Ubuntu 64-bit operating system is installed with version ```>=14.04```.
* ```200MB``` free disk space.

## Conceptions

Before reading detailed installation instructions, a few conceptions should be known first to better understand where and why we use this plug-in.

* Main Server:  ```Main Server``` is designed to provide the interface of the whole system, namely the website server. And it also provides a ```git/ssh``` server to transmit ```problem repository```.
* Database Server: ```Database Server``` is designed to store and transmit all the data used by the ```Main Server``` and the ```Judge Server```.
* Stream Server: ```Stream Server``` is used to dynamic transmit the judge runtime status from ```Judge Server``` to ```Main Server``` and finally to the client.
* Judge Server: ```Judge Server``` is the server to handle all the judge tasks.
* Work Queue Server: ```Work Queue``` server is designed to manage the judge tasks and distribute them to the ```Judge Server``` uniformly.

The five kinds of servers can be run on the same computer. But run ```Judge Server``` on another computer is suggested due to judge task being resource-consuming.

## Platform Installation

Web servers application must be installed firstly to run the website.

### Apache2

```Apache2``` is required to be installed on the ```Main Server```.

```bash
sudo apt-get install apache2
```

### PHP 7.0

```PHP7.0``` is required to be installed on the ```Main Server```.

```bash
sudo apt-get install php
sudo apt-get install php7.0-dev
sudo apt-get install libapache2-mod-php7.0
sudo apt-get install composer
```

### MySQL

#### For Database Server

```bash
sudo apt-get install mysql-server
sudo apt-get install libmysqlclient-dev
sudo apt-get install php-mysql
```

#### For Main Server and Judge Server

```bash
sudo apt-get install libmysqlclient-dev
```

### Python-PIP

```python-pip``` is required to be installed on the ```Main Server```.

```bash
sudo apt-get install python-pip
```

### GIT

#### For Main Server

```bash
sudo apt-get install git
sudo apt-get install openssh-server
```
#### For Judge Server

```bash
sudo apt-get install git
sudo pip install gitpython
```

### VALGRIND*

```valgrind``` is optimally required to be installed on the ```Judge Server``` if memory checkers are needed.

```bash
sudo apt-get install valgrind
```

### NodeJS

```nodejs``` is required to be installed on ```Main Server```.

```bash
sudo apt-get install nodejs-legacy
```

### SocketIO

#### For Main Server

```bash
npm install socketio
```

#### For Judge Server

```bash
sudo pip install socketIO_client
```

### RabbitMQ

```rabbitMQ``` is required to be installed on ```Work Queue Server```.

```bash
wget https://www.rabbitmq.com/rabbitmq-signing-key-public.asc
sudo apt-key add rabbitmq-signing-key-public.asc
rm rabbitmq-signing-key-public.asc
sudo apt-get install php7.0-bcmath
sudo apt-get install rabbitmq-server
sudo apt-get install librabbitmq-dev
sudo pip install pika
```

### Install Requirements for PHP

```bash
sudo composer install
```

## Platform Configuration

### Git on Main Server

```bash
cd files/probfile
git init
git add .
git config user.name "<your name>"
git config user.email "<your email>"
git commit -m "Initial commit."
cd ~
ln -s /path/to/files/probfile probfile
```

### PHP on Main Server

Add the following one line to ```/etc/php/7.0/apache2/php.ini```

```bash
extension=amqp.so
```

### Apache2 on Main Server

Change ```/etc/apache2/apache2.conf```

```bash
<Directory /var/www>
  ...
  AllowOverride **All** # modify here from None to All
  ...
</Directory>
```

Enable ```rewrite``` module in ```apache2```:

```bash
cd /etc/apache2/mods-enabled/
sudo ln -s ../mods-available/rewrite.load rewrite.load
```

### RabbitMQ on Work Queue Server

```bash
sudo rabbitmqctl add_user <your MQ username> <your MQ password>
sudo rabbitmqctl set_user_tags <your MQ username> administrator 
sudo rabbitmqctl set_permissions -p '/' <your MQ username> ".*" ".*" ".*"
```

### MySQL on Database Server

First login into MySQL server:

```bash
mysql -u <your mysql root username> -p
# input your mysql root password
```

Then execute the following commands:

```mysql
CREATE USER '<your mysql kitoj username>'@'%' IDENTIFIED BY '<your mysql kitoj password>';
GRANT ALL PRIVILEGES ON KitJudge.* TO '<your mysql kitoj username>'@'%' WITH GRANT OPTION;
```

Last, modify the MySQL ```conf``` file (Usually, that is ```/etc/mysql/mysql.conf.d/mysqld.cnf```):

```bash
# Change "bind-address=127.0.0.1" to "# bind-address=127.0.0.1"
```

## Website Configuration

#### KitInfo.php On Main Server

```KitInfo.php```(In folder ```application/libraries/```) is a file that contains all needed information used by ```Main Server```.  You can fill out each variable by substituting ```**``` to the actual value.

#### judger.py & Strategy.py On Judge Server 

You should subsitute all of the double starts(namely ```**```) in the file ```judger.py``` (In the folder ```tools/server/judger```) and in the file ```Strategy.py``` (In the folder ```tools/server/judger/Strategy/```)

#### deamon.py On Main Server

You should subsitute all the double starts(namely ```**```) in the file ```deamon.py``` (In the folder ```tools/deamon/```).

#### setup.py On Main Server

```setup.py``` is a tool to initialize the database of KitOJ. ```**``` are needed to be replaced by the actual value.

## Problem Configuration

### Default Problem

A problem consists of ```title```, ```source```, ```type``` and a ```JSON``` configuration file.

#### Title, Source and Type

You can specify the title, source and type parameter in the problem configuration page.

#### JSON Configuration File

A default ```JSON``` configuration file for ```A + B Problem``` is listed below:

```json
{
	"include": {
	},
	"tests": [{
		"repeat": 10,
		"judger": {
			"type": "default",
			"path": "spjInt",
			"lang": "exec"
		},
		"require": {
			"data/data[$INDEX].in": {
				"stage": 0,
				"target": "input.txt"
			},
			"data/data[$INDEX].out": {
				"stage": 1,
				"target": "output_std.txt"
			}
		},
		"input": "input.txt",
		"stdout": "output_std.txt",
		"output": "output.txt",
		"limit": {
			"C++": {
				"time": 1000,
				"memory": 65536
			}
		}
	}],
	"files": {
		"Code": {
			"name": "code.cpp",
			"lang": ["C++"]
		}
	},
	"main": "Code",
	"Module": {
		"Problem Statement": {
            "loader": "markdown",
            "style": "default",
            "content": "problem_statement.txt"
        },
        "Input Description": {
            "loader": "markdown",
            "style": "default",
            "content": "input_description.txt"
        },
        "Output Description": {
            "loader": "markdown",
            "style": "default",
            "content": "output_description.txt"
        },
        "Examples": {
            "style": "sample",
			"content": {
				"Input 1": {
					"path": "data/sample1.in"
				},
				"Output 1": {
					"path": "data/sample1.out"
				},
				"Input 2": {
					"path": "data/sample2.in"
				},
				"Output 2": {
					"path": "data/sample2.in"
				}
			}
        },
        "Hints": {
            "loader": "markdown",
            "style": "default",
            "content": "hints.txt"
        },
        "Limits": {
            "style": "limits"
        }
	}
}
```

And the API manual is listed below:

* ```include```: If the program needs some libraries to compile, then this field can be used to copy other libraries to the temporary working folder. For example, if you want to include a custom libraries uploaded in the problem repository folder, then you may add ```"include": ["<your library>"]``` to the configuration file.

* ```tests```: This field is used to tell the judge what the tests look like. The field ```tests``` is an array consisting of several kinds of tests, each of which is a dictionary. In this dictionary, the following keys are settled:

  * ```repeat```: Represent how many tests of this kind are there. This value may be used to substitute ```[$INDEX]``` in the ```require``` field.

  * ```judger```: Represent the special judge. This field has three subfields:

    * ```type```: Indicate the type of the special judge. This field is optional and it can be either be ```default``` or be ```custom```.
    * ```path```: Indicate the path to the special judge. If the ```type``` of the special judge is ```default``` then the path is the name of the default special judge.
    * ```lang```: Indicate the language of the judge. Currently, it can either be ```exec``` or ```C++```. If the field is not ```exec``` then the judge will be compiled firstly. But this will make a judge task time-consuming.

  * ```input```: Indicate the input file.

  * ```output```: Indicate the output file.

  * ```stdout```: Indicate the standard output file.

  * ```limit```: Indicate the limits of this kind of tests. The value of this field is an array, the element of which has two fields:

    * ```time```: The time limits.
    * ```memory```: The memory limits.

    And the key is the name of the language.

* ```files```: Indicate the code to be inputed or uploaded. The value of this field is an array, the element of which is a dictionary consisting of the following fields:

  * ```name```: Indicate the name of the file after being copied to the work path.
  * ```lang```: Indicate the language of the file(code).

  The key is the name of the file in the submit page.main: Indicate the key main file.

* ```main```: Indicate the key main file.

* ```Module```: This file is used to configure the problem statement page, including several sections. Each section has a title specified in the key and there are three styles of the module:

  * ```"style": "default"```: A ```default``` section is usually loaded with a markdown parser and alternative options are ```link``` which shows the ```content``` using ```<a>``` label and ```html``` which will show the content in HTML directly.
  * ```"style": "sample"```: The ```content``` field of ```sample``` section is an array consisting of several elements, the key of which is the title and the value has a field ```path``` to indicate the path to the file.
  * ```"style": "limits"```: The ```content``` field of ```limits``` section is automatically generated with the information filled in the ```tests``` section.
