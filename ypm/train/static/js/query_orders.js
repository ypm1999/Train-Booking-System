var user_id = 0, date = '2018-00-00';

$(function () {
    $('#date').datetimepicker({
      format: 'YYYY-MM-DD',
      locale: 'zh-CN'
    });
});


var exist = false;

function search(){
  if(!exist){
    var tmp = new TableInit;
    tmp.Init();
    exist = true;
  }
  else
    $('#station').bootstrapTable('refresh');
}

var TableInit = function() {
  var oTableInit = new Object();
  //初始化Table
  oTableInit.Init = function() {
    $('#station').bootstrapTable({
      url: '/Data/user/orders',
      method: 'GET',
      datetype: 'json',
      striped: true, //是否显示行间隔色
      cache: false,
      queryParams: oTableInit.getData,
      //responseHandler:load,
      clickToSelect: true, //是否启用点击选中行
      columns: [
          {
              field: 'train_id',
              title: '列车编号',
          }, {
              field: 'loc1',
              title: '出发地',
          }, {
              field: 'time1',
              title: '出发时间',
          }, {
              field: 'loc2',
              title: '目的地',
          }, {
              field: 'time2',
              title: '到达时间',
          }, {
              field: 'num',
              title: '购票数量',
          }, {
              field: 'ticket_kind',
              title: '座席',
          }, {
              field: 'price',
              title: '票价',
          }, {
            field: 'operate',
            title: '订票',
            events: operateEvents,
            formatter: '<input type="submit" id = "orderTicket" value="退票" class="btn btn-primary btn-md">'
          }
      ]
    });
  };

  window.operateEvents = {
      "click #orderTicket": function(e, value, row, index) {
        var order = document.getElementById("orderForm");
        order.user_id = user_id;
        order.train_id.value = row.train_id;
        order.loc1.value = row.loc1;
        order.loc2.value = row.loc2;
        order.date.value = date;
        order.ticket_kind.value = row.ticket_kind;
        order.max.value = row.num;
        $('#refund_massage').html('退订<strong>'+ row.train_id +'</strong>次列车<strong>'+date+'</strong>从<strong>'+ row.loc1 +'</strong>到<strong>'+ row.loc2 +'</strong>的车票, 价格为<strong>' + row.price + '元。');
        $('#order').modal();
      }
    }
  /*
  function load(res){
    for(i in res){
      res[i]['time1'] = res[i]['date1'] + "<br />" + res[i]['time1'];
      res[i]['time1'] = res[i]['date2'] + "<br />" + res[i]['time2'];
    }
    return res;
  }
  */
  oTableInit.getData = function(params) {
    user_id = $('#user_id').val();
    date = $('#date').val();
    return {
      user_id: user_id,
      date: date,
    };
  };
  return oTableInit;
};

function refund(){
  if($('#orderForm').num <=  $('#orderForm').max)
    return true;
  else
    alter('请输入正确的退票数');
  return false;
}
