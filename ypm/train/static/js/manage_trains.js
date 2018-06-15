var exist = false;
var seat = new Array();
function searchTrain() {
  if(!exist){
    var oTable = new TableInit();
    oTable.Init();
    exist = true;
  }
  else{
    $("#station").bootstrapTable('refresh');
  }
  $('#addTrain').hide();
  $('#addstation').hide();
  $('#station').show();
  $('#train').show();
}

var seats = ['商务座','特等座','一等座','二等座','高级软卧','软卧','动卧','硬卧','软座','硬座','无座'];

function showList(){
  var t = $('#addTrainForm').serializeArray();
  var temp = new Array();
  for(var i in t){
    temp[t[i]['name']] = t[i]['value'];
  }
  delete t;
  var n = Number(temp['stationNum']);
  if(temp['train_id'] == ''){
    alert("请输入车次编号！");
    return;
  }
  if(!(temp['catelog'].length == 1 && 'CDGKOTZ'.indexOf(temp['catelog']) > 0)){
    alert("请输入正确的车次种类！");
    return;
  }
  if(temp['name'] == ''){
    alert("请输入车次名称！");
    return;
  }
  if(n == undefined || n < 2){
    alert("请输入正确的车站数！");
    return;
  }
  seat = [];
  for(var i in seats){
    if(temp[seats[i]] == 'on')
      seat.push(seats[i]);
  }
  delete temp;
  if(seat.length == 0){
    alert("请选择座席！");
    return;
  }
  var head = new Array();
  head.push(['<th>',
            ' <div class="th-inner">车站名</div>',
            ' <div class="fht-cell"></div>',
            '</th>'
  ].join('\n'));
  head.push(['<th>',
            ' <div class="th-inner">到达时间</div>',
            ' <div class="fht-cell"></div>',
            '</th>'
  ].join('\n'));
  head.push(['<th>',
            ' <div class="th-inner">出发时间</div>',
            ' <div class="fht-cell"></div>',
            '</th>'
  ].join('\n'));
  head.push(['<th>',
            ' <div class="th-inner">停留时间</div>',
            ' <div class="fht-cell"></div>',
            '</th>'
  ].join('\n'));
  for(var i in seat){
    head.push(gethead('head' + i, seat[i]));
  }
  var station = new Array();
  for(var i = 0; i < n; i++){
    var s1 = 'required', s2 = 'required';
    if(i == 0)
      s1 = "readonly";
    if(i == n - 1)
      s2 = "readonly";
    station.push(getstation('station' + i, seat.length, s1, s2));
  }
  console.log($('#notes'));
  $('#notes').show();
  $('#seatNumber').val(seat.length);
  $('#stationnumber').val(n);
  $('#addstation').show();
  $('#seatsHead').html(head.join('\n'));
  $('#inputStations').html(station.join('\n'));
  $('#submitadd').attr('disabled',false);
}

function gethead(name, head){
  return [
    '<th>',
    '  <input type = "hidden" name = "' + name + '" value = "' + head + '"　/>',
    '  <div class="th-inner">' + head + '</div>',
    '  <div class="fht-cell"></div>',
    '</th>'
  ].join('\n');
}

function getstation(name, seatNum, arrive, leave){
  var tmp = [];
  name += '_';
  tmp.push('<tr>');
  tmp.push('<th>' + getinput(name + 'name', 'required maxlength = "8"') + '</th>');
  tmp.push('<th>' + getinput(name + 'arrive', arrive) + '</th>');
  tmp.push('<th>' + getinput(name + 'leave', leave) + '</th>');
  if(arrive != 'required')
    tmp.push('<th>' + getinput(name + 'stop', arrive) + '</th>');
  else if(leave != 'required')
    tmp.push('<th>' + getinput(name + 'stop', leave) + '</th>');
  else
    tmp.push('<th>' + getinput(name + 'stop', 'required') + '</th>');
  for(var i = 0; i < seatNum; i++)
    if(arrive != 'required')
      tmp.push('<th>' + getinputwithspan(name + i, 'readonly') + '</th>');
    else
      tmp.push('<th>' + getinputwithspan(name + i, 'required') + '</th>');
  tmp.push('</tr>');
  return tmp.join('\n');
}

function getinput(name, addtion){
  if(addtion == 'readonly')
    addtion += ' value = "xx:xx" ';
  return '<input type="text" class="form-control" name="' + name + '" ' + addtion + ' />';
}


function getinputwithspan(name, addtion){
  return [
    '<div class = "input-group">',
    ' <input type="text" class="form-control" name="' + name + '" ' + addtion + ' />',
    ' <span class="input-group-addon">￥</span>',
    '</div>'
  ].join('\n');
}

