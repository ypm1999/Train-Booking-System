

function searchTrain() {
  var oTable = new TableInit();
  oTable.Init();
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
  if(temp['name'] == ''){
    alert("请输入车次名称！");
    return;
  }
  if(n == undefined || n < 2){
    alert("请输入正确的车站数！");
    return;
  }
  var seat = new Array();
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
  var num = 0;
  for(var i in seat){
    num++;
    head.push(gethead('head' + num, seat[i]));
    console.log(gethead('head' + num, seat[i]));
  }
  var station = new Array();
  for(var i = 1; i <= n; i++){
    var s1 = 'required', s2 = 'required';
    if(i == 1)
      s1 = "forbidden";
    if(i == n)
      s2 = "forbidden";
    station.push(getstation('station' + i, seat.length, s1, s2));
    console.log(getstation('station' + i, seat.length, s1, s2));
  }
  console.log(seat.length);
  console.log(n);
  console.log(head.join('\n'));
  console.log(station.join('\n'));
  $('#addstation').show();
  $('#seatNumber').val(seat.length);
  $('#stationnumber').val(n);
  $('#seatsHead').html(head.join('\n'));
  $('#inputStations').html(station.join('\n'));

}

function gethead(id, head){
  return [
    '<th>',
    '  <div class="th-inner" id = "' + id + '">' + head + '</div>',
    '  <div class="fht-cell"></div>',
    '</th>'
  ].join('\n');
}

function getstation(id, seatNum, arrive, leave){
  var tmp = new Array();
  id+='_';
  tmp.push('<tr>');
  tmp.push('<th>' + getinput(id + 'name', 'required') + '</th>');
  tmp.push('<th>' + getinput(id + 'arrive', arrive) + '</th>');
  tmp.push('<th>' + getinput(id + 'leave', leave) + '</th>');
  for(var i = 1; i <= seatNum; i++)
    tmp.push('<th>' + getinput(id + i, 'required') + '</th>');
  tmp.push('</tr>');
  return tmp.join('\n');
}

function getinput(id, addtion){
  return '<input type="text" class="form-control" id="' + id + '" ' + addtion + ' />';
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
              field: 'arriveTime',
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
          }, {
              field: '其他',
              title: '其他',
          }
      ]
    });
  };
  //得到查询的参数
  var load = function(res){
    $('#train_row').html([
      '<th>' + res.train_id + '</th>',
      '<th>' + res.name + '</th>',
      '<th>' + res.saled　+ '</th>',
    ].join('\n'));
    if(res.saled == '否')
      $('#sale').html([
          '<input type = \"hidden\" value = \"'+ res.train_id + '\" />',
          '<button type = \"submit\" class = \"btn btn-success btn-lg\">',
          '<div class = \"mybtn\">发售</div>',
          '</button>',
        ].join('\n'));
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
