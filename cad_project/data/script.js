// Create events for the sensor readings
if (!!window.EventSource) {
  var source = new EventSource("/events");

  source.addEventListener(
    "open",
    function (e) {
      console.log("Events Connected");
    },
    false
  );

  source.addEventListener(
    "error",
    function (e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    },
    false
  );

  source.addEventListener(
    "gas_reading",
    function (e) {
      console.log("gas_reading", e.data);
      var obj = JSON.parse(e.data);
      updateValues(obj);
      var led1 = document.getElementById("mq2-led");
      if (parseInt(obj.mq2) > 0) {
        led1.classList.add("led-red");
      } else {
        led1.classList.remove("led-red");
      }
    },
    false
  );

  source.addEventListener(
    "ultrasonic_reading",
    function (e) {
      console.log("ultrasonic_reading", e.data);
      var obj = JSON.parse(e.data);
      updateValues(obj);
      var led2 = document.getElementById("distance-led");
      if (parseInt(obj.distance) > 50) {
        led2.classList.add("led-red");
      } else {
        led2.classList.remove("led-red");
      }
    },
    false
  );

  source.addEventListener(
    "chatter_reading",
    function (e) {
      console.log("chatter_reading", obj.vibration);
      var obj = JSON.parse(e.data);
      updateValues(obj);
      var led3 = document.getElementById("chatter-led");
      if (parseInt(obj.vibration) > 6) {
        led3.classList.add("led-red");
      } else {
        led3.classList.remove("led-red");
      }
    },
    false
  );

  source.addEventListener(
    "temperature_reading",
    function (e) {
      console.log("temperature_reading", e.data);
      var obj = JSON.parse(e.data);
      updateValues(obj);
      var led1 = document.getElementById("temp-led");
      if (parseInt(obj.temp) > 30) {
        led1.classList.add("led-red");
      } else {
        led1.classList.remove("led-red");
      }
    },
    false
  );

  source.addEventListener(
    "ip_addr",
    function (e) {
      console.log("ip_addr", e.data);
      var obj = JSON.parse(e.data);
      // const navigate = useNavigate();
      Navigate("/" + String(obj.ip) + "/homepage");
    },
    false
  );

  function updateValues(obj) {
    document.getElementById("mq2").innerHTML = obj.mq2;
    document.getElementById("distance").innerHTML = obj.distance;
    document.getElementById("vibration").innerHTML = obj.vibration;
    document.getElementById("temp").innerHTML = obj.temp;
  }
}
