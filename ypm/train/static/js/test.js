const $table = $('#table');
const $remove = $('#remove');
let selections = [];

$('#show_product').bootstrapTable({
    toolbar: '#toolbar', //工具按钮用哪个容器
    striped: true, //是否显示行间隔色
    cache: false, //是否使用缓存，默认为true，所以一般情况下需要设置一下这个属性（*）
    pagination: false, //是否显示分页（*）
    sortable: true, //是否启用排序
    sortOrder: "asc", //排序方式
    sidePagination: "client", //分页方式：client客户端分页，server服务端分页（*）
    pageNumber: 1, //初始化加载第一页，默认第一页
    pageSize: 10, //每页的记录行数（*）
    pageList: [10, 25, 50, 100], //可供选择的每页的行数（*）
    clickToSelect: false,
    showExport: true, //是否显示导出
    exportDataType: "selected", //basic', 'all', 'selected'.
    columns: [{ checkbox: true }, {
        field: "id",
        title: 'Id',
        visible: false
    }, {
        field: "name",
        title: '名称'
    }, {
        field: "shortName",
        title: '简称'
    }, {
        field: "barCode",
        title: '一维码',
        editable: {
            type: 'text',
            mode: 'inline'//直接在所在行编辑，不弹出
        },
        sortable: true
    }, {
        field: "remark",
        title: '备注'
    }, ],
    onEditableSave: function(field, row, oldValue, $el) {
        var newValue = row[field];//不能使用row.field
        if (!checkStrEqual(oldValue, newValue)) {
            $.ajax({
                type: "post",
                url: "/edit",
                data: {
                    'type': 'product',
                    'id': row.id,//获得所在行指定列的值
                    'newValue': newValue,
                    'field': field,
                    'oldValue':oldValue
                },
                success: function(data, status) {
                    if (status == "success") {
                        alert("编辑成功");
                    }
                },
                error: function() {
                    alert("Error");
                },
                complete: function() {

                }
            });
        }
    }
});


function initTable() {
  $table.bootstrapTable({
    toolbar: '#toolbar', //工具按钮用哪个容器
    striped: true, //是否显示行间隔色
    cache: false, //是否使用缓存，默认为true，所以一般情况下需要设置一下这个属性（*）
    pagination: false, //是否显示分页（*）
    sortable: true, //是否启用排序
    sortOrder: "asc", //排序方式
    sidePagination: "server", //分页方式：client客户端分页，server服务端分页（*）
    pageNumber: 1, //初始化加载第一页，默认第一页
    pageSize: 10, //每页的记录行数（*）
    pageList: [10, 25, 50, 100], //可供选择的每页的行数（*）
    clickToSelect: false,
    showExport: true, //是否显示导出
    height: getHeight(),
    columns: [
      [
        {
          field: 'state',
          checkbox: true,
          rowspan: 2,
          align: 'center',
          valign: 'middle'
        }, {
          title: 'Item ID',
          field: 'id',
          rowspan: 2,
          align: 'center',
          valign: 'middle',
          sortable: true
        }, {
          title: 'Item Detail',
          colspan: 3,
          align: 'center'
        }
      ],
      [
        {
          field: 'name',
          title: 'Item Name',
          sortable: true,
          editable: true,
          align: 'center'
        }, {
          field: 'price',
          title: 'Item Price',
          sortable: true,
          align: 'center',
          editable: {
            type: 'text',
            title: 'Item Price',
            validate(value) {
              value = $.trim(value);
              if (!value) {
                return 'This field is required';
              }
              if (!/^\$/.test(value)) {
                return 'This field needs to start width $.'
              }
              const data = $table.bootstrapTable('getData');
              const index = $(this).parents('tr').data('index');
              console.log(data[index]);
              return '';
            }
          },
          footerFormatter: totalPriceFormatter
        }, {
          field: 'operate',
          title: 'Item Operate',
          align: 'center',
          events: operateEvents,
          formatter: operateFormatter
        }
      ]
    ]
  });
  // sometimes footer render error.
  setTimeout(() => {
    $table.bootstrapTable('resetView');
  }, 200);
  $table.on('check.bs.table uncheck.bs.table ' +
            'check-all.bs.table uncheck-all.bs.table', () => {
    $remove.prop('disabled', !$table.bootstrapTable('getSelections').length);

    // save your data, here just save the current page
    selections = getIdSelections();
    // push or splice the selections if you want to save all data selections
  });
  $table.on('expand-row.bs.table', (e, index, row, $detail) => {
    if (index % 2 == 1) {
      $detail.html('Loading from ajax request...');
      $.get('LICENSE', res => {
        $detail.html(res.replace(/\n/g, '<br>'));
      });
    }
  });
  $table.on('all.bs.table', (e, name, args) => {
    console.log(name, args);
  });
  $remove.click(() => {
    const ids = getIdSelections();
    $table.bootstrapTable('remove', {
      field: 'id',
      values: ids
    });
    $remove.prop('disabled', true);
  });
  $(window).resize(() => {
    $table.bootstrapTable('resetView', {
      height: getHeight()
    });
  });
}



function getIdSelections() {
  return $.map($table.bootstrapTable('getSelections'), ({id}) => id);
}

function responseHandler(res) {
  $.each(res.rows, (i, row) => {
    row.state = $.inArray(row.id, selections) !== -1;
  });
  return res;
}

function detailFormatter(index, row) {
  const html = [];
  $.each(row, (key, value) => {
    html.push(`<p><b>${key}:</b> ${value}</p>`);
  });
  return html.join('');
}

function operateFormatter(value, row, index) {
  return [
    '<a class="like" href="javascript:void(0)" title="Like">',
    '<i class="fa fa-heart"></i>',
    '</a>  ',
    '<a class="remove" href="javascript:void(0)" title="Remove">',
    '<i class="fa fa-remove"></i>',
    '</a>'
  ].join('');
}

window.operateEvents = {
  'click .like': function (e, value, row, index) {
    alert(`You click like action, row: ${JSON.stringify(row)}`);
  },
  'click .remove': function(e, value, {id}, index) {
    $table.bootstrapTable('remove', {
      field: 'id',
      values: [id]
    });
  }
};

function totalPriceFormatter(data) {
  let total = 0;
  $.each(data, (i, {price}) => {
    total += +(price.substring(1));
  });
  return `$${total}`;
}

function getHeight() {
  return $(window).height() - $('h1').outerHeight(true);
}

$(() => {
	initTable();
})
