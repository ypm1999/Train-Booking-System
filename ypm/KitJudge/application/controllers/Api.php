<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Api extends CI_Controller
{
    public function problem()
    {
        if (!isset($_GET)) {
            echo json_encode(array(
                'error' => 'invalid request.'
            ));
        } else {
            $this->load->library('KitInfo');
            $this->load->model('KitProblem');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            echo json_encode($this->KitProblem->kitGetProblemsByVolumn(
                isset($_GET['page']) ? (int)($_GET['page']) : 1,
                KitInfo::$kitInfo['kitProblemsPERPage']
            )->result());
        }
    }

    public function contest($field = null, $contestId = null)
    {
        if ($field == 'board') {
            $this->load->library('KitInfo');
            $this->load->model('KitStatus');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            if (!is_numeric($contestId)) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            return exit(json_encode(array(
                    'verdict' => true,
                    'board' => $this->KitStatus->kitBoard(time(), $contestId))
            ));
        } else {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
    }
}
