<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Servers extends CI_Controller
{
    public function index()
    {
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'servers',
                'kitPageTitle' => 'Servers'
            )
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('servers/kit-servers-preload', $info);
        $this->load->view('servers/kit-servers', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('servers/kit-servers-footage');
        $this->load->view('kit-common-footage');
    }
}
