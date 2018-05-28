<?php
defined("BASEPATH") OR exit("No direct script access allowed");
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
    <link href="<?= $kitBasePath ?>/utility/css/bootstrap.min.css" rel="stylesheet">
    <link href="<?= $kitBasePath ?>/utility/css/prettify.css" meida="screen" rel="stylesheet">
    <script src="<?= $kitBasePath ?>/utility/js/jquery.min.js" type="application/javascript"></script>
    <script src="<?= $kitBasePath ?>/utility/js/bootstrap.min.js" type="application/javascript"></script>
    <script src="<?= $kitBasePath ?>/utility/js/LAB.js" type="application/javascript"></script>
    <script src="<?= $kitBasePath ?>/utility/js/kit-custom.js" type="application/javascript"></script>
    <script src="<?= $kitBasePath ?>/utility/js/prettify.js" type="application/javascript"></script>
    <link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/kit-custom.css">
    <link href="//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.css" rel="stylesheet">
    <title><?= isset($kitPageTitle) ? $kitPageTitle . ' - ' : '' ?><?= $kitTitle ?></title>
</head>
<body onload="prettyPrint()">
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/nprogress.min.css">
<script type="application/javascript">
    $LAB.script("<?=$kitBasePath?>/utility/js/nprogress.min.js").wait(function () {
        NProgress.start();
    })
</script>