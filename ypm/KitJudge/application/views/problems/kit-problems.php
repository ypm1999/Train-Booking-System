<div class="container">
    <div class="table-responsive">
        <table class="table table-hover">
            <thead>
            <tr>
                <th>
                <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                    <a href="<?= $kitBasePath ?>/problems/edit" class="btn btn-success btn-xs"><span
                                class="glyphicon glyphicon-plus"></span></a>
                <?php } else { ?>
                    # 
                <?php }?>
                </th>
                <th>Title</th>
                <th class="hidden-xs">Source</th>
                <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                    <th>Visibility</th>
                <?php } ?>
                <th class="hidden-xs">Ratio (Accepted / Submitted)</th>
                <th class="visible-xs">Ratio</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach ($kitProblems->result() as $problem) { ?>
                <tr>
                    <td class="col-md-1"><a
                                href="<?= $kitBasePath ?>/problems/show/<?= $problem->kitProbId ?>"><?= $problem->kitProbId ?></a>
                    </td>
                    <td class="col-lg-3 hidden-xs"><a
                                href="<?= $kitBasePath ?>/problems/show/<?= $problem->kitProbId ?>"><?= $problem->kitProbName ?></a>
                    </td>
                    <td class="col-lg-3"><?= $problem->kitProbSource ?></td>
                    <?php if (isset($_SESSION['kitUser']) && $_SESSION['kitUser']['priority'] >= 2) { ?>
                        <td><i><?= $problem->kitProbHidden ? 'Invisible' : 'Visible' ?></i></td>
                    <?php } ?>
                    <td>
                        <?php
                        $ratio = $problem->kitProbSubmitted == 0 ? 0.0 :
                            100.0 * $problem->kitProbAccepted / $problem->kitProbSubmitted;
                        ?>
                        <div class="progress" style="text-align: center; margin-bottom: 0 !important;">
                            <div class="progress-bar progress-bar-info" style="width: <?= $ratio ?>%;">
                                    <span class="progress-value" style="position: 50%; color: #0f0f0f;">
                                        <?php echo sprintf("%.2f", $ratio); ?>%(<?= $problem->kitProbAccepted ?>/<?= $problem->kitProbSubmitted ?>)
                                    </span>
                            </div>
                        </div>
                    </td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
    <div class="text-center">
        <ul class="pagination justify-content-center" id="kit-problem-list-pagination">
            <?php if ($kitCurPage == 1) { ?>
                <li class="page-item disabled">
                    <a class="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">&laquo;</span>
                        <span class="sr-only">Previous</span>
                    </a>
                </li>
            <?php } else { ?>
                <li class="page-item">
                    <a class="page-link" href="<?= $kitBasePath ?>/problems?page=<?= $kitCurPage - 1 ?>"
                       aria-label="Previous">
                        <span ari a-hidden="true">&laquo;</span>
                        <span class="sr-only">Previous</span>
                    </a>
                </li>
            <?php } ?>
            <?php if ($kitCurPage - 2 > 1) { ?>
                <li class="page-item"><a class="page-link" href="<?= $kitBasePath ?>/problems?page=1">1</a></li>
                <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
            <?php } ?>

            <?php for ($index = max(1, $kitCurPage - 2); $index < $kitCurPage; $index++) { ?>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/problems?page=<?= $index ?>"><?= $index ?></a></li>
            <?php } ?>
            <li class="active"><a href="#"><?= $kitCurPage ?></a></li>
            <?php for ($index = 1; $index <= 2 && $kitCurPage + $index <= $kitProblemsTotalPages; $index++) { ?>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/problems?page=<?= $kitCurPage + $index ?>"><?= $kitCurPage + $index ?></a>
                </li>
            <?php } ?>

            <?php if ($kitCurPage + 2 < $kitProblemsTotalPages) { ?>
                <li class="disabled"><span>&sdot;&sdot;&sdot;</span></li>
                <li class="page-item"><a class="page-link"
                                         href="<?= $kitBasePath ?>/problems?page=<?= $kitProblemsTotalPages ?>"><?= $kitProblemsTotalPages ?></a>
                </li>
            <?php } ?>

            <?php if ($kitCurPage == $kitProblemsTotalPages) { ?>
                <li class="page-item disabled">
                    <a class="page-link" href="#" aria-label="Previous">
                        <span aria-hidden="true">&raquo;</span>
                        <span class="sr-only">Next</span>
                    </a>
                </li>
            <?php } else { ?>
                <li class="page-item">
                    <a class="page-link" href="<?= $kitBasePath ?>/problems?page=<?= $kitCurPage + 1 ?>"
                       aria-label="Previous">
                        <span aria-hidden="true">&raquo;</span>
                        <span class="sr-only">Next</span>
                    </a>
                </li>
            <?php } ?>
        </ul>
    </div>
</div>
