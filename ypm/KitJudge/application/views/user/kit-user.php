<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<div class="container">
<div class="col-md-12">
    <form id="kit-user-form" class="form-horizontal" role="form" method="post">
        <div class="form-group">
            <label class="col-sm-2 control-label">Username</label>
            <div class="col-sm-3">
                <input type="text" class="form-control" id="kit-user-username"
                       value=<?=$kitUsername?> readonly="readonly">
            </div>
        </div>
        <div class="form-group">
            <label class="col-sm-2 control-label">Email</label>
            <div class="col-sm-3">
                <input type="text" class="form-control" id="kit-user-email"
                       value=<?=$kitEmail?>>
            </div>
        </div>
        <div class="form-group">
            <label class="col-sm-2 control-label">Old Password</label>
            <div class="col-sm-3">
                <input type="password" class="form-control" id="kit-user-old-password"
                       placeholder="Enter your old password here">
            </div>
        </div>
        <div class="form-group">
            <label class="col-sm-2 control-label">New Password</label>
            <div class="col-sm-3">
                <input type="password" class="form-control" id="kit-user-new-password"
                       placeholder="Enter your new password here">
            </div>
        </div>
        <div class="form-group">
            <label class="col-sm-2 control-label">Confirm Password</label>
            <div class="col-sm-3">
                <input type="password" class="form-control" id="kit-user-new-repassword"
                       placeholder="Repeat your new password here">
            </div>
        </div>
        <div class="form-group">
            <div class="col-sm-offset-2 col-sm-10 btn-group">
                <button type="submit" class="btn btn-primary">Submit</button>
                <button type="button" class="btn btn-default">Back</button>
            </div>
        </div>
    </form>
    <link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/bootstrap-validator.min.css">
    <script type="application/javascript">
        function kitProfileFormValidator() {
            $("#kit-user-form").bootstrapValidator({
                message: "This value is not valid",
                feedbackIcons: {
                    valid: "glyphicon glyphicon-ok",
                    invalid: "glyphicon glyphicon-remove",
                    validating: "glyphicon glyphicon-refresh"
                },
                fields: {
                    oldPassword: {
                        selector: '#kit-user-old-password',
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
                    newPassword: {
                        selector: '#kit-user-new-password',
                        validators: {
                            stringLength: {
                                min: 6,
                                max: 64,
                                message: "The length of the password should be between 6 and 16."
                            }
                        }
                    },
                    repassword: {
                        selector: '#kit-user-new-repassword',
                        validators: {
                            stringLength: {
                                min: 6,
                                max: 64,
                                message: "The length of the password should be between 6 and 16."
                            }
                        }
                    },
                    email: {
                        selector: '#kit-user-email',
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
                    if ($('#kit-user-new-password').val() !== $('#kit-user-new-repassword').val()) {
                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                            .wait(function () {
                                $.jGrowl('The passwords you typed do not match. Type the same password in both text boxes.', {
                                    position: 'bottom-right',
                                    sticky: true
                                });
                            });
                        $("#kit-user-form").data("bootstrapValidator").destroy();
                        $("#kit-user-form").data("bootstrapValidator", null);
                        kitRegFormValidator();
                        $("#kit-user-username").val("").focus();
                        $("#kit-user-new-password").val("");
                        $("#kit-user-new-repassword").val("");
                    } else {
                        var hashOldPassword = new jsSHA("SHA-256", "TEXT", {numRounds: parseInt("1", 10)});
                        hashOldPassword.update($("#kit-user-old-password").val());
                        hashOldPassword = hashOldPassword.getHash('HEX');
                        if ($("#kit-user-new-password").val() != '') {
                            var hashNewPassword = new jsSHA("SHA-256", "TEXT", {numRounds: parseInt("1", 10)});
                            hashNewPassword.update($("#kit-user-new-password").val());
                            hashNewPassword = hashNewPassword.getHash('HEX');
                        } else {
                            var hashNewPassword = "";
                        }
                        var i = "username=" + $("#kit-user-username").val()
                            + "&oldPassword=" + hashOldPassword
                            + "&newPassword=" + hashNewPassword
                            + "&email=" + encodeURIComponent($("#kit-user-email").val())
                            + "&session=<?=$kitSessionId?>";
                        $.ajax({
                            type: "POST",
                            url: "<?=$kitBasePath?>/user/update",
                            cache: false,
                            data: i,
                            success: function (data) {
                                var result = eval('(' + data + ')');
                                if (result.verdict) {
                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                            .wait(function () {
                                                $.jGrowl('Successfully update the information.', {
                                                    position: 'bottom-right'
                                                });
                                            });
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
                kitProfileFormValidator();
                $("#kit-user").on("show.bs.modal", function () {
                    $("#kit-user-username").focus()
                });
                $("#kit-user").on("hidden.bs.modal", function () {
                    $(this).find("#kit-user-form")[0].reset()
                });
                $("#kit-user").on("hidden.bs.modal", function () {
                    $("#kit-user-form").data("bootstrapValidator").destroy();
                    $("#kit-user-form").data("bootstrapValidator", null);
                    kitRegFormValidator();
                });
            });
    </script>
</div>
</div>