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
    <script type="application/javascript">
        var localTime = Math.floor(Date.now() / 1000);
        var remoteTime = <?=$kitTime?>;
        var startRemoteTime = <?=$kitStartTime?>;
    </script>
    <?php if ($kitStartTime > $kitTime) { ?>
        <div class="jumbotron">
            <h1 style="text-align: center"><?= $kitContest->kitContestName ?></h1>
            <h2 style="text-align: center" id="kit-contest-time-countdown"></h2>
            <script type="application/javascript">
                function kitRefreshCountDown() {
                    var curLocalTime = Math.floor(Date.now() / 1000);
                    if (curLocalTime >= startRemoteTime) window.location.reload();
                    var curRemoteTime = curLocalTime + remoteTime - localTime;
                    var remainTime = startRemoteTime - curRemoteTime;
                    var days = Math.floor(remainTime / 1440 / 60);
                    var hours = Math.floor((remainTime - days * 1440 * 60) / 3600);
                    var minutes = Math.floor((remainTime - days * 1440 * 60 - hours * 3600) / 60);
                    var seconds = (remainTime - days * 1440 * 60 - hours * 3600 - minutes * 60);
                    var message = '';
                    if (days > 0) message = message + (days.toString()) + ' Day' + ((days > 1) ? 's ' : ' ');
                    message = message + (hours.toString()) + ' Hour' + ((hours > 1) ? 's ' : ' ');
                    message = message + (minutes.toString()) + ' Minute' + ((minutes > 1) ? 's ' : ' ');
                    message = message + (seconds.toString()) + ' Second' + ((seconds > 1) ? 's ' : ' ');
                    $('#kit-contest-time-countdown').html(message);
                    setTimeout(kitRefreshCountDown, 1000);
                }
                kitRefreshCountDown();
            </script>
        </div>
    <?php } else { ?>
    <?php } ?>
    <?php if ($kitStartTime <= $kitTime || (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2)) { ?>
        <p></p>
        <style>
            .table-curved {
                border-collapse: separate;
            }

            .table-curved {
                border: solid #ccc 1px;
                border-radius: 10px;
                border-left: 0;
            }

            .table-curved td, .table-curved th {
                border-left: 1px solid #ccc;
                border-top: 1px solid #ccc;
            }

            .table-curved th {
                border-top: none;
            }

            .table-curved th:first-child {
                border-radius: 10px 0 0 0;
            }

            .table-curved th:last-child {
                border-radius: 0 10px 0 0;
            }

            .table-curved th:only-child {
                border-radius: 10px 10px 0 0;
            }

            .table-curved tr:last-child td:first-child {
                border-radius: 0 0 0 10px;
            }

            .table-curved tr:last-child td:last-child {
                border-radius: 0 0 10px 0;
            }

            .table-align tr, th {
                text-align: center;
            }

            .table > tbody > tr > td {
                vertical-align: middle;
            }

            .kit-cell-0 {
                height: 2cm;
            }

            .kit-cell-1 {
                height: 2cm;
                background-color: #98F898;
            }

            .kit-cell-2 {
                height: 2cm;
                background-color: #FFDAB9;
            }

            .kit-cell-3 {
                height: 2cm;
                background-color: #DDA0DD;
            }
        </style>
        <table class="table table-striped table-hover table-curved table-align">
            <thead id="kit-board-thead" style="font-size: 120%">
            <tr style="height: 1cm">
                <th style="width: 5%"></th>
                <th><strong>User</strong></th>
                <?php $width = min(9, 1.0 * (100 - 26) / (count($kitProbTags) + 1));?>
                <?php foreach ($kitProbTags as $tag) { ?>
                    <th style="width: <?=$width?>%"><strong><?= $tag ?></strong></th>
                <?php } ?>
                <th style="width: 7%"><strong>Total</strong></th>
                <th style="width: 7%"><strong>Penalty</strong></th>
            </tr>
            </thead>
            <tbody id="kit-board-tbody" valign="middle" style="font-size: 120%">
            </tbody>
        </table>
        <script type="application/javascript">
            function kitRenderBoard(data) {
                console.log(data);
                var performances = [];
                for (var i = 0; i < data.length; i++) {
                    var rid = data[i].rid;
                    var user = data[i].user;
                    var rtime = data[i].rtime;
                    var verdict = data[i].verdict;
                    var tag = data[i].tag;
                    var time = data[i].time;
                    if (!(user in performances)) {
                        performances[user] = {
                            'penalty': 0,
                            'solve': 0,
                            'submission': 0,
                            'probinf': []
                        };
                    }
                    if (verdict === "10") continue;
                    if (!(tag in performances[user]['probinf'])) {
                        performances[user]['probinf'][tag] = {
                            'inf': [],
                            'status': 0, // 0: not try, 1: accepted, 2: tried but not accepted, 3: pending
                            'submission': 0
                        };
                    }
                    performances[user]['probinf'][tag]['inf'].push({
                        'runid': rid,
                        'rtime': rtime,
                        'verdict': verdict,
                        'time': time
                    });
                }
                for (var user in performances) { // penalty
                    for (var tag in performances[user]['probinf']) {
                        var actime = undefined;
                        for (var submission in performances[user]['probinf'][tag]['inf']) {
                            submission = performances[user]['probinf'][tag]['inf'][submission];
                            if (submission['verdict'] === "8" || submission['verdict'] === "9" || submission['verdict'] === "12") {
                                if (performances[user]['probinf'][tag]['status'] !== 1) { // pending
                                    performances[user]['probinf'][tag]['status'] = 3;
                                }
                                continue;
                            }
                            if (submission['verdict'] === "0") {
                                performances[user]['probinf'][tag]['status'] = 1;
                                if (actime === undefined) {
                                    actime = parseInt(submission['rtime']);
                                } else {
                                    actime = Math.min(parseInt(submission['rtime']), actime);
                                }
                            } else {
                                if (performances[user]['probinf'][tag]['status'] !== 1) { // pending
                                    performances[user]['probinf'][tag]['status'] = 2;
                                }
                            }
                        }
                        if (actime !== undefined) {
                            performances[user]['penalty'] += actime;
                            for (var submission in performances[user]['probinf'][tag]['inf']) {
                                if (submission['verdict'] === "8" || submission['verdict'] === "9" || submission['verdict'] === "12") continue;
                                submission = performances[user]['probinf'][tag]['inf'][submission];
                                if (submission['rtime'] <= actime) {
                                    performances[user]['probinf'][tag]['submission']++;
                                    performances[user]['penalty'] += 20 * 60;
                                    performances[user]['submission']++;
                                }
                            }
                            performances[user]['solve']++;
                            performances[user]['probinf'][tag]['actime'] = actime;
                        } else {
                            for (var submission in performances[user]['probinf'][tag]['inf']) {
                                if (submission['verdict'] === "8" || submission['verdict'] === "9" || submission['verdict'] === "12") continue;
                                performances[user]['probinf'][tag]['submission']++;
                                performances[user]['submission']++;
                            }
                        }
                    }
                }
                var items = Object.keys(performances).map(function (key) {
                    return [key, performances[key]];
                });
                items.sort(function (first, second) {
                    if (first[1]['solve'] > second[1]['solve']) return -1;
                    if (first[1]['solve'] < second[1]['solve']) return 1;
                    return first[1]['penalty'] - second[1]['penalty'];
                });
                var index = 0;
                items.forEach(function (performance) {
                    var tbHTML = "";
                    tbHTML = tbHTML + '<tr><td>' + (++index) + '</td>';
                    tbHTML = tbHTML + '<td>' + performance[0] + '</td>';
                    <?php foreach ($kitProbTags as $tag) {?>
                    if (performance[1]['probinf']['<?=$tag?>'] !== undefined) {
                        console.log(performance[1]['probinf']['<?=$tag?>']['status']);
                        tbHTML = tbHTML + '<td class="kit-cell-' + performance[1]['probinf']['<?=$tag?>']['status'] + '">';
                        if (performance[1]['probinf']['<?=$tag?>']['status'] === 1) {
                            tbHTML = tbHTML + performance[1]['probinf']['<?=$tag?>']['submission'] + 'Y<br />';
                            tbHTML = tbHTML + Math.floor(performance[1]['probinf']['<?=$tag?>']['actime'] / 60);
                        } else {
                            tbHTML = tbHTML + performance[1]['probinf']['<?=$tag?>']['submission'];
                        }
                        tbHTML = tbHTML + '</td>';
                    } else {
                        tbHTML = tbHTML + '<td class="kit-cell-0"></td>';
                    }
                    <?php }?>
                    tbHTML = tbHTML + '<td>' + performance[1]['solve'] + '/' + performance[1]['submission'] + '</td>';
                    tbHTML = tbHTML + '<td>' + Math.floor(performance[1]['penalty'] / 60) + '</td>';
                    tbHTML = tbHTML + '</tr>';
                    $('#kit-board-tbody').append(tbHTML);
                });
            }
            $.ajax({
                type: "POST",
                url: "/KitJudge/api/contest/board/<?=$kitContestId?>",
                cache: false,
                data: {},
                success: function (data) {
                    data = eval('(' + data + ')');
                    if (!data.verdict) {
                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                            .wait(function () {
                                $.jGrowl(data.message, {
                                    position: 'bottom-right',
                                    sticky: true
                                });
                            });
                    } else {
                        kitRenderBoard(data.board);
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
        </script>
    <?php } ?>
</div>