<?php
defined('BASEPATH') OR exit('No direct script access allowed');

use PhpAmqpLib\Connection\AMQPStreamConnection;
use PhpAmqpLib\Message\AMQPMessage;

class KitMQHandler
{
    public static function publish($mqconf, $message)
    {
        $connection = new AMQPConnection($mqconf['connection']);
        if (!$connection->connect()) {
            return false;
        }
        $channel = new AMQPChannel($connection);
        $exchange = new AMQPExchange($channel);
        $exchange->setName($mqconf['exchange']);
        $exchange->setType(AMQP_EX_TYPE_DIRECT);
        $exchange->setFlags(AMQP_DURABLE);
        $exchange->declareExchange();
        $exchange->bind($mqconf['route']);
        $queue = new AMQPQueue($channel);
        $queue->setName($mqconf['queue']);
        $queue->setFlags(AMQP_DURABLE);
        $queue->declareQueue();
        $queue->bind($mqconf['route']);
        $exchange->publish($message);
        $connection->disconnect();
        return true;
    }
}