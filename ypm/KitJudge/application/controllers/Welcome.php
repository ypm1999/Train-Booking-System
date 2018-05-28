<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Welcome extends CI_Controller
{
    public function index()
    {
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'welcome',
                'kitSessionId' => session_id()
            )
        );
        setcookie('session', $info['kitSessionId']);
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('welcome/kit-welcome-preload', $info);
        $this->load->view('welcome/kit-welcome', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('welcome/kit-welcome-footage');
        $this->load->view('kit-common-footage');
    }
}
