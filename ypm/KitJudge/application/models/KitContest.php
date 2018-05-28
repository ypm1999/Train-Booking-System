<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitContest extends CI_Model
{
    public function kitGetContestById($cid)
    {
        return $this->db->query("SELECT * FROM KitContest WHERE kitContestId='$cid'");
    }

    public function kitGetContestByFilter($page, $perpage, $status = null, $priority = 0)
    {
        $sql = '';
        $first = true;
        if ($priority >= 2) {
            $sql = "SELECT * FROM KitContest";
        } else {
            $sql = "SELECT * FROM KitContest WHERE kitContestHidden=FALSE";
            $first = false;
        }
        if ($status != null) {
            if ($first) {
                $sql = $sql . ' WHERE ';
            } else {
                $sql = $sql . ' AND ';
            }
            $now = date('Y-m-d H:i:s', time());
            if ($status == 'upcoming') {
                $sql = $sql . "$now<kitContestStart";
            } else if ($status == 'running') {
                $sql = $sql . "$now>=kitContestStart AND kitContestEnd>$now";
            } else {
                $sql = $sql . "kitContestEnd<=$now";
            }
        }
        $start = ($page - 1) * $perpage;
        $sql = $sql . " order by kitContestId limit {$start},{$perpage}";
        return $this->db->query($sql);
    }

    public function kitCountContestByFilter($status = null, $priority = 0)
    {
        $first = true;
        if ($priority >= 2) {
            $sql = "SELECT count(*) FROM KitContest";
        } else {
            $sql = "SELECT count(*) FROM KitContest WHERE kitContestHidden=FALSE";
            $first = false;
        }
        if ($status != null) {
            if ($first) {
                $sql = $sql . ' AND ';
            } else {
                $sql = $sql . ' WHERE ';
            }
            $now = date('Y-m-d H:i:s', time());
            if ($status == 'upcoming') {
                $sql = $sql . "$now<kitContestStart";
            } else if ($status == 'running') {
                $sql = $sql . "$now>=kitContestStart AND kitContestEnd>$now";
            } else {
                $sql = $sql . "kitContestEnd<=$now";
            }
        }
        return ((array)$this->db->query($sql)->row())['count(*)'];
    }

    public function kitCountContest()
    {
        return ((array)$this->db->query('SELECT count(*) FROM KitContest')->row())['count(*)'];
    }

    public function kitInsertContest($name, $type, $src, $start, $duration, $hidden)
    {
        $sqlstr = $this->db->insert_string('KitContest', array(
            'kitContestName' => $name,
            'kitContestType' => $type,
            'kitContestSource' => $src,
            'kitContestStart' => $start,
            'kitContestDuration' => $duration,
            'kitContestEnd' => date('Y-m-d H:i:s', strtotime($start) + $duration),
            'kitContestHidden' => $hidden
        ));
        $result = $this->db->query($sqlstr);
        if (!$result) {
            return -1;
        } else {
            return $this->db->insert_id();
        }
    }

    public function kitIsExistContestById($cid, $priority)
    {
        if ($priority >= 2) {
            $sql = "SELECT count(*) FROM KitContest";
        } else {
            $sql = "SELECT count(*) FROM KitContest WHERE kitContestHidden=FALSE";
        }
        return ((array)$this->db->query($sql)->row())['count(*)'] > 0;
    }

    public function kitUpdateContest($cid, $name, $type, $src, $start, $duration, $hidden)
    {
        $sqlstr = $this->db->update_string('KitContest',
            array(
                'kitContestName' => $name,
                'kitContestType' => $type,
                'kitContestSource' => $src,
                'kitContestStart' => $start,
                'kitContestDuration' => $duration,
                'kitContestEnd' => date('Y-m-d H:i:s', strtotime($start) + $duration),
                'kitContestHidden' => $hidden
            ),
            "kitContestId='$cid'"
        );
        $this->db->query($sqlstr);
    }
}