var TableInit = function() {
  var oTableInit = new Object();
  //初始化Table
  oTableInit.Init = function() {
    $('#station').bootstrapTable({
      url: '/Data/searchTrain',
      method: 'GET',
      datetype: 'json',
      striped: true, //是否显示行间隔色
      cache: false,
      queryParams: oTableInit.getData,
      responseHandler:load,
      clickToSelect: true, //是否启用点击选中行
      columns: [
          {
              field: 'name',
              title: '车站名',
              width: 80
          }, {
              field: 'arrive',
              title: '到达时间',
              width: 80,
          }, {
              field: 'leave',
              title: '离开时间',
              width: 80,
          }, {
              field: '商务座',
              title: '商务座<br />特等座',
          }, {
              field: '一等座',
              title: '一等座',
          }, {
              field: '二等座',
              title: '二等座',
          }, {
              field: '高级软卧',
              title: '高级<br/>软卧',
          }, {
              field: '软卧',
              title: '软卧',
          }, {
              field: '动卧',
              title: '动卧',
          }, {
              field: '硬卧',
              title: '硬卧',
          }, {
              field: '软座',
              title: '软座',
          }, {
              field: '硬座',
              title: '硬座',
          }, {
              field: '无座',
              title: '无座',
          }
      ]
    });
  };
  //得到查询的参数
  var load = function(res){

    $('#train_row').html([
      '<td>' + res.train_id + '</td>',
      '<td>' + res.name + '</td>',
      '<td>' + res.saled　+ '</td>',
    ].join('\n'));
    if(res.saled == '否'){
      $('#search_id').val(res.train_id);
      $('#buttons').show();
    }
    else
      $('#buttons').hide();
    return res.station;
  }
  oTableInit.getData = function(params) {
    var temp = $('#trainID').val();
    console.log(temp);
    if (temp == undefined)
      temp = '';
    return {
      train_id: temp,
    };
  };
  return oTableInit;
};
/**
 * [getDataFromTrElems 获取表格行元素数据]
 * @param  {[Object]} trElems [trs dom]
 * @param  {[String]} type    [type]
 * @return {[Array]}
 */
function getDataFromTrElems(trElems, type) {

    if (!trElems || !trElems.length) {
        return [];
    }

    var data = [];
    var cell = type == 'head' ? 'th' : 'td';

    for (var i = 0, n = trElems.length; i < n; i++) {

        var trElem = $(trElems[i]);
        var cellElems = trElem.children(cell);
        var trData = [];

        for (var j = 0, m = cellElems.length; j < m; j++) {

            cellElem = $(cellElems[j]);
            colspan = +cellElem.attr('colspan');
            if (!(colspan > 0)) {
                colspan = 1;
            }

            rowspan = +cellElem.attr('rowspan');
            if (!(rowspan > 0)) {
                rowspan = 1;
            }

            text = $.trim(cellElem.text());
            trData.push({
                colspan: colspan,
                rowspan: rowspan,
                text: text
            });
        }

        data.push(trData);
    }

    return data;
}
/**
 * [getDataFromHead 获取表格head数据]
 * @param  {[Object]} elem [head dom]
 * @return {[Array]}
 */
function getDataFromHead(elem) {

    if (!elem || !elem.length) {
        return [];
    }

    var trElems = elem.children('tr');
    return getDataFromTrElems(trElems, 'head');
}
/**
 * [getDataFromBody 获取表格body数据]
 * @param  {[Object]} elem [body dom]
 * @return {[Array]}
 */
function getDataFromBody(elem) {

    if (!elem || !elem.length) {
        return [];
    }

    var trElems = elem.children('tr');
    return getDataFromTrElems(trElems, 'body');
}
/**
 * [getDataFromTbaleElem 获取表格元素数据]
 * @param  {[Object]} elem [table dom]
 * @return {[Array]}
 */
function getDataFromTbaleElem(elem) {

    if (!elem || !elem.length) {
        return [];
    }

    var headElem = elem.children('thead');
    var bodyElem = elem.children('tbody');
    var headData = getDataFromHead(headElem);
    var bodyData = getDataFromBody(bodyElem);
    var rawData = headData.concat(bodyData);

    return formRawData(rawData);
}
/**
 * [formRawData 格式化表格初始数据]
 * @param  {[Array]} rawData [raw data]
 * @return {[Array]}
 */
function formRawData(rawData) {

    var data = [];

    rawData.forEach(function(trData, i) {

        data[i] = data[i] && data[i].length ? data[i] : [];
        var colIndex = -1;

        trData.forEach(function(cellData, j) {

            var text = cellData.text;

            for (var c = 0; c < cellData.colspan; c++) {
                ++colIndex;

                if (typeof data[i][colIndex] != 'undefined') {
                    ++colIndex;
                }

                data[i][colIndex] = text;

                for (var r = 2; r <= cellData.rowspan; r++) {
                    var rowIndex = i + r - 1;
                    rawData[rowIndex].splice(colIndex, 0, {
                        text: text,
                        colspan: 1,
                        rowspan: 1
                    });
                }
            }
        });
    });
    return data;
}

