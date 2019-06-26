/*
 * WebPage.h
 * Developed by Erratum Solutions
 * Contact us @ support@erratums.com
 * https://erratums.com
 * Date created: 26-Jun-2019
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
</style></head>

<body>
<div class="container1" style="background-color:#f1f1f1">
<div class="container" style="background-color:#ffffff">
<h2 align="center">Single Channel Relay Module</h2>
<form action="/turnon" method="get">
  <div class="container"> <button type="submit" class="btnTurnOn">Turn On Relay</button> </div>
</form>
<form action="/turnoff" method="get">
  <div class="container"> <button type="submit" class="btnTurnOff">Turn Off Relay</button> </div>
</form>
<form action="/toggle" method="get">
  <div class="container"> <button type="submit" class="btnTurnOff">Toggle Relay State</button> </div>
</form>
</div></div>
</body></html>
)=====";
