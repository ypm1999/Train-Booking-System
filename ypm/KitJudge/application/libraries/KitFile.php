<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class KitFile
{
    public static function kitRemoveTree($dir)
    {
        foreach (scandir($dir) as $file) {
            if ('.' === $file || '..' === $file) continue;
            if (is_dir("$dir/$file")) {
                if (!KitFile:: kitRemoveTree("$dir/$file")) {
                    return false;
                }
            } else {
                if (!unlink("$dir/$file")) {
                    return false;
                }
            }
        }
        if (!rmdir($dir)) {
            return false;
        }
        return true;
    }

    public static function kitGetVersion()
    {
        return (int)(file_get_contents('files/probfile/version'));
    }

    public static function kitGetCodeExtends($kitCodeLang)
    {
        if ($kitCodeLang == "Cpp") {
            return "cpp";
        } else if ($kitCodeLang == "Java") {
            return 'java';
        } else {
            return "code";
        }
    }

    public static function kitReadCode($kitRunId, $kitUser, $kitPath)
    {
        return file_get_contents("userdata/code/" . $kitRunId . '/' . $kitPath);
    }

    public static function kitSaveCode($kitRunId, $kitUser, $kitCode, $kitPath)
    {
        if (!is_dir("files/userfile/$kitUser/$kitRunId")) {
            mkdir("files/userfile/$kitUser/$kitRunId", 0777, true);
            chmod("files/userfile/$kitUser/$kitRunId", 0777);
        }
        $fp = fopen("files/userfile/$kitUser/$kitRunId/$kitPath", "w");
        fwrite($fp, $kitCode);
        fclose($fp);
    }

    public static function kitCommitChange()
    {
        $version = (int)(file_get_contents("files/probfile/version"));
        $fp = fopen("files/probfile/version", "w");
        fwrite($fp, $version + 1);
        fclose($fp);
        // $now = shell_exec("python files/probfile/gitcommit.py");
        // if ($now != "Commited.\n") {
        //     die("Commit Failed.: Message = " . $now);
        // }
        return true;
    }

    public static function kitListTreeView($dir, $accumulate, $name)
    {
        if (is_dir($dir)) {
            $path = scandir($dir);
            $ret = array();
            foreach ($path as $key => $content) {
                if ($content != '.' && $content != '..' && is_dir($dir . '/' . $content)) {
                    $ret = array_merge($ret,
                        array(
                            KitFile:: kitListTreeView($dir . '/' . $content, $accumulate . '/' . $content, $content)
                        )
                    );
                }
            }
            foreach ($path as $key => $content) {
                if ($content != '.' && $content != '..' && !is_dir($dir . '/' . $content)) {
                    $ret = array_merge($ret,
                        array(
                            KitFile:: kitListTreeView($dir . '/' . $content, $accumulate . '/' . $content, $content)
                        )
                    );
                }
            }
            if ($accumulate != "") {
                $ret = array(
                    'text' => $name,
                    'nodes' => $ret,
                    'type' => 'dir'
                );
            } else {
                $ret = array(array(
                    'text' => 'ROOT',
                    'nodes' => $ret,
                    'type' => 'root'
                ));
            }
            return $ret;
        } else {
            $ret = array(
                'text' => $name,
                'type' => 'file'
            );
        }
        return $ret;
    }
}