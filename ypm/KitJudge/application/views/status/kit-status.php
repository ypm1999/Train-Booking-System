<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/kit-result.css">
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12">
            <div class="table-responsive">
                <table id='kit-status-form' class="table table-striped table-hover table-curved">
                    <thead>
                    <tr>
                        <th>
                        <button class="btn btn-primary btn-xs" data-toggle="modal" data-target="#kit-status-filter-modal"><span
                            class="glyphicon glyphicon-filter"></span></button>
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
                        </th>
                        <th class="hidden-xs hidden-sm">Submission Time</th>
                        <th>User</th>
                        <th class="hidden-xs hidden-sm">Contest</th>
                        <th class="hidden-xs">Problem</th>
                        <th class="visible-xs">Prob</th>
                        <th class="hidden-xs">Language</th>
                        <th class="visible-xs">Lang</th>
                        <th>Verdict</th>
                        <th>Time</th>
                        <th>Memory</th>
                        <th>Length</th>
                        <th></th>
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
                        '<span class="kit-mlk">Memory Error</span>',
                    );
                    $kitVerdictTranslatorMobile = array(
                        '<span class="kit-ac">AC</span>',
                        '<span class="kit-tle">TLE</span>',
                        '<span class="kit-re">RE</span>',
                        '<span class="kit-old">OLE</span>',
                        '<span class="kit-mle">MLE</span>',
                        '<span class="kit-rjd">Reject</span>',
                        '<span class="kit-jgf">Fail</span>',
                        '<span class="kit-wa">WA</span>',
                        "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                        "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                        '<span class="kit-ce">CE</span>',
                        '<span class="kit-pe">PE</span>',
                        "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                        '<span class="kit-mlk">ME</span>'
                    )
                    ?>
                    <?php foreach ($kitStatus as $status) { ?>
                        <tr id="status-<?= $status->kitStatusId ?>">
                            <?php if ($status->valid){ ?>
                                <td><a href="<?= $kitBasePath ?>/status/details/<?= $status->kitStatusId ?>"><?= $status->kitStatusId ?></a></td>
                            <?php }else {?>
                                <td><?= $status->kitStatusId ?></a></td>
                            <?php } ?>
                            <td class="hidden-xs hidden-sm"><?= $status->kitStatusTime ?></td>
                            <td><?= $status->kitStatusUser ?></td>
                            <td class="hidden-xs hidden-sm"><?= isset($status->kitStatusContestId) ? "<a href='$kitBasePath/contests/show/$status->kitStatusContestId'>$status->kitStatusContestId</a>" : '' ?></td>
                            <td>
                                <a href="<?= isset($status->kitStatusContestId) ? $kitBasePath . "/contests/problems/show/$status->kitStatusContestId/$status->kitStatusContestTag" : $kitBasePath . "/problems/show/$status->kitStatusProbId" ?>"><?= isset($status->kitStatusContestId) ? "$status->kitStatusContestId-$status->kitStatusContestTag" : $status->kitStatusProbId ?></a>
                            </td>
                            <td><?= $status->kitStatusLanguage ?></td>
                            <td class="hidden-xs"><?= $status->kitStatusVerdict == 9 ? "<span class='kit-ing'>Running on test $status->kitStatusExtraMessage</span><img src='$kitBasePath/utility/img/ajax-loader.gif'>" : $kitVerdictTranslator[(int)($status->kitStatusVerdict)] ?></td>
                            <td class="visible-xs"><?= $status->kitStatusVerdict == 9 ? "<img src='$kitBasePath/utility/img/ajax-loader.gif'>" : $kitVerdictTranslatorMobile[(int)($status->kitStatusVerdict)] ?></td>
                            <td><?= $status->kitStatusUsedTime != null ? $status->kitStatusUsedTime . ' MS' : '' ?></td>
                            <td><?= $status->kitStatusUsedMemory != null ? $status->kitStatusUsedMemory . ' KB' : '' ?></td>
                            <?php if ($status->kitStatusLanguage != 'Git') { ?>
                                <td><?= $status->kitStatusLength ?> B</td>
                            <?php } else { ?>
                                <td></td>
                            <?php } ?>
                            <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) {?>
                                <td>
                                    <a href="#" id="rejudge-<?= $status->kitStatusId?>" onclick="rejudge(<?= $status->kitStatusId?>)"><span class="glyphicon glyphicon-refresh"></span></a>
                                </td>
                            <?php } ?>
                        </tr>
                    <?php } ?>
                    <script type="application/javascript">
                        function rejudge(id) {
                            var data = 'session=<?=$kitSessionId?>';
                            data += '&id=' + id.toString();
                            $.ajax({
                                type: 'POST',
                                url: '<?=$kitBasePath?>/status/rejudge',
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
                                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                            .wait(function () {
                                                $.jGrowl('Successfully rejudge the code', {
                                                    position: 'bottom-right'
                                                });
                                                setTimeout("location.reload()", 1000);
                                            });
                                    }
                                },
                                error: function () {
                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                        .wait(function () {
                                            $.jGrowl('Fatal error occured, please contact the administrator.', {
                                                position: 'bottom-right'
                                            });
                                        });
                                }
                            })
                        };
                    </script>
                    </tbody>
                </table>
            </div>
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
                    '<span class="kit-mlk">Memory Error</span>'
                ];
                var kitVerdictTranslatorMobile = [
                    '<span class="kit-ac">AC</span>',
                    '<span class="kit-tle">TLE</span>',
                    '<span class="kit-re">RE</span>',
                    '<span class="kit-old">OLE</span>',
                    '<span class="kit-mle">MLE</span>',
                    '<span class="kit-rjd">Reject</span>',
                    '<span class="kit-jgf">Fail</span>',
                    '<span class="kit-wa">WA</span>',
                    "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    '<span class="kit-ce">CE</span>',
                    '<span class="kit-pe">PE</span>',
                    "<img src='$kitBasePath/utility/img/ajax-loader.gif'>",
                    '<span class="kit-mlk">ME</span>'
                ];
                $LAB.script('<?=$kitBasePath?>/utility/js/socket.io.js')
                    .wait(function () {
                        var socket = io('http://<?=$kitSocket['host']?>:<?=$kitSocket['port']?>');
                        socket.on('status',
                            function (data) {
                                console.log(data);
                                $('#kit-status-form').find('tr').each(function (index) {
                                    if ($(this).children('td').eq(0).children('a').text() === data['runid'].toString()) {
                                        if (data['case'] !== 'END') {
                                            $(this).children('td').eq(6).html('<span class="kit-ing">' + data['case'].toString() + '</span><img src="<?=$kitBasePath?>/utility/img/ajax-loader.gif">');
                                        } else {
                                            if (data['verdict'] === 0) {
                                                $(this).children('td').eq(8).html(data['t'] + " MS");
                                                $(this).children('td').eq(9).html(data['m'] + " KB");
                                            }
                                            $(this).children('td').eq(6).html(kitVerdictTranslator[parseInt(data['verdict'])]);
                                            $(this).children('td').eq(7).html(kitVerdictTranslatorMobile[parseInt(data['verdict'])]);
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
                            <a class="page-link" href="<?= $kitBasePath ?>/status?page=<?= $kitCurPage - 1 ?>"
                               aria-label="Previous">
                                <span aria-hidden="true">&laquo;</span>
                                <span class="sr-only">Previous</span>
                            </a>
                        </li>
                    <?php } ?>
                    <?php if ($kitCurPage - 2 > 1) { ?>
                        <li class="page-item"><a class="page-link" href="<?= $kitBasePath ?>/status?page=1">1</a></li>
                        <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                    <?php } ?>

                    <?php for ($index = max(1, $kitCurPage - 2); $index < $kitCurPage; $index++) { ?>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/status?page=<?= $index ?>"><?= $index ?></a>
                        </li>
                    <?php } ?>
                    <li class="active"><a href="#"><?= $kitCurPage ?></a></li>
                    <?php for ($index = 1; $index <= 2 && $kitCurPage + $index <= $kitStatusTotalPages; $index++) { ?>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/status?page=<?= $kitCurPage + $index ?>"><?= $kitCurPage + $index ?></a>
                        </li>
                    <?php } ?>

                    <?php if ($kitCurPage + 2 < $kitStatusTotalPages) { ?>
                        <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                        <li class="page-item"><a class="page-link"
                                                 href="<?= $kitBasePath ?>/status?page=<?= $kitStatusTotalPages ?>"><?= $kitStatusTotalPages ?></a>
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
                            <a class="page-link" href="<?= $kitBasePath ?>/status?page=<?= $kitCurPage + 1 ?>"
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
