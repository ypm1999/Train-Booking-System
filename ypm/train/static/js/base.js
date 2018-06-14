function check_login(){
	var id = $("#user_id").val();
  var password = $("#passwd").val();
  if(!(/^[0-9]+$/.test(id)) || Number(id) < 2018){
    alert("用户ID格式错误");
  	return false;
  }
  if(!(/^[0-9a-zA-Z_@]+$/.test(password))　||　password.length > 18 && password.length < 5){
    alert("密码格式错误");
  	return false;
  }
  return true;
}
