var username = document.forms['Login_form']['Username']
var password = document.forms['Login_form']['Password']
var username_err = document.getElementById("Username_error")
var pass_err = document.getElementById("Pass_error")
username.addEventListener('textInput',usernameVerify)
password.addEventListener('textInput',passVerify)
function validated(){
    if(username.value != "Rajputsanskar96"){
        username.style.border = "1px solid red";
        username_err.style.display = "block";
        username.focus();
        return false;
    }
    if(password.value != "1234567"){
        password.style.border = "1px solid red";
        pass_err.style.display = "block";
        password.focus();
        return false;
    }
    
}
function usernameVerify(){
    if(username.value === "Rajputsanskar96"){
        username.style.border = "1px solid silver";
        username_err.style.display = "none";
        return true;
    }
}
function passVerify(){
    if(password.value === "1234567"){
        password.style.border = "1px solid silver";
        pass_err.style.display = "none";
        return true;
    }
}