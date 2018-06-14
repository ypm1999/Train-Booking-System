function check(){
	var phone = $("#phone").val();
  var name = $("#phone").val();
  var password = $("#password").val();
  var password１ = $("#password１").val();
  if(!(/^[0-9a-zA-Z_]+$/.test(name))){
    alert("用户名只能包含数字,字母或_");
  	return false;
  }
  if(!(/^[0-9a-zA-Z_@]+$/.test(password))){
    alert("密码只能包含数字,字母,_或者＠");
  	return false;
  }
  if(password != password1){
    alert("两次输入的密码不相同");
  	return false;
  }
  if(!(/^1[3|4|5|8]\d{9}$/.test(phone))){
    alert("请输入正确的11位手机号");
  	return false;
  }
  return true;
}
