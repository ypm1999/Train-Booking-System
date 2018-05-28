<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<div class="container">
    <div class="jumbotron">
        <h1>Welcome to <?= $kitTitle ?>!</h1>
        <p><?= $kitWelcomeMessage ?></p>
        <?php if (!isset($_SESSION['kitUser'])) { ?>
            <p>
                <button class="btn btn-primary btn-lg" data-toggle="modal" data-target="#kit-login-modal">Login</button>
                <button class="btn btn-success btn-lg" data-toggle="modal" data-target="#kit-register-modal">Register
                </button>
            </p>
        <?php } ?>
    </div>
</div>
<div class="container hidden-xs hidden-sm">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="row">
                <div class="col-md-4">
                    <div class="thumbnail">
                        <img style="width:350px;height:120px"
                             src="<?= $kitBasePath ?>/utility/img/kit-welcome-screen-1.png" alt="">
                        <div class="caption">
                            <h3>Problem Set</h3>
                            <p>There are thousands of problems to be solved. Challenge yourself and improve your
                                skills!</p>
                            <p><a class="btn btn-primary" href="problems">Solve Problems!</a></p>
                        </div>
                    </div>
                </div>
                <div class="col-md-4">
                    <div class="thumbnail">
                        <img style="width:350px;height:120px"
                             src="<?= $kitBasePath ?>/utility/img/kit-welcome-screen-2.png" alt="">
                        <div class="caption">
                            <h3>Contest</h3>
                            <p>If your are very proud of your programming skills, just compete with others. Wish your
                                high rating!</p>
                            <p><a class="btn btn-success" href="contests">Participate Contests!</a></p>
                        </div>
                    </div>
                </div>
                <div class="col-md-4">
                    <div class="thumbnail">
                        <img style="width:350px;height:120px"
                             src="<?= $kitBasePath ?>/utility/img/kit-welcome-screen-3.jpg" alt="">
                        <div class="caption">
                            <h3>F.A.Qs</h3>
                            <p>There are severl frequently asked questions, you should learn it to solve problems
                                better.</p>
                            <p><a class="btn btn-warning" href="#">See F.A.Qs</a></p>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
