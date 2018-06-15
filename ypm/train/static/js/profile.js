function check(){
	var phone = $("#phone").val();
  var name = $("#name").val();
  var password = $("#password").val();
	var password1 = $("#password1").val();
  if(!(/^[0-9a-zA-Z_]+$/.test(name))){
    alert("用户名只能包含数字,字母或_");
  	return false;
  }
  if(!(/^[0-9a-zA-Z_@]+$/.test(password))){
    alert("密码只能包含数字,字母,_或者＠");
  	return false;
  }
  if(!(/^1[3|4|5|7|8]\d{9}$/.test(phone))){
    alert("请输入正确的11位手机号");
  	return false;
  }
	if(password1 == '')
		$("#password1").val(password);
  return true;
}
function to_manager(){
	document.getElementById('form').action = '/user/toManager';
//	$('#form').attr('action', '/user/toManager');
}
function to_user(){
	$('#form').attr('action', '/user/toRoot');
}
function to_user(){
	$('#form').attr('action', '/user/toUser');
}
