<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<?php if (array_key_exists('login', $kitPages[$kitView]['resources']['modals'])) { ?>
    <div class="modal fade" id="kit-login-modal" tabindex="-1" role="dialog" aria-labelledby="kit-login-modal"
         aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <h4 class="modal-title" id="kitLoginModalTitle">Login</h4>
                </div>
                <div class="modal-body">
                    <form id="kit-login-modal-form" class="form-horizontal" role="form" method="post">
                        <div id="kit-login-modal-messsage" style="display: none;"></div><div id="kit-login-modal-messsage" style="display: none;"></div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Username</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="kit-login-modal-username"
                                       placeholder="Enter your username here">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Password</label>
                            <div class="col-sm-10">
                                <input type="password" class="form-control" id="kit-login-modal-password"
                                       placeholder="Enter your password here">
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-2 col-sm-10 btn-group">
                                <button type="submit" class="btn btn-primary">Login</button>
                                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            </div>
                        </div>
                    </form>
                    <link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/bootstrap-validator.min.css">
                    <script type="application/javascript">
                        var setLoginModalFormValidator = function () {
                            $LAB.script('<?=$kitBasePath?>/utility/js/bootstrap-validator.min.js')
                                .script('<?=$kitBasePath?>/utility/js/sha.js')
                                .wait(function () {
                                    $('#kit-login-modal-form').bootstrapValidator({
                                        feedbackIcons: {
                                            valid: "glyphicon glyphicon-ok",
                                            invalid: "glyphicon glyphicon-remove",
                                            validating: "glyphicon glyphicon-refresh"
                                        },
                                        fields: {
                                            username: {
                                                selector: "#kit-login-modal-username",
                                                validators: {
                                                    notEmpty: {
                                                        message: 'The username is empty.'
                                                    },
                                                    stringLength: {
                                                        min: 4,
                                                        max: 16,
                                                        message: "The length of the password should be between 4 and 16."
                                                    }
                                                }
                                            },
                                            password: {
                                                selector: "#kit-login-modal-password",
                                                validators: {
                                                    notEmpty: {
                                                        message: 'The password is empty.'
                                                    },
                                                    stringLength: {
                                                        min: 6,
                                                        max: 16,
                                                        message: "The length of the password should be between 6 and 16."
                                                    }
                                                }
                                            }
                                        }
                                    }).on("success.form.bv", function (o) {
                                        if (!o.isDefaultPrevented()) {
                                            o.preventDefault();
                                            var hashPassword = new jsSHA("SHA-256", "TEXT", {numRounds: parseInt("1", 10)});
                                            hashPassword.update($("#kit-login-modal-password").val());
                                            hashPassword = hashPassword.getHash('HEX');
                                            var clear = function () {
                                                $("#kit-login-modal-form")
                                                    .data("bootstrapValidator").destroy();
                                                $("#kit-login-modal-username").val("").focus();
                                                $("#kit-login-modal-password").val("");
                                                setLoginModalFormValidator();
                                            };
                                            $.ajax({
                                                type: "POST",
                                                url: "<?=$kitBasePath?>/user/login",
                                                cache: false,
                                                data: "username=" + $("#kit-login-modal-username").val()
                                                + "&password=" + hashPassword + "&session=<?=$kitSessionId?>",
                                                success: function (message) {
                                                    var result = eval('(' + message + ')');
                                                    if (result.verdict) {
                                                        window.location.reload()
                                                    } else {
                                                        $('#kit-login-modal-messsage')
                                                            .html(result.message)
                                                            .addClass('alert')
                                                            .addClass('alert-warning')
                                                            .addClass('alert-dismissable')
                                                            .show();
                                                        clear();
                                                    }
                                                },
                                                error: function () {
                                                    $('#kit-login-modal-messsage')
                                                        .html('A fatal error occured, please contact <a class="text-danger" href="mailto:<?=$kitAdminEmail?>"><strong><?=$kitAdminEmail?></strong></a>.')
                                                        .addClass('alert')
                                                        .addClass('alert-danger')
                                                        .addClass('alert-dismissable')
                                                        .show();
                                                    clear();
                                                }
                                            });
                                        }
                                    })
                                });
                        };
                        setLoginModalFormValidator();
                    </script>
                </div>
            </div>
        </div>
    </div>
<?php } ?>
<?php if (array_key_exists('folder', $kitPages[$kitView]['resources']['modals'])) { ?>
    <div class="modal fade" id="kit-folder-modal" tabindex="-1" role="dialog" aria-labelledby="folder"
         aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <h4 class="modal-title">New Folder</h4>
                </div>
                <div class="modal-body">
                    <form id="kit-folder-modal-form" class="form-horizontal" role="form" method="post">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Url</label>
                            <div class="col-sm-9"><p class="form-control" id="kit-folder-url"></p></div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Folder Name</label>
                            <div class="col-sm-9">
                                <input type="text" class="form-control" id="kit-folder-name"
                                       placeholder="Enter folder name here">
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-3 col-sm-10 btn-group">
                                <button type="submit" class="btn btn-primary">Submit</button>
                                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
    <script type="application/javascript">
        function kitFolderFormValidator() {
            $("#kit-folder-modal-form").bootstrapValidator({
                message: "This value is not valid",
                feedbackIcons: {
                    valid: "glyphicon glyphicon-ok",
                    invalid: "glyphicon glyphicon-remove",
                    validating: "glyphicon glyphicon-refresh"
                },
                fields: {
                    kitFolderFormFolderNameText: {
                        message: "Error: ",
                        validators: {
                            notEmpty: {
                                message: "The field username is empty."
                            }
                        }
                    }
                }
            }).on("success.form.bv", function (o) {
                if (!o.isDefaultPrevented()) {
                    o.preventDefault();
                    var data = 'url=' + encodeURIComponent($('#kit-folder-url').text()) + '&name=' + encodeURIComponent($('#kit-folder-name').val()) + '&session=<?=$kitSessionId?>';
                    $.ajax({
                        type: "POST",
                        url: "<?=$kitBasePath?>/problems/folder/<?=$kitProbId?>",
                        cache: false,
                        data: data,
                        success: function (msg) {
                            var data = eval('(' + msg + ')');
                            if (data.verdict) {
                                getTree();
                                $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                    .wait(function () {
                                        $.jGrowl("Successfully created the new folder \"" + $('#kit-folder-name').val() + "\".", {
                                            position: 'bottom-right'
                                        });
                                    });

                                $('#kit-folder-modal').modal('toggle');
                            } else {
                                $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                    .wait(function () {
                                        $.jGrowl(data.message, {
                                            position: 'bottom-right',
                                            sticky: true
                                        });
                                    });
                            }
                        },
                        error: function () {
                            $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                .wait(function () {
                                    $.jGrowl('Error occured when creating folder.', {
                                        position: 'bottom-right',
                                        sticky: true
                                    });
                                });
                        }
                    });
                }
            })
        }
        $LAB.script('<?=$kitBasePath?>/utility/js/bootstrap-validator.min.js')
            .wait(function () {
                $("#kit-folder-modal-form").on("show.bs.modal", function () {
                    $("#kit-folder-name").focus()
                });
                $("#kit-folder-modal").on("hidden.bs.modal", function () {
                    $(this).find("#kit-folder-modal-form")[0].reset();
                    var object = $("#kit-folder-modal-form");
                    object.data("bootstrapValidator").destroy();
                    object.data("bootstrapValidator", null);
                    kitFolderFormValidator();
                });
                kitFolderFormValidator();
            });
    </script>
<?php } ?>
<?php if (array_key_exists('status-filter', $kitPages[$kitView]['resources']['modals'])) { ?>
    <div class="modal fade" id="kit-status-filter-modal" tabindex="-1" role="dialog" aria-labelledby="Status Filter"
         aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">
                        &times;
                    </button>
                    <h4 class="modal-title">
                        Filter
                    </h4>
                </div>
                <div class="modal-body">
                    <form id="kit-status-filter-modal-form" class="form-horizontal" role="form">
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Problem</label>
                            <div class="col-sm-10">
                                <input id="kit-status-filter-modal-probid" type="text" class="form-control"
                                       placeholder="Enter the problem id here"
                                       value="<?= isset($_SESSION['status-filter']['prob']) ? $_SESSION['status-filter']['prob'] : '' ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Username</label>
                            <div class="col-sm-10">
                                <input id="kit-status-filter-modal-username" type="text" class="form-control"
                                       placeholder="Enter the username id here"
                                       value="<?= isset($_SESSION['status-filter']['user']) ? $_SESSION['status-filter']['user'] : '' ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Result</label>
                            <div class="col-sm-10">
                                <select id="kit-status-filter-modal-verdict" class="selectpicker show-tick form-control"
                                        data-live-search="false">
                                    <option value="*">Any Verdicts</option>
                                    <?php foreach ($kitSupportedVerdicts as $caption => $data) { ?>
                                        <?php if (isset($_SESSION['status-filter']['verdict']) && $_SESSION['status-filter']['verdict'] == $caption) { ?>
                                            <option value="<?= $caption ?>" selected><?= $data ?></option>
                                        <?php } else { ?>
                                            <option value="<?= $caption ?>"><?= $data ?></option>
                                        <?php } ?>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Language</label>
                            <div class="col-sm-10">
                                <select id="kit-status-filter-modal-language"
                                        class="selectpicker show-tick form-control" data-live-search="false">
                                    <option value="*">Any Languages</option>
                                    <?php foreach ($kitSupportedLanguages as $caption => $data) { ?>
                                        <?php if (isset($_SESSION['status-filter']['lang']) && $_SESSION['status-filter']['lang'] == $caption) { ?>
                                            <option value="<?= $caption ?>" selected><?= $caption ?></option>
                                        <?php } else { ?>
                                            <option value="<?= $caption ?>"><?= $caption ?></option>
                                        <?php } ?>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-2 col-sm-10 btn-group">
                                <button type="submit" class="btn btn-primary">Submit</button>
                                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            </div>
                        </div>
                    </form>
                    <script type="application/javascript">
                        $('#kit-status-filter-modal-form').submit(function (e) {
                            e.preventDefault();
                            var data = "session=<?=$kitSessionId?>", first = false, object;
                            object = $("#kit-status-filter-modal-probid").val();
                            if (object !== "") {
                                data = data + "&prob=" + object;
                                first = false;
                            }
                            object = $("#kit-status-filter-modal-username").val();
                            if (object !== "") {
                                data = data + "&user=" + encodeURIComponent(object);
                                first = false;
                            }
                            object = $("#kit-status-filter-modal-verdict").val();
                            if (object !== "*") {
                                data = data + "&verdict=" + object;
                                first = false;
                            }
                            object = $("#kit-status-filter-modal-language").val();
                            if (object !== "*") {
                                data = data + "&lang=" + encodeURIComponent(object);
                                first = false;
                            }
                            $(function () {
                                $.ajax({
                                    type: "POST",
                                    url: "<?=$kitBasePath?>/status/filter",
                                    cache: false,
                                    data: data,
                                    success: function (msg) {
                                        window.location.reload();
                                    }
                                });
                            });
                        })
                    </script>
                </div>
            </div>
        </div>
    </div>
<?php } ?>
<?php if (array_key_exists('register', $kitPages[$kitView]['resources']['modals'])) { ?>
    <div class="modal fade" id="kit-register-modal" tabindex="-1" role="dialog" aria-labelledby="register"
         aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <h4 class="modal-title">Register</h4>
                </div>
                <div class="modal-body">
                    <form id="kit-register-modal-form" class="form-horizontal" role="form" method="post">
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Username</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="kit-register-modal-username"
                                       placeholder="Enter your username here">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Password</label>
                            <div class="col-sm-10">
                                <input type="password" class="form-control" id="kit-register-modal-password"
                                       placeholder="Enter your password here">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Confirm</label>
                            <div class="col-sm-10">
                                <input type="password" class="form-control" id="kit-register-modal-password-again"
                                       placeholder="Re-enter your password here">
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-2 control-label">Email</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="kit-register-modal-email"
                                       placeholder="Enter your email here">
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-2 col-sm-10 btn-group">
                                <button type="submit" class="btn btn-primary">Register</button>
                                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            </div>
                        </div>
                    </form>
                    <script type="application/javascript">
                        function kitRegFormValidator() {
                            $("#kit-register-modal-form").bootstrapValidator({
                                message: "This value is not valid",
                                feedbackIcons: {
                                    valid: "glyphicon glyphicon-ok",
                                    invalid: "glyphicon glyphicon-remove",
                                    validating: "glyphicon glyphicon-refresh"
                                },
                                fields: {
                                    username: {
                                        selector: '#kit-register-modal-username',
                                        validators: {
                                            notEmpty: {
                                                message: "The field username is empty."
                                            },
                                            regexp: {
                                                regexp: "51[6-7]0[0-9]{8,8}$",
                                                message: "The username must be your student id."
                                            }
                                        }
                                    },
                                    password: {
                                        selector: '#kit-register-modal-password',
                                        validators: {
                                            notEmpty: {
                                                message: "The field password is empty."
                                            },
                                            stringLength: {
                                                min: 6,
                                                max: 64,
                                                message: "The length of the password should be between 6 and 16."
                                            }
                                        }
                                    },
                                    repassword: {
                                        selector: '#kit-register-modal-password-again',
                                        validators: {
                                            notEmpty: {
                                                message: "The field password is empty."
                                            },
                                            stringLength: {
                                                min: 6,
                                                max: 64,
                                                message: "The length of the password should be between 6 and 16."
                                            }
                                        }
                                    },
                                    email: {
                                        selector: '#kit-register-modal-email',
                                        validators: {
                                            notEmpty: {
                                                message: 'The email cannot be empty.'
                                            },
                                            emailAddress: {
                                                message: 'Invalid email address.'
                                            }
                                        }
                                    }
                                }
                            }).on("success.form.bv", function (o) {
                                if (!o.isDefaultPrevented()) {
                                    o.preventDefault();
                                    if ($('#kit-register-modal-password').val() !== $('#kit-register-modal-password-again').val()) {
                                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                            .wait(function () {
                                                $.jGrowl('The passwords you typed do not match. Type the same password in both text boxes.', {
                                                    position: 'bottom-right',
                                                    sticky: true
                                                });
                                            });
                                        $("#kit-register-modal-form").data("bootstrapValidator").destroy();
                                        $("#kit-register-modal-form").data("bootstrapValidator", null);
                                        kitRegFormValidator();
                                        $("#kit-register-modal-username").val("").focus();
                                        $("#kit-register-modal-password").val("");
                                    } else {
                                        var hashPassword = new jsSHA("SHA-256", "TEXT", {numRounds: parseInt("1", 10)});
                                        hashPassword.update($("#kit-register-modal-password").val());
                                        hashPassword = hashPassword.getHash('HEX');
                                        var i = "username=" + $("#kit-register-modal-username").val()
                                            + "&password=" + hashPassword
                                            + "&email=" + encodeURIComponent($("#kit-register-modal-email").val())
                                            + "&session=<?=$kitSessionId?>";
                                        $.ajax({
                                            type: "POST",
                                            url: "<?=$kitBasePath?>/user/register",
                                            cache: false,
                                            data: i,
                                            success: function (data) {
                                                var result = eval('(' + data + ')');
                                                if (result.verdict) {
                                                    window.location.reload();
                                                } else {
                                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                        .wait(function () {
                                                            $.jGrowl(result.message, {
                                                                position: 'bottom-right',
                                                                sticky: true
                                                            });
                                                        });
                                                }
                                            },
                                            error: function () {
                                                $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                    .wait(function () {
                                                        $.jGrowl('A fatal error occured, please contact the administrator.', {
                                                            position: 'bottom-right',
                                                            sticky: true
                                                        });
                                                    });
                                            }
                                        });
                                    }
                                }
                            });
                        }
                        $LAB.script('<?=$kitBasePath?>/utility/js/bootstrap-validator.min.js')
                            .script('<?=$kitBasePath?>/utility/js/sha.js')
                            .wait(function () {
                                kitRegFormValidator();
                                $("#kit-register-modal").on("show.bs.modal", function () {
                                    $("#kit-register-modal-username").focus()
                                });
                                $("#kit-register-modal").on("hidden.bs.modal", function () {
                                    $(this).find("#kit-register-modal-form")[0].reset()
                                });
                                $("#kit-register-modal").on("hidden.bs.modal", function () {
                                    $("#kit-register-modal-form").data("bootstrapValidator").destroy();
                                    $("#kit-register-modal-form").data("bootstrapValidator", null);
                                    kitRegFormValidator();
                                });
                            });
                    </script>
                </div>
            </div>
        </div>
    </div>
<?php } ?>
