<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Problems extends CI_Controller
{
    public function submit()
    {
        if (!isset($_POST) || !isset($_POST['session'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        $this->load->library('KitInfo');
        $this->load->library('KitFile');
        $this->load->model('KitProblem');
        $this->load->model('KitStatus');
        session_start();
        session_write_close();
        if (!isset($_SESSION['kitUser']) || $_POST['session'] != $_COOKIE['session']) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'session expired.',
            )));
        }
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $curTime = null;
        if (isset($_POST['cid'])) {
            if (!isset($_POST['tag'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $this->load->model('KitContest');
            $contest = $this->KitContest->kitGetContestById($_POST['cid']);
            if (empty($contest->result_array())) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $contest = $contest->row();
            $curTime = time();
            if ($_SESSION['kitUser']['priority'] < 2 && $contest->kitContestHidden) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            if ($curTime < strtotime($contest->kitContestStart) || $curTime >= strtotime($contest->kitContestEnd)) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Contest has been already ended.'
                )));
            }
            $curTime = $curTime - strtotime($contest->kitContestStart);
            $this->load->model('KitContestProblem');
            $contestProb = $this->KitContestProblem->kitGetProblemByContestId($_POST['cid']);
            if (empty($contestProb->result_array())) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $problem = $this->KitProblem->kitGetProblemById($contestProb->row()->kitProbId, 5);
            if (!file_exists('files/probfile/' . $contestProb->row()->kitProbId . '/problem.json')) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            if (empty($problem->result_array())) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $probtype = $problem->row()->kitProbType;
            $problem = json_decode(file_get_contents('files/probfile/' . $contestProb->row()->kitProbId . '/problem.json'));
            $commit_data['probid'] = $contestProb->row()->kitProbId;
        } else {
            if (!isset($_POST['prob'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $problem = $this->KitProblem->kitGetProblemById($_POST['prob'], $_SESSION['kitUser']['priority']);
            if (!file_exists('files/probfile/' . $_POST['prob'] . '/problem.json')) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            if (empty($problem->result_array())) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $probtype = $problem->row()->kitProbType;
            $problem = json_decode(file_get_contents('files/probfile/' . $_POST['prob'] . '/problem.json'));
            $commit_data['probid'] = $_POST['prob'];

        }
        $submit_lang = null;
        $submit_length = 0;
        foreach ($problem->files as $caption => $conf) {
            if (!isset($_POST[$caption]) || !isset($_POST[$caption . 'lang'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            if ($submit_lang == null) {
                $submit_lang = $_POST[$caption . 'lang'];
            } else {
                $submit_lang = 'Mixed';
            }
            $submit_length += strlen($_POST[$caption]);
        }
        $commit_data = array_merge($commit_data, array(
            'type' => $probtype,
            'runid' => $this->KitStatus->kitAddStatus(
                $_SESSION['kitUser']['name'],
                $commit_data['probid'],
                $submit_lang,
                $submit_length,
                isset($_POST['cid']) ? $_POST['cid'] : null,
                isset($_POST['tag']) ? $_POST['tag'] : null,
                $curTime
            ),
            'user' => $_SESSION['kitUser']['name'],
            'version' => KitFile::kitGetVersion()
        ));
        if ($_SESSION['kitUser']['name'] != 'root') {
            $this->KitProblem->kitIncreaseSubmitted($commit_data['probid']);
        }
        if (!is_dir('files/userfile/' . $_SESSION['kitUser']['name'] . '/code/' . $commit_data['runid'])) {
            mkdir('files/userfile/' . $_SESSION['kitUser']['name'] . '/code/' . $commit_data['runid'], 0777, true);
            chmod('files/userfile/' . $_SESSION['kitUser']['name'] . '/code/' . $commit_data['runid'], 0777);
        }
        if ($commit_data['runid'] == -1) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Cannot insert such status.'
            )));
        }
        foreach ($problem->files as $caption => $file) {
            if (!isset($_POST[$caption])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            file_put_contents('files/userfile/' . $_SESSION['kitUser']['name'] . '/code/' . $commit_data['runid'] . '/' . $caption, $_POST[$caption]);
            file_put_contents('files/userfile/' . $_SESSION['kitUser']['name'] . '/code/' . $commit_data['runid'] . '/' . $caption . 'lang', $_POST[$caption . 'lang']);
            $commit_data = array_merge($commit_data, array($caption => $_POST[$caption]));
            $commit_data = array_merge($commit_data, array($caption . 'lang' => $_POST[$caption . 'lang']));
        }
        $this->load->library('KitMQHandler');
        if (!KitMQHandler::publish(KitInfo::$kitInfo['kitMQ'], json_encode($commit_data))) {
            echo json_encode(array(
                'verdict' => false,
                'message' => 'Cannot publish judge task.'
            ));
        } else {
            echo json_encode(array(
                'verdict' => true,
                'runid' => $commit_data['runid']
            ));
        }
    }

    public function index()
    {
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        session_start();
        session_write_close();
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'problems',
                'kitPageTitle' => 'Problem Set',
                'kitCurPage' => isset($_GET['page']) ? (int)($_GET['page']) : 1,
                'kitProblems' => $this->KitProblem->kitGetProblemsByVolumn(
                    isset($_GET['page']) ? (int)($_GET['page']) : 1,
                    KitInfo::$kitInfo['kitProblemsPERPage'],
                    isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0
                ),
                'kitProblemsTotalPages' => max((int)floor(
                    (
                        $this->KitProblem->kitCountProblems(isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0)
                        + KitInfo::$kitInfo['kitProblemsPERPage'] - 1
                    ) / KitInfo::$kitInfo['kitProblemsPERPage']),
                    1
                )
            )
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('problems/kit-problems-preload', $info);
        $this->load->view('problems/kit-problems', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('problems/kit-problems-footage');
        $this->load->view('kit-common-footage');
    }

    public function editdata()
    {
        if (!isset($_POST) || !isset($_POST['title'])
            || !isset($_POST['source']) || !isset($_POST['conf']) || !isset($_POST['type']) || !isset($_POST['hidden'])
            || !isset($_POST['session'])
        ) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        if ($_POST['session'] != $_COOKIE['session']) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.' . $_POST['session'] . ' ' . $_COOKIE['session']
            )));
        }
        session_start();
        session_write_close();
        if (!isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $isHidden = ($_POST['hidden'] == '1');
        if (!isset($_POST['prob'])) {
            if (($probId = $this->KitProblem->kitInsertProblem($_POST['title'], $_POST['type'], $_POST['source'], $isHidden)) < 0) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Cannot insert such problem.'
                )));
            }
            if (!is_dir("files/probfile/$probId")) {
                if (!mkdir("files/probfile/$probId", 0777, true)) {
                    exit(json_encode(array(
                        'verdict' => false,
                        'message' => 'Cannot establlish the folder of the problem.'
                    )));
                }
                chmod("files/probfile/$probId", 0777);
            }
        } else {
            $probId = $_POST['prob'];
            if (!$this->KitProblem->kitIsExistProblemById($_POST['prob'], $_SESSION['kitUser']['priority'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            if (!is_dir('files/probfile/' . $_POST['prob'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $this->KitProblem->kitUpdateProblem($_POST['prob'], $_POST['title'], $_POST['type'], $_POST['source'], $isHidden);
        }
        file_put_contents("files/probfile/$probId/problem.json", $_POST['conf']);
        $this->load->library('KitFile');
        if (!KitFile::kitCommitChange()) {
            echo json_encode(array('verdict' => false, 'message' => 'Commitment failed.'));
        } else {
            echo json_encode(array('verdict' => true));
        }
    }

    public function edit($problemId = null)
    {
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            show_404();
        }
        $info = array_merge(
            array(
                'kitView' => 'edit-problem',
                'kitSessionId' => session_id(),
                'kitPageTitle' => 'Edit Problem ' . $problemId
            ),
            KitInfo::$kitInfo
        );
        setcookie('session', $info['kitSessionId']);
        if ($problemId != null) {
            if (!is_numeric($problemId)) {
                show_404();
            }
            $this->load->model('KitProblem');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            $result = $this->KitProblem->kitGetProblemById($problemId, $_SESSION['kitUser']['priority'])->result_array();
            if (empty($result)) {
                show_404();
            }
            $info = array_merge($info, array(
                'kitProblem' => $result[0],
                'kitProbId' => $result[0]['kitProbId']
            ));
        } else {
            $this->load->model('KitProblem');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            $problemId = $this->KitProblem->kitNextProblemId($_SESSION['kitUser']['priority']);
            // $problemId = 1000 + $this->KitProblem->kitCountProblems($_SESSION['kitUser']['priority']);
            $info = array_merge($info, array('kitProbId' => $problemId));
        }
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('problems/edit/kit-edit-problem-preload', $info);
        $this->load->view('problems/edit/kit-edit-problem', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('problems/edit/kit-edit-problem-footage');
        $this->load->view('kit-common-footage');
    }

    public function show($problemId)
    {
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        $this->load->model('KitStatus');
        session_start();
        session_write_close();
        $priority = isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0;
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        if ($this->isProblemIdValid($problemId, $priority) != null) {
            show_404();
        }
        $probdata = $this->KitProblem->kitGetProblemById($problemId, $priority);
        if (empty($probdata->result_array())) {
            show_404();
        }
        if (!file_exists("files/probfile/$problemId/problem.json")) {
            show_404();
        }
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'show-problem',
                'kitProblem' => $probdata->row(),
                'kitPageTitle' => $probdata->row()->kitProbName,
                'kitSessionId' => session_id(),
                'kitProblemConf' => json_decode(file_get_contents("files/probfile/$problemId/problem.json"), true)
            )
        );
        setcookie('session', $info['kitSessionId']);
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('problems/show/kit-show-problem-preload', $info);
        $this->load->view('problems/show/kit-show-problem', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('problems/show/kit-show-problem-footage');
        $this->load->view('kit-common-footage');
    }

    private function isProblemIdValid($problemId, $priority)
    {
        if (!is_numeric($problemId)) {
            return "Invalid request.";
        }
        if (!$this->KitProblem->kitIsExistProblemById($problemId, $priority)) {
            return "Invalid request.";
        }
        if (!file_exists("files/probfile/$problemId/problem.json")) {
            return "Invalid request.";
        }
        return null;
    }

    public function tree($problemId = null)
    {
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            show_404();
        }
        $this->load->library('KitFile');
        echo json_encode(KitFile:: kitListTreeView('files/probfile/' . $problemId, '', 'ProbConf'), JSON_PRETTY_PRINT);
    }

    public function download($problemId = null)
    {
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            exit('Invalid request');
        }
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit($message);
        } else if (!isset($_GET) || !isset($_GET['url']) || !$this->isUrlValid($_GET['url'])) {
            exit('Invalid request');
        } else if (!file_exists("files/probfile/$problemId/" . $_GET['url'])) {
            exit('Invalid request');
        } else {
            $handle = fopen("files/probfile/$problemId/" . $_GET['url'], "r") or exit("Invalid request");
            header('Content-Type:application/octet-stream');
            header('Content-Disposition: attachment; filename="' . pathinfo("files/probfile/$problemId/" . $_GET['url'])['basename'] . '"');
            if ($handle) {
                while (!feof($handle)) {
                    $buffer = fgets($handle, 1048576);
                    echo $buffer;
                }
                fclose($handle);
            }
        }
    }

    private static function isUrlValid($str)
    {
        return !strpos($str, '../');
    }

    public function folder($problemId = null)
    {
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (!isset($_POST) || !isset($_POST['url']) || !isset($_POST['name'])
            || !$this->isUrlValid($_POST['url'] . $_POST['name'])
        ) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (file_exists("files/probfile/$problemId" . $_POST['url'] . $_POST['name'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'This file is already existed.'
            )));
        } else {
            if (!mkdir("files/probfile/$problemId" . $_POST['url'] . $_POST['name'], 0777, true)) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Cannot establish such folder.'
                )));
            }
            if (!chmod("files/probfile/$problemId" . $_POST['url'] . $_POST['name'], 0777)) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Cannot change the rights of such folder.'
                )));
            }
            $this->load->library('KitFile');
            if (KitFile::kitCommitChange()) {
                echo(json_encode(array(
                    'verdict' => true
                )));
            } else {
                echo(json_encode(array(
                    'verdict' => false,
                    'message' => 'Commit failed.'
                )));
            }
        }
    }

    public function remove($problemId)
    {
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (!isset($_POST) || !isset($_POST['url']) || !$this->isUrlValid($_POST['url'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (!file_exists("files/probfile/$problemId" . $_POST['url'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'The file/folder is not existed.'
            )));
        } else {
            $this->load->library('KitFile');
            if (is_dir("files/probfile/$problemId" . $_POST['url'])) {
                if (!KitFile::kitRemoveTree("files/probfile/$problemId" . $_POST['url'])) {
                    exit(json_encode(array(
                        'verdict' => false,
                        'message' => 'Cannot erase such folder.'
                    )));
                }
            } else {
                if (!unlink("files/probfile/$problemId" . $_POST['url'])) {
                    exit(json_encode(array(
                        'verdict' => false,
                        'message' => 'Cannot erase such folder.'
                    )));
                }
            }
            if (KitFile::kitCommitChange()) {
                echo(json_encode(array(
                    'verdict' => true
                )));
            } else {
                echo(json_encode(array(
                    'verdict' => false,
                    'message' => 'Commit failed.'
                )));
            }
        }
    }

    public function upload($problemId)
    {
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        }
        $this->load->library('KitInfo');
        $this->load->library('KitFile');
        $this->load->model('KitProblem');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (!isset($_POST) || !isset($_POST['url']) || !$this->isUrlValid($_POST['url'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'Invalid request.'
            )));
        } else if (!file_exists("files/probfile/$problemId" . $_POST['url'])) {
            exit(json_encode(array(
                'verdict' => false,
                'message' => 'The file/folder is not existed.'
            )));
        } else {
            if (file_exists("files/probfile/$problemId" . $_POST['url'])) {
                if (isset($_FILES['file_data']['tmp_name'])) {
                    if ($_POST['new'] == 'true') {
                        if (!move_uploaded_file($_FILES['file_data']['tmp_name'], "files/probfile/$problemId" . $_POST['url'] . '/' . $_FILES['file_data']['name'])) {
                            exit(json_encode(array('error' => 'Cannot move the uploaded file')));
                        } else if (!KitFile:: kitCommitChange()) {
                            exit(json_encode(array('error' => 'Commitment failed')));
                        } else {
                            exit(json_encode(array()));
                        }
                    } else {
                        if (!move_uploaded_file($_FILES['file_data']['tmp_name'], "files/probfile/$problemId" . $_POST['url'])) {
                            exit(json_encode(array('error' => 'Cannot move the uploaded file')));
                        } else if (!KitFile:: kitCommitChange()) {
                            exit(json_encode(array('error' => 'Commitment failed')));
                        } else {
                            exit(json_encode(array()));
                        }
                    }
                } else {
                    exit(json_encode(array('error' => 'Cannot move the uploaded file')));
                }
            }
        }
    }

    public function file($problemId=null)
    {
        if ($problemId == null || !is_numeric($problemId)) {
            show_404();
        }
        $this->load->library('KitInfo');
        $this->load->model('KitProblem');
        session_start();
        session_write_close();
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit($message);
        } else {
            $array = $this->uri->segment_array();
            array_shift($array);
            array_shift($array);
            array_shift($array);
            $url = join('/', $array);
            if (!$this->isUrlValid($url) || !file_exists("files/probfile/$problemId/share/" . $url)) {
                exit('Invalid request');
            } else {
                $handle = fopen("files/probfile/$problemId/" . $url, "r") or exit("Invalid request");
                header('Content-Type:application/octet-stream');
                header('Content-Disposition: attachment; filename="' . pathinfo("files/probfile/$problemId/share/" . $url)['basename'] . '"');
                if ($handle) {
                    while (!feof($handle)) {
                        $buffer = fgets($handle, 1048576);
                        echo $buffer;
                    }
                    fclose($handle);
                }
            }
        }
    }
}
