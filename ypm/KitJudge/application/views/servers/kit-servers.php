<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<div class="container">
    <table class="table table-hover">
        <caption>Main Server Information<img src="<?= $kitBasePath ?>/utility/img/active.png"></caption>
        <tbody>
        <tr>
            <td>PHP Version</td>
            <td><?= phpversion(); ?></td>
        </tr>
        <tr>
            <td>Server Name</td>
            <td><?= php_uname('n'); ?></td>
        </tr>
        <tr>
            <td>Server OS</td>
            <td><?= php_uname('s') . ' ' . php_uname('r'); ?></td>
        </tr>
        <tr>
            <td>Server Architecture</td>
            <td><?= php_uname('m'); ?></td>
        </tr>
        </tbody>
    </table>
</div>