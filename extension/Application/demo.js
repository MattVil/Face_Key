var demo = null;
console.debug = function() {};

var socket = chrome.sockets.udp;
var PORT = 3001;
var HOST = '127.0.0.1';
var sockInfo;

chrome.app.runtime.onLaunched.addListener(function() {
    // Create window
    chrome.app.window.create('echo_mco.html', {
        'bounds': {
            'width': 400,
            'height': 500
        }
    });
});

function splitString(stringToSplit, separator) {
  return arrayOfStrings = stringToSplit.split(separator);
}

chrome.runtime.onMessageExternal.addListener(
  function(message, sender, sendResponse) {
    // TODO: Validate that sender.id is allowed to invoke the app!
    console.log('TODO: Do something with ' + message );
    if(message=="300"){
      send("300");
    }
    var slash = "/";
    var ptvirgule = ";";
    var splitmsg = splitString(message, ptvirgule);
    var code = splitmsg[0];
    if(code=="302"){
      var domainname = splitmsg[1];
      send(domainname);
      console.log("I just sent to client the domain name : " + domainname);
    }

    sendResponse('Processed file');
});

socket.create({}, function(_sockInfo){
        sockInfo = _sockInfo;
        socket.bind(sockInfo.socketId, HOST, 3002, function(result){
            if(result < 0){
                throw "Bind failed"
            }
            console.log("Socket created and bound.");
            socket.onReceive.addListener(function(info){
                console.log('Received packet from ' + info.remoteAddress + 
                  ' : ' + ab2str(info.data) + ' ' + info.remotePort + ', length ' + info.data.length);
               if(ab2str(info.data)=="309"){
                           console.log("I just sent to client i want to connect");
                  messagepassing("301");
                }
                var slash = "/";
                var ptvirgule = ";";
                var splitmsg = splitString(ab2str(info.data), ptvirgule);
                var code = splitmsg[0];
                if(code=="305"){
                  messagepassing(ab2str(info.data));
                  console.log("I just sent to client the login ids : " + splitmsg[1]);
                }
            });
        });
    }); 

function send(wholeString){
  chrome.sockets.udp.create({}, function (socketInfo) {
      // The socket is created, now we can send some data
      var socketId = socketInfo['socketId'];
      var arrayBuffer = stringToArrayBuffer("hello");
      chrome.sockets.udp.bind(socketId, HOST, 0, function (result) {
          chrome.sockets.udp.send(socketId, stringToArrayBuffer(wholeString), HOST, PORT, function (sendInfo) {
              console.log("sent " + wholeString + " total bytes sent : " +  sendInfo.bytesSent);
              if (sendInfo.resultCode < 0) {
                  console.log("Error listening: " + chrome.runtime.lastError.message);
              }
          });
      });
  });

}

function stringToArrayBuffer(string) {
    var arrayBuffer = new ArrayBuffer(string.length * 2);
    var buffer = new Uint8Array(arrayBuffer);
    for (var i = 0, stringLength = string.length; i < stringLength; i++) {
        buffer[i] = string.charCodeAt(i);
        // Was: buffer = string.charCodeAt(i);
    }
    return buffer;
}
function ab2str(buf) {
  return String.fromCharCode.apply(null, new Uint8Array(buf));
}

function messagepassing(message){
var app_id = "bgldmojaamnojiefdcobliedlibdkcnd";
    chrome.runtime.sendMessage(app_id, message, function(result) {
        if (chrome.runtime.lastError) {
            // Handle error, e.g. app not installed
            console.warn('Error: ' + chrome.runtime.lastError.message);
        } else {
            // Handle success
            console.log('Reply from app: ', result);
        }
    });
}



