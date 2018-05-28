<div class="container">
    
    <?php if ($kitStartTime < $kitTime || (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2)) { ?>
        <?php if ($kitStartTime < $kitTime) { ?>
            <h1 style="text-align: center"><?= $kitContest->kitContestName ?></h1>
            <h2 style="text-align: center" id="kit-contest-time-countdown"></h2>
        <?php } ?>
        <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
            <a href="<?= $kitBasePath ?>/contests/problems/edit/<?= $kitContestId ?>" class="btn btn-success btn-sm"><span
                        class="glyphicon glyphicon-plus"></span></a>
            <a href="<?= $kitBasePath ?>/contests/edit/<?= $kitContestId ?>" class="btn btn-primary btn-sm"><span
                        class="glyphicon glyphicon-edit"></span></a>
        <?php } ?>
        <a href="<?= $kitBasePath ?>/contests/show/<?= $kitContestId ?>" class="btn btn-success btn-sm">Problems</a>
        <a href="<?= $kitBasePath ?>/contests/board/<?= $kitContestId ?>" class="btn btn-danger btn-sm">Board</a>
        <?php if (isset($_SESSION['kitUser'])) { ?>
            <a href="<?= $kitBasePath ?>/contests/submission/<?= $kitContestId ?>" class="btn btn-info btn-sm">Submission</a>
        <?php } ?>
        <hr>
    <?php } ?>
    <?php if ($kitStartTime > $kitTime) { ?>
        <div class="jumbotron">
            <h1 style="text-align: center"><?= $kitContest->kitContestName ?></h1>
            <h2 style="text-align: center" id="kit-contest-time-countdown"></h2>
            <script type="application/javascript">
                var localTime = Math.floor(Date.now() / 1000);
                var remoteTime = <?=$kitTime?>;
                var startRemoteTime = <?=$kitStartTime?>;
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
    <?php } ?>
    <?php if ($kitStartTime <= $kitTime || (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2)) { ?>
        <table class="table table-hover">
            <thead>
            <tr>
                <th>#</th>
                <th>Name</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach ($kitProblems as $caption => $problem) { ?>
                <tr>
                    <td>
                        <a href="<?= $kitBasePath ?>/contests/problems/show/<?= $kitContestId ?>/<?= $caption ?>"><?= $caption ?></a>
                    </td>
                    <td>
                        <a href="<?= $kitBasePath ?>/contests/problems/show/<?= $kitContestId ?>/<?= $caption ?>"><?= $problem->kitProbName ?></a>
                    </td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    <?php } ?>
</div>