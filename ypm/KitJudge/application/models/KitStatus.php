<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitStatus extends CI_Model
{
    public function kitGetStatusById($runId)
    {
        return $this->db->query("SELECT * FROM KitStatus WHERE kitStatusId={$runId}");
    }
    
    public function kitGetLastStatus($user, $prob)
    {
        $sql = "SELECT kitStatusId FROM KitStatus WHERE (kitStatusProbId=$prob AND kitStatusUser='$user') ORDER BY kitStatusId DESC";
        $sql = $this->db->query($sql);
        return count($sql->result()) ? $sql->row()->kitStatusId : null;
    }

    public function kitBoard($time, $contestId)
    {
        $sql = "SELECT kitContestStart,kitContestType FROM KitContest WHERE kitContestId=$contestId";
        $sql = $this->db->query($sql)->row();
        $time = $time - strtotime($sql->kitContestStart);
        if ($sql->kitContestType == 'ICPC') {
            $sql = "SELECT kitStatusId AS rid,kitStatusUser AS user,kitStatusContestTime AS rtime,kitStatusVerdict AS verdict,kitStatusContestTag AS tag,kitStatusTime as time FROM KitStatus WHERE kitStatusContestTime<=$time AND kitStatusContestId=$contestId";
        } else if ($sql->kitContestType == 'ICPC/OI') {
            $sql = "SELECT kitStatusId AS rid,kitStatusUser AS user,kitStatusContestTime AS rtime,kitStatusVerdict AS verdict,kitStatusContestTag AS tag,kitStatusTime as time,kitStatusScore as score FROM KitStatus WHERE kitStatusContestTime<=$time AND kitStatusContestId=$contestId";
        }
        return $this->db->query($sql)->result_array();
    }

    public function kitGetStatusByFiliter($time, $login_user, $user, $prob, $lang, $verdict, $page, $perpage, $contestId = null, $orderBy = null)
    {
        $time = date('Y-m-d H:i:s', $time);
        $first = true;
        if ($login_user != null && $login_user['priority'] >= 2) {
            $sql = "SELECT * FROM KitStatus";
        } else {
            $sql = "SELECT * FROM KitStatus WHERE 
              (
                ((kitStatusUser='" . $login_user['name'] . "' AND kitStatusContestId IN (SELECT kitContestId FROM KitContest WHERE '$time' >= kitContestStart)) 
                OR 
                (kitStatusContestId IN (SELECT kitContestId FROM KitContest WHERE '$time' > kitContestEnd)) 
                OR 
                (kitStatusContestId IS NULL AND kitStatusProbId IN (SELECT kitProbId FROM KitProblem WHERE kitProbHidden=FALSE)))
                AND kitStatusUser != 'root'
              )";
            $first = false;
        }
        if ($user != null) {
            if (!$first) $sql = "$sql AND kitStatusUser='{$user}'";
            else $sql = "$sql WHERE kitStatusUser='{$user}'";
            $first = false;
        }
        if ($lang != null) {
            if (!$first) $sql = "$sql AND kitStatusLanguage='{$lang}'";
            else $sql = "$sql WHERE kitStatusLanguage='{$lang}'";
            $first = false;
        }
        if ($prob != null) {
            if (!$first) $sql = "$sql AND kitStatusProbId={$prob}";
            else $sql = "$sql WHERE kitStatusProbId={$prob}";
            $first = false;
        }
        if ($verdict != null) {
            if (!$first) $sql = "$sql AND kitStatusVerdict={$verdict}";
            else $sql = "$sql WHERE kitStatusVerdict={$verdict}";
            $first = false;
        }
        if ($contestId != null) {
            if (!$first) $sql = "$sql AND kitStatusContestId={$contestId}";
            else $sql = "$sql WHERE kitStatusContestId={$contestId}";
            $first = false;
        }
        if ($orderBy != null) {
            $sql = "$sql order by $orderBy";
        } else {
            $sql = "$sql order by kitStatusId DESC";
        }
        $start = ($page - 1) * $perpage;
        $sql = "$sql limit {$start},{$perpage}";
        return $this->db->query($sql);
    }

    public function kitAddStatus($user, $probId, $lang, $length, $cid = null, $tag = null, $time = null)
    {
        $sqlstr = $this->db->insert_string('KitStatus', array(
            'kitStatusProbId' => $probId,
            'kitStatusVerdict' => 8, // Running & Judging
            'kitStatusUser' => $user,
            'kitStatusLanguage' => $lang,
            'kitStatusLength' => $length,
            'kitStatusContestId' => $cid,
            'kitStatusContestTag' => $tag,
            'kitStatusContestTime' => $time
        ));
        $result = $this->db->query($sqlstr);
        if (!$result) {
            return -1;
        } else {
            return $this->db->insert_id();
        }
    }

    public function kitRejudgeStatus($sid)
    {
        $sqlstr = $this->db->update_string('KitStatus',
            array('kitStatusType' => 8),
            "kitStatusId='$sid'"
        );
        $this->db->query($sqlstr);
    }

    public function kitCountStatus($time, $login_user, $user, $prob, $lang, $verdict, $contestId = null)
    {
        $time = date('Y-m-d H:i:s', $time);
        $first = true;
        if ($login_user != null && $login_user['priority'] >= 2) {
            $sql = "SELECT count(*) FROM KitStatus";
        } else {
            $sql = "SELECT count(*) FROM KitStatus WHERE 
              (
                ((kitStatusUser='" . $login_user['name'] . "' AND kitStatusContestId IN (SELECT kitContestId FROM KitContest WHERE '$time' >= kitContestStart)) 
                OR 
                (kitStatusContestId IN (SELECT kitContestId FROM KitContest WHERE '$time' > kitContestEnd)) 
                OR 
                (kitStatusContestId IS NULL AND kitStatusProbId IN (SELECT kitProbId FROM KitProblem WHERE kitProbHidden=FALSE)))
                AND kitStatusUser != 'root'
              )";
            $first = false;
        }
        if ($user != null) {
            if (!$first) $sql = "$sql AND kitStatusUser='{$user}'";
            else $sql = "$sql WHERE kitStatusUser='{$user}'";
            $first = false;
        }
        if ($lang != null) {
            if (!$first) $sql = "$sql AND kitStatusLanguage='{$lang}'";
            else $sql = "$sql WHERE kitStatusLanguage='{$lang}'";
            $first = false;
        }
        if ($prob != null) {
            if (!$first) $sql = "$sql AND kitStatusProbId={$prob}";
            else $sql = "$sql WHERE kitStatusProbId={$prob}";
            $first = false;
        }
        if ($verdict != null) {
            if (!$first) $sql = "$sql AND kitStatusVerdict={$verdict}";
            else $sql = "$sql WHERE kitStatusVerdict={$verdict}";
            $first = false;
        }
        if ($contestId != null) {
            if (!$first) $sql = "$sql AND kitStatusContestId={$contestId}";
            else $sql = "$sql WHERE kitStatusContestId={$contestId}";
            $first = false;
        }
        return ((array)($this->db->query($sql)->row()))['count(*)'];
    }
}
