<?php
defined('BASEPATH') OR exit('No direct script access allowed');
?>
<div class="container">
    <div class="table-responsive">
        <table class="table table-hover">
            <thead>
            <tr>
                <th>
                <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                    <a href="<?= $kitBasePath ?>/contests/edit" class="btn btn-success btn-xs"><span class="glyphicon glyphicon-plus"></span></a>
                <?php } else { ?>
                        # 
                    <?php }?>
                </th>
                <th>Name</th>
                <th>Type</th>
                <th class="hidden-xs">Source</th>
                <th>Start</th>
                <th>End</th>
                <th class="hidden-xs">Duration</th>
                <th>Status</th>
                <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                    <th>Visibility</th>
                <?php } ?>
            </tr>
            </thead>
            <tbody>
            <?php foreach ($kitContests->result() as $contest) { ?>
                <tr>
                    <td class="col-md-1"><a href="<?= $kitBasePath ?>/contests/show/<?= $contest->kitContestId ?>"><?= $contest->kitContestId ?></a></td>
                    <td class="col-md-2"><a href="<?= $kitBasePath ?>/contests/show/<?= $contest->kitContestId ?>"><?= $contest->kitContestName ?></a></td>
                    <td class="col-md-1"><?= $contest->kitContestType ?></td>
                    <td class="col-md-2 hidden-xs"><?= $contest->kitContestSource ?></td>
                    <td class="col-md-2"><?= $contest->kitContestStart ?></td>
                    <td class="col-md-2"><?= date('Y-m-d H:i:s', strtotime($contest->kitContestStart) + $contest->kitContestDuration) ?></td>
                    <td class="col-md-1 hidden-xs"><?= floor($contest->kitContestDuration / 3600) ?>:<?= floor($contest->kitContestDuration % 3600 / 60) ?>:<?= floor($contest->kitContestDuration % 60) ?></td>
                    <td class="col-md-2"><strong>
                            <?php
                            $curTime = time();
                            $startTime = strtotime($contest->kitContestStart);
                            if ($curTime < $startTime) echo "<span class='text-danger'>Upcoming</span>";
                            else if ($curTime < $startTime + $contest->kitContestDuration) {
                                echo "<span class='text-primary'>Running</span>";
                            } else {
                                echo "<span class='text-default'>Ended</span>";
                            }
                            ?>
                        </strong></td>
                    <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                        <td><i><?= $contest->kitContestHidden ? 'Invisible' : 'Visible' ?></i></td>
                    <?php } ?>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
    <div class="text-center">
        <ul class="pagination justify-content-center" id="kit-contest-list-pagination">
            <?php if ($kitCurPage == 1) { ?>
                <li class="page-item disabled">
                    <a class="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">&laquo;</span>
                        <span class="sr-only">Previous</span>
                    </a>
                </li>
            <?php } else { ?>
                <li class="page-item">
                    <a class="page-link" href="<?= $kitBasePath ?>/contests?page=<?= $kitCurPage - 1 ?>"
                       aria-label="Previous">
                        <span ari a-hidden="true">&laquo;</span>
                        <span class="sr-only">Previous</span>
                    </a>
                </li>
            <?php } ?>
            <?php if ($kitCurPage - 2 > 1) { ?>
                <li class="page-item"><a class="page-link" href="<?= $kitBasePath ?>/contests?page=1">1</a></li>
                <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
            <?php } ?>

            <?php for ($index = max(1, $kitCurPage - 2); $index < $kitCurPage; $index++) { ?>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/contests?page=<?= $index ?>"><?= $index ?></a></li>
            <?php } ?>
            <li class="active"><a href="#"><?= $kitCurPage ?></a></li>
            <?php for ($index = 1; $index <= 2 && $kitCurPage + $index <= $kitContestsTotalPages; $index++) { ?>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/contests?page=<?= $kitCurPage + $index ?>"><?= $kitCurPage + $index ?></a>
                </li>
            <?php } ?>

            <?php if ($kitCurPage + 2 < $kitContestsTotalPages) { ?>
                <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/contests?page=<?= $kitContestsTotalPages ?>"><?= $kitContestsTotalPages ?></a>
                </li>
            <?php } ?>

            <?php if ($kitCurPage == $kitContestsTotalPages) { ?>
                <li class="page-item disabled">
                    <a class="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">&raquo;</span>
                        <span class="sr-only">Next</span>
                    </a>
                </li>
            <?php } else { ?>
                <li class="page-item">
                    <a class="page-link" href="<?= $kitBasePath ?>/contests?page=<?= $kitCurPage + 1 ?>"
                       aria-label="Previous">
                        <span aria-hidden="true">&raquo;</span>
                        <span class="sr-only">Next</span>
                    </a>
                </li>
            <?php } ?>
        </ul>
    </div>
</div>
