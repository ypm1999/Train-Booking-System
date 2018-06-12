var trains = 'CDGKOTZ';
var train = '';
var seats = ['商务座', '一等座', '二等座', '高级软卧', '软卧', '动卧', '硬卧', '硬座', '软座', '无座', '其他'];

$(function () {
    $('#datetimepicker').datetimepicker({
      format: 'YYYY-MM-DD',
      locale: 'zh-CN'
    });

    makeCheckBox();

    //1.初始化Table
    var oTable = new TableInit();
    oTable.Init();
});

function makeCheckBox(){
  var content = '列车种类：&nbsp&nbsp\n';
  for (i in trains){
    if(form[trains[i]] == 'on'){
      train += trains[i];
      content += getCheckBox(trains[i], 'checked');
    }
    else
      content += getCheckBox(trains[i], '');
  }
  document.getElementById('trainsCheckBox').innerHTML = content;
}

function getCheckBox(name, checked){
  return [
    '<div class=\"checkbox-inline\">',
    ' <input type=\"checkbox\" name = \"' + name + '\"' + checked + '/>' + name + '&nbsp&nbsp',
    '</div>'
  ].join('\n');
}

var TableInit = function () {
    var oTableInit = new Object();
    //初始化Table
    oTableInit.Init = function () {
        $('#transfer').bootstrapTable({
            url: '/Data/trains/transfer',         //请求后台的URL（*）
            method: 'GET',                      //请求方式（*）
            datetype: 'json',
            toolbar: '#toolbar',                //工具按钮用哪个容器
            striped: true,                      //是否显示行间隔色
            cache: false,
            sortable: true,                     //是否启用排序
            sortOrder: "asc",
            responseHandler:load,                 //排序方式
            queryParams:{
                loc1 : form.loc1,
                loc2 : form.loc2,
                date : form.date,
                catelog : train,
            },
            clickToSelect: true,                //是否启用点击选中行
            columns: [
              {
                field: 'id',
                title: '车次',
              }, {
                  field: 'loc1',
                  title: '出发地',
                  width: 60
              }, {
                  field: 'startTime',
                  title: '出发时间',
                  width: 90
              },{
                  field: 'loc2',
                  title: '目的地',
                  width: 60
              }, {
                  field: 'arriveTime',
                  title: '到达时间',
                  width: 90
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
                field: 'operate',
                title: '订票',
                width: 70,
                events: operateEvents,
                formatter: operateFormatter
              }
            ]
        });
        $('#train').bootstrapTable({
            url: '/Data/trains',         //请求后台的URL（*）
            method: 'GET',                      //请求方式（*）
            datetype: 'json',
            toolbar: '#toolbar',                //工具按钮用哪个容器
            striped: true,                      //是否显示行间隔色
            cache: false,
            sortable: true,                     //是否启用排序
            sortOrder: "asc",
            responseHandler:load,                 //排序方式
            queryParams:{
                loc1 : form.loc1,
                loc2 : form.loc2,
                date : form.date,
                catelog : train,
            },
            clickToSelect: true,                //是否启用点击选中行
            columns: [{
                field: 'id',
                title: '车次',
            }, {
                field: 'loc1',
                title: '出发地',
                width: 60
            }, {
                field: 'startTime',
                title: '出发时间',
                width: 90
            },{
                field: 'loc2',
                title: '目的地',
                width: 60
            }, {
                field: 'arriveTime',
                title: '到达时间',
                width: 90
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
              field: 'operate',
              title: '订票',
              width: 70,
              events: operateEvents,
              formatter: operateFormatter
            }
          ]
        });
      }
    var load = function(res){
      console.log(res);
      return res;
    }
    window.operateEvents = {
        "click #orderTicket": function(e, value, row, index) {
          var order = document.getElementById("orderForm");
          order.train_id.value = row.train_id;
          order.loc1.value = form.loc1;
          order.loc2.value = form.loc2;
          order.date.value = form.date;
          var box = '';
          for (i in seats){
            console.log(row[seats[i]]);
            if (row[seats[i]])
              box += getRadio('seat', seats[i]);
          }
          document.getElementById("checkboxs").innerHTML = box;
          $('#order').modal();
        }
    };
    function getRadio(name, text){
      return [
        '<label class=\"radio-inline\">',
        '<input type=\"radio\" name=\"' + name + '\" value = \"' + text + '\" required>' + text,
        '</label>',
      ].join('\n')
    };

    function operateFormatter(value, row, index) {
       return [
         '<input type="submit" id = "orderTicket" value="预订" class="btn btn-primary btn-sm">',
       ].join('');
     };

    return oTableInit;
};
