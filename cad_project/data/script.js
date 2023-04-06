// var username = document.forms['Login_form']['Username']
// var password = document.forms['Login_form']['Password']
// var username_err = document.getElementById("Username_error")
// var pass_err = document.getElementById("Pass_error")
// username.addEventListener('textInput',usernameVerify)
// password.addEventListener('textInput',passVerify)
// function validated(){
//     if(username.value != "Rajputsanskar96"){
//         username.style.border = "1px solid red";
//         username_err.style.display = "block";
//         username.focus();
//         return false;
//     }
//     if(password.value != "1234567"){
//         password.style.border = "1px solid red";
//         pass_err.style.display = "block";
//         password.focus();
//         return false;
//     }
    
// }
// function usernameVerify(){
//     if(username.value === "Rajputsanskar96"){
//         username.style.border = "1px solid silver";
//         username_err.style.display = "none";
//         return true;
//     }
// }
// function passVerify(){
//     if(password.value === "1234567"){
//         password.style.border = "1px solid silver";
//         pass_err.style.display = "none";
//         return true;
//     }
// }

// Create events for the sensor readings
if (!!window.EventSource) {
    var source = new EventSource('/events');
  
    source.addEventListener('open', function(e) {
      console.log("Events Connected");
    }, false);
  
    source.addEventListener('error', function(e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    }, false);
    
    source.addEventListener('gas_reading', function(e) {
        console.log("gas_reading", e.data);
        var obj = JSON.parse(e.data);
        document.getElementById("mq2").innerHTML = obj.mq2;
        document.getElementById("distance").innerHTML = obj.distance;
        document.getElementById("vibration").innerHTML = obj.vibration;
        document.getElementById("temp").innerHTML = obj.temp;
    }, false);

    source.addEventListener('ultrasonic_reading', function(e) {
        console.log("ultrasonic_reading", e.data);
        var obj = JSON.parse(e.data);
        document.getElementById("mq2").innerHTML = obj.mq2;
        document.getElementById("distance").innerHTML = obj.distance;
        document.getElementById("vibration").innerHTML = obj.vibration;
        document.getElementById("temp").innerHTML = obj.temp;
    }, false);

    source.addEventListener('chatter_reading', function(e) {
        console.log("chatter_reading", obj.vibration);
        var obj = JSON.parse(e.data);
        document.getElementById("mq2").innerHTML = obj.mq2;
        document.getElementById("distance").innerHTML = obj.distance;
        document.getElementById("vibration").innerHTML = obj.vibration;
        document.getElementById("temp").innerHTML = obj.temp;
        
    }, false);

    source.addEventListener('temperature_reading', function(e) {
        console.log("temperature_reading", e.data);
        var obj = JSON.parse(e.data);
        document.getElementById("mq2").innerHTML = obj.mq2;
        document.getElementById("distance").innerHTML = obj.distance;
        document.getElementById("vibration").innerHTML = obj.vibration;
        document.getElementById("temp").innerHTML = obj.temp;
    }, false);
}
