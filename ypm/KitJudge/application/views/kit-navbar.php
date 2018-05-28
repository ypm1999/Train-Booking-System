<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
    <div class="container-fluid">
        <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse"
                    data-target="#example-navbar-collapse">
                <span class="sr-only">切换导航</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="<?= $kitBasePath ?>/welcome"><?= $kitTitle ?></a>
        </div>
        <div class="collapse navbar-collapse" id="example-navbar-collapse">
            <ul class="nav navbar-nav">
                <?php foreach ($kitNavbar as $name => $item) {
                    if ($kitView == $name) { ?>
                        <li class="active"><a href="<?= $kitBasePath ?>/<?= $item[1] ?>"><?= $item[0] ?></a></li>
                    <?php } else { ?>
                        <li><a href="<?= $kitBasePath ?>/<?= $item[1] ?>"><?= $item[0] ?></a></li>
                    <?php }
                } ?>
            </ul>
            <?php if (isset($_SESSION['kitUser'])) { ?>
                <ul class="nav navbar-nav navbar-right">
                    <li class="dropdown">
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown"><?= $_SESSION['kitUser']['name'] ?>
                            <b class="caret"></b></a>
                        <ul class="dropdown-menu">
                            <li><a href="<?= $kitBasePath ?>/user">Profile</a></li>
                            <li><a href="#">Submissions</a></li>
                            <li><a href="#">Settings</a></li>
                            <?php if ($_SESSION['kitUser']['priority'] >= 3) { ?>
                                <li class="divider"></li>
                                <li><a href="<?= $kitBasePath ?>/netdisk">Netdisk</a></li>
                            <?php } ?>
                            <?php if ($_SESSION['kitUser']['priority'] >= 5) { ?>
                                <li><a href="<?= $kitBasePath ?>/runtime">Website Runtime</a></li>
                                <li><a href="//<?= $_SERVER['HTTP_HOST'] ?>:<?= $kitMQ['admin_port'] ?>">RabbitMQ
                                        Runtime</a></li>
                            <?php } ?>
                            <li class="divider"></li>
                            <li><a href="#" id="kit-navbar-logout">Logout</a></li>
                            <script type="application/javascript">
                                $('#kit-navbar-logout').click(function () {
                                    $.ajax({
                                        type: "POST",
                                        url: "<?=$kitBasePath?>/user/logout",
                                        cache: false,
                                        data: {},
                                        success: function () {
                                            window.location.reload();
                                        }
                                    });
                                });
                            </script>
                        </ul>
                    </li>
                </ul>
            <?php } ?>
        </div>
    </div>
</nav>
