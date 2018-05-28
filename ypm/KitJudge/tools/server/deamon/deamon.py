import traceback

import MySQLdb
import pika
import sys
import datetime
import socketIO_client
import json

kitSocketHost = 'localhost'
kitSocketPort = 15485
kitMQHost = 'localhost'
kitMQPort = 5672
kitMQUsername = '****'
kitMQPassword = '****'
kitMQQueueName = '****'
kitMQHeartBeat = None
# kitMQHeartBeat = 0
kitDBHost = 'localhost'
kitDBPort = 3306
kitDBUsername = '****'
kitDBPassword = '****'
kitDBName = '****'


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


def kitMQListen(connection, queue_name):
    kitConsole('creating RabbitMQ.Channel(queue={})'.format(queue_name))
    channel = connection.channel()
    channel.queue_declare(queue=queue_name, durable=True)
    kitConsole('channel connected.')
    channel.basic_qos(prefetch_count=1)
    channel.basic_consume(kitConsumer, queue=queue_name)
    channel.start_consuming()


def kitConsumer(channel, method, properites, body):
    data = json.loads(body)
    kitConsole('body = {}'.format(body))
    runid = data['runid']
    kitConsole('[{}] Receive completed judgement'.format(runid))
    connection = MySQLdb.connect(kitDBHost, kitDBUsername, kitDBPassword, kitDBName)
    cursor = connection.cursor()
    try:
        kitConsole('[{}] verdict={}'.format(runid, data['verdict']))
        if data['verdict'] == 0:
            if data.has_key('score'):
                cursor.execute("UPDATE KitStatus SET kitStatusExtraMessage='',kitStatusVerdict=" + str(
                    data['verdict']) + ",kitStatusUsedTime=" + str(data['time']) + ",kitStatusUsedMemory=" + str(
                    data['memo']) + ",kitStatusScore=" + str(data['score']) + " WHERE kitStatusId='" + str(
                    data['runid']) + "'")
            else:
                cursor.execute("UPDATE KitStatus SET kitStatusExtraMessage='',kitStatusVerdict=" + str(
                    data['verdict']) + ",kitStatusUsedTime=" + str(data['time']) + ",kitStatusUsedMemory=" + str(
                    data['memo']) + " WHERE kitStatusId='" + str(data['runid']) + "'")
            if data['user'] != 'root':
                cursor.execute("UPDATE KitProblem SET kitProbAccepted=kitProbAccepted+1 WHERE kitProbId=" + str(data['probid']))
            socket.emit('pub', {'runid': runid, 'case': 'END', 'verdict': data['verdict'], 't': data['time'], 'm': data['memo']})
        else:
            if data.has_key('score'):
                cursor.execute("UPDATE KitStatus SET kitStatusExtraMessage='',kitStatusVerdict=" + str(
                    data['verdict']) + ",kitStatusScore=" + str(data['score']) + " WHERE kitStatusId='" + str(data['runid']) + "'")
            else:
                cursor.execute("UPDATE KitStatus SET kitStatusExtraMessage='',kitStatusVerdict=" + str(
                    data['verdict']) + " WHERE kitStatusId='" + str(data['runid']) + "'")
            socket.emit('pub', {'runid': runid, 'case': 'END', 'verdict': data['verdict']})
        json.dump(data, open('../../../files/userfile/' + data['user'] + '/code/' + str(runid) + '/report', 'w'))
    except Exception as e:
        kitConsole(e.__doc__)
        kitConsole(e.message)
        cursor.execute("UPDATE KitStatus SET kitStatusVerdict=6 WHERE kitStatusId='" + str(data['runid']) + "'")
        traceback.print_exc()
    connection.commit()
    connection.close()
    channel.basic_ack(delivery_tag=method.delivery_tag)


if __name__ == '__main__':
    socket = kitConnectSocket(kitSocketHost, kitSocketPort, {'author': 'judger'})
    rabbitMQConnection = kitConnectRabbitMQ(kitMQHost, kitMQPort, kitMQUsername, kitMQPassword, kitMQHeartBeat)
    kitMQListen(rabbitMQConnection, kitMQQueueName)
