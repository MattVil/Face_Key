var demo = null;
console.debug = function() {};

window.addEventListener("load", function() {
  var connect = document.getElementById("connect");
  var address = document.getElementById("address");

  var echoClient = newEchoClient(address.value);
  connect.onclick = function(ev) {
    echoClient.disconnect();
    echoClient = newEchoClient(address.value);
  };
  address.onkeydown = function(ev) {
    if (ev.which == 13) {
      echoClient.disconnect();
      echoClient = newEchoClient(address.value);
    }
  };

});

var newEchoClient = function(address) {
  var ec = new chromeNetworking.clients.echoClient();
  var hostnamePort = address.split(":");
  var hostname = hostnamePort[0];
  var port = (hostnamePort[1] || 7) | 0;
  ec.connect(
    hostname, port,
    function() {
      console.log("Connected");
    }
  );
  return ec;
};
