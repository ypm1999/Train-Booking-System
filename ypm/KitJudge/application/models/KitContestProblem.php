<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitContestProblem extends CI_Model
{
    public function kitGetProblemByEntryId($eid)
    {
        return $this->db->query("SELECT * FROM KitContestProblem WHERE kitEntryId='$eid'");
    }

    public function kitGetProblemByContestId($cid, $tag = null)
    {
        $sql = "SELECT * FROM KitContestProblem WHERE kitContestId='$cid'";
        if ($tag != null) {
            $sql = "$sql AND kitProbTag='$tag'";
        }
        $sql = "$sql ORDER BY kitProbTag";
        return $this->db->query($sql);
    }

    public function kitGetEIDByProblemTag($cid, $tag)
    {
        $sql = "SELECT kitEntryId FROM KitContestProblem WHERE kitContestId='$cid' AND kitProbTag='$tag'";
        if (empty(($sql = $this->db->query($sql))->result_array())) {
            return -1;
        }
        return ((array)$sql->row())['kitEntryId'];
    }

    public function kitInsertProblem($cid, $pid, $tag)
    {
        $sqlstr = $this->db->insert_string('KitContestProblem', array(
            'kitContestId' => $cid,
            'kitProbId' => $pid,
            'kitProbTag' => $tag
        ));
        $result = $this->db->query($sqlstr);
        if (!$result) {
            return -1;
        } else {
            return $this->db->insert_id();
        }
    }

    public function kitUpdateProblem($eid, $cid, $pid, $tag)
    {
        $sqlstr = $this->db->update_string('KitContestProblem',
            array(
                'kitContestId' => $cid,
                'kitProbId' => $pid,
                'kitProbTag' => $tag
            ),
            "kitEntryId='$eid'"
        );
        $this->db->query($sqlstr);
    }
}