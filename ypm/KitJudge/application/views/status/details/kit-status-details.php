<div class="container">
    <div class="row clearfix">
        <div class="col-md-12">
            <?php
            $kitVerdictTranslator = array(
                'Accepted',
                'Time Limit Exceeded',
                'Runtime Error',
                'Output Limit Exceeded',
                'Memory Limit Exceeded',
                'Rejected',
                'Judgement Failed',
                'Wrong Answer',
                "",
                "",
                'Compilation Error',
                'Presentation Error',
                "",
                'Memory Error',
            );
            ?>
            <?php foreach ($kitConf->files as $caption => $data) { ?>
                <?php if (file_get_contents("files/userfile/$kitUser/code/$kitId/$caption" . 'lang') != "Git") {?> 
                    <pre class="prettyprint lang-<?= $kitSupportedLanguages[file_get_contents("files/userfile/$kitUser/code/$kitId/$caption" . 'lang')][1]?> linenums"><?= htmlspecialchars(file_get_contents("files/userfile/$kitUser/code/$kitId/$caption")); ?></pre>
                <?php } else { ?>
                    <pre class="prettyprint"><span class="nocode"><?= htmlspecialchars(file_get_contents("files/userfile/$kitUser/code/$kitId/$caption")); ?></pre>
                <?php } ?>
            <?php } ?>
            <?php if ($kitVerdict == 10 || $kitVerdict == 6) { ?>
                <pre class="prettyprint"><?= htmlspecialchars(json_decode(file_get_contents("files/userfile/$kitUser/code/$kitId/report"))->report) ?></pre>
            <?php } else if (($kitVerdict != 8 && $kitVerdict != 9 && $kitVerdict != 12)) { ?>
                <?php $kitReport = (array)(json_decode(file_get_contents("files/userfile/$kitUser/code/$kitId/report"))) ?>
                <?php $caseId = 0; ?>
                <?php $flag = false; ?>
                <?php foreach ($kitConf->tests as $test) { ?>
                    <?php for ($index = 1; $index <= $test->repeat; $index++) { ?>
                        <?php $caseId++; ?>
                        <?php if (array_key_exists("verdict-$caseId", $kitReport)) { ?>
                            <?php if ($kitVerdictTranslator[$kitReport["verdict-$caseId"]] != "Accepted") { ?>
                                <?php $flag = true; ?>
                            <?php } ?>
                            <hr>
                            <div>
                                <p>
                                    <strong style="font-size: small">
                                        Test: #<?= $caseId ?>, time: <?= $kitReport["time-$caseId"] ?> MS,
                                        memory: <?= $kitReport["memo-$caseId"] ?> KB, exit
                                        code: <?= $kitReport["exit-$caseId"] ?>,
                                        verdict: <?= $kitVerdictTranslator[$kitReport["verdict-$caseId"]] ?>
                                    </strong>
                                </p>
                                <?php if (array_key_exists("input-$caseId", $kitReport)) { ?>
                                    <p style="font-size: small">Input</p>
                                    <pre class="prettyprint"><span class="nocode"><?= htmlspecialchars($kitReport["input-$caseId"]) ?></span></pre>
                                <?php } ?>
                                <?php if (array_key_exists("output-$caseId", $kitReport)) { ?>
                                    <p style="font-size: small">Output</p>
                                    <pre class="prettyprint"><span class="nocode"><?= htmlspecialchars($kitReport["output-$caseId"]) ?></span></pre>
                                <?php } ?>
                                <?php if (array_key_exists("stdout-$caseId", $kitReport)) { ?>
                                    <p style="font-size: small">Answer</p>
                                    <pre class="prettyprint"><span class="nocode"><?= htmlspecialchars($kitReport["stdout-$caseId"]) ?></span></pre>
                                <?php } ?>
                                <?php if (array_key_exists("report-$caseId", $kitReport)) { ?>
                                    <p style="font-size: small">Report</p>
                                    <pre class="prettyprint"><span class="nocode"><?= isset($kitReport["report-$caseId"]) ? htmlspecialchars($kitReport["report-$caseId"]) : '' ?></span></pre>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    <?php } ?>
                    <?php if ($flag) break; ?>
                <?php } ?>
            <?php } ?>
        </div>
    </div>
</div>