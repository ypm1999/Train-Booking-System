<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/codemirror.css">
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/jquery.datetimepicker.min.css">
<div class="container">
    <div class="col-md-12">
        <?php if (isset($kitContest)) { ?>
            <h3 style="text-align: center">Edit Contest <?= $kitContestId ?>. <span
                        id="kit-contest-name"><?= $kitContest['kitContestName'] ?></span></h3>
        <?php } else { ?>
            <h3 style="text-align: center">Add Contest <?= $kitContestId ?>. <span id="kit-contest-name"></span></h3>
        <?php } ?>
        <form id="kit-edit-contest-form" class="form-horizontal" role="form" method="post">
            <div id="kit-edit-contest-messsage" style="display: none;"></div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Title</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-edit-contest-title"
                           placeholder="Enter contest title here"
                           onchange="$('#kit-contest-name').html($('#kit-edit-contest-title').val());"
                           value="<?= isset($kitContest) ? $kitContest['kitContestName'] : '' ?>">
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Type</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-edit-contest-type"
                           placeholder="Enter contest title here"
                           value="<?= isset($kitContest) ? $kitContest['kitContestType'] : '' ?>">
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label"></label>
                <div class="col-sm-1">
                    <?php if (!isset($kitContest) || $kitContest['kitContestHidden']) { ?>
                        <input type="checkbox" id="kit-edit-contest-hidden" checked> Hidden
                    <?php } else { ?>
                        <input type="checkbox" id="kit-edit-contest-hidden"> Hidden
                    <?php } ?>
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Source</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-edit-contest-source"
                           placeholder="Enter contest source here"
                           value="<?= isset($kitContest) ? $kitContest['kitContestSource'] : '' ?>">
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Start</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control form_datetime" id="kit-edit-contest-start"
                           value="<?= isset($kitContest) ? $kitContest['kitContestStart'] : date('Y-m-d H:i') ?>">
                </div>
                <script type="application/javascript">
                    $LAB.script('<?=$kitBasePath?>/utility/js/jquery.datetimepicker.full.js')
                        .wait(function () {
                            $('#kit-edit-contest-start').datetimepicker({
                                format: 'Y-m-d H:i'
                            });
                        });
                </script>
            </div>
            <div class="form-group">
                <label class="col-sm-2 control-label">Duration</label>
                <div class="col-sm-4">
                    <input type="text" class="form-control" id="kit-edit-contest-duration"
                           placeholder="Enter contest duration(in seconds) here"
                           value="<?= isset($kitContest) ? $kitContest['kitContestDuration'] : '' ?>">
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
            $('#kit-edit-contest-form').submit(function (e) {
                e.preventDefault();
                var data = "session=<?=$kitSessionId?><?=isset($kitContest) ? '&contest=' . $kitContest['kitContestId'] : ''?>&title="
                    + encodeURIComponent($('#kit-edit-contest-title').val())
                    + "&type=" + encodeURIComponent($('#kit-edit-contest-type').val())
                    + "&source=" + encodeURIComponent($('#kit-edit-contest-source').val())
                    + "&start=" + encodeURIComponent($('#kit-edit-contest-start').val())
                    + "&duration=" + encodeURIComponent($('#kit-edit-contest-duration').val());
                if ($('#kit-edit-contest-hidden')[0].checked) {
                    data = data + '&hidden=1';
                } else {
                    data = data + '&hidden=0';
                }
                $.ajax({
                    type: 'POST',
                    url: "<?=$kitBasePath?>/contests/editdata",
                    cache: false,
                    data: data,
                    success: function (message) {
                        var result = eval('(' + message + ')');
                        if (!result.verdict) {
                            $('#kit-edit-contest-messsage')
                                .addClass('alert')
                                .addClass('alert-warning')
                                .addClass('alert-dismissable')
                                .html(result.message)
                                .show();
                        } else {
                            window.location.reload();
                        }
                    },
                    error: function () {
                        $('#kit-edit-contest-messsage')
                            .addClass('alert')
                            .addClass('alert-danger')
                            .addClass('alert-dismissable')
                            .html('A fatal error occured, please contact <a class="text-danger" href="mailto:<?=$kitAdminEmail?>"><strong><?=$kitAdminEmail?></strong></a>.')
                            .show();
                    }
                });
            })
        </script>
    </div>
</div>