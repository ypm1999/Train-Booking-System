<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/kit-result.css">
<div class="container">
    <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
        <a href="<?= $kitBasePath ?>/contests/problems/edit/<?= $kitContestId ?>" class="btn btn-success btn-sm"><span
                    class="glyphicon glyphicon-plus"></span></a>
        <a href="<?= $kitBasePath ?>/contests/edit/<?= $kitContestId ?>" class="btn btn-primary btn-sm"><span
                    class="glyphicon glyphicon-edit"></span></a>
    <?php } ?>
    <?php if ($kitStartTime < $kitTime || (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2)) { ?>
        <?php if ($kitStartTime < $kitTime) { ?>
            <h1 style="text-align: center"><?= $kitContest->kitContestName ?></h1>
            <h2 style="text-align: center" id="kit-contest-time-countdown"></h2>
        <?php } ?>
        <a href="<?= $kitBasePath ?>/contests/show/<?= $kitContestId ?>" class="btn btn-success btn-sm">Problems</a>
        <a href="<?= $kitBasePath ?>/contests/board/<?= $kitContestId ?>" class="btn btn-danger btn-sm">Board</a>
        <?php if (isset($_SESSION['kitUser'])) { ?>
            <a href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>" class="btn btn-info btn-sm">Submission</a>
        <?php } ?>
        <hr>
    <?php } ?>
    <div class="row clearfix">
        <div class="col-md-12">
            <?php if (isset($_SESSION['status-filter'])) { ?>
                <button class="btn btn-danger btn-sm" id="kit-status-filter-reset-button"><span
                            class="glyphicon glyphicon-remove-circle"></span></button>
                <script type="application/javascript">
                    $('#kit-status-filter-reset-button').click(function (e) {
                        e.preventDefault();
                        $.ajax({
                            type: "POST",
                            url: "<?=$kitBasePath?>/status/filter",
                            cache: false,
                            data: "session=<?=$kitSessionId?>",
                            success: function (msg) {
                                window.location.reload();
                            }
                        });
                    })
                </script>
            <?php } ?>
            <table id='kit-status-form' class="table table-striped table-hover table-curved">
                <thead>
                <tr>
                    <th>#</th>
                    <th>Submission Time</th>
                    <th>Relative Time</th>
                    <th>User</th>
                    <th>Problem</th>
                    <th>Language</th>
                    <th>Verdict</th>
                    <th>Time</th>
                    <th>Memory</th>
                    <th>Length</th>
                </tr>
                </thead>
                <tbody>
                <?php
                $kitVerdictTranslator = array(
                    '<span class="kit-ac">Accepted</span>',
                    '<span class="kit-tle">Time Limit Exceeded</span>',
                    '<span class="kit-re">Runtime Error</span>',
                    '<span class="kit-old">Output Limit Exceeded</span>',
                    '<span class="kit-mle">Memory Limit Exceeded</span>',
                    '<span class="kit-rjd">Rejected</span>',
                    '<span class="kit-jgf">Judgement Failed</span>',
                    '<span class="kit-wa">Wrong Answer</span>',
                    "<span class='kit-ing'>Queueing</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    "",
                    '<span class="kit-ce">Compilation Error</span>',
                    '<span class="kit-pe">Presentation Error</span>',
                    "<span class='kit-ing'>Compiling</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    '<span class="kit-mlk">Memory leaked</span>',
                );
                ?>
                <?php foreach ($kitStatus->result() as $status) { ?>
                    <tr id="status<?= $status->kitStatusId ?>">
                        <td><a href="<?= $kitBasePath ?>/status/details/<?= $status->kitStatusId ?>"><?= $status->kitStatusId ?></a></td>
                        <td><?= $status->kitStatusTime ?></td>
                        <td><?= floor($status->kitStatusContestTime / 60) ?> Minutes</td>
                        <td><?= $status->kitStatusUser ?></td>
                        <td>
                            <a href="<?= isset($status->kitStatusContestId) ? $kitBasePath . "/contests/problems/show/$status->kitStatusContestId/$status->kitStatusContestTag" : $kitBasePath . "/problems/show/$status->kitStatusProbId" ?>"><?= isset($status->kitStatusContestId) ? $status->kitStatusContestTag : $status->kitStatusProbId ?></a>
                        </td>
                        <td><?= $status->kitStatusLanguage ?></td>
                        <td><?= $status->kitStatusVerdict == 9 ? "<span class='kit-ing'>Running on test $status->kitStatusExtraMessage</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>" : $kitVerdictTranslator[(int)($status->kitStatusVerdict)] ?></td>
                        <td><?= $status->kitStatusUsedTime != null ? $status->kitStatusUsedTime . ' MS' : '' ?></td>
                        <td><?= $status->kitStatusUsedMemory != null ? $status->kitStatusUsedMemory . ' KB' : '' ?></td>
                        <td><?= $status->kitStatusLength ?> B</td>
                    </tr>
                <?php } ?>
                </tbody>
            </table>
            <script type="application/javascript">
                var kitVerdictTranslator = [
                    '<span class="kit-ac">Accepted</span>',
                    '<span class="kit-tle">Time Limit Exceeded</span>',
                    '<span class="kit-re">Runtime Error</span>',
                    '<span class="kit-old">Output Limit Exceeded</span>',
                    '<span class="kit-mle">Memory Limit Exceeded</span>',
                    '<span class="kit-rjd">Rejected</span>',
                    '<span class="kit-jgf">Judgement Failed</span>',
                    '<span class="kit-wa">Wrong Answer</span>',
                    "<span class='kit-ing'>Queueing</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    "<span class='kit-ing'>Running</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    '<span class="kit-ce">Compilation Error</span>',
                    '<span class="kit-pe">Presentation Error</span>',
                    "<span class='kit-ing'>Compiling</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    '<span class="kit-mlk">Memory leaked</span>'
                ];
                $LAB.script('<?=$kitBasePath?>/utility/js/socket.io.js')
                    .wait(function () {
                        var socket = io('http://<?=$kitSocket['host']?>:<?=$kitSocket['port']?>');
                        socket.on('status',
                            function (data) {
                                console.log(data);
                                $('#kit-status-form').find('tr').each(function (index) {
                                    if ($(this).children('td').eq(0).children('a').text() === data['runid'].toString()) {
                                        if (data['case'] === 'COMPILING') {
                                            $(this).children('td').eq(6).html('<span class="kit-ing">Compiling</span><img src="<?=$kitBasePath?>/utility/img/ajax-loader.gif">');
                                        } else if (data['case'] !== 'END') {
                                            $(this).children('td').eq(6).html('<span class="kit-ing">Running on test ' + data['case'].toString() + '</span><img src="<?=$kitBasePath?>/utility/img/ajax-loader.gif">');
                                        } else {
                                            if (data['verdict'] === 0) {
                                                $(this).children('td').eq(7).html(data['t'] + " MS");
                                                $(this).children('td').eq(8).html(data['m'] + " KB");
                                            }
                                            $(this).children('td').eq(6).html(kitVerdictTranslator[parseInt(data['verdict'])]);
                                        }
                                    }
                                })
                            }
                        );
                    });

            </script>
            <div class="text-center">
                <ul class="pagination justify-content-center" id="kit-status-pagination">
                    <?php if ($kitCurPage == 1) { ?>
                        <li class="page-item disabled">
                            <a class="page-link" href="#" aria-label="Previous">
                                <span aria-hidden="true">&laquo;</span>
                                <span class="sr-only">Previous</span>
                            </a>
                        </li>
                    <?php } else { ?>
                        <li class="page-item">
                            <a class="page-link"
                               href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=<?= $kitCurPage - 1 ?>"
                               aria-label="Previous">
                                <span aria-hidden="true">&laquo;</span>
                                <span class="sr-only">Previous</span>
                            </a>
                        </li>
                    <?php } ?>
                    <?php if ($kitCurPage - 2 > 1) { ?>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=1">1</a>
                        </li>
                        <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                    <?php } ?>

                    <?php for ($index = max(1, $kitCurPage - 2); $index < $kitCurPage; $index++) { ?>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=<?= $index ?>"><?= $index ?></a>
                        </li>
                    <?php } ?>
                    <li class="active"><a href="#"><?= $kitCurPage ?></a></li>
                    <?php for ($index = 1; $index <= 2 && $kitCurPage + $index <= $kitStatusTotalPages; $index++) { ?>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=<?= $kitCurPage + $index ?>"><?= $kitCurPage + $index ?></a>
                        </li>
                    <?php } ?>

                    <?php if ($kitCurPage + 2 < $kitStatusTotalPages) { ?>
                        <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=<?= $kitStatusTotalPages ?>"><?= $kitStatusTotalPages ?></a>
                        </li>
                    <?php } ?>

                    <?php if ($kitCurPage == $kitStatusTotalPages) { ?>
                        <li class="page-item disabled">
                            <a class="page-link" href="#" aria-label="Previous">
                                <span aria-hidden="true">&raquo;</span>
                                <span class="sr-only">Next</span>
                            </a>
                        </li>
                    <?php } else { ?>
                        <li class="page-item">
                            <a class="page-link"
                               href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>?page=<?= $kitCurPage + 1 ?>"
                               aria-label="Previous">
                                <span aria-hidden="true">&raquo;</span>
                                <span class="sr-only">Next</span>
                            </a>
                        </li>
                    <?php } ?>
                </ul>
            </div>
        </div>
    </div>
</div>
