<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Contests extends CI_Controller
{
    public function editdata()
    {
        if (!isset($_POST) || !isset($_POST['title']) || !isset($_POST['start'])
            || !isset($_POST['source']) || !isset($_POST['hidden'])
            || !isset($_POST['duration']) || !isset($_POST['type'])
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
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $isHidden = ($_POST['hidden'] == '1');
        if (!isset($_POST['contest'])) {
            if (($contestId = $this->KitContest->kitInsertContest($_POST['title'], $_POST['type'], $_POST['source'], $_POST['start'], $_POST['duration'], $isHidden)) < 0) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Cannot insert such contest.'
                )));
            }
        } else {
            $contestId = $_POST['contest'];
            if (!$this->KitContest->kitIsExistContestById($contestId, $_SESSION['kitUser']['priority'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => 'Invalid request.'
                )));
            }
            $this->KitContest->kitUpdateContest($contestId, $_POST['title'], $_POST['type'], $_POST['source'], $_POST['start'], $_POST['duration'], $isHidden);
        }
        echo json_encode(array('verdict' => true));
    }

    public function submission($contestId = null)
    {
        if (!is_numeric($contestId)) {
            show_404();
        }
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        if (!isset($_SESSION['kitUser'])) {
            show_404();
        }
        $this->load->model('KitContest');
        $this->load->model('KitStatus');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $result = $this->KitContest->kitGetContestById($contestId);
        if (empty($result->result_array())) {
            show_404();
        }
        $result = $result->row();
        $info = array_merge(
            array(
                'kitTime' => time(),
                'kitView' => 'submission-contest',
                'kitCurPage' => isset($_GET['page']) ? (int)($_GET['page']) : 1,
                'kitStatus' => $this->KitStatus->kitGetStatusByFiliter(
                    time(),
                    $_SESSION['kitUser'],
                    $_SESSION['kitUser']['name'],
                    null,
                    null,
                    null,
                    isset($_GET['page']) ? (int)($_GET['page']) : 1,
                    KitInfo::$kitInfo['kitStatusPERPage'],
                    $contestId
                ),
                'kitStatusTotalPages' => max((int)floor(
                    (
                        $this->KitStatus->kitCountStatus(
                            time(),
                            $_SESSION['kitUser'],
                            $_SESSION['kitUser']['name'],
                            null,
                            null,
                            null,
                            $contestId
                        )
                        + KitInfo::$kitInfo['kitStatusPERPage'] - 1
                    ) / KitInfo::$kitInfo['kitStatusPERPage']),
                    1
                ),
                'kitContest' => $result,
                'kitContestId' => $contestId,
                'kitSessionId' => session_id(),
                'kitStartTime' => strtotime($result->kitContestStart),
                'kitPageTitle' => $result->kitContestName . ' Submissions'
            ),
            KitInfo::$kitInfo
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('contests/submission/kit-contest-submission-preload', $info);
        $this->load->view('contests/submission/kit-contest-submission', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('contests/submission/kit-contest-submission-footage');
        $this->load->view('kit-common-footage');
    }

    public function board($contestId = null)
    {
        if (!is_numeric($contestId)) {
            show_404();
        }
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $result = $this->KitContest->kitGetContestById($contestId);
        if (empty($result->result_array())) {
            show_404();
        }
        $result = $result->row();
        $tags = array();
        $this->load->model('KitContestProblem');
        foreach ($this->KitContestProblem->kitGetProblemByContestId($contestId)->result_array() as $prob) {
            $tags[] = $prob['kitProbTag'];
        }
        $info = array_merge(
            array(
                'kitTime' => time(),
                'kitView' => 'board-contest',
                'kitContest' => $result,
                'kitContestId' => $contestId,
                'kitStartTime' => strtotime($result->kitContestStart),
                'kitPageTitle' => $result->kitContestName . ' Board',
                'kitProbTags' => $tags
            ),
            KitInfo::$kitInfo
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('contests/board/kit-board-contest-preload', $info);
        if ($result->kitContestType == 'ICPC') {
            $this->load->view('contests/board/icpc/kit-board-contest', $info);
        } else if ($result->kitContestType == 'ICPC/OI') {
            $this->load->view('contests/board/icpc-oi/kit-board-contest', $info);
        }
        $this->load->view('kit-common-bottom');
        $this->load->view('contests/board/kit-board-contest-footage');
        $this->load->view('kit-common-footage');

    }

    public function show($contestId = null)
    {
        if (!is_numeric($contestId)) {
            show_404();
        }
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $result = $this->KitContest->kitGetContestById($contestId);
        if (empty($result->result_array())) {
            show_404();
        }
        $result = $result->row();
        $info = array_merge(
            array(
                'kitTime' => time(),
                'kitView' => 'show-contest',
                'kitContest' => $result,
                'kitContestId' => $contestId,
                'kitSessionId' => session_id(),
                'kitStartTime' => strtotime($result->kitContestStart),
                'kitPageTitle' => $result->kitContestName
            ),
            KitInfo::$kitInfo
        );
        $problems = array();
        $this->load->model('KitContestProblem');
        $this->load->model('KitProblem');
        foreach ($this->KitContestProblem->kitGetProblemByContestId($contestId)->result_array() as $prob) {
            $problems[$prob['kitProbTag']] = $this->KitProblem->kitGetProblemById((int)($prob['kitProbId']), 5)->row();
        }
        $info['kitProblems'] = $problems;
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('contests/show/kit-show-contest-preload', $info);
        $this->load->view('contests/show/kit-show-contest', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('contests/show/kit-show-contest-footage');
        $this->load->view('kit-common-footage');
    }

    public function edit($contestId = null)
    {
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
            show_404();
        }
        $info = array_merge(
            array(
                'kitView' => 'edit-contest',
                'kitSessionId' => session_id(),
                'kitPageTitle' => 'Edit Contest ' . $contestId
            ),
            KitInfo::$kitInfo
        );
        setcookie('session', $info['kitSessionId']);
        if ($contestId != null) {
            if (!is_numeric($contestId)) {
                show_404();
            }
            $this->load->model('KitContest');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            $result = $this->KitContest->kitGetContestById($contestId)->result_array();
            if (empty($result)) {
                show_404();
            }
            $info = array_merge($info, array(
                'kitContest' => $result[0],
                'kitContestId' => $result[0]['kitContestId']
            ));
        } else {
            $this->load->model('KitContest');
            $this->load->database(KitInfo::$kitInfo['kitDatabase']);
            $info = array_merge($info, array('kitContestId' => $this->KitContest->kitCountContest() + 1));
        }
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('contests/edit/kit-edit-contest-preload', $info);
        $this->load->view('contests/edit/kit-edit-contest', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('contests/edit/kit-edit-contest-footage');
        $this->load->view('kit-common-footage');
    }

    public function index()
    {
        $this->load->library('KitInfo');
        session_start();
        session_write_close();
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $info = array_merge(KitInfo::$kitInfo,
            array(
                'kitView' => 'contests',
                'kitCurPage' => (int)(isset($_GET['page']) ? (int)($_GET['page']) : 1),
                'kitPageTitle' => 'Contests',
                'kitContests' => $this->KitContest->kitGetContestByFilter(
                    isset($_GET['page']) ? (int)($_GET['page']) : 1,
                    KitInfo::$kitInfo['kitContestPERPage'],
                    null,
                    isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0
                ),
                'kitContestsTotalPages' => max((int)ceil($this->KitContest->kitCountContestByFilter(
                        null,
                        isset($_SESSION['kitUser']) ? $_SESSION['kitUser']['priority'] : 0
                    ) / KitInfo::$kitInfo['kitContestPERPage']),
                    1
                )
            )
        );
        $this->load->view('kit-common-header', $info);
        $this->load->view('kit-navbar');
        $this->load->view('kit-modal', $info);
        $this->load->view('contests/kit-contests-preload', $info);
        $this->load->view('contests/kit-contests', $info);
        $this->load->view('kit-common-bottom');
        $this->load->view('contests/kit-contests-footage');
        $this->load->view('kit-common-footage');
    }

    public function problems($method = null, $contestId = null, $data = null)
    {
        $this->load->library('KitInfo');
        $this->load->model('KitStatus');
        session_start();
        session_write_close();
        $info = array(
            'kitView' => 'edit-contest',
            'kitSessionId' => session_id(),
            'kitPageTitle' => 'Edit Problem in Contest ' . $contestId
        );
        setcookie('session', $info['kitSessionId']);
        if (!is_numeric($contestId)) {
            show_404();
        }
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        $result = $this->KitContest->kitGetContestById($contestId)->result_array();
        if ($method == 'show') {
            if (empty($result) || $data == null) {
                show_404();
            }
            if ((!isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) && time() < strtotime($result[0]['kitContestStart'])) {
                show_404();
            }
            $this->load->model('KitContestProblem');
            $probdata = $this->KitContestProblem->kitGetProblemByContestId($contestId, $data);
            if (empty($probdata->result_array())) {
                show_404();
            }
            $probdata = $probdata->row();
            $this->load->model('KitProblem');
            $problemId = $probdata->kitProbId;
            $probdata = $this->KitProblem->kitGetProblemById($problemId, 5);
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
                    'kitProblemConf' => json_decode(file_get_contents("files/probfile/$problemId/problem.json"), true),
                    'kitContestId' => $contestId,
                    'kitProblemTag' => $data,
                    'kitProblemContestStartTime' => strtotime($result[0]['kitContestStart'])
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
        } else if ($method == 'edit') {
            if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
                show_404();
            }
            if (empty($result)) {
                show_404();
            }
            $this->load->model('KitContestProblem');
            $info = array_merge($info, array(
                'kitContestId' => $contestId
            ));
            if ($data != null) {
                $result = $this->KitContestProblem->kitGetProblemByEntryId($data);
                if (empty($result->result_array())) {
                    show_404();
                }
                $info = array_merge($info, array(
                    'kitContestProblem' => $result->row()
                ));
            }
            $info = array_merge($info, KitInfo::$kitInfo);
            $this->load->view('kit-common-header', $info);
            $this->load->view('kit-navbar');
            $this->load->view('kit-modal', $info);
            $this->load->view('contests/problems/edit/kit-edit-contest-problem-preload', $info);
            $this->load->view('contests/problems/edit/kit-edit-contest-problem', $info);
            $this->load->view('kit-common-bottom');
            $this->load->view('contests/problems/edit/kit-edit-contest-problem-footage');
            $this->load->view('kit-common-footage');
        } else if ($method == 'editdata') {
            if (!isset($_SESSION) || !isset($_SESSION['kitUser']) || $_SESSION['kitUser']['priority'] < 2) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => "Invalid request."
                )));
            }
            $this->load->model('KitContestProblem');
            if (empty($result) || !isset($_POST['prob']) || !isset($_POST['tag'])) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => "Invalid request."
                )));
            }
            $this->load->model('KitProblem');
            if (!is_numeric($_POST['prob']) || !$this->KitProblem->kitIsExistProblemById($_POST['prob'], 5)) {
                exit(json_encode(array(
                    'verdict' => false,
                    'message' => "Invalid problem."
                )));
            }
            $eid = $this->KitContestProblem->kitGetEIDByProblemTag($contestId, $_POST['tag']);
            if ($eid == -1) {
                $this->KitContestProblem->kitInsertProblem($contestId, $_POST['prob'], $_POST['tag']);
            } else {
                $this->KitContestProblem->kitUpdateProblem($eid, $contestId, $_POST['prob'], $_POST['tag']);
            }
            echo json_encode(array(
                'verdict' => true
            ));
        } else {
            show_404();
        }
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

    private static function isUrlValid($str)
    {
        return !strpos($str, '../');
    }

    public function file($contestId, $probTag)
    {
        $this->load->library('KitInfo');
        if (!is_numeric($contestId)) {
            show_404();
        }
        $this->load->model('KitContest');
        $this->load->database(KitInfo::$kitInfo['kitDatabase']);
        if (empty($this->KitContest->kitGetContestById($contestId)->result_array())) {
            show_404();
        }
        $this->load->model('KitContestProblem');
        $probdata = $this->KitContestProblem->kitGetProblemByContestId($contestId, $probTag);
        if (empty($probdata->result_array())) {
            show_404();
        }
        $this->load->model('KitProblem');
        session_start();
        session_write_close();
        $problemId = $probdata->row()->kitProbId;
        $message = $this->isProblemIdValid($problemId, $_SESSION['kitUser']['priority']);
        if ($message != null) {
            exit($message);
        } else {
            $array = $this->uri->segment_array();
            array_shift($array);
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
