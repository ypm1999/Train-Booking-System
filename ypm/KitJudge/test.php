<?php
use PhpAmqpLib\Connection\AMQPStreamConnection;
use PhpAmqpLib\Message\AMQPMessage;

class KitInfo
{
    public static $kitInfo = array(
        'kitNavbar' => array(
            'problems' => array('Problems', 'problems'),
            'contests' => array('Contests', 'contests'),
            'status' => array('Status', 'status'),
            'servers' => array('Servers', 'servers')
        ),
        'kitPages' => array(
            'welcome' => array(
                'resources' => array(
                    'modals' => array(
                        'register' => null,
                        'login' => null
                    )
                )
            ),
            'contests' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'servers' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'problems' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'show-problem' => array(
                'resources' => array(
                    'modals' => array(
                        'login' => null
                    )
                )
            ),
            'edit-problem' => array(
                'resources' => array(
                    'modals' => array(
                        'folder' => null
                    )
                )
            ),
            'status' => array(
                'resources' => array(
                    'modals' => array(
                        'status-filter' => null
                    )
                )
            ),
            'status-details' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'edit-contest' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'show-contest' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'submission-contest' => array(
                'resources' => array(
                    'modals' => array()
                )
            ),
            'board-contest' => array(
                'resources' => array(
                    'modals' => array()
                )
            )
        ),
        'kitSupportedLanguages' => array(
            'C++' => ['text/x-c++src', 'cpp'],
            'Java' => ['text/x-java', 'java']
        ),
        'kitSupportedVerdicts' => array(
            'Accepted',
            'Time Limit Exceeded',
            'Runtime Error',
            'Output Limit Exceeded',
            'Memory Limit Exceeded',
            'Rejected',
            'Judgement Failed',
            'Wrong Answer',
            'Queueing',
            'Running',
            'Compilation Error'
        ),
        'kitProblemsPERPage' => 12,
        'kitStatusPERPage' => 20,
        'kitContestPERPage' => 12,
        /* Here are some custom configuration */
        'kitTitle' => 'SJTU Online Judge',
        'kitWelcomeMessage' => 'Enjoy the exciting moments when thinking, coding and solving problems!',
        'kitBasePath' => '/KitJudge',
		'kitAdminEmail' => '"root@acm.sjtu.edu.cn"',
        'kitDatabase' => array(
			'hostname' => 'localhost',
			'username' => 'root',
			'password' => 'acmclass',
			'database' => 'KitJudge',
                'dbdriver' => 'mysqli',
                'dbprefix' => '',
                'pconnect' => false,
                'db_debug' => true,
                'cache_on' => false,
                'cachedir' => '',
                'char_set' => 'utf8',
                'dbcollat' => 'utf8_general_ci',
                'port' => 3306,
        ),
        'kitMQ' => array(
            'connection' => array(
                'host' => 'localhost', // To be deterimed
                'port' => 5672,
				'login' => 'KitOJ',
				'password' => 'acmclass'
            ),
            'exchange' => 'kitMQ',
            'route' => 'kitMQ',
            'queue' => 'kitMQ',
            'admin_port' => 15672
        ),
        'kitCompileMQ' => array(
            'connection' => array(
				'host' => 'localhost',
                'port' => 5672,
				'login' => 'KitOJ',
				'password' => 'acmclass'
            ),
            'exchange' => 'kitCompileMQ',
            'route' => 'kitCompileMQ',
            'queue' => 'kitCompileMQ',
            'admin_port' => 15672
        ),
        'kitSocket' => array(
			'host' => 'localhost', // To be deterimed 
            'port' => 15485
        )
    );
}
$connection = new AMQPConnection(KitInfo::$kitInfo['kitMQ']['connection']);