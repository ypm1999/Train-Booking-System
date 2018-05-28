from Strategy.DefaultStrategy import DefaultStrategy
from Strategy.OIStrategy import OIStrategy
from Strategy.Strategy import Strategy

import pika
import sys
import datetime
import socketIO_client
import json
import time
import traceback

kitSocketHost = 'localhost'
kitSocketPort = 15485
kitMQHost = 'localhost'
kitMQPort = 5672
kitMQUsername = '****'
kitMQPassword = '****'
kitMQQueueName = 'kitMQ'
kitMQHeartBeat = 20


def kitConsole(message):
    sys.stderr.write('  ' + datetime.datetime.now().strftime('[%b-%d-%y %H:%M:%S] ') + str(message) + '\n')
    sys.stderr.flush()


def kitConnectSocket(host, port, args):
    kitConsole('connecting sockets(host={}, port={}, args={})'.format(host, port, args))
    connection = socketIO_client.SocketIO(host=host, port=port, params=args)
    kitConsole('socket connected.')
    return connection


def kitConnectRabbitMQ(host, port, username, password, heartbeat):
    kitConsole('connecting RabbitMQ(host={}, port={}, user={}, heartbeat={})'.format(host, port, username, heartbeat))
    connection = pika.BlockingConnection(pika.ConnectionParameters(
        host=host,
        port=port,
        credentials=pika.PlainCredentials(
            username=username,
            password=password
        ),
        heartbeat_interval=heartbeat
    ))
    kitConsole('RabbitMQ connected.')
    return connection

def kitGetRabbitMQChannel(connection, queue_name, consumer):
    kitConsole('creating RabbitMQ.Channel(queue={})'.format(queue_name))
    channel = connection.channel()
    channel.queue_declare(queue=queue_name, durable=True)
    channel.basic_qos(prefetch_count=1)
    channel.basic_consume(consumer, queue=queue_name)
    kitConsole('channel created.')
    return channel

def kitTaskEnded(method):
    global rabbitMQChannel
    global rabbitMQConnection
    kitConsole('telling server the task is processed...')
    while True:
        try:
            if rabbitMQConnection.is_closed or rabbitMQChannel.is_closed:
                raise Exception('closed connection')
            rabbitMQChannel.basic_ack(delivery_tag=method.delivery_tag)
            kitConsole('told.')
            break
        except:
            traceback.print_exc()
            kitConsole('failed to execute channel.basic_ack, trying to recreate channel...')
            while True:
                try:
                    rabbitMQChannel = kitGetRabbitMQChannel(rabbitMQConnection, kitMQQueueName, kitConsumer)
                    break
                except:
                    traceback.print_exc()
                    kitConsole('failed to recreate channel, trying to reconnect server...')
                    while True:
                        try:
                            rabbitMQConnection = kitMQConnector()
                            break
                        except:
                            traceback.print_exc()
                            kitConsole('failed to reconnect server, waiting to reconnect again in 5 second(s)...')
                            time.sleep(5)


def kitConsumer(channel, method, properites, body):
    data = json.loads(body)
    if data['type'] == 'default':
        DefaultStrategy(socket, rabbitMQConnection, kitConsole).process(data)
    elif data['type'] == 'OI':
        OIStrategy(socket, rabbitMQConnection, kitConsole).process(data)
    else:
        Strategy(socket, rabbitMQConnection, kitConsole).process(data)
    kitTaskEnded(method)


def kitErrorConsumer(channel, method, properites, body):
    kitTaskEnded(method)
    channel.stop_consuming()


def kitMQConnector():
    return kitConnectRabbitMQ(kitMQHost, kitMQPort, kitMQUsername, kitMQPassword, kitMQHeartBeat)


def kitSocketConnector():
    return kitConnectSocket(kitSocketHost, kitSocketPort, {'author': 'judger'})

if __name__ == '__main__':
    global rabbitMQChannel
    global rabbitMQConnection
    socket = kitSocketConnector()
    while True:
        failed_last_time = False
        try:
            rabbitMQConnection = kitMQConnector()
            if not failed_last_time:
                rabbitMQChannel = kitGetRabbitMQChannel(rabbitMQConnection, kitMQQueueName, kitConsumer)
                rabbitMQChannel.start_consuming()
            else:
                rabbitMQChannel = kitGetRabbitMQChannel(rabbitMQConnection, kitMQQueueName, kitErrorConsumer)
                rabbitMQChannel.start_consuming()
                rabbitMQChannel = kitGetRabbitMQChannel(rabbitMQConnection, kitMQQueueName, kitConsumer)
                rabbitMQChannel.start_consuming()
                failed_last_time = False
        except Exception as e:
            failed_last_time = True
            kitConsole('connection lost, waiting to reconnect in 5 second(s)...')
            traceback.print_exc()
            time.sleep(5)
