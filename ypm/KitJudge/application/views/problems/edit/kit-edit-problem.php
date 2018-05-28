<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/codemirror.css">
<link href="<?= $kitBasePath ?>/utility/css/fileinput.min.css" media="all" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" href="<?= $kitBasePath ?>/utility/css/bootstrap-treeview.css">
<div class="container">
    <div class="col-md-12">
        <?php if (isset($kitProblem)) { ?>
            <h3 style="text-align: center">Edit Problem <?= $kitProblem['kitProbId'] ?>. <span
                        id="kit-prob-name"><?= $kitProblem['kitProbName'] ?></span></h3>
        <?php } else { ?>
            <h3 style="text-align: center">Add Problem <?= $kitProbId ?>. <span id="kit-prob-name"></span></h3>
        <?php } ?>
        <ul class="nav nav-tabs" role="tablist">
            <li class="active"><a href="#kit-tab-general" role="tab" data-toggle="tab"><span
                            class="glyphicon glyphicon-book"></span> General</a></li>
            <li><a href="#kit-tab-files" role="tab" data-toggle="tab"><span class="glyphicon glyphicon-book"></span>
                    Files</a></li>
        </ul>
        <div class="tab-content">
            <div class="tab-pane active" id="kit-tab-general">
                <p></p>
                <form id="kit-edit-problem-form" class="form-horizontal" role="form" method="post">
                    <div id="kit-edit-problem-messsage" style="display: none;"></div>
                    <div class="form-group">
                        <label class="col-sm-2 control-label">Title</label>
                        <div class="col-sm-3">
                            <input type="text" class="form-control" id="kit-edit-problem-title"
                                   placeholder="Enter problem title here"
                                   onchange="$('#kit-prob-name').html($('#kit-edit-problem-title').val());">
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="col-sm-2 control-label"></label>
                        <div class="col-sm-1">
                            <?php if (!isset($kitProblem) || $kitProblem['kitProbHidden']) { ?>
                                <input type="checkbox" id="kit-edit-problem-hidden" checked> Hidden
                            <?php } else { ?>
                                <input type="checkbox" id="kit-edit-problem-hidden"> Hidden
                            <?php } ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="col-sm-2 control-label">Source</label>
                        <div class="col-sm-3">
                            <input type="text" class="form-control" id="kit-edit-problem-source"
                                   placeholder="Enter problem source here">
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="col-sm-2 control-label">Type</label>
                        <div class="col-sm-3">
                            <input type="text" class="form-control" id="kit-edit-problem-type"
                                   placeholder="Enter problem type here">
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="col-sm-2 control-label">Configuration</label>
                        <div class="col-sm-10">
                            <textarea id="kit-edit-problem-conf" type="text" class="form-control" rows="100"
                                      style="overflow: hidden; word-wrap: break-word; resize: vertical; height: 800px; "
                                      placeholder="Enter program configuration here"><?= file_exists("files/probfile/" . (string)($kitProbId) . "/problem.json") ? file_get_contents("files/probfile/" . (string)($kitProbId) . "/problem.json") : '' ?></textarea>
                            <script type="application/javascript">
                                $LAB.script('<?=$kitBasePath?>/utility/js/codemirror.js')
                                    .script('<?=$kitBasePath?>/utility/js/mode/javascript/javascript.js')
                                    .wait(function () {
                                        var cme = CodeMirror.fromTextArea(document.getElementById('kit-edit-problem-conf'), {
                                            matchBrackets: true,
                                            lineWrapping: false,
                                            styleActiveLine: true,
                                            indentUnit: 4,
                                            indentWithTabs: true,
                                            theme: 'default',
                                            lineNumbers: true,
                                            mode: "application/json"
                                        });
                                        $(document).ready(function () {
                                            cme.refresh();
                                            $(cme.getWrapperElement()).css('box-shadow', '0 2px 10px rgba(0,0,0,0.2)');
                                            cme.focus();
                                        });
                                    });
                                <?php if (isset($kitProblem)) {?>
                                $('#kit-edit-problem-title').val('<?=$kitProblem['kitProbName']?>');
                                $('#kit-edit-problem-source').val('<?=$kitProblem['kitProbSource']?>');
                                $('#kit-edit-problem-type').val('<?=$kitProblem['kitProbType']?>');
                                <?php }?>
                            </script>
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
                    $('#kit-edit-problem-form').submit(function (e) {
                        e.preventDefault();
                        var data = "session=<?=$kitSessionId?><?=isset($kitProblem) ? '&prob=' . $kitProblem['kitProbId'] : ''?>&type="
                            + encodeURIComponent($('#kit-edit-problem-type').val())
                            + "&title=" + encodeURIComponent($('#kit-edit-problem-title').val())
                            + "&source=" + encodeURIComponent($('#kit-edit-problem-source').val())
                            + "&conf=" + encodeURIComponent($('#kit-edit-problem-conf').next('.CodeMirror')[0].CodeMirror.getValue());
                        if ($('#kit-edit-problem-hidden')[0].checked) {
                            data = data + '&hidden=1';
                        } else {
                            data = data + '&hidden=0';
                        }
                        $.ajax({
                            type: 'POST',
                            url: "<?=$kitBasePath?>/problems/editdata",
                            cache: false,
                            data: data,
                            success: function (message) {
                                var result = eval('(' + message + ')');
                                if (!result.verdict) {
                                    $('#kit-edit-problem-messsage')
                                        .addClass('alert')
                                        .addClass('alert-warning')
                                        .addClass('alert-dismissable')
                                        .html(result.message)
                                        .show();
                                } else {
                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                            .wait(function () {
                                                $.jGrowl('Successfully edit the problem', {
                                                    position: 'bottom-right'
                                                });
                                            });
                                    setTimeout(function(){
                                        window.location.reload();
                                    }, 1000);
                                }
                            },
                            error: function () {
                                $('#kit-edit-problem-messsage')
                                    .addClass('alert')
                                    .addClass('alert-danger')
                                    .addClass('alert-dismissable')
                                    .html('A fatal error occured, please contact <a class="text-danger" href="mailto:<?=$kitAdminEmail?>"><strong><?=$kitAdminEmail?></strong></a>.')
                                    .show();
                            }
                        })
                    })
                </script>
            </div>
            <div class="tab-pane" id="kit-tab-files" class="col-sm-12">
                <p></p>
                <div id="kit-file-tree" class="col-sm-3" style="max-height:500px; overflow-y: scroll;"></div>
                <div class="col-sm-9" id="kit-file-show-download-area" style="display:none;">
                    <div class="panel panel-default">
                        <div class="panel-heading"><h4 class="panel-title" id="kit-file-panel-title"></h4></div>
                        <div class="panel-body">
                            <form class="form-horizontal" role="form" id="kit-file-form">
                                <div class="form-group">
                                    <script src="<?= $kitBasePath ?>/utility/js/plugins/canvas-to-blob.min.js"
                                            type="text/javascript"></script>
                                    <script src="<?= $kitBasePath ?>/utility/js/plugins/sortable.min.js"
                                            type="text/javascript"></script>
                                    <script src="<?= $kitBasePath ?>/utility/js/plugins/purify.min.js"
                                            type="text/javascript"></script>
                                    <script src="<?= $kitBasePath ?>/utility/js/fileinput.js"></script>
                                    <script src="<?= $kitBasePath ?>/utility/js/themes/fa/theme.js"></script>
                                    <script src="<?= $kitBasePath ?>/utility/js/locales/LANG.js"></script>
                                    <div class="col-sm-12" id="kit-file-input-div"></div>
                                </div>
                                <div class="form-group">
                                    <div class="col-sm-12" id="kit-file-button-group">
                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
                <script type="application/javascript">
                    function getPath(node) {
                        if (node.parentId !== undefined) {
                            var value = null;
                            $LAB.script("<?=$kitBasePath?>/utility/js/bootstrap-treeview.js")
                                .wait(function () {
                                    value = getPath($('#kit-file-tree').treeview('getNode', node.parentId)) + '/' + node.text;
                                });
                            return value;
                        } else {
                            if (node.type === 'root') {
                                return '/';
                            } else {
                                return node.text + '/';
                            }
                        }
                    }
                    function getFileExt(str) {
                        var reg = /(\\+)/g;
                        var pfn = str.replace(reg, "#");
                        var arrpfn = pfn.split("#");
                        var fn = arrpfn[arrpfn.length - 1];
                        var arrfn = fn.split(".");
                        if (arrfn[arrfn.length - 1] === str) return "";
                        else return arrfn[arrfn.length - 1];
                    }
                    function getTree() {
                        $.ajax({
                            type: "GET",
                            url: "<?=$kitBasePath?>/problems/tree/<?=$kitProbId?>",
                            data: "",
                            success: function (msg) {
                                $LAB.script("<?=$kitBasePath?>/utility/js/bootstrap-treeview.js")
                                    .wait(function () {
                                        $('#kit-file-tree').treeview({
                                            data: msg,
                                            onNodeSelected: function (event, data) {
                                                $('#kit-file-show-download-area').hide();
                                                $('#kit-file-show-download-button').hide();
                                                $("#kit-file-button-group").html('');
                                                if (data.type !== 'root') {
                                                    $('#kit-file-button-group').append('<a class="btn btn-danger" id="kit-file-show-remove-button"><span class="glyphicon glyphicon-remove"></span> Remove</a>');
                                                    $('#kit-file-show-remove-button').click(function () {
                                                        $.ajax({
                                                            type: 'POST',
                                                            url: "<?=$kitBasePath?>/problems/remove/<?=$kitProbId?>",
                                                            data: "url=" + getPath(data),
                                                            success: function (msg) {
                                                                var data = eval('(' + msg + ')');
                                                                if (data.verdict) {
                                                                    getTree();
                                                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                                        .wait(function () {
                                                                            $.jGrowl("Successfully removed the file", {
                                                                                position: 'bottom-right'
                                                                            });
                                                                        });
                                                                } else {
                                                                    $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                                        .wait(function () {
                                                                            $.jGrowl(data.message, {
                                                                                position: 'bottom-right',
                                                                                sticky: true
                                                                            });
                                                                        });
                                                                }
                                                            }
                                                        });
                                                    });
                                                }
                                                console.log(getPath(data));
                                                if (data.type === 'file') {
                                                    $('#kit-file-button-group').prepend('<a class="btn btn-success" id="kit-file-show-download-button"><span class="glyphicon glyphicon-download"></span> Download</a>\n');
                                                    $('#kit-file-panel-title').text(data.text);
                                                    $('#kit-file-show-download-button')
                                                        .bind('click', function () {
                                                            window.location.assign('<?=$kitBasePath?>/problems/download/<?=$kitProbId?>?url=' + encodeURIComponent(getPath(data)));
                                                        }).show();
                                                    $("#kit-file-input-div")
                                                        .html('')
                                                        .append('<input id="kit-file-input" type="file" />')
                                                        .attr('accept', '.' + getFileExt(data.text))
                                                    $("#kit-file-input")
                                                        .fileinput({
                                                            'showPreview': false,
                                                            'showRemove': false,
                                                            'uploadUrl': '<?=$kitBasePath?>/problems/upload/<?=$kitProbId?>',
                                                            'uploadExtraData': {
                                                                url: getPath(data),
                                                                new: false
                                                            }
                                                        })
                                                        .on('fileuploaded', function (event, data, previewId, index) {
                                                            getTree();
                                                            $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                                .wait(function () {
                                                                    $.jGrowl("Successfully altered the file.", {
                                                                        position: 'bottom-right',
                                                                    });
                                                                });
                                                        });
                                                } else {
                                                    var filename = '';
                                                    $('#kit-folder-url').text(getPath(data));
                                                    $('#kit-file-button-group').prepend('<a class="btn btn-success" data-toggle="modal" data-target="#kit-folder-modal"><span class="glyphicon glyphicon-folder-open"></span> New Folder</a>\n');
                                                    if (data.type === 'root') {
                                                        $('#kit-file-panel-title').text("Upload new files to the folder \"/\"");
                                                    } else {
                                                        $('#kit-file-panel-title').text("Upload new files to the folder \"" + getPath(data) + "\"");
                                                    }
                                                    $("#kit-file-input-div")
                                                        .html('')
                                                        .append('<input id="kit-file-input" multiple type="file" />');
                                                    $("#kit-file-input")
                                                        .fileinput({
                                                            'showPreview': false,
                                                            'showRemove': false,
                                                            'uploadUrl': '<?=$kitBasePath?>/problems/upload/<?=$kitProbId?>',
                                                            'uploadExtraData': {
                                                                url: getPath(data),
                                                                new: true
                                                            }
                                                        });
                                                    $("#kit-file-input").on('fileuploaded', function (event, data, previewId, index) {
                                                        getTree();
                                                        $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                                            .wait(function () {
                                                                $.jGrowl("Successfully uploaded the file", {
                                                                    position: 'bottom-right'
                                                                });
                                                            });
                                                    });
                                                }
                                                $('#kit-file-show-download-area').fadeIn(300);
                                            }
                                        });
                                    });
                            },
                            error: function () {
                                $LAB.script('//cdn.bootcss.com/jquery-jgrowl/1.4.5/jquery.jgrowl.js')
                                    .wait(function () {
                                        $.jGrowl("Error occured when getting the information of the files.", {
                                            position: 'bottom-right',
                                            sticky: true
                                        });
                                    });
                            }
                        });
                    }
                    getTree();
                </script>
            </div>
        </div>
    </div>
</div>