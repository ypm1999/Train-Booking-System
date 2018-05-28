<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12 column">
            <div class="row">
                <div class="col-md-12">
                    <div class="body-footer">
                        <!-- <span><a href="https://github.com/Kipsora/KitJudge/blob/master/README.md"
                                 target="_blank">About</a>
                            | <a href="<?= $kitBasePath ?>/faq">F.A.Qs</a>
                            | <a href="https://github.com/Kipsora/KitJudge/blob/master/LICENSE"
                                 target="_blank">License</a>
                            | <a href="https://github.com/Kipsora/KitJudge" target="_blank">GitHub Project</a>
                            | <a href="mailto:kipsora@gmail.com">Author</a></span><br>-->
                            All Copyright Reserved ©2010-<?= date("Y")?> ACM Class<br>
                        <p id="kitServerTime"></p>
                        <script>
                            function kitGetServerTime() {
                                $.ajax({
                                    type: "GET", url: "<?=$kitBasePath?>/time",
                                    cache: false,
                                    success: function (e) {
                                        $("#kitServerTime").html("Server Time: " + e)
                                    },
                                    error: function () {
                                        $("#kitServerTime").html("disconnected")
                                    }
                                });
                                setTimeout(kitGetServerTime, 1e4)
                            }
                            kitGetServerTime();
                        </script>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
