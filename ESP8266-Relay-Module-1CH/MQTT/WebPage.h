/*
 * WebPage.h
 * Developed by Muhammad Ajmal P
 * Contact me @ ajumalp@gmail.com
 * https://owner.erratums.com
 * Date created: 11-Sep-2018
 */

const char cWebPageServer[] PROGMEM = R"=====(
<html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {font-family: Arial, Helvetica, sans-serif;}
form {border: 3px solid #f1f1f1;}

input[type=text], input[type=password] {
    width: 100%;
    padding: 12px 20px;
    margin: 8px 0;
    display: inline-block;
    border: 1px solid #ccc;
    box-sizing: border-box;
}
button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 8px 0;
    border: none;
    cursor: pointer;
    width: 100%;
}
button:hover {opacity: 0.8;}
.container {padding: 16px;}
.container1 {padding: 8px;}
</style></head><body>

<div class="container" style="background-color:#ffffff">
<table width="100%">
  <tr>
    <td>
      <form action="/turnon" method="get">
        <div class="container"> <button type="submit" class="btnTurnOn">Turn On Relay</button> </div>
      </form>
    </td>
    <td></td>
    <td>
      <form action="/turnoff" method="get">
        <div class="container"> <button type="submit" class="btnTurnOff">Turn Off Relay</button> </div>
      </form>
    </td>
  </tr>
</table>
<form action="/saveSett" method="post">
  <div class="container">
    <h2 align="center">Wi-Fi Settings</h2>
    <label for="ssid"><b>Wi-Fi Name</b></label>
    <input type="text" placeholder="Enter Your Wi-Fi Name" name="ssid">

    <label for="psw"><b>Password</b></label>
    <input type="password" placeholder="Enter Your Wi-Fi Password" name="psw">

    <button type="submit" class="savebtn">Save</button>
  </div>
</form>
<form action="/saveHotspot" method="post">
  <div class="container">
    <h2 align="center">Hotspot Settings</h2>
    <label for="hp_ssid"><b>Wi-Fi Name</b></label>
    <input type="text" placeholder="Enter Your Wi-Fi Name" name="hp_ssid" id="hp_ssid">

    <label for="hp_psw"><b>Password</b></label>
    <input type="password" placeholder="Enter Your Wi-Fi Password" name="hp_psw" id="hp_psw">

    <button type="submit" class="savebtn">Save</button>
  </div>
</form>
<form action="/saveTopic" method="post">
  <div class="container">
  <h2 align="center">MQTT Settings</h2>
    <label for="ssid"><b>Topic Name</b></label>
    <input type="text" placeholder="Enter Topic Name" name="mqttTopic" id="mqttTopic">
    <button type="submit" class="savebtn">Save</button>
  </div>
</form>
</div>
</body></html>
)=====";
