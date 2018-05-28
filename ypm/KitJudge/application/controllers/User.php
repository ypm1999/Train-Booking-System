<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class User extends CI_Controller
{
    public function register()
    {
        if (!isset($_POST) || !isset($_POST['username']) || !isset($_POST['password']) || !isset($_POST['email']) || !$this->input->is_ajax_request()) {
            echo json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            ));
        } else {
            $username = $_POST['username'];
            $password = $_POST['password'];
            $email = $_POST['email'];
            $session = $_POST['session'];
            if ($session != $_COOKIE['session']) {
                echo json_encode(array(
                    'verdict' => false,
                    'message' => 'session expired.',
                ));
            } else {
                $this->load->model('KitUser');
                $this->load->library('KitInfo');
                $this->load->database(KitInfo::$kitInfo['kitDatabase']);
                $verdict = $this->KitUser->kitInsertUser($username, $password, $email);
                if ($verdict['verdict']) {
                    echo json_encode(array(
                        'verdict' => true
                    ));
                } else {
                    echo json_encode(array(
                        'verdict' => false,
                        'message' => $verdict['message']
                    ));
                }
            }
        }
    }

    public function login()
    {
        if (!isset($_POST) || !isset($_POST['username']) || !isset($_POST['password']) || !isset($_POST['session']) || !$this->input->is_ajax_request()) {
            echo json_encode(array(
                'verdict' => false,
                'message' => 'session expired.'
            ));
        } else {
            $username = $_POST['username'];
            $password = $_POST['password'];
            $session = $_POST['session'];
            if ($session != $_COOKIE['session']) {
                echo json_encode(array(
                    'verdict' => false,
                    'message' => 'session expired.',
                ));
            } else {
                $this->load->model('KitUser');
                $this->load->library('KitInfo');
                $this->load->database(KitInfo::$kitInfo['kitDatabase']);
                $verdict = $this->KitUser->kitVerdictUser($username, $password);
                if ($verdict['verdict']) {
                    session_start();
                    $_SESSION['kitUser'] = array(
                        'name' => $verdict['username'],
                        'priority' => $verdict['priority']
                    );
                    session_write_close();
                    echo json_encode(array(
                        'verdict' => true
                    ));
                } else {
                    echo json_encode(array(
                        'verdict' => false,
                        'message' => $verdict['message']
                    ));
                }
            }
        }
    }

    public function logout()
    {
        session_start();
        unset($_SESSION['kitUser']);
        session_write_close();
    }

    public function update()
    {
        if (!isset($_POST) || !isset($_POST['username']) || !isset($_POST['oldPassword']) || !isset($_POST['session']) || !$this->input->is_ajax_request()) {
            echo json_encode(array(
                'verdict' => false,
                'message' => 'session expired1.'
            ));
        } else {
            $username = $_POST['username'];
            $oldPassword = $_POST['oldPassword'];
            $newPassword = $_POST['newPassword'] != '' ? $_POST['newPassword'] : null;
            $email = $_POST['email'] != '' ? $_POST['email'] : null;
            $session = $_POST['session'];
            if ($session != $_COOKIE['session']) {
                echo json_encode(array(
                    'verdict' => false,
                    'message' => 'session expired.',
                ));
            } else {
                $this->load->model('KitUser');
                $this->load->library('KitInfo');
                $this->load->database(KitInfo::$kitInfo['kitDatabase']);
                $verdict = $this->KitUser->kitVerdictUser($username, $oldPassword);
                if ($verdict['verdict']) {
                    $verdict = $this->KitUser->kitUpdateUser($username, $newPassword, $email);
                    if ($verdict['verdict']) {
                        echo json_encode(array(
                            'verdict' => true
                        ));
                    }
                    else {
                        echo json_encode(array(
                            'verdict' => false,
                            'message' => $verdict['message']
                        ));
                    }
                } else {
                    echo json_encode(array(
                        'verdict' => false,
                        'message' => $verdict['message']
                    ));
                }
            }
        }
    }

    public function index()
    {
        $this->load->model('KitUser');
        $this->load->library('KitInfo');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        session_start();
        session_write_close();
        if (!isset($_SESSION['kitUser'])) {
            $kitBasePath = KitInfo::$kitInfo['kitBasePath'];
            echo "<script> {window.alert('Please login first.');location.href='$kitBasePath'} </script>";
            return;
        }
        $username = $_SESSION['kitUser']['name'];
        $userInfo = $this->KitUser->kitUserInfo($username);
        if (!$userInfo['verdict']) {
            $kitBasePath = KitInfo::$kitInfo['kitBasePath'];
            logout();
            echo "<script> {window.alert('Please login first.');location.href='$kitBasePath'} </script>";
            return;
        }
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'user',
                'kitPageTitle' => 'Profile',
                'kitSessionId' => session_id()                
            ),
            $userInfo
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('user/kit-user-preload', $info);
        $this->load->view('user/kit-user', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('user/kit-user-footage');
        $this->load->view('kit-common-footage');
    }
}
