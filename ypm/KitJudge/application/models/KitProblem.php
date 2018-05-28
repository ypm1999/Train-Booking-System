<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitProblem extends CI_Model
{
    public function kitGetProblemsByVolumn($volumn, $perpage, $priority = 0)
    {
        $start = ($volumn - 1) * $perpage;
        if ($priority >= 2) {
            return $this->db->query("SELECT * from KitProblem order by kitProbId limit {$start},{$perpage}");
        } else {
            return $this->db->query("SELECT * from KitProblem WHERE kitProbHidden=FALSE order by kitProbId limit {$start},{$perpage}");
        }
    }

    public function kitCountProblems($priority = 0)
    {
        if ($priority >= 2) {
            $sqlstr = "SELECT count(*) FROM KitProblem";
            return ((array)($this->db->query($sqlstr)->row()))['count(*)'];
        } else {
            $sqlstr = "SELECT count(*) FROM KitProblem WHERE kitProbHidden=FALSE";
            return ((array)($this->db->query($sqlstr)->row()))['count(*)'];
        }
    }

    public function kitNextProblemId($priority = 0)
    {
        if ($priority >= 2) {
            $sqlstr = "SELECT max(kitProbId) FROM KitProblem";
            return ((array)($this->db->query($sqlstr)->row()))['max(kitProbId)'] + 1;
        } else {
            return -1;
        }
    }

    public function kitIsExistProblemById($pid, $priority = 0)
    {
        if ($priority >= 2) {
            $sqlstr = "SELECT count(*) from KitProblem WHERE kitProbId={$pid}";
            return ((array)($this->db->query($sqlstr)->row()))['count(*)'] > 0;
        } else {
            $sqlstr = "SELECT count(*) from KitProblem WHERE kitProbId={$pid} AND kitProbHidden=FALSE";
            return ((array)($this->db->query($sqlstr)->row()))['count(*)'] > 0;
        }
    }

    public function kitGetProblemById($pid, $priority = 0)
    {
        if ($priority >= 2) {
            return $this->db->query("SELECT * from KitProblem WHERE kitProbId={$pid}");
        } else {
            return $this->db->query("SELECT * from KitProblem WHERE kitProbId={$pid} AND kitProbHidden=FALSE");
        }
    }

    public function kitIncreaseSubmitted($pid)
    {
        $this->db->query("UPDATE KitProblem SET kitProbSubmitted=kitProbSubmitted+1 WHERE kitProbId='$pid'");
    }

    public function kitDecreaseAccepted($pid)
    {
        $this->db->query("UPDATE KitProblem SET kitProbAccepted=kitProbAccepted-1 WHERE kitProbId='$pid'");
    }

    public function kitInsertProblem($name, $type, $src, $hidden)
    {
        $sqlstr = $this->db->insert_string('KitProblem', array(
            'kitProbName' => $name,
            'kitProbSource' => $src,
            'kitProbType' => $type,
            'kitProbHidden' => $hidden
        ));
        $result = $this->db->query($sqlstr);
        if (!$result) {
            return -1;
        } else {
            return $this->db->insert_id();
        }
    }

    public function kitUpdateProblem($pid, $name, $type, $src, $hidden)
    {
        $sqlstr = $this->db->update_string('KitProblem',
            array(
                'kitProbName' => $name,
                'kitProbSource' => $src,
                'kitProbType' => $type,
                'kitProbHidden' => $hidden
            ),
            "kitProbId='$pid'"
        );
        $this->db->query($sqlstr);
    }
}