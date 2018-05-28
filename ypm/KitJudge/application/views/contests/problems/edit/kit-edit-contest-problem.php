<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/codemirror.css">
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/jquery.datetimepicker.min.css">
<div class="container">
    <div class="col-md-12">
        <?php if (isset($kitContestProblem)) { ?>
            <h3 style="text-align: center">Edit Problem in Contest <?= $kitContestId ?></h3>
        <?php } else { ?>
            <h3 style="text-align: center">Add Problem in Contest <?= $kitContestId ?></h3>
        <?php } ?>
        <form id="kit-edit-contest-problem-form" class="form-horizontal" role="form" method="post">
            <div class="form-group">
                <label class="col-sm-2 control-label">Problem</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-problem-id" placeholder="Enter problem id here"
                           value="<?= isset($kitContestProblem) ? $kitContestProblem->kitProbId : '' ?>">
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Problem Tag</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-problem-tag" placeholder="Enter problem tag here"
                           value="<?= isset($kitContestProblem) ? $kitContestProblem->kitProbTag : '' ?>">
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-offset-5 btn-group">
                    <button type="submit" class="btn btn-primary">Submit</button>
                    <button type="reset" class="btn btn-default">Revert</button>
                </div>
            </div>
        </form>
        <script type="application/javascript">
            $('#kit-edit-contest-problem-form').submit(function (e) {
                e.preventDefault();
                var data = "session=<?=$kitSessionId?>&prob=" + $('#kit-problem-id').val() + "&tag=" + $('#kit-problem-tag').val();
                $.ajax({
                    type: 'POST',
                    url: "<?=$kitBasePath?>/contests/problems/editdata/<?=$kitContestId?>",
                    cache: false,
                    data: data,
                    success: function (message) {
                        var result = eval('(' + message + ')');
                        if (!result.verdict) {
                            $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                .wait(function () {
                                    $.jGrowl(result.message, {
                                        position: 'bottom-right',
                                        sticky: true
                                    });
                                });
                        } else {
                            window.location.reload();
                        }
                    },
                    error: function () {
                        $LAB.script('<?=$kitBasePath?>/utility/js/jGrowl.min.js')
                            .wait(function () {
                                $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                    .wait(function () {
                                        $.jGrowl('A fatal error occured, please contact <a class="text-danger" href="mailto:<?=$kitAdminEmail?>"><strong><?=$kitAdminEmail?></strong></a>', {
                                            position: 'bottom-right',
                                            sticky: true
                                        });
                                    });
                            });
                    }
                });
            })
        </script>
    </div>
</div>