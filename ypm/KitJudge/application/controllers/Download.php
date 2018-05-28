<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Download extends CI_Controller
{
    public function probfile($problemId=null)
    {
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        session_start();
        session_write_close();
        if (!is_numeric($problemId)) {
            show_404();
            exit;
        }
        if (!isset($_SESSION) || !isset($_SESSION['kitUser'])) {
            echo '<script type="text/javascript">alert("Please login first");history.go(-1)</script>';
            exit;
        }
        $filename = "files/probfile/$problemId/download.zip";
        if (!file_exists($filename)) {
            show_404();
            exit;
        }
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $priority = isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0;
        $probdata = $this->KitProblem->kitGetProblemById($problemId, $priority);
        $ProbName = $probdata->row()->kitProbName;
        $this->load->helper('download');
        $data = file_get_contents($filename);
        $name = "$problemId $ProbName.zip";
        force_download($name, $data);
    }
}
