/**
 * Add gobals here
 */
var seconds 	= null;
var otaTimerVar =  null;
var wifiConnectInterval = null;

/**
 * Initialize functions here.
 */
$(document).ready(function(){
  getSSID();
	getUpdateStatus();
  startDHTSensorInterval();
  getConnectInfo();
  startLocalTimeInterval();
  $("#connect_wifi").on("click", function() {
    checkCredentials();
  });
  $("#disconnect_wifi").on("click", function() {
    disconnectWifi();
  });
});   

/**
 * Gets file name and size for display on the web page.
 */        
function getFileInfo() 
{
    var x = document.getElementById("selected_file");
    var file = x.files[0];

    document.getElementById("file_info").innerHTML = "<h4>File: " + file.name + "<br>" + "Size: " + file.size + " bytes</h4>";
}

/**
 * Handles the firmware update.
 */
function updateFirmware() 
{
    // Form Data
    var formData = new FormData();
    var fileSelect = document.getElementById("selected_file");
    
    if (fileSelect.files && fileSelect.files.length == 1) 
	{
        var file = fileSelect.files[0];
        formData.set("file", file, file.name);
        document.getElementById("ota_update_status").innerHTML = "Uploading " + file.name + ", Firmware Update in Progress...";

        // Http Request
        var request = new XMLHttpRequest();

        request.upload.addEventListener("progress", updateProgress);
        request.open('POST', "/OTAupdate");
        request.responseType = "blob";
        request.send(formData);
    } 
	else 
	{
        window.alert('Select A File First')
    }
}

/**
 * Progress on transfers from the server to the client (downloads).
 */
function updateProgress(oEvent) 
{
    if (oEvent.lengthComputable) 
	{
        getUpdateStatus();
    } 
	else 
	{
        window.alert('total size is unknown')
    }
}

/**
 * Posts the firmware udpate status.
 */
function getUpdateStatus() 
{
    var xhr = new XMLHttpRequest();
    var requestURL = "/OTAstatus";
    xhr.open('POST', requestURL, false);
    xhr.send('ota_update_status');

    if (xhr.readyState == 4 && xhr.status == 200) 
	{		
        var response = JSON.parse(xhr.responseText);
						
	 	document.getElementById("latest_firmware").innerHTML = response.compile_date + " - " + response.compile_time

		// If flashing was complete it will return a 1, else -1
		// A return of 0 is just for information on the Latest Firmware request
        if (response.ota_update_status == 1) 
		{
    		// Set the countdown timer time
            seconds = 10;
            // Start the countdown timer
            otaRebootTimer();
        } 
        else if (response.ota_update_status == -1)
		{
            document.getElementById("ota_update_status").innerHTML = "!!! Upload Error !!!";
        }
    }
}

/**
 * Displays the reboot countdown.
 */
function otaRebootTimer() 
{	
    document.getElementById("ota_update_status").innerHTML = "OTA Firmware Update Complete. This page will close shortly, Rebooting in: " + seconds;

    if (--seconds == 0) 
	{
        clearTimeout(otaTimerVar);
        window.location.reload();
    } 
	else 
	{
        otaTimerVar = setTimeout(otaRebootTimer, 1000);
    }
}

/**
 * Gets DHT temp and humidity values for display on the web page.
 */
function getDHTJson() {
  $.getJSON('/dht.json', function(data) {
    $("#temperature_reading").text(data["temp"]);
    $("#humidity_reading").text(data["humidity"]);
  });
}

/**
 * Starts the polling for DHT sensor data
*/
function startDHTSensorInterval() {
  setInterval(getDHTJson, 5000);
}

function stopWifiConnectStatusInterval() {
  if (wifiConnectInterval != null) {
    clearInterval(wifiConnectInterval);
    wifiConnectInterval = null;
  }
}

function getWifiConnectStatus() {
  console.log('Getting /wifiConnectStatus...');
  var xhr = new XMLHttpRequest();
  var requestUrl = '/wifiConnectStatus';
  xhr.open('POST', requestUrl, false);
  xhr.send('wifi_connect_status');

  console.log('Request sent...');
  if (xhr.readyState == 4 && xhr.status == 200) {
    var response = JSON.parse(xhr.responseText);

    document.getElementById("wifi_connect_status").innerHTML = "Connecting...";
 
    if (response.wifi_connect_status == 2) {
      document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Failed to connect. Please check your credentials.</h4>";
    } else if (response.wifi_connect_status == 3) {
      document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connection success!</h4>";
      stopWifiConnectStatusInterval();
      getConnectInfo();
    }
  } else {
    console.log('Status Check skipped');
  }
}

function startWifiConnectStatusInterval() {
  wifiConnectInterval = setInterval(getWifiConnectStatus, 2800);
}
/**
 * Calls wifi connect methods
 * and triggers the status check interval
 */
function connectWifi() {
  ssid = $("#connect_ssid").val();
  pass = $("#connect_pass").val();

  console.log(`Connecting to wifi with credentials:\n${ssid}\n${pass}...`);
  $.ajax({
    url: '/wifiConnect.json',
    dataType: 'json',
    method: 'POST',
    cache: false,
    headers: { 'my-connect-ssid': ssid, 'my-connect-pwd': pass },
    data: { 'timestamp': Date.now() },
  });

  startWifiConnectStatusInterval();
}

/**
 * Checks credentials inputs and tries to connect
 */
function checkCredentials() {
  console.log("Checking credentials...");
  errorsList = "";
  areCredsOkay = true;

  ssid = $("#connect_ssid").val();
  pass = $("#connect_pass").val();

  if (ssid == "") {
    errorsList += "<h4>SSID cannot be blank.</h4>";
    areCredsOkay = false;
  }

  if (pass == "") {
    errorsList += "<h4>Password cannot be blank.</h4>";
    areCredsOkay = false;
  }

  if (areCredsOkay == false) {
    console.log("Creds not okay.");
    $("#wifi_connect_credentials_errors").html(errorsList);
  } else {
    console.log("Creds okay");
    $("#wifi_connect_credentials_errors").html("");
    connectWifi();
  }
}


function showPassword() {
	var x = document.getElementById("connect_pass");
	if (x.type === "password")
	{
		x.type = "text";
	}
	else
	{
		x.type = "password";
	}
}

function getConnectInfo() {
  $.getJSON("/wifiConnectInfo.json", function(data) {
    $("#connected_ap_label").html("Connected to: ");
    $("#connected_ap").text(data["ap"]);

    $("#ip_address_label").html("IP Address: ");
    $("#wifi_connect_ip").text(data["ip"]);

    $("#netmask_label").html("Netmask: ");
    $("#wifi_connect_netmask").text(data["netmask"]);

    $("#gateway_label").html("Gateway: ");
    $("#wifi_connect_gw").text(data["gw"]);

    document.getElementById("disconnect_wifi").style.display = "block";
  });
}

function disconnectWifi() {
  $.ajax({
    url: "/wifiDisconnect.json",
    dataType: "json",
    method: "DELETE",
    cache: false,
    data: { "timestamp": Date.now() }
  });

  setTimeout(() => location.reload(true), 2000);
}

function getLocalTime() {
  $.getJSON('/localTime.json', function(data) {
    $("#local_time").text(data["time"]);
  });
}

function startLocalTimeInterval() {
  setInterval(getLocalTime, 10000);
}

function getSSID() {
  $.getJSON('/ap_ssid.json', function(data) {
    $("#ap_ssid").text(data["ssid"]);
  });
}
