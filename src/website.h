
#pragma once

const char *before PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="refresh" content="5"/>
    <style>
      h1 {    color: blue;    font-family: verdana;    font-size: 100%;
      }
      p  {
              color: red;     font-family: courier;    font-size: 100%;
      }
      table {
	border-collapse: collapse;
    font-family: Tahoma, Geneva, sans-serif;
}
table td {
	padding: 15px;
}
table thead td {
	background-color: #54585d;
	color: #ffffff;
	font-weight: bold;
	font-size: 13px;
	border: 1px solid #54585d;
}
table tbody td {
	color: #636363;
	border: 1px solid #dddfe1;
}
table tbody tr {
	background-color: #f9fafb;
}
table tbody tr:nth-child(odd) {
	background-color: #ffffff;
}
    .landingPage {
        margin: 0 auto 0 auto;
        width: 100%;
        min-height: 100vh;
        
        /* For centering content */
        display: flex;
        align-items: center;
        justify-content: center;
    }
    </style>
  </head>
  <body>
  <div class="landingPage">
    <div>
      <table>
        <tr>
          <td><form action='/'><input type='submit' value='Home' /></form></td>
          <td><form action='/settings'><input type='submit' value='Settings' /></form></td>
          <td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>
        </tr>
      </table>
      <table>
        <tr>  
)=====";

const char *after PROGMEM = R"=====(
        </tr>
      </table>
      <table>
        <tr>
          <td><form action='/'><input type='submit' value='Home' /></form></td>
          <td><form action='/settings'><input type='submit' value='Settings' /></form></td>
          <td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>
        </tr>
      </table>
      </div></div>
  </body>
</html>  
)=====";
