<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Time extends CI_Controller
{
    public function index()
    {
        date_default_timezone_set('Asia/Shanghai');
        echo date('Y-m-d H:i:s', time());
    }
}