function modify() {
    var tableElems = $('#train');
    train = getDataFromTbaleElem($(tableElems[0]));
    var tableElems = $('#station');
    var station = getDataFromTbaleElem($(tableElems[0]));
    var n = station.length - 1;
    var seat = [],head = [], body = [];
    for(var i = 1; i <= n; i++){
      var name = "station" + i + '_';
      var tmp = [];
      var arrive  = 'required value = "' + station[i][1] + '"';
      var leave = 'required value = "' + station[i][2] + '"';
      var stop;
      if(i == 1){
        stop = 'required value = "xx:xx"';
        arrive = 'readonly';
      }
      if(i == n){
        stop = 'required value = "xx:xx"';
        leave = 'readonly';
      }
      if(i > 1 && i < n){
        var hour = Number(station[i][2].substring(0, 2)) - Number(station[i][1].substring(0, 2));
        var min = Number(station[i][2].substring(3, 5)) - Number(station[i][1].substring(3, 5));
        console.log(hour, min);
        if(min < 0)
          min += 60, hour -= 1;
        if(hour < 0)
          hour += 24;
        stop = 'required value = "';
        if(hour < 10) stop += '0';
        stop += hour + ':'
        if(min < 10) stop += '0';
        stop += min + '"';
      }
      tmp.push('<tr>');
      tmp.push('<th>' + getinput(name + 'name', 'required value = "' + station[i][0] + '"') + '</th>');
      tmp.push('<th>' + getinput(name + 'arrive', arrive) + '</th>');
      tmp.push('<th>' + getinput(name + 'leave', leave) + '</th>');
      tmp.push('<th>' + getinput(name + 'stop', stop) + '</th>');
      for(var j = 3; j < 13; j++)
        if(station[i][j] != "-"){
          tmp.push('<th>' + getinputwithspan(name + i, 'required value = "' + station[i][j].substring(1) + '"') + '</th>');
        }
      tmp.push('</tr>');
      body.push(tmp.join('\n'));
    }

    for(var i = 3; i < 13; i++)
      if(station[1][i] != "-")
        seat.push(station[0][i]);

    head.push(['<th>',
              ' <div class="th-inner">车站名</div>',
              ' <div class="fht-cell"></div>',
              '</th>'
    ].join('\n'));
    head.push(['<th>',
              ' <div class="th-inner">到达时间</div>',
              ' <div class="fht-cell"></div>',
              '</th>'
    ].join('\n'));
    head.push(['<th>',
              ' <div class="th-inner">出发时间</div>',
              ' <div class="fht-cell"></div>',
              '</th>'
    ].join('\n'));
    head.push(['<th>',
              ' <div class="th-inner">停留时间</div>',
              ' <div class="fht-cell"></div>',
              '</th>'
    ].join('\n'));

    for(var i in seat){
      head.push(gethead('head' + i, seat[i]));
    }
    $('#train_id').val(train[1][0]);
    $('#train_id').readOnly = true;
    $('#name').val(train[1][1]);
    $('#stationNum').val(n);
    $('#seatNumber').val(seat.length);
    $('#stationnumber').val(n);
    $('#seatsHead').html(head.join('\n'));
    $('#inputStations').html(body.join('\n'));
    $('#submitadd').attr('disabled',false);
    $('#addstation').show();
    $('#addTrain').show();
    $('#station').hide();
    $('#train').hide();
}

function checkadd(){
  var m = form.seatNumber;
  var n = form.stationnumber;
  var t = $('#checkbox').serializeArray();
  var cnt = 0;
  for(var i in t){
    if(t[i]['value'] == 'on')
      if(t[i]['name'] in seats)
        cnt++;
      else{
        alter('请在修改列车信息后重新生成车站表单！');
        return false;
      }
  }
  if(cnt != n){
    alter('请在修改列车信息后重新生成车站表单！');
    return false;
  }
  var tim = /^(20|21|22|23|[0-1]\d):[0-5]\d$/;
  var price = /(^[1-9]\d*(\.\d{1,2})?$)|(^0(\.\d{1,2})?$)/
  for(i = 0; i < n; i++){
    var name = $("input[name='station" + i + "'_name]").val();
    var name1 = $("input[name='station" + i + "'_arrive]").val();
    var name2 = $("input[name='station" + i + "'_leave]").val();
    var name3 = $("input[name='station" + i + "'_stop]").val();
    if(!tim.test(name1) || !tim.test(name2) || !tim.test(name3)){
      alter('请在'+ name +'站(第'+ (i + 1) + '个)输入正确的时间格式');
      return false;
    }
    for(var j = 0; j < m; j++){
      var pri = $("input[name='station" + i + "'_"+ j +"]").val();
      if(!price.test(pri)){
        alter('请在'+ name +'站(第'+ (i + 1) + '个)输入正确的价格');
        return false;
      }
    }
  }
  return true;
}

function sale(){
  $('#buttons').action = '/manage/train/sale';
  $('#buttons').submit();
}
function delete_train(){
  $('#buttons').action = '/manage/train/delete';
  $('#buttons').submit();
}
