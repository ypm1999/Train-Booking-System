<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitUser extends CI_Model
{
    public function kitVerdictUser($kitUserName, $kitUserPassword)
    {
        $result = $this->db->query("SELECT kitUserName, kitUserPassword, kitUserPriority FROM KitUser WHERE kitUserName='" . $kitUserName . "'");
        foreach ($result->result() as $row) {
            if ($row->kitUserPassword == $kitUserPassword) {
                return array(
                    'verdict' => True,
                    'username' => $row->kitUserName,
                    'priority' => $row->kitUserPriority
                );
            }
        }
        return array(
            'verdict' => false,
            'message' => 'username & password don\'t match.'
        );
    }

    public function kitInsertUser($username, $password, $email)
    {
        $result = $this->db->query("SELECT * FROM KitUser WHERE kitUserName='$username' OR kitUserEmail='$email'")->result_array();
        if (!empty($result)) {
            if ($result[0]['kitUserName'] == $username) {
                return array(
                    'verdict' => false,
                    'message' => 'The username has already been registered.'
                );
            } else {
                return array(
                    'verdict' => false,
                    'message' => 'The email has already been registered.'
                );
            }
        } else {
            if (!$this->db->query("INSERT INTO KitUser (kitUserName, kitUserPassword, kitUserEmail) VALUES ('$username', '$password', '$email')")) {
                return array(
                    'verdict' => false,
                    'message' => 'Cannot insert such user.'
                );
            } else {
                return array('verdict' => true);
            }
        }
    }

    public function kitUpdateUser($username, $password, $email)
    {
        $sql = "UPDATE KitUser SET ";
        $first = true;
        if ($email) {
            if (!$first) $sql = "$sql, ";
            $sql = "$sql kitUserEmail='$email'";
            $first = false;
        }
        if ($password) {
            if (!$first) $sql = "$sql, ";
            $sql = "$sql kitUserPassword='$password'";
            $first = false;
        }
        $sql = "$sql WHERE `kitUserName`='$username'";
        // echo json_encode(array(
        //     "sql"  => $sql
        // ));
        if (!$this->db->query($sql)) {
            return array(
                'verdict' => false,
                'message' => 'Cannot update userinfo.'
            );
        } else {
            return array('verdict' => true);
        }
    }

    public function kitUserInfo($username)
    {
        $result = $this->db->query("SELECT * FROM KitUser WHERE kitUserName='$username'")->result_array();
        if (empty($result)) {
            return array(
                'verdict' => false,
                'message' => 'cannot find this user.'
            );
        } else {
            if (count($result) > 1) {
                return array(
                    'verdict' => false,
                    'message' => 'find more than one user.'
                );
            }
            else {
                return array(
                    'verdict' => true,
                    'kitUsername' => $result[0]['kitUserName'],
                    'kitEmail' => $result[0]['kitUserEmail']
                );
            }
        }
    }
}