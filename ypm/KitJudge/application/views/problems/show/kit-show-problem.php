<script type="application/javascript">
    $LAB.script('http://cdn.bootcss.com/mathjax/2.7.0/MathJax.js?config=TeX-AMS-MML_HTMLorMML')
        .wait(function () {
            MathJax.Hub.Config({
                showProcessingMessages: false,
                tex2jax: {
                    inlineMath: [["$", "$"], ["\\\\(", "\\\\)"]],
                    displayMath: [['$$', '$$']],
                    processEscapes: true
                },
                menuSettings: {
                    zoom: "Hover"
                }
            });
        });
</script>
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/codemirror.css">
<div class="container">
    <div class="row clearfix">
        <div class="col-md-12">
            <h3 style="text-align: center">
                Problem <?= isset($kitProblemTag) ? $kitProblemTag : $kitProblem->kitProbId ?>
                . <?= $kitProblem->kitProbName ?></h3>
            <ul class="nav nav-tabs" role="tablist">
                <li class="active"><a href="#kit-tab-statement" role="tab" data-toggle="tab"><span
                                class="glyphicon glyphicon-book"></span> Problem</a></li>
                <?php if (isset($_SESSION['kitUser'])) { ?>
                    <li><a href="#kit-tab-submit" role="tab" data-toggle="tab"><span
                                    class="glyphicon glyphicon-chevron-right"></span> Submit</a></li>
                    <?php if ($_SESSION['kitUser']['priority'] >= 2) { ?>
                        <li><a href="<?= $kitBasePath ?>/problems/edit/<?= $kitProblem->kitProbId ?>"><span
                                        class="glyphicon glyphicon-plus"></span> Edit</a></li>
                    <?php } ?>
                <?php } else { ?>
                    <li><a href="#" role="tab" data-toggle="modal" data-target="#kit-login-modal"><span
                                    class="glyphicon glyphicon-chevron-right"></span> Login</a></li>
                <?php } ?>
            </ul>
            <div class="tab-content">
                <div class="tab-pane active" id="kit-tab-statement">
                    <p></p>
                    <div id="kit-code-alert" style="display:none;"></div>
                    <?php foreach ($kitProblemConf["Module"] as $key => $mod) { ?>
                        <?php if ($mod['style'] == 'default') { ?>
                            <p class="text-default text-left"><strong style="font-size: medium;"><?= $key ?></strong>
                            </p>
                        <?php if ($mod['loader'] == 'html') { ?>
                            <p id="mod-<?= $key ?>"><?= $mod['content'] ?></p>
                        <?php } else if ($mod['loader'] == 'markdown') { ?>
                        <?php if (file_exists("files/probfile/" . $kitProblem->kitProbId . "/" . $mod["content"])) { ?>
                            <p id="mod-<?= $key ?>"><?= file_get_contents("files/probfile/" . $kitProblem->kitProbId . "/" . $mod["content"]); ?></p>
                        <?php } else { ?>
                            <p id="mod-<?= $key ?>"><strong class="text-warning">File "<?= $mod["content"] ?>" doesn't exists.</strong></p>
                        <?php } ?>
                            <script type="application/javascript">
                                $LAB.script('<?=$kitBasePath?>/utility/js/markdown.min.js')
                                    .wait(function () {
                                        var text = $('#mod-<?=$key?>').html();
                                        console.log("%s", text);
                                        <?php if (isset($kitContestId)) {?>
                                        text = text.replace(/\[\$PATH]/g, "<?=$kitBasePath?>/contests/file/<?=$kitContestId?>/<?=$kitProblemTag?>")
                                            .replace(/_/g, "\_")
                                            .replace(/{/g, "\{")
                                            .replace(/}/g, "\}");
                                        <?php } else {?>
                                        text = text.replace(/\[\$PATH]/g, "<?=$kitBasePath?>/problems/file/<?=$kitProblem->kitProbId?>")
                                            .replace(/_/g, "\_")
                                            .replace(/{/g, "\{")
                                            .replace(/}/g, "\}");
                                        <?php }?>
                                        $('#mod-<?=$key?>').html(markdown.toHTML(text));
                                    });
                            </script>
                        <?php } ?>
                        <?php } else if ($mod['style'] == 'limits') { ?>
                            <p class="text-default text-left"><strong style="font-size: medium;"><?= $key ?></strong>
                            </p>
                            <?php foreach ($kitProblemConf['files'][$kitProblemConf['main']]['lang'] as $lang) { ?>
                            <strong><?= $lang ?></strong>
                            <table class="table table-hover">
                                <thead>
                                <tr>
                                    <th>#</th>
                                    <th>Caption</th>
                                    <th>Time Limits</th>
                                    <th>Memory Limits</th>
                                    <th>Comparator</th>
                                    <?php if ($kitProblem->kitProbType == 'ds-homework') echo '<th>Memory Check Point</th>'; ?>
                                </tr>
                                </thead>
                                <tbody>
                                <?php $testId = 0 ?>
                                <?php foreach ($kitProblemConf['tests'] as $test) { ?>
                                    <?php if ($test['repeat'] < 5) { ?>
                                        <?php for ($tmp = 0; $tmp < $test['repeat']; $tmp++) { ?>
                                            <tr>
                                                <td><?= ++$testId ?></td>
                                                <td><?= array_key_exists('caption', $test) ? $test['caption'] : ''?></td>
                                                <td><?= $test['limit'][$lang]['time'] ?> MS</td>
                                                <td><?= $test['limit'][$lang]['memory'] >> 10 ?> MB</td>
                                                <td><?= $test['judger']['type'] == 'default' ? $test['judger']['path'] : '<i>Special Judge</i>' ?></td>
                                                <?php if ($kitProblem->kitProbType == 'ds-homework') { ?>
                                                    <?php if ($test['memcheck']) { ?>
                                                        <td><span class="glyphicon glyphicon-ok"></span></td>
                                                    <?php } else { ?>
                                                        <td><span class="glyphicon glyphicon-remove"></span></td>
                                                    <?php } ?>
                                                <?php } ?>
                                            </tr>
                                        <?php } ?>
                                    <?php } else { ?>
                                        <tr>
                                            <td><?= $testId + 1; ?>~<?= ($testId += $test['repeat']); ?></td>
                                            <td><?= array_key_exists('caption', $test) ? $test['caption'] : ''?></td>
                                            <td><?= $test['limit'][$lang]['time'] ?> MS</td>
                                            <td><?= $test['limit'][$lang]['memory'] >> 10 ?> MB</td>
                                            <td><?= $test['judger']['type'] == 'default' ? $test['judger']['path'] : '<i>Special Judge</i>' ?></td>
                                            <?php if ($kitProblem->kitProbType == 'ds-homework') { ?>
                                                <?php if ($test['memcheck']) { ?>
                                                    <td><span class="glyphicon glyphicon-ok"></span></td>
                                                <?php } else { ?>
                                                    <td><span class="glyphicon glyphicon-remove"></span></td>
                                                <?php } ?>
                                            <?php } ?>
                                        </tr>
                                    <?php } ?>
                                <?php } ?>
                                </tbody>
                            </table>
                        <?php } ?>
                        <?php } else if ($mod['style'] == 'sample') { ?>
                            <p class="text-default text-left"><strong style="font-size: medium;"><?= $key ?></strong>
                            </p>
                            <div id="mod-<?= $key ?>">
                                <?php foreach ($mod['content'] as $caption => $sample) { ?>
                                    <strong><?= $caption ?></strong>
                                    <pre><?= htmlspecialchars(file_get_contents("files/probfile/" . $kitProblem->kitProbId . "/" . $sample["path"])); ?></pre>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    <?php } ?>
                </div>
                <div class="tab-pane" id="kit-tab-submit">
                    <p></p>
                    <form id="kit-code-submit-form" class="form-horizontal" role="form">
                        <?php $fileid = 0; ?>
                        <?php foreach ($kitProblemConf['files'] as $caption => $conf) { ?>
                            <p class="text-default text-left"><strong
                                        style="font-size: medium;"><?= $caption ?></strong></p>
                            <div class="form-group">
                                <div class="col-sm-12">
                                    <label class="col-sm-1 control-label">Language</label>
                                    <div class="col-sm-2">
                                        <select id="kit-code-lang-<?= ++$fileid ?>"
                                                class="selectpicker show-tick form-control" data-live-search="false"
                                                onchange="$('#kit-code-<?= $fileid ?>').next('.CodeMirror')[0].CodeMirror.setOption('mode', translator[$('#kit-code-lang-<?= $fileid ?>').val()]);">
                                            <?php $first = true; ?>
                                            <?php foreach ($conf['lang'] as $lang) { ?>
                                                <?php if ($first) { ?>
                                                    <option value="<?= $lang ?>"
                                                            selected="selected"><?= $lang ?></option>
                                                    <?php $first = false ?>
                                                <?php } else { ?>
                                                    <option value="<?= $lang ?>"><?= $lang ?></option>
                                                <?php } ?>
                                            <?php } ?>
                                        </select>
                                    </div>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-sm-12">
                                    <label class="col-sm-1 control-label"><?= $caption ?></label>
                                    <div class="col-sm-11">
                                        <script type="application/javascript">
                                            translator = {
                                                'C++': 'text/x-c++src',
                                                'Java': 'text/x-java'
                                            }
                                        </script>
                                        <?php $kitUser = $_SESSION['kitUser']['name'];?>
                                        <?php $kitId = $this->KitStatus->kitGetLastStatus($kitUser, $kitProblem->kitProbId);?>
                                        <?php $preCode = $kitId ? htmlspecialchars(file_get_contents("files/userfile/$kitUser/code/$kitId/$caption")) : "";?>
                                        <textarea class="form-control" id="kit-code-<?= $fileid ?>"
                                                  style="overflow: hidden; word-wrap: break-word; resize: horizontal; height: 100px; "><?=$preCode ?></textarea>
                                        <script type="application/javascript">
                                            $(document).on('shown.bs.tab', 'a[data-toggle="tab"]', function (e) {
                                                $LAB.script('<?=$kitBasePath?>/utility/js/codemirror.js')
                                                    .script('<?=$kitBasePath?>/utility/js/mode/clike/clike.js')
                                                    .wait(function () {
                                                        if ($('#kit-code-<?=$fileid?>').data('code-box') === undefined) {
                                                            var CodeMirrorEditor = CodeMirror.fromTextArea(document.getElementById('kit-code-<?=$fileid?>'), {
                                                                matchBrackets: true,
                                                                lineWrapping: false,
                                                                styleActiveLine: true,
                                                                indentUnit: 4,
                                                                indentWithTabs: true,
                                                                theme: 'default',
                                                                lineNumbers: true,
                                                                mode: translator[$('#kit-code-lang-<?=$fileid?>').val()]
                                                            });
                                                            CodeMirrorEditor.refresh();
                                                            $(CodeMirrorEditor.getWrapperElement()).css('box-shadow', '0 2px 10px rgba(0,0,0,0.2)');
                                                            $('#kit-code-<?=$fileid?>').data('code-box', CodeMirrorEditor);
                                                        }
                                                    });
                                            });
                                        </script>
                                    </div>
                                </div>
                            </div>
                        <?php } ?>
                        <div class="form-group">
                            <div class="col-sm-offset-5 col-sm-12 btn-group">
                                <button type="submit" class="btn btn-primary">Submit</button>
                                <button type="reset" class="btn btn-default">Revert</button>
                            </div>
                        </div>
                    </form>
                    <script type="application/javascript">
                        $('#kit-code-submit-form').submit(function (e) {
                            e.preventDefault();
                            var data = 'session=<?=$kitSessionId?>';
                            <?php $fileid = 0?>
                            <?php foreach ($kitProblemConf['files'] as $caption => $conf) {?>
                                data = data + '&<?=$caption?>=' + encodeURIComponent($('#kit-code-<?=++$fileid?>').next('.CodeMirror')[0].CodeMirror.getValue());
                                data = data + '&<?=$caption?>lang=' + encodeURIComponent($('#kit-code-lang-<?=$fileid?>').val());
                            <?php }?>
                            <?php if (isset($kitContestId)) {?>
                            data = data + '&cid=<?=$kitContestId?>';
                            data = data + '&tag=<?=$kitProblemTag?>';
                            <?php } else {?>
                            data = data + '&prob=<?=$kitProblem->kitProbId?>';
                            <?php }?>
                            function clear() {
                                for (var i = <?=$fileid?>; i >= 1; i--) {
                                    $('#kit-code-' + i.toString()).val('').focus();
                                }
                            }

                            $.ajax({
                                type: 'POST',
                                url: '<?=$kitBasePath?>/problems/submit',
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
                                        clear();
                                    } else {
                                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                            .wait(function () {
                                                $.jGrowl('Successfully submitted the code', {
                                                    position: 'bottom-right'
                                                });
                                            });
                                        setTimeout("window.location.href='<?=$kitBasePath?>/status'",800);
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
                        });
                    </script>
                </div>
            </div>
        </div>
    </div>
</div>